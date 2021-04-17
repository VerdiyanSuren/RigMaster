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
			VF_LUA_NEW_TABLE(L, "vector");
			VF_LUA_ADD_TABLE_FIELD(L, "new",				create);
			VF_LUA_ADD_TABLE_FIELD(L, "set",				vector4_set);
			VF_LUA_ADD_TABLE_FIELD(L, "length",				vector4_length);
			VF_LUA_ADD_TABLE_FIELD(L, "normalize",			vector4_normalize);
			VF_LUA_ADD_TABLE_FIELD(L, "ortho_to",			vector4_ortho_to);
			VF_LUA_ADD_TABLE_FIELD(L, "parallel_to",		vector4_get_parallel_to);
			VF_LUA_ADD_TABLE_FIELD(L, "distance_to",		vector4_distance_to);
			VF_LUA_ADD_TABLE_FIELD(L, "dot",				vector4_dot);
			VF_LUA_ADD_TABLE_FIELD(L, "cross",				vector4_cross);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string",			vector4_to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type",			vector4_to_type);

			VF_LUA_NEW_META_TABLE(L, "vector4Meta");
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc",			vector4_destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add",			vector4_add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub",			vector4_sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm",			vector4_unm);			
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul",			vector4_mul);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__div",			vector4_div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index",		vector4_index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex",	vector4_new_index);
		}
		static bool				get_vector_param(lua_State* L,int p_lua_index, vufVector4<T>& p_res_ptr)
		{
			auto l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, p_lua_index, "vector4Meta");
			if ( l_vector_ptr == nullptr)
			{
				return false;
			}
			p_res_ptr = *l_vector_ptr;
			return true;
		}
		// create as new user data
		static vufVector4<T>*	create_new(lua_State* L)
		{
			vufVector4<T>* l_vector_ptr = (vufVector4<T>*)lua_newuserdata(L, sizeof(vufVector4<T>));
			new (l_vector_ptr) vufVector4<T>();
			luaL_getmetatable(L, "vector4Meta");
			lua_setmetatable(L, -2);
			return l_vector_ptr;
		}
		// create as lightuser data
		static vufVector4<T>*	create_new_as_lightuser_data(lua_State* L)
		{

		}
	private:
		static int create(lua_State* L)
		{
			void* l_vector_ptr = lua_newuserdata(L, sizeof(vufVector4<T>));
			new (l_vector_ptr) vufVector4<T>();
			luaL_getmetatable(L, "vector4Meta");
			lua_setmetatable(L, -2);
			return 1;
		}
		static int vector4_set(lua_State* L)
		{
			int l_number_of_argiments = lua_gettop(L);
			if (l_number_of_argiments > 5)
			{
				return luaL_error(L, "expecting exactly  4 numerical  arguments");
			}
			T l_w = (T)luaL_checknumber(L, -1);
			T l_z = (T)luaL_checknumber(L, -2);
			T l_y = (T)luaL_checknumber(L, -3);
			T l_x = (T)luaL_checknumber(L, -4);

			auto* l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -l_number_of_argiments, "vector4Meta");
			if (l_vector_ptr != nullptr)
			{
				l_vector_ptr->set(l_x, l_y, l_z, l_w);
			}
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	"vector4Meta",	vufVector4<T>, normalize_in_place, vector4_normalize);

		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER("vector4Meta",	vufVector4<T>, length,			vector4_length);

		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"vector4Meta",	vufVector4<T>, get_ortho_to,	vector4_ortho_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"vector4Meta",	vufVector4<T>, get_parallel_to, vector4_get_parallel_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"vector4Meta",	vufVector4<T>, get_cross,		vector4_cross);

		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER("vector4Meta", vufVector4<T>, distance_to,		vector4_distance_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER("vector4Meta", vufVector4<T>, dot,				vector4_dot);
		
		static int vector4_to_string(lua_State* L)
		{
			auto* l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -1, "vector4Meta");
			if (l_vector_ptr != nullptr)
			{
				auto l_info = l_vector_ptr->to_string();
				lua_pushstring(L, l_info.c_str());
				return 1;
			}
			return 0;
		}
		static int vector4_to_type(lua_State* L)
		{
			auto* l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -1, "vector4Meta");
			if (l_vector_ptr != nullptr)
			{				
				lua_pushstring(L, "vector4");
				return 1;
			}
			return 0;
		}


		static int vector4_destroy(lua_State* L)
		{
			auto l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -1, "vector4Meta");
			l_vector_ptr->~vufVector4<T>();
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE("vector4Meta", vufVector4<T>, vector4_add);		
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE("vector4Meta", vufVector4<T>, vector4_sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE("vector4Meta", vufVector4<T>, vector4_unm);
		static int vector4_mul(lua_State* L)
		{
			//  vector * number
			if (lua_isnumber(L, -1))
			{
				T l_val = (T)lua_tonumber(L, -1);
				auto l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -2, "vector4Meta");
				if (l_vector_ptr == nullptr)	return 0;
				auto l_res_ptr = (vufVector4<T>*)lua_newuserdata(L, sizeof(vufVector4<T>));
				luaL_getmetatable(L, "vector4Meta");
				lua_setmetatable(L, -2);
				*l_res_ptr = (*l_vector_ptr)*l_val;
				return 1;
			}
			// To Do  vector * matrix
			
			// something * vector
			auto l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -1, "vector4Meta");
			if (l_vector_ptr == nullptr) return 0;
			// number * vector
			if ( lua_isnumber(L,-2))
			{
				T l_val = (T)lua_tonumber(L, -2);
				auto l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -2, "vector4Meta");
				if (l_vector_ptr == nullptr)	return 0;
				auto l_res_ptr = (vufVector4<T>*)lua_newuserdata(L, sizeof(vufVector4<T>));
				luaL_getmetatable(L, "vector4Meta");
				lua_setmetatable(L, -2);
				*l_res_ptr = (*l_vector_ptr) * l_val;
				return 1;
			}

			// To Do matrix * vector
			return 0;
		}
		static int vector4_div(lua_State* L)
		{
			if (lua_isnumber(L, -1))
			{
				T l_val = (T)lua_tonumber(L, -1);
				if (l_val == 0.)
				{
					return luaL_error(L, "Error dividing by zero");
				}
				auto l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -2, "vector4Meta");
				if (l_vector_ptr == nullptr)	return 0;
				auto l_res_ptr = (vufVector4<T>*)lua_newuserdata(L, sizeof(vufVector4<T>));
				luaL_getmetatable(L, "vector4Meta");
				lua_setmetatable(L, -2);
				*l_res_ptr = (*l_vector_ptr) * (1./l_val);
				return 1;
			}
			return luaL_error(L, "expecting  numerical  arguments");
		}
		static int vector4_index(lua_State* L)
		{
			int l_number_of_argiments = lua_gettop(L);
			const char* l_key = luaL_checkstring(L,-1); 
			auto* l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -2, "vector4Meta");
			
			if (l_key == nullptr || l_vector_ptr == nullptr)
			{
				return 0;
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] ==  'x')
				{
					lua_pushnumber(L, l_vector_ptr->x);
					return 1;
				}
				else if (l_key[0] == 'y') 
				{
					lua_pushnumber(L, l_vector_ptr->y);
					return 1;
				}
				else if (l_key[0] ==  'z')
				{
					lua_pushnumber(L, l_vector_ptr->z);
					return 1;
				}
				else if (l_key[0] == 'w')
				{
					lua_pushnumber(L, l_vector_ptr->w);
					return 1;
				}
			}
			lua_getglobal(L, "vector");
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
		static int vector4_new_index(lua_State* L)
		{
			//std::cout << "new_index()" << std::endl;
			//int l_number_of_argiments = lua_gettop(L);
			auto* l_vector_ptr = (vufVector4<T>*)luaL_checkudata(L, -3, "vector4Meta");
			const char* l_key = luaL_checkstring(L, -2); 
			if (l_key == nullptr || l_vector_ptr == nullptr)
			{
				return 0;
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					l_vector_ptr->x = (T)luaL_checknumber(L, -1);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					l_vector_ptr->y = (T)luaL_checknumber(L, -1);
					return 1;
				}
				else if (l_key[0] == 'z')
				{
					l_vector_ptr->z = (T)luaL_checknumber(L, -1);
					return 1;
				}
				else if (l_key[0] == 'w')
				{
					l_vector_ptr->w = (T)luaL_checknumber(L, -1);
					return 1;
				}
			}
			return 0;
		}
	};



