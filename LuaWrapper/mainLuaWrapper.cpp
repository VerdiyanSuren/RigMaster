#include <iostream>
#include <string>
#include <new>

#include "include/vufLuaWrapper.h"
#include <vufVectorLua.h>
#include <vufQuaternionLua.h>

VF_DECLARE_STD_LOGGER();
VF_LOG_DEFINE_STD_LOGGER();

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
	
	// user data 
	/*
	auto l_create_test = [](lua_State* L) ->int
	{
		luaTest* l_tst = (luaTest*)lua_newuserdata(L, sizeof(luaTest));	// -1
		new (l_tst) luaTest();
		luaL_getmetatable(L, "TSTMetaTable");	// -1  l_tst ->-2
		lua_setmetatable(L, -2);
		return 1;
	};
	auto l_sum_test = [](lua_State* L)->int
	{
		luaTest* l_arg_1 = (luaTest*)luaL_checkudata(L, -1, "TSTMetaTable");
		luaTest* l_arg_2 = (luaTest*)luaL_checkudata(L, -2, "TSTMetaTable");
		if (l_arg_1 != nullptr && l_arg_2 != nullptr)
		{
			luaTest* l_tst = (luaTest*)lua_newuserdata(L, sizeof(luaTest));	// -1
			new (l_tst) luaTest();
			luaL_getmetatable(L, "TSTMetaTable");	// -1  l_tst ->-2
			lua_setmetatable(L, -2);
			l_tst->x = l_arg_1->x + l_arg_2->x;
			l_tst->y = l_arg_1->y + l_arg_2->y;
			l_tst->z = l_arg_1->z + l_arg_2->z;
			return 1;
		}
		else
		{
			std::cout << "Error sum" << std::endl;
		}
		return 1;
	};
	auto l_log_test = [](lua_State* L)->int
	{
		luaTest* l_arg_1 = (luaTest*)luaL_checkudata(L, -1, "TSTMetaTable");
		if (l_arg_1 != nullptr)
		{
			std::cout << "x = " << l_arg_1->x << " y = " << l_arg_1->y << " z = " << l_arg_1->z << std::endl;
		}
		else
		{
			std::cout << "Error log" << std::endl;
		}
		return 0;
	};
	auto l_delete_test = [](lua_State* L)->int
	{
		luaTest* l_arg_1 = (luaTest*)luaL_checkudata(L, -1, "TSTMetaTable");
		l_arg_1->~luaTest();
		return 0;
	};
	auto l_index_test = [](lua_State* L)->int
	{
		const char* l_char_ptr	= luaL_checkstring(L, -1);		
		luaTest* l_arg_2		= (luaTest*)luaL_checkudata(L, -2, "TSTMetaTable");
		if (l_char_ptr == nullptr || l_arg_2 == nullptr)
		{
			std::cout << "Error index" << std::endl;
			return 0;
		}
		if (l_char_ptr[1] == '\0')
		{
			if (l_char_ptr[0] == 'x')
			{
				std::cout << " get x value" << std::endl;
				lua_pushnumber(L, l_arg_2->x);
				return 1;
			}
			if (l_char_ptr[0] == 'y' )
			{
				lua_pushnumber(L, l_arg_2->y);
				return 1;
			}
			if (l_char_ptr[0] == 'z')
			{
				lua_pushnumber(L, l_arg_2->z);
				return 1;
			}
		}
		lua_getglobal(L, "TST");
		lua_pushstring(L, l_char_ptr);
		lua_rawget(L, -2);
		return 1;
	};
	auto l_new_index_test = [](lua_State* L) ->  int
	{
		lua_Number l_value = luaL_checknumber(L, -1);
		// -1 is value we want to set
		const char* l_char_ptr = luaL_checkstring(L, -2);
		luaTest* l_arg_2 = (luaTest*)luaL_checkudata(L, -3, "TSTMetaTable");
		if (l_char_ptr[1] == '\0')
		{
			if (l_char_ptr[0] == 'x')
			{
				std::cout << " set x value" << std::endl;
				l_arg_2->x = l_value;
				return 0;
			}
			if (l_char_ptr[0] == 'y')
			{
				l_arg_2->y = l_value;
				return 0;
			}
			if (l_char_ptr[0] == 'z')
			{
				l_arg_2->z = l_value;
				return 0;
			}
		}
		//handle error
		return 0;
	};

	lua_newtable(L);							// create table and store functions as fields
	int l_test_table_idx = lua_gettop(L);		// get table id in stack
	lua_pushvalue(L, l_test_table_idx);			// push id in stack, because setglobal will pop this and table will stay in stack 
	lua_setglobal(L, "TST");					// assign global name to table and pop previous value. table stay in stack
	
	// add functions as fields
	lua_pushcfunction(L, l_create_test);
	lua_setfield(L, l_test_table_idx, "new");// add field to tableand pop function

	lua_pushcfunction(L, l_log_test);
	lua_setfield(L, l_test_table_idx, "log");	// add field to table and pop function


	luaL_newmetatable(L, "TSTMetaTable");
	
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, l_delete_test);
	lua_settable(L, -3);

	lua_pushstring(L, "__add");
	lua_pushcfunction(L, l_sum_test);
	lua_settable(L, -3);

	// bind table and metatable
	lua_pushstring(L, "__index");
	//lua_pushvalue(L, l_test_table_idx); 	// bind table and metatable
	lua_pushcfunction(L, l_index_test);
	lua_settable(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, l_new_index_test);
	lua_settable(L, -3);
	//lua_pushstring(L, "log");
	//lua_pushcfunction(L, l_log_test);
	//lua_settable(L, -3);
	

	std::string l_str;
	std::getline(std::cin, l_str);
	while (l_str.empty() == false)
	{
		l_w.do_string(L,l_str);
		std::getline(std::cin, l_str);
	}

	*/
	lua_close(L);

	system("pause");
	return 0;
}