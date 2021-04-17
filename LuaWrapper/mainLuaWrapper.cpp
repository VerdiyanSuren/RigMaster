//#include <iostream>
#include <string>
#include <iostream>
#include <new>

#include <vufLog.h>
#include "include/vufLuaWrapper.h"
#include <vufLuaVector4.h>
#include <vufLuaQuaternion.h>

VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;

int main()
{
	
	constexpr char* l_lua_str_1 = R"(
	function Pythagoras(a,b)
		return (a*a + b*b)
	end
	)";
	
	constexpr char* l_lua_str_2 = R"(
	print(Pythagoras(3,4))
	)";

	constexpr char* l_lua_str_3 = R"(
	print(d)
	)";
	luaWrapper l_w;
	l_w.open_machine();
	lua_State* L = l_w.get_lua_state();

	vufLuaVector4<double>::registrator(L);
	vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::registrator(L);
	vufLuaQuaternion_4<double>::registrator(L);

	l_w.dump_stack();
	//lua_pushnumber(L, 123);
	
	if (l_w.load_and_compile(l_lua_str_1) == false)
	{
		VF_LOG_ERR("FAILED TO COMPILE");
	}
	l_w.do_compiled();
	VF_LOG_INFO("-----------------------Start dumping----------------------");
	//l_w.clear_stack();
	//l_w.dump_stack();
	double l_d = 1.0;
	lua_pushnumber(L, l_d);
	lua_setglobal(L, "d");
	l_w.dump_stack(); 
	//l_w.do_string(l_lua_str_1);
	
	
	
	l_w.do_string(l_lua_str_3);

	std::string l_str;
	std::getline(std::cin, l_str);
	while (l_str.empty() == false)
	{
		l_w.do_string(l_str);
		l_w.dump_stack();
		std::getline(std::cin, l_str);
	}

	lua_close(L);

	system("pause");
	return 0;
}

