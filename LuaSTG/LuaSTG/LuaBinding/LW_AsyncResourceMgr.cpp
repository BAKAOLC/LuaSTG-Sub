#include "LuaBinding/LuaWrapper.hpp"
#include "lua/plus.hpp"
#include "GameResource/AsyncResourceLoader.hpp"
#include "AppFrame.h"

namespace luastg::binding
{
    // 内联辅助函数：读取通用精灵参数
    namespace detail
    {
        // 读取通用精灵参数（Sprite、Animation、Particle 共享）
        inline void ReadCommonSpriteParams(lua_State* L, int table_index,
                                          double& x, double& y, double& w, double& h,
                                          std::optional<double>& anchor_x, std::optional<double>& anchor_y, 
                                          double& a, double& b, bool& is_rect)
        {
            lua::stack_t stack(L);
            lua::stack_index_t table_idx(table_index);
            
            if (stack.has_map_value(table_idx, "x")) x = stack.get_map_value<double>(table_idx, "x");
            if (stack.has_map_value(table_idx, "y")) y = stack.get_map_value<double>(table_idx, "y");
            if (stack.has_map_value(table_idx, "w")) w = stack.get_map_value<double>(table_idx, "w");
            if (stack.has_map_value(table_idx, "h")) h = stack.get_map_value<double>(table_idx, "h");
            if (stack.has_map_value(table_idx, "anchor_x")) anchor_x = stack.get_map_value<double>(table_idx, "anchor_x");
            if (stack.has_map_value(table_idx, "anchor_y")) anchor_y = stack.get_map_value<double>(table_idx, "anchor_y");
            if (stack.has_map_value(table_idx, "a")) a = stack.get_map_value<double>(table_idx, "a");
            if (stack.has_map_value(table_idx, "b")) b = stack.get_map_value<double>(table_idx, "b");
            if (stack.has_map_value(table_idx, "rect")) is_rect = stack.get_map_value<bool>(table_idx, "rect");
        }
    }
    
    // LoadingTask Lua 包装
    struct LuaLoadingTask
    {
        static constexpr std::string_view const ClassID{ "LuaSTG.Sub.LoadingTask" };
        
        std::shared_ptr<ResourceLoadingTask> task;
        
        // 内联辅助宏：检查任务有效性
        #define CHECK_TASK_VALID() \
            if (!self->task) { \
                return luaL_error(L, "Invalid loading task"); \
            }
        
        static int api_getId(lua_State* L)
        {
            lua::stack_t S(L);
            auto* self = cast(L, 1);
            CHECK_TASK_VALID()
            S.push_value(self->task->GetId());
            return 1;
        }
        
        static int api_getProgress(lua_State* L)
        {
            lua::stack_t S(L);
            auto* self = cast(L, 1);
            CHECK_TASK_VALID()
            S.push_value(self->task->GetCompletedCount());
            S.push_value(self->task->GetTotalCount());
            return 2;
        }
        
        static int api_isCompleted(lua_State* L)
        {
            lua::stack_t S(L);
            auto* self = cast(L, 1);
            CHECK_TASK_VALID()
            S.push_value(self->task->IsCompleted());
            return 1;
        }
        
        static int api_isCancelled(lua_State* L)
        {
            lua::stack_t S(L);
            auto* self = cast(L, 1);
            CHECK_TASK_VALID()
            S.push_value(self->task->IsCancelled());
            return 1;
        }
        
        static int api_getStatus(lua_State* L)
        {
            lua::stack_t S(L);
            auto* self = cast(L, 1);
            CHECK_TASK_VALID()
            auto status = self->task->GetStatus();
            std::string_view status_str = "unknown";
            
            switch (status)
            {
            case LoadingTaskStatus::Pending:
                status_str = "pending";
                break;
            case LoadingTaskStatus::Loading:
                status_str = "loading";
                break;
            case LoadingTaskStatus::Completed:
                status_str = "completed";
                break;
            case LoadingTaskStatus::Failed:
                status_str = "failed";
                break;
            case LoadingTaskStatus::Cancelled:
                status_str = "cancelled";
                break;
            }
            
            S.push_value(status_str);
            return 1;
        }
        
        static int api_cancel(lua_State* L)
        {
            auto* self = cast(L, 1);
            CHECK_TASK_VALID()
            self->task->Cancel();
            return 0;
        }
        
