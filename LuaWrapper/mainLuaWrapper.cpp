//#include <iostream>
#include <string>
#include <new>

#include <vufLog.h>
#include "include/vufLuaWrapper.h"
#include <vufVectorLua.h>
#include <vufQuaternionLua.h>

//VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;

int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	vufVector_4Lua<double>::register_vufVector_4(L);
	vufQuaternion_4Lua<double>::register_vufQuaternion(L);
	//lua_pushnumber(L, 123);
	luaWrapper l_w;
	l_w.dump_stack(L);
	l_w.clear_stack(L);
	l_w.dump_stack(L);
	
	lua_close(L);

	system("pause");
	return 0;
}

