#ifndef VF_STD_VECTOR_ARRAY_H
#define VF_STD_VECTOR_ARRAY_H

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

#include <iostream>
#include <vector>

namespace vuf
{
	extern const char l_tbl[] = "arr";
	extern const char l_mtbl[] = "arrM";

	template <typename T, const char* TBL_NAME, const char* META_TBL_NAME >
	class vufLuaSTDVector
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, TBL_NAME);

			VF_LUA_ADD_TABLE_FIELD(L, "new", create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy", copy);

			VF_LUA_ADD_TABLE_FIELD(L, "set_length", set_length);
			VF_LUA_ADD_TABLE_FIELD(L, "length",		get_length);
			VF_LUA_ADD_TABLE_FIELD(L, "set", set);
			VF_LUA_ADD_TABLE_FIELD(L, "get", get);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_string", vector4_to_string);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_type", vector4_to_type);

			VF_LUA_NEW_META_TABLE(L, META_TBL_NAME);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", vector4_array_new_index);
		}
		/// bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)	
		VF_LUA_SET_USER_DATA_GLOBAL(std::string(META_TBL_NAME), std::vector<T>, vufLuaSTDVectorWrapper<T>);
		/// bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	
		VF_LUA_GET_USER_DATA_GLOBAL(META_TBL_NAME, std::vector<T>, vufLuaSTDVectorWrapper<T>);
		/// bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>& p_res_ptr)
		VF_LUA_GET_USER_DATA_PARAM(META_TBL_NAME, std::vector<T>, vufLuaSTDVectorWrapper<T>);
		/// create as reference of new user data and push to stack
		/// vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)
		VF_LUA_CREATE_USER_NEW_REF(META_TBL_NAME, std::vector<T>, vufLuaSTDVectorWrapper<T>);
		/// create mew user data and push to stack
		///  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)
		VF_LUA_CREATE_USER_DATA(META_TBL_NAME, std::vector<T>, vufLuaSTDVectorWrapper<T>);
	private:
		static int create(lua_State* L)
		{
			create_user_data(L);
			return 1;		
		}
		static int copy(lua_State* L)
		{
			auto l_data_ptr = (vufLuaSTDVectorWrapper<T>*)luaL_checkudata(L, -1, META_TBL_NAME);
			if (l_data_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " got null object.");
			}
				auto l_res_ptr = (vufLuaSTDVectorWrapper<T>*)lua_newuserdata(L, sizeof(vufLuaSTDVectorWrapper<T>));
				new (l_res_ptr) vufLuaSTDVectorWrapper<T>(std::vector<T>());
				luaL_getmetatable(L, META_TBL_NAME);
				lua_setmetatable(L, -2);
				l_res_ptr->set_data(l_data_ptr->get_data());
				return 1;
		}

		static int set_length(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				return luaL_error(L, "expect exactly  1 numerical  arguments");
			}
			bool l_status;
			int l_size;
			VF_LUA_READ_INTEGER(L, -1, l_size, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " expect integer as new length argument.");
			}
			std::vector<T>* l_res_ptr;
			if (get_param(L, -2, &l_res_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, "failed to get vector object.");
			}
			l_res_ptr->resize(l_size);
			return 0;
		}
		static int get_length(lua_State* L)
		{
			//std::cout << "GET LENGTH" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			std::vector<T>* l_res_ptr;
			if (get_param(L, -1, &l_res_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, "failed to get vector object.");
			}
			int l_size = (int)l_res_ptr->size();
			lua_pushnumber(L, l_size);
			return 1;
		}
		static int set(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 3)
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " wrong number of arguments. Expect 2");
			}
			bool l_status;
			//read i,j
			int l_i;
			double  l_val;
			// read and check value
			VF_LUA_READ_NUMBER(L, -1, l_val, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " value has to be number");
			}
			// read and check row
			VF_LUA_READ_INTEGER(L, -2, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " index has to be number");
			}
			// matrix
			std::vector<T>* l_vec_ptr;
			if (get_param(L, -3, &l_vec_ptr ) == false)
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " Failed tp get array onbject");
			}

			// check indecies
			if (l_i < 0 || l_i >= l_vec_ptr->size())
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " index is out of range");
			}
			(*l_vec_ptr)[l_i] = l_val;
			return 0;
		}
		static int get(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " wrong number of arguments. Expect 1");
			}
			bool l_status;
			//read i,j
			int l_i;
			// read and check row
			VF_LUA_READ_INTEGER(L, -1, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " index has to be number");
			}
			// read and check column
			// check indecies
			// matrix
			std::vector<T>* l_vec_ptr;
			if (get_param(L, -2, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " Failed tp get matrix4");
			}
			if (l_i < 0 || l_i >= l_vec_ptr->size())
			{
				VF_LUA_THROW_ERROR(L, META_TBL_NAME, " index of column is out of range");
			}
			T l_val = (*l_vec_ptr)[l_i];
			lua_pushnumber(L, l_val);
			return 1;
		}


		VF_LUA_IMPLEMENT_DESTROY(META_TBL_NAME, vufLuaSTDVectorWrapper<T>);
		/*
		static int destroy(lua_State* L)
		{
			std::cout << "DESTROY ARRAY" << std::endl;
			auto l_ptr = (vufLuaSTDVectorWrapper<T>*)luaL_checkudata(L, -1, META_TBL_NAME);
			l_ptr->~vufLuaSTDVectorWrapper<T>();
			return 0;
		}
		*/
		static int index(lua_State* L)
		{
			const char* l_key = luaL_checkstring(L, -1);
			lua_getglobal(L, TBL_NAME);
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}

	};
	
}
#endif // !VF_STD_VECTOR_ARRAY_H