        static int api_wait(lua_State* L)
        {
            auto* self = cast(L, 1);
            CHECK_TASK_VALID()
            // 简单的轮询等待
            while (!self->task->IsCompleted() && !self->task->IsCancelled())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            return 0;
        }
        
        static int api_getResults(lua_State* L)
        {
            lua::stack_t stack(L);
            auto* self = cast(L, 1);
            CHECK_TASK_VALID()
            
            auto results = self->task->GetResults();
            
            // 创建结果表
            auto results_array = stack.create_array(results.size());
            
            for (size_t i = 0; i < results.size(); ++i)
            {
                const auto& result = results[i];
                
                // 创建单个结果表
                auto result_map = stack.create_map(4);
                
                // name
                stack.set_map_value(result_map, "name", result.name);
                
                // success
                stack.set_map_value(result_map, "success", result.success);
                
                // error (如果有)
                if (!result.success && !result.error_message.empty())
                {
                    stack.set_map_value(result_map, "error", result.error_message);
                }
                
                // type
                stack.set_map_value(result_map, "type", result.type);
                
                // 设置到结果数组
                stack.set_array_value(results_array, lua::stack_index_t(i + 1), result_map);
            }
            
            return 1;
        }
        
        static int api___gc(lua_State* L)
        {
            auto* self = cast(L, 1);
            // 显式调用析构函数
            self->~LuaLoadingTask();
            return 0;
        }
        
        static int api___tostring(lua_State* L)
        {
            lua::stack_t S(L);
            auto* self = cast(L, 1);
            if (self->task)
            {
                char buffer[128];
                snprintf(buffer, sizeof(buffer), "LoadingTask(id=%llu, %zu/%zu)", 
                         static_cast<unsigned long long>(self->task->GetId()),
                         self->task->GetCompletedCount(),
                         self->task->GetTotalCount());
                S.push_value(buffer);
            }
            else
            {
                S.push_value<std::string_view>(ClassID);
            }
            return 1;
        }
        
        static LuaLoadingTask* create(lua_State* L, const std::shared_ptr<ResourceLoadingTask>& task)
        {
            lua::stack_t S(L);
            
            auto* self = S.create_userdata<LuaLoadingTask>();
            
            new (self) LuaLoadingTask();
            
            auto const self_index = S.index_of_top();
            S.set_metatable(self_index, ClassID);
            
            self->task = task;
            return self;
        }
        
        static LuaLoadingTask* cast(lua_State* L, int idx)
        {
            return static_cast<LuaLoadingTask*>(luaL_checkudata(L, idx, ClassID.data()));
        }
        
        static void registerClass(lua_State* L)
        {
            [[maybe_unused]] lua::stack_balancer_t SB(L);
            lua::stack_t S(L);
            
            // method
            auto const method_table = S.create_map();
            S.set_map_value(method_table, "getId", &api_getId);
            S.set_map_value(method_table, "getProgress", &api_getProgress);
            S.set_map_value(method_table, "isCompleted", &api_isCompleted);
            S.set_map_value(method_table, "isCancelled", &api_isCancelled);
            S.set_map_value(method_table, "getStatus", &api_getStatus);
            S.set_map_value(method_table, "cancel", &api_cancel);
            S.set_map_value(method_table, "wait", &api_wait);
            S.set_map_value(method_table, "getResults", &api_getResults);
            
            // metatable
            auto const metatable = S.create_metatable(ClassID);
            S.set_map_value(metatable, "__gc", &api___gc);
            S.set_map_value(metatable, "__tostring", &api___tostring);
            S.set_map_value(metatable, "__index", method_table);
        }
        
        #undef CHECK_TASK_VALID
    };
    
