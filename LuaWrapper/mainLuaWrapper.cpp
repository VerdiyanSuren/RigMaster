//#include <iostream>
#include <string>
#include <iostream>
#include <new>

#include <vufLog.h>
#include "include/vufLuaWrapper.h"
#include <vufLuaVector4.h>
#include <vufLuaMatrix4.h>
#include <vufLuaQuaternion.h>
#include <unitTest/vufLuaMathUT.h>


VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
std::string vufLuaVector4<double>::g_table_name;
std::string vufLuaVector4<double>::g_metatable_name;
std::string vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::g_table_name;
std::string vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::g_metatable_name;

std::string vufLuaMatrix4<double>::g_table_name;		// matrix table
std::string vufLuaMatrix4<double>::g_metatable_name;	// matrix metatable
std::string vufLuaMatrix4<double>::g_q_table_name;		// quaternion table
std::string vufLuaMatrix4<double>::g_q_metatable_name;	// quaternion metatable
std::string vufLuaMatrix4<double>::g_v4_table_name;		// vector4 table
std::string vufLuaMatrix4<double>::g_v4_metatable_name;	// vector4 metatable

int main()
{
	vufLuaMathUT<double> l_lua_math_ut;
	l_lua_math_ut.run();




	vufLuaSessionDataStore< std::vector < vufVector4<double>> > l_vec_arr_store;
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
	vufLuaWrapper l_w;
	l_w.open_machine();
	lua_State* L = l_w.get_lua_state();

	vufLuaVector4<double>::registrator(L, "vector4", "vector4Meta"); 
	vufLuaMatrix4<double>::registrator(L, "matrix4", "matrix4Meta", "quat", "quatMeta","vector4", "vector4Meta");
	vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::registrator(L, "vectorArray", "vectorArrM", l_vec_arr_store);
	//vufLuaQuaternion_4<double>::registrator(L);

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

