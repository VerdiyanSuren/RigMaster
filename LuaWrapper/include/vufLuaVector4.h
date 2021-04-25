#ifndef VF_MATH_VECTOR_4_LUA_H
#define VF_MATH_VECTOR_4_LUA_H

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <new>
#include <cassert>
#include <vufLuaWrapUtil.h>
#include <vufLuaUserDataWrapper.h>
#include <vufLuaStatic.h>
#include <vufVector.h>

#include <iostream>

namespace vuf
{
	template <typename T>
	class vufLuaVector4
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaStatic::g_vec4_tbl_name.c_str());
			VF_LUA_ADD_TABLE_FIELD(L, "new",				create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",				copy);
			VF_LUA_ADD_TABLE_FIELD(L, "set",				set);
			VF_LUA_ADD_TABLE_FIELD(L, "length",				length);
			VF_LUA_ADD_TABLE_FIELD(L, "normalize",			normalize);
			VF_LUA_ADD_TABLE_FIELD(L, "ortho_to",			ortho_to);
			VF_LUA_ADD_TABLE_FIELD(L, "parallel_to",		parallel_to);
			VF_LUA_ADD_TABLE_FIELD(L, "distance_to",		distance_to);
			VF_LUA_ADD_TABLE_FIELD(L, "dot",				dot);
			VF_LUA_ADD_TABLE_FIELD(L, "cross",				cross);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string",			to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type",			to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaStatic::g_vec4_meta_name.c_str());
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc",			destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add",			add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub",			sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm",			unm);			
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul",			mul);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__div",			div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index",		index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex",	new_index);
		}
		
		static bool	get_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)
		{
			lua_getglobal(L, p_var_name.c_str());
			auto l_vec_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_vec4_meta_name.c_str());
			if (l_vec_ptr == nullptr)
			{
				return false;
			}
			p_res = l_vec_ptr->get_data();
			return true;
		}
		static bool	get_param(lua_State* L,int p_lua_index, vufVector4<T>& p_res_ptr)
		{
			auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, p_lua_index, vufLuaStatic::g_vec4_meta_name.c_str());
			if ( l_vector_ptr == nullptr)
			{
				return false;
			}
			p_res_ptr = l_vector_ptr->get_data();
			return true;
		}
		// create as new user data
		static vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)
		{
			vufLuaVector4Wrapper<T>* l_vector_ptr = (vufLuaVector4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaVector4Wrapper<T>));
			new (l_vector_ptr) vufLuaVector4Wrapper<T>(l_vector_ptr, l_ref_vector);
			luaL_getmetatable(L, vufLuaStatic::g_vec4_meta_name.c_str());
			lua_setmetatable(L, -2);
			return l_vector_ptr;
		}
	private:		
		static int create(lua_State* L)
		{
			void* l_vector_ptr = lua_newuserdata(L, sizeof(vufLuaVector4Wrapper<T>));
			new (l_vector_ptr) vufLuaVector4Wrapper<T>();
			luaL_getmetatable(L, vufLuaStatic::g_vec4_meta_name.c_str());
			lua_setmetatable(L, -2);
			return 1;
		}
		/*
		static int copy(lua_State* L)
		{
			auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_vec4_meta_name.c_str());
			if (l_vector_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
			}
			vufLuaVector4Wrapper<T>* l_res_ptr = (vufLuaVector4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaVector4Wrapper<T>));
			new (l_res_ptr) vufLuaVector4Wrapper<T>();
			luaL_getmetatable(L, vufLuaStatic::g_vec4_meta_name.c_str());
			lua_setmetatable(L, -2);
			l_res_ptr->set_data(l_vector_ptr->get_data());
			return 1;
		}
		*/
		VF_LUA_IMPLEMENT_COPY(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>);
		static int set(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			T l_x = .0, l_y = .0, l_z = .0, l_w = 1.;
			if (l_number_of_arguments > 1)
			{
				l_x = (T)luaL_checknumber(L, -l_number_of_arguments + 1);
			}
			if (l_number_of_arguments > 2)
			{
				l_y = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
			}
			if (l_number_of_arguments > 3)
			{
				l_z = (T)luaL_checknumber(L, -l_number_of_arguments + 3);
			}
			if (l_number_of_arguments > 4)
			{
				l_w = (T)luaL_checknumber(L, -l_number_of_arguments + 4);
			}

			auto* l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_vec4_meta_name.c_str());
			if (l_vector_ptr != nullptr)
			{
				l_vector_ptr->get_data().set(l_x, l_y, l_z, l_w);
				return 0;
			}
			VF_LUA_THROW_ERROR( L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, normalize_in_place,	normalize);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, get_ortho_to,			ortho_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, get_parallel_to,		parallel_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, get_cross,			cross);

		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, length,				length);

		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, distance_to,		distance_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, dot,				dot);
		
		static int to_string(lua_State* L)
		{
			auto* l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_vec4_meta_name.c_str());
			if (l_vector_ptr != nullptr)
			{
				auto l_info = l_vector_ptr->get_data().to_string();
				lua_pushstring(L, l_info.c_str());
				return 1;
			}
			return 0;
		}
		static int to_type(lua_State* L)
		{
			auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_vec4_meta_name.c_str());
			if (l_vector_ptr != nullptr)
			{				
				lua_pushstring(L, vufLuaStatic::g_vec4_tbl_name.c_str());
				return 1;
			}
			return 0;
		}


		static int destroy(lua_State* L)
		{
			auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_vec4_meta_name.c_str());
			//std::cout << "VECTOR DESTROY has to call destructor" << l_vector_ptr->has_to_destroy() <<  std::endl;
			//if (l_vector_ptr->has_to_destroy() == true)
			//{
			l_vector_ptr->~vufLuaVector4Wrapper<T>();
			//}
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, unm);
		static int mul(lua_State* L)
		{
			// double * vector
			if ( lua_isnumber(L,-2))
			{
				auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_vec4_meta_name.c_str());
				if (l_vector_ptr == nullptr)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
				}
				T l_val = (T)lua_tonumber(L, -2);				
				auto l_res_ptr = (vufLuaVector4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaVector4Wrapper<T>));
				luaL_getmetatable(L, vufLuaStatic::g_vec4_meta_name.c_str());
				lua_setmetatable(L, -2);
				l_res_ptr->set_data(l_vector_ptr->get_data() * l_val);
				return 1;
			}
			//  vector * number
			if (lua_isnumber(L, -1))
			{
				T l_val = (T)lua_tonumber(L, -1);
				auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -2, vufLuaStatic::g_vec4_meta_name.c_str());
				if (l_vector_ptr == nullptr)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
				}
				auto l_res_ptr = (vufLuaVector4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaVector4Wrapper<T>));
				luaL_getmetatable(L, vufLuaStatic::g_vec4_meta_name.c_str());
				lua_setmetatable(L, -2);
				l_res_ptr->set_data(l_vector_ptr->get_data() * l_val);
				return 1;
			}
			// vector * matrix
			if (lua_isuserdata(L, -1))
			{
				auto l_vector_ptr	= (vufLuaVector4Wrapper<T>*)luaL_checkudata(L,  -2, vufLuaStatic::g_vec4_meta_name.c_str());
				auto l_matr_ptr		= (vufLuaMattrix4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_mat4_meta_name.c_str());
				if (l_vector_ptr == nullptr)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
				}
				if (l_matr_ptr == nullptr)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " got null object.");
				}
				auto l_res_ptr = (vufLuaVector4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaVector4Wrapper<T>));
				luaL_getmetatable(L, vufLuaStatic::g_vec4_meta_name.c_str());
				lua_setmetatable(L, -2);
				l_res_ptr->set_data(l_vector_ptr->get_data() * l_matr_ptr->get_data());
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
		}
		static int div(lua_State* L)
		{
			if (lua_isnumber(L, -1))
			{
				T l_val = (T)lua_tonumber(L, -1);
				if (l_val == 0.)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " Error dividing by zero.");
				}
				l_val = 1. / l_val;
				auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -2, vufLuaStatic::g_vec4_meta_name.c_str());
				if (l_vector_ptr == nullptr)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null vector object.");
				}
				auto l_res_ptr = (vufLuaVector4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaVector4Wrapper<T>));
				luaL_getmetatable(L, vufLuaStatic::g_vec4_meta_name.c_str());
				lua_setmetatable(L, -2);
				l_res_ptr->set_data(l_vector_ptr->get_data() * l_val);
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " expecting  numerical  argument.");
		}
		static int index(lua_State* L)
		{
			//std::cout << "VECTOR INDEX" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_vec4_meta_name.c_str());
			if (l_vector_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
			}
			vufVector4<T>& l_vec = l_vector_ptr->get_data();
			if (lua_isinteger(L,-1))
			{
				int32_t l_index = (int32_t)lua_tonumber(L, -l_number_of_arguments + 1);
				if (l_index < 0 || l_index > 3)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " index is out of range.");
				}
				lua_pushnumber(L, l_vec[l_index]);
				return 1;
			}
			
			const char* l_key = luaL_checkstring(L,-1); 
			
			if (l_key == nullptr || l_vector_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] ==  'x')
				{
					lua_pushnumber(L, l_vec.x);
					return 1;
				}
				else if (l_key[0] == 'y') 
				{
					lua_pushnumber(L, l_vec.y);
					return 1;
				}
				else if (l_key[0] ==  'z')
				{
					lua_pushnumber(L, l_vec.z);
					return 1;
				}
				else if (l_key[0] == 'w')
				{
					lua_pushnumber(L, l_vec.w);
					return 1;
				}
			}
			lua_getglobal(L, vufLuaStatic::g_vec4_tbl_name.c_str());
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
		static int new_index(lua_State* L)
		{
			//std::cout << "VECTOR NEW_INDEX" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			// in lua a[0] = 5
			// a.x = 5
			auto* l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_vec4_meta_name.c_str());
			if (l_vector_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " vector object is null.");
			}

			if (lua_isnumber(L, -l_number_of_arguments + 1) == 1)
			{
				int32_t l_index = (int32_t)lua_tonumber(L, -l_number_of_arguments + 1);
				if (l_index < 0 || l_index > 3)
				{
					//return luaL_error(L, "dfdsafdsfdsfds");
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " index is out of range.");
				}
				if (lua_isnumber(L, -l_number_of_arguments + 2) != 1)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " expect number.");
				}
				T l_val = (T)lua_tonumber(L, -l_number_of_arguments + 2);
				l_vector_ptr->get_data()[l_index] = l_val;
				return 0;
			}
			
			const char* l_key = luaL_checkstring(L, -l_number_of_arguments + 1);
			if ( l_key == nullptr )
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " unknown key");
			}
			vufVector4<T>& l_vec = l_vector_ptr->get_data();
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					l_vec.x = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'y')
				{
					l_vec.y = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'z')
				{
					l_vec.z = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'w')
				{
					l_vec.w = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " unknown key");
		}
	};



