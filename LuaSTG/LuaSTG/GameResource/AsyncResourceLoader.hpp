#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <memory>
#include <atomic>
#include <functional>
#include <variant>
#include "core/SmartReference.hpp"
#include "core/Graphics/Sprite.hpp"
#include "GameResource/ResourceBase.hpp"

namespace core { struct IAudioDecoder; }
namespace luastg { class ResourcePool; }

namespace luastg
{
    // 资源加载任务状态
    enum class LoadingTaskStatus
    {
        Pending,    // 等待中
        Loading,    // 加载中
        Completed,  // 已完成
        Failed,     // 失败
        Cancelled   // 已取消
    };

    // 纹理加载参数
    struct TextureLoadParams
    {
        std::string path;
        bool mipmaps = true;
        int width = 0;  // 用于 CreateTexture
        int height = 0;
    };
    
    // 精灵加载参数
    struct SpriteLoadParams
    {
        std::string texture_name;  // 用于资源池 API
        core::ITexture2D* texture_object = nullptr;  // 用于现代 API
        double x = 0.0, y = 0.0, w = 0.0, h = 0.0;
        double anchor_x = 0.0, anchor_y = 0.0;
        bool is_rect = false;
    };
    
    // 动画加载参数
    struct AnimationLoadParams
    {
        std::string texture_name;  // 用于从纹理创建
        double x = 0.0, y = 0.0, w = 0.0, h = 0.0;
        int n = 1, m = 1;  // 行列数
        int interval = 1;  // 帧间隔
        double anchor_x = 0.0, anchor_y = 0.0;
        bool is_rect = false;
        std::vector<std::string> sprite_names;  // 用于从精灵列表创建动画
    };
    
    // 音乐加载参数
    struct MusicLoadParams
    {
        std::string path;
        double loop_start = 0.0;
        double loop_end = 0.0;
        bool once_decode = false;
    };
    
    // 音效加载参数
    struct SoundEffectLoadParams
    {
        std::string path;
    };
    
    // SpriteFont 加载参数
    struct SpriteFontLoadParams
    {
        std::string path;
        std::string font_tex_path;  // 用于 fancy2d 字体，为空则使用 HGE 格式
        bool mipmaps = true;
    };
    
    // TrueTypeFont 加载参数
    struct TrueTypeFontLoadParams
    {
        std::string path;
        float font_width = 0.0f;
        float font_height = 0.0f;
    };
    
    // FX 加载参数
    struct FXLoadParams
    {
        std::string path;
    };
    
    // Model 加载参数
    struct ModelLoadParams
    {
        std::string path;
    };
    
    // 粒子加载参数
    struct ParticleLoadParams
    {
        std::string path;
        std::string particle_img_name;
        double anchor_x = 0.0;
        double anchor_y = 0.0;
        bool is_rect = false;
    };
    
    // 使用 variant 存储不同类型的参数
    using ResourceLoadParams = std::variant<
        TextureLoadParams,
        SpriteLoadParams,
        AnimationLoadParams,
        MusicLoadParams,
        SoundEffectLoadParams,
        SpriteFontLoadParams,
        TrueTypeFontLoadParams,
        FXLoadParams,
        ModelLoadParams,
        ParticleLoadParams
    >;
    
    // 单个资源加载请求
    struct ResourceLoadRequest
    {
        ResourceType type;
        std::string name;
        ResourceLoadParams params;
        ResourcePool* target_pool = nullptr;
    };

    // 单个资源加载结果
    struct ResourceLoadResult
    {
        std::string name;
        ResourceType type;
        bool success = false;
        std::string error_message;
        
        bool registered_to_pool = false;
        
        core::SmartReference<IResourceBase> resource;
        core::SmartReference<core::ITexture2D> texture;
        core::SmartReference<core::Graphics::ISprite> sprite;
        
        core::SmartReference<core::IAudioDecoder> audio_decoder;
        std::vector<uint8_t> file_data;
        
        // 纹理加载的中间数据（在工作线程中准备，主线程中上传）
        core::SmartReference<core::IImage> image_data;  // 图像数据
        bool needs_mipmap_generation = false;  // 是否需要在主线程生成 mipmap
        
        // 是否需要GPU操作（纹理、模型等）
        // false = 纯CPU资源，主线程只需快速注册，不受限制
        // true = GPU资源，主线程需要上传，受每帧限制
        bool requires_gpu = true;
    };

    // 资源加载任务
    class ResourceLoadingTask
    {
    public:
        using TaskId = uint64_t;
        
    private:
        TaskId m_id;
        std::vector<ResourceLoadRequest> m_requests;
        std::vector<ResourceLoadResult> m_results;
        std::atomic<size_t> m_completed_count{0};
        std::atomic<LoadingTaskStatus> m_status{LoadingTaskStatus::Pending};
        std::atomic<bool> m_cancelled{false};
        std::mutex m_mutex;
        
        bool m_use_resource_pool = true;
        ResourcePool* m_target_pool = nullptr;
        
    public:
        ResourceLoadingTask(TaskId id, std::vector<ResourceLoadRequest> requests, bool use_pool = true, ResourcePool* target_pool = nullptr);
        
        TaskId GetId() const { return m_id; }
        size_t GetTotalCount() const { return m_requests.size(); }
        size_t GetCompletedCount() const { return m_completed_count.load(); }
        LoadingTaskStatus GetStatus() const { return m_status.load(); }
        bool IsCancelled() const { return m_cancelled.load(); }
        bool IsCompleted() const { return m_status.load() == LoadingTaskStatus::Completed; }
        bool UseResourcePool() const { return m_use_resource_pool; }
        ResourcePool* GetTargetPool() const { return m_target_pool; }
        
