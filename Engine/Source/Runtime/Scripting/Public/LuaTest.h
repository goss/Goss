#pragma once
#include <include/lua.hpp>

namespace Goss
{
	class LuaTest
	{
	public:
		static bool CheckLua(lua_State *state, int results);
		static int lua_HostFunction(lua_State *state);
		static void Run();
	};
}
