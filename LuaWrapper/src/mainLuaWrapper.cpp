//#include <iostream>
#include <string>
#include <iostream>
#include <new>

#include <vufLog.h>
#include "vufLuaWrapper.h"
#include <vufLuaVector4.h>
#include <vufLuaMatrix4.h>
//#include <vufLuaQuaternion.h>
#include <vufLuaStatic.h>
#include <unitTest/vufLuaMathUT.h>
#include <vufLuaMemPool.h>


VF_LOG_DEFINE_STD_LOGGER();
VF_LUA_STATIC_INITIALIZE()
using namespace vuf;


std::string vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::g_table_name;
std::string vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::g_metatable_name;


//constexpr uint64_t			vufLuaChunk <std::vector<double>>::m_chunk_size = 2048;
void vec_init(void* p_ptr)
{
	//std::cout << "vec constructor" << std::endl;
	std::vector<double>* l_vec = (std::vector<double>*)p_ptr;
	new (l_vec) std::vector<double>();
}
void vec_clear(void* p_ptr)
{
	std::cout << "vec destructor" << std::endl;
	std::vector<double>* l_vec = (std::vector<double>*)p_ptr;
	l_vec->~vector();
}
std::function<void(void*)>	vufLuaMemPoolChunk <std::vector<double>>::m_init_function		= vec_init;
std::function<void(void*)>	vufLuaMemPoolChunk <std::vector<double>>::m_release_function	= vec_clear;


int main()
{
	vufLuaMemPoolChunk <std::vector<double>> l_chunk;
	for (int j = 0; j < 5; ++j)
	{
		std::vector<double>* l_vector = l_chunk.new_pointer();
		l_vector->resize(10);
		for (int i = 0; i < 10; ++i)
		{
			l_vector->operator[](i) = i;
		}
	}
	std::cout << "data size: " << sizeof(std::vector<double>) << std::endl;
	std::cout << "chunk counts: " << l_chunk.get_chunk_count() << std::endl;
	l_chunk.reset();
	for (int j = 0; j < 5; ++j)
	{
		std::vector<double>* l_vector = l_chunk.new_pointer();
		l_vector->resize(10);
		for (int i = 0; i < 10; ++i)
		{
			l_vector->operator[](i) = i;
		}
	}
	std::cout << "chunk counts: " << l_chunk.get_chunk_count() << std::endl;



	l_chunk.delete_chunks();
	system("pause");
	return 0;


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

	vufLuaVector4<double>::registrator(L); 
	vufLuaMatrix4<double>::registrator(L);
	vufLuaQuaternion<double>::registrator(L);
	//vufLuaVector4Array< vufVector4<double>, std::vector<vufVector4<double>>>::registrator(L, "vectorArray", "vectorArrM", l_vec_arr_store);
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
	
	vufVector4<double> l_vec;
	l_vec.x = 13;
	l_vec.y = -2;
	l_vec.z = 9;
	l_vec.w = 2;

	
	l_w.do_string(l_lua_str_3);
	vufLuaVector4<double>::set_global(l_w.get_lua_state(), "vctr", l_vec);
	l_w.do_string("print(vctr:to_string())");
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

