#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"


#ifdef ADDON_lua
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#endif

#ifdef ADDON_lua
extern "C" {
	static int l_cppfunction(lua_State *L) {
		double arg = luaL_checknumber(L, 1);
		lua_pushnumber(L, arg * 0.5);
		return 1;
	}
}
#endif

class LuaExample : public OPgameState {
	void Init(OPgameState* last) {
#ifdef ADDON_lua
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);

		if (luaL_loadfile(L, "luascript.lua")) {
			//cerr << "Something went wrong loading the chunk (syntax error?)" << endl;
			//cerr << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);
		}

		//cout << "** Make a insert a global var into Lua from C++" << endl;
		lua_pushnumber(L, 1.1);
		lua_setglobal(L, "cppvar");

		//cout << "** Execute the Lua chunk" << endl;
		if (lua_pcall(L, 0, LUA_MULTRET, 0)) {
			//cerr << "Something went wrong during execution" << endl;
			//cerr << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);
		}

		//cout << "** Read a global var from Lua into C++" << endl;
		lua_getglobal(L, "luavar");
		double luavar = lua_tonumber(L, -1);
		lua_pop(L, 1);
		//cout << "C++ can read the value set from Lua luavar = " << luavar << endl;

		//cout << "** Execute a Lua function from C++" << endl;
		lua_getglobal(L, "myluafunction");
		lua_pushnumber(L, 5);
		lua_pcall(L, 1, 1, 0);
		//cout << "The return value of the function was " << lua_tostring(L, -1) << endl;
		lua_pop(L, 1);

		//cout << "** Execute a C++ function from Lua" << endl;
		//cout << "**** First register the function in Lua" << endl;
		lua_pushcfunction(L, l_cppfunction);
		lua_setglobal(L, "cppfunction");

		//cout << "**** Call a Lua function that uses the C++ function" << endl;
		lua_getglobal(L, "myfunction");
		lua_pushnumber(L, 5);
		lua_pcall(L, 1, 1, 0);
		//cout << "The return value of the function was " << lua_tonumber(L, -1) << endl;
		lua_pop(L, 1);

		//cout << "** Rel
#endif
	}

	OPint Update(OPtimer* time) {

		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0, 0, 0);

		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		return 0;
	}
};


OPint GS_EXAMPLE_LUA_AVAILABLE = 1;
// This is the Game State for this IMGUIExample
// Each entry is a function pointer for Initialize, Update, Destroy
LuaExample _GS_EXAMPLE_LUA;
OPgameState* GS_EXAMPLE_LUA = &_GS_EXAMPLE_LUA;