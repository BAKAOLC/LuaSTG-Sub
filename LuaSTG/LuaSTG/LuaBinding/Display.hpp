#pragma once
#include <string_view>
#include "lua.hpp"
#include "Core/Graphics/Window.hpp"

namespace LuaSTG::Sub::LuaBinding {
	
	struct Display {

		static std::string_view class_name;

		[[maybe_unused]] Core::Graphics::IDisplay* data{};

		static bool is(lua_State* L, int index);

		static Display* as(lua_State* L, int index);

		static Display* create(lua_State* L);

		static void registerClass(lua_State* L);

	};

}