#define VF_ARR_TABLE_NAME		"vectorArray"
#define VF_ARR_METATABLE_NAME	"vectorArrM"
#define VF_ARR_RESIZE			resize
#define VF_ARR_GET_SIZE			size()
#define VF_ARR_ELMNT_CREATE		vufLuaVector4<double>::create_new_ref
#define VF_ARR_ELMNT_GET		vufLuaVector4<double>::get_param

	template <typename ELL_TYPE, typename ARR_TYPE>
	class vufLuaVector4Array
	{
	public:
		static void registrator(lua_State* L, const std::string& p_table_name, const std::string& p_matatable_name, vufLuaSessionDataStore<ARR_TYPE>& p_store)
		{
			g_table_name		= p_table_name;
			g_metatable_name	= p_matatable_name;

			VF_LUA_NEW_TABLE(L, p_table_name.c_str());
			
			lua_pushlightuserdata(L, &p_store);
			lua_pushcclosure(L, create, 1);
			lua_setfield(L, -2, "new");
			
			//VF_LUA_ADD_TABLE_FIELD(L, "new",		create);
			VF_LUA_ADD_TABLE_FIELD(L, "set_length", set_length);
			VF_LUA_ADD_TABLE_FIELD(L, "length",		get_length);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_string", vector4_to_string);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_type", vector4_to_type);

			VF_LUA_NEW_META_TABLE(L, g_metatable_name.c_str());
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", vector4_array_index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", vector4_array_new_index);
		}
	private:
		static int create(lua_State* L)
		{
//std::cout << "CREATE ARRAY" << std::endl;
			vufLuaSessionDataStore<ARR_TYPE>* l_store_ptr = (vufLuaSessionDataStore<ARR_TYPE>*)lua_touserdata(L, lua_upvalueindex(1));
			l_store_ptr->log();
			auto l_arr_ptr = (ARR_TYPE**)lua_newuserdata(L, sizeof(ARR_TYPE*));
			*l_arr_ptr = l_store_ptr->get_new();
			luaL_getmetatable(L, g_metatable_name.c_str());
			lua_setmetatable(L, -2);
			return 1;
		}
		static int set_length(lua_State* L)
		{
//std::cout << "SET LENGTH" << std::endl;
			int l_number_of_argiments = lua_gettop(L);
			if (l_number_of_argiments != 2)
			{
				return luaL_error(L, "expect exactly  1 numerical  arguments");
			}
			auto l_new_size = (unsigned int)luaL_checkinteger(L, -1);
			auto l_arr_ptr = (ARR_TYPE**)luaL_checkudata(L, -l_number_of_argiments, VF_ARR_METATABLE_NAME);
			if (l_arr_ptr != nullptr)
			{
				(*l_arr_ptr)->VF_ARR_RESIZE(l_new_size);
			}
			return 0;
		}
		static int get_length(lua_State* L)
		{
//std::cout << "GET LENGTH" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			auto l_arr_ptr = (ARR_TYPE**)luaL_checkudata(L, -l_number_of_arguments, VF_ARR_METATABLE_NAME);
			if (l_arr_ptr != nullptr)
			{
				auto l_length = (uint32_t)(*l_arr_ptr)->VF_ARR_GET_SIZE;
				lua_pushnumber(L, l_length);	
				return 1;
			}
			return 0;
		}
		static int destroy(lua_State* L)
		{
//std::cout << "DESTROY" << std::endl;
			//auto l_vector_arr_ptr = (ARR_TYPE*)luaL_checkudata(L, -1, VF_ARR_METATABLE_NAME);
			//l_vector_arr_ptr->~ARR_TYPE();
			return 0;
		}
		static int vector4_array_index(lua_State* L)
		{
std::cout << "INDEX" << std::endl;
vufLuaWrapper::dump_stack(L);
			auto l_arr_ptr = (ARR_TYPE**)luaL_checkudata(L, -2, VF_ARR_METATABLE_NAME);
			if (l_arr_ptr == nullptr)
			{
				return luaL_error(L,"Failed to get array userdata");
			}
			if (lua_isnumber(L, -1) == 1)
			{
std::cout << "INDEX []" << std::endl;
				int32_t l_index = (int32_t)lua_tonumber(L, -1);
				if (l_index < 0 || l_index >= (*l_arr_ptr)->VF_ARR_GET_SIZE)
				{
					return luaL_error(L, "index out of range");
				}
				VF_ARR_ELMNT_CREATE(L,&(*l_arr_ptr)->operator[](l_index));
				return 1;
			}
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key != nullptr )
			{
std::cout << "INDEX KEY" << std::endl;

				lua_getglobal(L, VF_ARR_TABLE_NAME);
				lua_pushstring(L, l_key);
				lua_rawget(L, -2);
				return 1;
			}
			return luaL_error(L,"Unexpected error");
		}
		static int vector4_array_new_index(lua_State* L)
		{
std::cout << "NEW INDEX" << std::endl;

			//std::cout << "new_index()" << std::endl;
			//int l_number_of_argiments = lua_gettop(L);

			auto l_arr_ptr = (ARR_TYPE**)luaL_checkudata(L, -3, VF_ARR_METATABLE_NAME);
			int l_index = (int)luaL_checkinteger(L, -2);
			
			if (l_index >0 && l_index < (*l_arr_ptr)->VF_ARR_GET_SIZE)
			{
				ELL_TYPE l_val;
				if (VF_ARR_ELMNT_GET(L, -1, l_val) == true)
				{
std::cout << "NEW INDEX	" << l_index << " " << l_val << std::endl;

					(*l_arr_ptr)->operator[](l_index) = l_val;
					return 1;
				}
			}
			return 0;
		}
		
		static std::string g_table_name;
		static std::string g_metatable_name;

	};


#undef VF_ARR_TABLE_NAME
#undef VF_ARR_METATABLE_NAME
#undef VF_ARR_RESIZE
#undef VF_ARR_GET_SIZE
#undef VF_ARR_ELMNT_CREATE
#undef VF_ARR_ELMNT_GET
}
#endif // !VF_MATH_VECTOR_LUA_H
