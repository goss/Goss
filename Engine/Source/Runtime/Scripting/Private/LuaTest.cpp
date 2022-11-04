#include "gepch.h"
#include "LuaTest.h"

namespace Goss
{
	bool LuaTest::CheckLua(lua_State *state, const int results)
	{
		if (results != LUA_OK)
		{
			const std::string string = lua_tostring(state, -1);
			std::cout << string << std::endl;
			return false;
		}
		return true;
	}

	int LuaTest::lua_HostFunction(lua_State *state)
	{
		const float a = static_cast<float>(lua_tonumber(state, 1));
		const float b = static_cast<float>(lua_tonumber(state, 2));
		std::cout << "[cpp] HostFunction(" << a << ", " << b << ") called from Lua" << std::endl;
		const float c = a * b;
		lua_pushnumber(state, c);
		return 1;
	}


	void LuaTest::Run()
	{
		lua_State *luaState = luaL_newstate();

		// Add standard libraries to Lua Virtual Machine
		luaL_openlibs(luaState);

		lua_register(luaState, "HostFunction", lua_HostFunction);

		// Load and parse the Lua File
		if(CheckLua(luaState, luaL_dofile(luaState, "Assets/Lua/LuaTest.lua")))
		{
			struct Player
			{
				std::string name = "unknown";
				int level = 0;
			} player;

			// Reading variables
			std::cout << "[Reading variables]" << std::endl;
			lua_getglobal(luaState, "a");
			if (lua_isnumber(luaState, -1))
			{
				std::cout << "A = " << static_cast<int>(lua_tointeger(luaState, -1)) << std::endl;
			}
			lua_getglobal(luaState, "b");
			if (lua_isnumber(luaState, -1))
			{
				std::cout << "B = " << static_cast<int>(lua_tointeger(luaState, -1)) << std::endl;
			}
			lua_getglobal(luaState, "c");
			if (lua_isnumber(luaState, -1))
			{
				std::cout << "C = " << static_cast<int>(lua_tointeger(luaState, -1)) << std::endl;
			}
			lua_getglobal(luaState, "d");
			if (lua_isstring(luaState, -1))
			{
				std::cout << "D = " << lua_tostring(luaState, -1) << std::endl << std::endl;
			}

			// Reading Table Object
			std::cout << "[Reading table object]" << std::endl;
			lua_getglobal(luaState, "player");
			if (lua_istable(luaState, -1))
			{
				lua_pushstring(luaState, "Name");
				lua_gettable(luaState, -2);
				player.name = lua_tostring(luaState, -1);
				lua_pop(luaState, 1);

				lua_pushstring(luaState, "Level");
				lua_gettable(luaState, -2);
				player.level = static_cast<int>(lua_tointeger(luaState, -1));
				lua_pop(luaState, 1);
			}
			std::cout << "[Player: " << player.name << "] [Lvl: " << player.level << "]" << std::endl << std::endl; 

			// Calling Lua Function
			std::cout << "[Calling a lua function]" << std::endl;
			lua_getglobal(luaState, "LuaFunc");
			if (lua_isfunction(luaState, -1))
			{
				lua_pushnumber(luaState, 5.0f);
				lua_pushnumber(luaState, 6.0f);
				lua_pushstring(luaState, "Bwa ha ha!");
				if (CheckLua(luaState, lua_pcall(luaState, 3, 1, 0)))
				{
					std::cout << "[lua Func] returned " << static_cast<float>(lua_tonumber(luaState, -1)) << std::endl << std::endl;
				}
			}
		}
		lua_close(luaState);
	}
}