        void Cancel() { m_cancelled.store(true); }
        
        const std::vector<ResourceLoadRequest>& GetRequests() const { return m_requests; }
        std::vector<ResourceLoadResult> GetResults();
        
        void SetStatus(LoadingTaskStatus status) { m_status.store(status); }
        void SetResult(size_t index, ResourceLoadResult result);
        void IncrementCompleted() { m_completed_count.fetch_add(1); }
    };

    // 异步资源加载器
    class AsyncResourceLoader
    {
    private:
        // 根据 CPU 核心数自动计算合适的线程数
        static size_t GetOptimalThreadCount() noexcept;
        
        static constexpr size_t MIN_THREAD_COUNT = 1;
        static constexpr size_t MAX_THREAD_COUNT = 16;
        static constexpr size_t DEFAULT_THREAD_COUNT = 0; // 0 表示自动检测
        
        std::vector<std::thread> m_worker_threads;
        std::queue<std::shared_ptr<ResourceLoadingTask>> m_task_queue;
        std::mutex m_queue_mutex;
        std::condition_variable m_queue_cv;
        std::atomic<bool> m_shutdown{false};
        
        std::unordered_map<ResourceLoadingTask::TaskId, std::shared_ptr<ResourceLoadingTask>> m_active_tasks;
        std::mutex m_tasks_mutex;
        
        std::atomic<ResourceLoadingTask::TaskId> m_next_task_id{1};
        
        // 主线程完成队列（用于 GPU 资源上传）
        struct CompletionItem
        {
            std::shared_ptr<ResourceLoadingTask> task;
            size_t request_index;
            ResourceLoadResult result;
        };
        std::queue<CompletionItem> m_completion_queue;
        std::mutex m_completion_mutex;
        
        // 每帧最多处理的需要主线程的资源数量（避免阻塞主线程）
        size_t m_max_gpu_items_per_frame = 5;
    public:
        // thread_count: 工作线程数量
        //   0 = 自动检测（推荐）
        //   1-16 = 指定线程数
        AsyncResourceLoader(size_t thread_count = DEFAULT_THREAD_COUNT);
        ~AsyncResourceLoader();
        
        size_t GetThreadCount() const noexcept { return m_worker_threads.size(); }
        
        void SetMaxGPUItemsPerFrame(size_t count) noexcept { m_max_gpu_items_per_frame = count; }
        size_t GetMaxGPUItemsPerFrame() const noexcept { return m_max_gpu_items_per_frame; }
        
        std::shared_ptr<ResourceLoadingTask> SubmitTask(
            std::vector<ResourceLoadRequest> requests,
            bool use_resource_pool = true,
            ResourcePool* target_pool = nullptr
        );
        
        // 获取任务
        std::shared_ptr<ResourceLoadingTask> GetTask(ResourceLoadingTask::TaskId id);
        
        // 主线程更新（处理 GPU 资源上传）
        void Update();
        
        // 取消任务
        void CancelTask(ResourceLoadingTask::TaskId id);
        
        // 等待所有任务完成
        void WaitAll();
        
        void ClearAllTasks();
        
        void ClearTasksForPool(ResourcePool* pool);
        
    private:
        void WorkerThread();
        void ProcessRequest(
            std::shared_ptr<ResourceLoadingTask> task,
            size_t request_index
        );
        
        // 工具函数：检查文件是否为 DDS 格式（通过魔数）
        static bool IsDDSFormat(const void* data, size_t size);
        
        // 工具函数：在工作线程中加载图像数据
        // 返回 true 表示成功，图像数据保存在 out_image_data 或 out_file_data 中
        static bool LoadImageDataInWorkerThread(
            const std::string& file_path,
            bool enable_mipmaps,
            core::SmartReference<core::IImage>& out_image_data,
            std::vector<uint8_t>& out_file_data,
            bool& out_needs_mipmap_generation,
            std::string& out_error_message
        );
        
        // 辅助函数：初始化Worker结果
        static ResourceLoadResult InitWorkerResult(const ResourceLoadRequest& request, bool requires_gpu = true);
        
        // 辅助函数：检查文件存在性
        static bool ValidateFilePath(const std::string& path, ResourceLoadResult& result);
        
        // 辅助函数：获取目标资源池
        ResourcePool* GetTargetResourcePool(std::shared_ptr<ResourceLoadingTask> task, const ResourceLoadRequest& request) const;
        
        // 辅助函数：包装Complete函数的通用逻辑
        template<typename Func>
        void ExecuteComplete(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result, Func&& func);
        
        // 资源加载函数（在工作线程中调用，只做 CPU 端工作）
        ResourceLoadResult LoadTextureWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadSpriteWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadAnimationWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadMusicWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadSoundEffectWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadSpriteFontWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadTrueTypeFontWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadFXWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadModelWorker(const ResourceLoadRequest& request);
        ResourceLoadResult LoadParticleWorker(const ResourceLoadRequest& request);
        
        // 主线程完成函数（GPU 资源创建和资源池注册）
        void CompleteTexture(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteSprite(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteAnimation(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteMusic(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteSoundEffect(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteSpriteFont(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteTrueTypeFont(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteFX(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteModel(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
        void CompleteParticle(std::shared_ptr<ResourceLoadingTask> task, size_t index, ResourceLoadResult& result);
    };
}