    // 异步资源管理器 Lua API
    struct AsyncResourceManager
    {
        // 从 Lua 表中读取参数到 request
        static void ReadRequestParams(lua_State* L, int table_index, ResourceLoadRequest& request)
        {
            lua::stack_t stack(L);
            lua::stack_index_t table_idx(table_index);
            
            if (stack.has_map_value(table_idx, "name"))
            {
                request.name = stack.get_map_value<std::string>(table_idx, "name");
            }
            
            if (stack.has_map_value(table_idx, "pool"))
            {
                auto pool = stack.get_map_value<std::string>(table_idx, "pool");
                if (pool == "global")
                {
                    request.target_pool = LRES.GetResourcePool(ResourcePoolType::Global);
                }
                else if (pool == "stage")
                {
                    request.target_pool = LRES.GetResourcePool(ResourcePoolType::Stage);
                }
            }
            
            switch (request.type)
            {
            case ResourceType::Texture:
            {
                TextureLoadParams params;
                if (std::holds_alternative<TextureLoadParams>(request.params))
                {
                    params = std::get<TextureLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "path")) params.path = stack.get_map_value<std::string>(table_idx, "path");
                if (stack.has_map_value(table_idx, "mipmaps")) params.mipmaps = stack.get_map_value<bool>(table_idx, "mipmaps");
                if (stack.has_map_value(table_idx, "width")) params.width = stack.get_map_value<int32_t>(table_idx, "width");
                if (stack.has_map_value(table_idx, "height")) params.height = stack.get_map_value<int32_t>(table_idx, "height");
                
                request.params = params;
                break;
            }
            
            case ResourceType::Sprite:
            {
                SpriteLoadParams params;
                if (std::holds_alternative<SpriteLoadParams>(request.params))
                {
                    params = std::get<SpriteLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "texture")) params.texture_name = stack.get_map_value<std::string>(table_idx, "texture");
                detail::ReadCommonSpriteParams(L, table_index, params.x, params.y, params.w, params.h,
                                      params.anchor_x, params.anchor_y, params.a, params.b, params.is_rect);
                
                request.params = params;
                break;
            }
            
            case ResourceType::Animation:
            {
                AnimationLoadParams params;
                if (std::holds_alternative<AnimationLoadParams>(request.params))
                {
                    params = std::get<AnimationLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "texture")) params.texture_name = stack.get_map_value<std::string>(table_idx, "texture");
                detail::ReadCommonSpriteParams(L, table_index, params.x, params.y, params.w, params.h,
                                      params.anchor_x, params.anchor_y, params.a, params.b, params.is_rect);
                if (stack.has_map_value(table_idx, "n")) params.n = stack.get_map_value<int32_t>(table_idx, "n");
                if (stack.has_map_value(table_idx, "m")) params.m = stack.get_map_value<int32_t>(table_idx, "m");
                if (stack.has_map_value(table_idx, "interval")) params.interval = stack.get_map_value<int32_t>(table_idx, "interval");
                
                stack.push_map_value(table_idx, "sprites");
                if (stack.is_table(-1))
                {
                    size_t sprite_count = stack.get_array_size(-1);
                    for (size_t j = 1; j <= sprite_count; ++j)
                    {
                        stack.push_array_value_zero_base(-1, j - 1);
                        if (stack.is_string(-1))
                        {
                            params.sprite_names.push_back(stack.get_value<std::string>(-1));
                        }
                        stack.pop_value();
                    }
                }
                stack.pop_value();
                
                request.params = params;
                break;
            }
            
            case ResourceType::Music:
            {
                MusicLoadParams params;
                if (std::holds_alternative<MusicLoadParams>(request.params))
                {
                    params = std::get<MusicLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "path")) params.path = stack.get_map_value<std::string>(table_idx, "path");
                if (stack.has_map_value(table_idx, "loop_start")) params.loop_start = stack.get_map_value<double>(table_idx, "loop_start");
                if (stack.has_map_value(table_idx, "loop_end")) params.loop_end = stack.get_map_value<double>(table_idx, "loop_end");
                if (stack.has_map_value(table_idx, "once_decode")) params.once_decode = stack.get_map_value<bool>(table_idx, "once_decode");
                