#define VF_ARR_TABLE_NAME		"vectorArray"
#define VF_ARR_METATABLE_NAME	"vectorArrM"
#define VF_ARR_RESIZE			resize
#define VF_ARR_GET_SIZE			size()
#define VF_ARR_ELMNT_CREATE		vufLuaVector4<double>::create_new
#define VF_ARR_ELMNT_GET		vufLuaVector4<double>::get_vector_param

	template <typename ELL_TYPE, typename ARR_TYPE>
	class vufLuaVector4Array
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, VF_ARR_TABLE_NAME);
			VF_LUA_ADD_TABLE_FIELD(L, "new",		create);
			VF_LUA_ADD_TABLE_FIELD(L, "set_length", set_length);
			VF_LUA_ADD_TABLE_FIELD(L, "length",		get_length);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_string", vector4_to_string);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_type", vector4_to_type);

			VF_LUA_NEW_META_TABLE(L, VF_ARR_METATABLE_NAME);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", vector4_array_index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", vector4_array_new_index);
		}
	private:
		static int create(lua_State* L)
		{
std::cout << "CREATE ARRAY" << std::endl;
			void* l_vector_arr_ptr = lua_newuserdata(L, sizeof(ARR_TYPE));
			new (l_vector_arr_ptr) ARR_TYPE();
			luaL_getmetatable(L, VF_ARR_METATABLE_NAME);
			lua_setmetatable(L, -2);
			return 1;
		}
		static int set_length(lua_State* L)
		{
std::cout << "SET LENGTH" << std::endl;
			int l_number_of_argiments = lua_gettop(L);
			if (l_number_of_argiments != 2)
			{
				return luaL_error(L, "expect exactly  1 numerical  arguments");
			}
			auto l_new_size = (unsigned int)luaL_checkinteger(L, -1);
			auto* l_vector_arr_ptr = (ARR_TYPE*)luaL_checkudata(L, -l_number_of_argiments, VF_ARR_METATABLE_NAME);
			if (l_vector_arr_ptr != nullptr)
			{
				l_vector_arr_ptr->VF_ARR_RESIZE(l_new_size);
			}
			return 0;
		}
		static int get_length(lua_State* L)
		{
std::cout << "GET LENGTH" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			auto* l_vector_arr_ptr = (ARR_TYPE*)luaL_checkudata(L, -l_number_of_arguments, VF_ARR_METATABLE_NAME);
			if (l_vector_arr_ptr != nullptr)
			{
				auto l_length = (uint32_t)l_vector_arr_ptr->VF_ARR_GET_SIZE;
				lua_pushnumber(L, l_length);	/* set result of function*/
				return 1;
			}
			return 0;
		}
		static int destroy(lua_State* L)
		{
std::cout << "DESTROY" << std::endl;
			auto l_vector_arr_ptr = (ARR_TYPE*)luaL_checkudata(L, -1, VF_ARR_METATABLE_NAME);
			l_vector_arr_ptr->~ARR_TYPE();
			return 0;
		}
		static int vector4_array_index(lua_State* L)
		{
std::cout << "INDEX" << std::endl;
luaWrapper::dump_stack(L);
			auto* l_vector_arr_ptr = (ARR_TYPE*)luaL_checkudata(L, -2, VF_ARR_METATABLE_NAME);
			if (l_vector_arr_ptr == nullptr)
			{
				return luaL_error(L,"Failed to get array userdata");
			}
			if (lua_isnumber(L, -1) == 1)
			{
std::cout << "INDEX []" << std::endl;
				int32_t l_index = (int32_t)lua_tonumber(L, -1);
				if (l_index < 0 || l_index >= l_vector_arr_ptr->VF_ARR_GET_SIZE)
				{
					return luaL_error(L, "index out of range");
				}
				auto l_ptr = VF_ARR_ELMNT_CREATE(L);
				*l_ptr = l_vector_arr_ptr->operator[](l_index);
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

			auto* l_vector_arr_ptr = (ARR_TYPE*)luaL_checkudata(L, -3, VF_ARR_METATABLE_NAME);
			int l_index = (int)luaL_checkinteger(L, -2);
			
			if (l_index >0 && l_index < l_vector_arr_ptr->VF_ARR_GET_SIZE)
			{
				ELL_TYPE l_val;
				if (VF_ARR_ELMNT_GET(L, -1, l_val) == true)
				{
std::cout << "NEW INDEX	" << l_index << " " << l_val << std::endl;

					l_vector_arr_ptr->operator[](l_index) = l_val;
					return 1;
				}
			}
			return 0;
		}

	};


#undef VF_ARR_TABLE_NAME
#undef VF_ARR_METATABLE_NAME
#undef VF_ARR_RESIZE
#undef VF_ARR_GET_SIZE
#undef VF_ARR_ELMNT_CREATE
#undef VF_ARR_ELMNT_GET
}
#endif // !VF_MATH_VECTOR_LUA_H
