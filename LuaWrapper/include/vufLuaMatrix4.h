#ifndef VF_MATH_MATRIX_4_LUA_H
#define VF_MATH_MATRIX_4_LUA_H


extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <vufLuaUserDataWrapper.h>
#include <new>
#include <cassert>
#include <vufLuaWrapUtil.h>
#include <vufLuaStatic.h>
#include <vufVector.h>
#include <vufMatrix.h>

namespace vuf
{
	template <typename T>
	class vufLuaMatrix4
	{
	public:
		static void registrator(lua_State* L )
		{
			VF_LUA_NEW_TABLE(L, vufLuaStatic::g_mat4_tbl_name.c_str());
			VF_LUA_ADD_TABLE_FIELD(L, "new",			create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",			copy);
			VF_LUA_ADD_TABLE_FIELD(L, "numerate",		numerate);
			VF_LUA_ADD_TABLE_FIELD(L, "get_transpose",	get_transpose);
			VF_LUA_ADD_TABLE_FIELD(L, "transpose",		transpose);
			VF_LUA_ADD_TABLE_FIELD(L, "get_transpose3",	get_transpose3);
			VF_LUA_ADD_TABLE_FIELD(L, "transpose3",		transpose3);
			VF_LUA_ADD_TABLE_FIELD(L, "determinant",	determinant);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string",		to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type",		to_type);
			//VF_LUA_ADD_TABLE_FIELD(L, "log", to_string);

			VF_LUA_NEW_META_TABLE(L, vufLuaStatic::g_mat4_meta_name.c_str());
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc",destroy);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index",	index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex",new_index);
		}
		static bool	get_global(lua_State* L, const std::string& p_var_name, vufMatrix4<T>& p_res)
		{
			lua_getglobal(L, p_var_name.c_str());
			auto v_ptr = (vufLuaMattrix4Wrapper<T>*)lua_touserdata(L, -1);
			if (v_ptr == nullptr)
			{
				return false;
			}
			p_res = v_ptr->get_data();
			return true;
		}
		static vufLuaMattrix4Wrapper<T>* create_new_ref(lua_State* L, vufMatrix4<T>* l_ref_matr)
		{
			vufLuaMattrix4Wrapper<T>* l_matr_ptr = (vufLuaMattrix4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaMattrix4Wrapper<T>));
			new (l_matr_ptr) vufLuaMattrix4Wrapper<T>(l_matr_ptr, l_ref_matr);
			luaL_getmetatable(L, g_metatable_name.c_str());
			lua_setmetatable(L, -2);
			return l_matr_ptr;
		}
	private:
		static int create(lua_State* L)
		{
			auto l_matr_wrap_ptr = (vufLuaMattrix4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaMattrix4Wrapper<T>));
			new (l_matr_wrap_ptr) vufLuaMattrix4Wrapper<T>();
			luaL_getmetatable(L, vufLuaStatic::g_mat4_meta_name.c_str());
			lua_setmetatable(L, -2);
			return 1;
		}
		VF_LUA_IMPLEMENT_COPY(vufLuaStatic::g_mat4_meta_name, vufLuaMattrix4Wrapper<T>);

		static int numerate(lua_State* L)
		{
			auto l_matr_wrap_ptr = (vufLuaMattrix4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaMattrix4Wrapper<T>));
			new (l_matr_wrap_ptr) vufLuaMattrix4Wrapper<T>();
			luaL_getmetatable(L, vufLuaStatic::g_mat4_meta_name.c_str());
			lua_setmetatable(L, -2);
			l_matr_wrap_ptr->set_data(vufMatrix4<T>::numerate_matrix());
			return 1;
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaStatic::g_mat4_meta_name, vufLuaMattrix4Wrapper<T>,	get_transposed_4,		get_transpose);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaStatic::g_mat4_meta_name, vufLuaMattrix4Wrapper<T>,	transpose_4_in_place,	transpose);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaStatic::g_mat4_meta_name, vufLuaMattrix4Wrapper<T>,	get_transposed_3,		get_transpose3);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaStatic::g_mat4_meta_name, vufLuaMattrix4Wrapper<T>,	transpose_3_in_place,	transpose3);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaStatic::g_mat4_meta_name, vufLuaMattrix4Wrapper<T>,	determinant,			determinant);

		static int to_string(lua_State* L)
		{
			auto l_matr_wrap_ptr = (vufLuaMattrix4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_mat4_meta_name.c_str());
			if (l_matr_wrap_ptr != nullptr)
			{
				auto l_info = l_matr_wrap_ptr->get_data().to_string();
				lua_pushstring(L, l_info.c_str());
				return 1;
			}
			return 0;
		}
		static int to_type(lua_State* L)
		{
			auto l_mat_ptr = (vufLuaMattrix4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_mat4_meta_name.c_str());
			if (l_mat_ptr != nullptr)
			{
				lua_pushstring(L, vufLuaStatic::g_mat4_tbl_name.c_str());
				return 1;
			}
			return 0;
		}


		static int destroy(lua_State* L)
		{
			auto l_vector_ptr = (vufLuaMattrix4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_mat4_meta_name.c_str());
			//std::cout << "VECTOR DESTROY has to call destructor" << l_vector_ptr->has_to_destroy() <<  std::endl;
			//if (l_vector_ptr->has_to_destroy() == true)
			//{
			l_vector_ptr->~vufLuaMattrix4Wrapper<T>();
			//}
			return 0;
		}
		static int index(lua_State* L)
		{
			// in lua a = m[0][0] 
			//std::cout << "MATR INDEX" << std::endl;
			int l_number_of_argiments = lua_gettop(L);
			auto l_matr_wrap_ptr = (vufLuaMattrix4Wrapper<T>*)luaL_checkudata(L, -2, vufLuaStatic::g_mat4_meta_name.c_str());
			if (l_matr_wrap_ptr == nullptr)
			{
				return luaL_error(L, "Failed to get matrix4 userdata");
			}
			// handle []
			if (lua_isinteger(L, -1) == 1)
			{
				int32_t l_index = (int32_t)lua_tointeger(L, -1);
				if (l_index < 0 || l_index > 3)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " index is out of range");
				}
				vufVector4<T>& l_row = l_matr_wrap_ptr->get_data().get_row(l_index);
				vufLuaVector4Wrapper<T>* l_res = vufLuaVector4<T>::create_new_ref(L, &l_row);
				return 1;
			}
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key != nullptr)
			{
				lua_getglobal(L, vufLuaStatic::g_mat4_tbl_name.c_str());
				lua_pushstring(L, l_key);
				lua_rawget(L, -2);
				return 1;
			}
			return luaL_error(L, "Unexpected matrix4 error");
		}
		static int new_index(lua_State* L)
		{
			std::cout << "MATR NEW_INDEX" << std::endl;
			// in lua m[0][0] = 5
			auto l_matr_wrap_ptr = (vufLuaMattrix4Wrapper<T>*)luaL_checkudata(L, -3, vufLuaStatic::g_mat4_meta_name.c_str());
			int l_index = (int)luaL_checkinteger(L, -2);
			if (l_index < 0 || l_index > 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " index is out of range");
			}
			vufVector4<T>& l_row = l_matr_wrap_ptr->get_data().get_row(l_index);
			vufLuaVector4Wrapper<T>* l_res = vufLuaVector4<T>::create_new_ref(L, &l_row);
			return 1;
		}
	};
}
#endif