                request.params = params;
                break;
            }
            
            case ResourceType::SoundEffect:
            {
                SoundEffectLoadParams params;
                if (std::holds_alternative<SoundEffectLoadParams>(request.params))
                {
                    params = std::get<SoundEffectLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "path")) params.path = stack.get_map_value<std::string>(table_idx, "path");
                
                request.params = params;
                break;
            }
            
            case ResourceType::SpriteFont:
            {
                SpriteFontLoadParams params;
                if (std::holds_alternative<SpriteFontLoadParams>(request.params))
                {
                    params = std::get<SpriteFontLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "path")) params.path = stack.get_map_value<std::string>(table_idx, "path");
                if (stack.has_map_value(table_idx, "tex_path")) params.font_tex_path = stack.get_map_value<std::string>(table_idx, "tex_path");
                if (stack.has_map_value(table_idx, "mipmaps")) params.mipmaps = stack.get_map_value<bool>(table_idx, "mipmaps");
                
                request.params = params;
                break;
            }
            
            case ResourceType::TrueTypeFont:
            {
                TrueTypeFontLoadParams params;
                if (std::holds_alternative<TrueTypeFontLoadParams>(request.params))
                {
                    params = std::get<TrueTypeFontLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "path")) params.path = stack.get_map_value<std::string>(table_idx, "path");
                if (stack.has_map_value(table_idx, "width")) params.font_width = static_cast<float>(stack.get_map_value<double>(table_idx, "width"));
                if (stack.has_map_value(table_idx, "height")) params.font_height = static_cast<float>(stack.get_map_value<double>(table_idx, "height"));
                
                request.params = params;
                break;
            }
            
            case ResourceType::FX:
            {
                FXLoadParams params;
                if (std::holds_alternative<FXLoadParams>(request.params))
                {
                    params = std::get<FXLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "path")) params.path = stack.get_map_value<std::string>(table_idx, "path");
                
                request.params = params;
                break;
            }
            
            case ResourceType::Model:
            {
                ModelLoadParams params;
                if (std::holds_alternative<ModelLoadParams>(request.params))
                {
                    params = std::get<ModelLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "path")) params.path = stack.get_map_value<std::string>(table_idx, "path");
                
                request.params = params;
                break;
            }
            
            case ResourceType::Particle:
            {
                ParticleLoadParams params;
                if (std::holds_alternative<ParticleLoadParams>(request.params))
                {
                    params = std::get<ParticleLoadParams>(request.params);
                }
                
                if (stack.has_map_value(table_idx, "path")) params.path = stack.get_map_value<std::string>(table_idx, "path");
                if (stack.has_map_value(table_idx, "img_name")) params.particle_img_name = stack.get_map_value<std::string>(table_idx, "img_name");
                if (stack.has_map_value(table_idx, "a")) params.a = stack.get_map_value<double>(table_idx, "a");
                if (stack.has_map_value(table_idx, "b")) params.b = stack.get_map_value<double>(table_idx, "b");
                if (stack.has_map_value(table_idx, "rect")) params.is_rect = stack.get_map_value<bool>(table_idx, "rect");
                
                request.params = params;
                break;
            }
            
            default:
                break;
            }
        }
        
        // 解析加载请求表，支持默认参数和覆盖
        // table_index: 资源列表表的索引
        // defaults_index: 默认参数表的索引（0 表示没有）
        // default_type: 默认资源类型
        // 返回：请求列表和默认资源池
        static std::pair<std::vector<ResourceLoadRequest>, ResourcePool*> ParseLoadRequests(lua_State* L, int table_index, int defaults_index, ResourceType default_type)
        {
            lua::stack_t stack(L);
            std::vector<ResourceLoadRequest> requests;
            ResourcePool* default_pool = nullptr;
            
            if (!stack.is_table(table_index))
            {
                luaL_error(L, "Expected table of load requests");
                return {requests, default_pool};
            }
            
            ResourceLoadRequest default_request;
            default_request.type = default_type;
            
            if (defaults_index != 0 && stack.is_table(defaults_index))
            {
                ReadRequestParams(L, defaults_index, default_request);
                default_pool = default_request.target_pool;
            }
            
            size_t count = stack.get_array_size(table_index);
            requests.reserve(count);
            
            for (size_t i = 1; i <= count; ++i)
            {
                stack.push_array_value_zero_base(table_index, i - 1);
                
                if (!stack.is_table(-1))
                {
                    stack.pop_value();
                    continue;
                }
                
                ResourceLoadRequest request = default_request;
                ReadRequestParams(L, -1, request);
                
                requests.push_back(std::move(request));
                stack.pop_value();
            }
            
            return {requests, default_pool};
        }
        
        // 通用的异步加载函数模板
        template<ResourceType Type>
        static inline int LoadResourceAsync(lua_State* L) noexcept
        {
            try
            {
                lua::stack_t stack(L);
                int defaults_index = (stack.index_of_top() >= 2 && stack.is_table(2)) ? 2 : 0;
                auto [requests, default_pool] = ParseLoadRequests(L, 1, defaults_index, Type);
                
                if (requests.empty())
                {
                    return luaL_error(L, "No valid load requests");
                }
                
                auto* loader = LAPP.GetAsyncResourceLoader();
                if (!loader)
                {
                    return luaL_error(L, "AsyncResourceLoader not initialized");
                }
                
                auto task = loader->SubmitTask(std::move(requests), true, default_pool);
                if (!task)
                {
                    return luaL_error(L, "Failed to submit loading task");
                }
                
                LuaLoadingTask::create(L, task);
                return 1;
            }
            catch (const std::exception& e)
            {
                return luaL_error(L, "LoadResourceAsync failed: %s", e.what());
            }
        }
        
        static int LoadTextureAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::Texture>(L);
        }
        
        static int LoadSpriteAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::Sprite>(L);
        }
        
        static int LoadAnimationAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::Animation>(L);
        }
        
        static int LoadMusicAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::Music>(L);
        }
        
        static int LoadSoundAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::SoundEffect>(L);
        }
        
        static int LoadFontAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::TrueTypeFont>(L);
        }
        
        static int LoadSpriteFontAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::SpriteFont>(L);
        }
        
        static int LoadFXAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::FX>(L);
        }
        
        static int LoadModelAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::Model>(L);
        }
        
        static int LoadParticleAsync(lua_State* L) noexcept
        {
            return LoadResourceAsync<ResourceType::Particle>(L);
        }
        
        static int GetAsyncLoaderThreadCount(lua_State* L) noexcept
        {
            lua::stack_t stack(L);
            auto* loader = LAPP.GetAsyncResourceLoader();
            stack.push_value(loader ? loader->GetThreadCount() : 0);
            return 1;
        }
        
        static int SetAsyncLoaderMaxItemsPerFrame(lua_State* L) noexcept
        {
            auto* loader = LAPP.GetAsyncResourceLoader();
            if (!loader)
            {
                return luaL_error(L, "AsyncResourceLoader not initialized");
            }
            lua_Integer count = luaL_checkinteger(L, 1);
            if (count < 1)
            {
                return luaL_error(L, "MaxGPUItemsPerFrame must be at least 1");
            }
            loader->SetMaxGPUItemsPerFrame(static_cast<size_t>(count));
            return 0;
        }
        
        static int GetAsyncLoaderMaxItemsPerFrame(lua_State* L) noexcept
        {
            lua::stack_t stack(L);
            auto* loader = LAPP.GetAsyncResourceLoader();
            stack.push_value(loader ? loader->GetMaxGPUItemsPerFrame() : 0);
            return 1;
        }
        
        static int ClearAsyncLoaderTasks(lua_State* L) noexcept
        {
            auto* loader = LAPP.GetAsyncResourceLoader();
            if (loader)
            {
                loader->ClearAllTasks();
            }
            return 0;
        }
        
        static void Register(lua_State* L) noexcept
        {
            // 注册 LoadingTask 类
            LuaLoadingTask::registerClass(L);
            
            // 注册异步加载函数
            luaL_Reg const lib[] = {
                { "LoadTextureAsync", &LoadTextureAsync },
                { "LoadSpriteAsync", &LoadSpriteAsync },
                { "LoadAnimationAsync", &LoadAnimationAsync },
                { "LoadMusicAsync", &LoadMusicAsync },
                { "LoadSoundAsync", &LoadSoundAsync },
                { "LoadFontAsync", &LoadFontAsync },
                { "LoadSpriteFontAsync", &LoadSpriteFontAsync },
                { "LoadFXAsync", &LoadFXAsync },
                { "LoadModelAsync", &LoadModelAsync },
                { "LoadParticleAsync", &LoadParticleAsync },
                { "GetAsyncLoaderThreadCount", &GetAsyncLoaderThreadCount },
                { "SetAsyncLoaderMaxItemsPerFrame", &SetAsyncLoaderMaxItemsPerFrame },
                { "GetAsyncLoaderMaxItemsPerFrame", &GetAsyncLoaderMaxItemsPerFrame },
                { "ClearAsyncLoaderTasks", &ClearAsyncLoaderTasks },
                { nullptr, nullptr }
            };
            
            luaL_register(L, "lstg", lib);
            lua_pop(L, 1);
        }
    };
}

// 在 LuaWrapper.cpp 中调用此函数注册
namespace luastg::binding
{
    void RegisterAsyncResourceManager(lua_State* L)
    {
        AsyncResourceManager::Register(L);
    }
}

