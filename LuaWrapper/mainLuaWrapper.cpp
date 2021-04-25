//#include <iostream>
#include <string>
#include <iostream>
#include <new>

#include <vufLog.h>
#include "include/vufLuaWrapper.h"
#include <vufLuaVector4.h>
#include <vufLuaMatrix4.h>
//#include <vufLuaQuaternion.h>
#include <vufLuaStatic.h>
#include <unitTest/vufLuaMathUT.h>


VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
std::string vufLuaStatic::g_vec4_tbl_name	= "vec4";
std::string vufLuaStatic::g_vec4_meta_name	= "vec4M";

std::string vufLuaStatic::g_mat4_tbl_name	= "mat4";
std::string vufLuaStatic::g_mat4_meta_name	= "mat4M";

std::string vufLuaStatic::g_quat_tbl_name	= "quat";
std::string vufLuaStatic::g_quat_meta_name	= "quatM";


std::string vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::g_table_name;
std::string vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::g_metatable_name;


int main()
{
	vufLuaMathUT<double> l_lua_math_ut;
	l_lua_math_ut.run();

	system("pause");


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

	vufLuaVector4<double>::registrator(L); 
	vufLuaMatrix4<double>::registrator(L);
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

