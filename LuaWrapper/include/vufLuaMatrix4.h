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
			VF_LUA_ADD_TABLE_FIELD(L, "set",			set);	// in place
			VF_LUA_ADD_TABLE_FIELD(L, "get",			get);
			VF_LUA_ADD_TABLE_FIELD(L, "set_row",		set_row);
			VF_LUA_ADD_TABLE_FIELD(L, "get_row",		get_row);
			VF_LUA_ADD_TABLE_FIELD(L, "numerate",		numerate);
			VF_LUA_ADD_TABLE_FIELD(L, "transpose",		transpose);
			VF_LUA_ADD_TABLE_FIELD(L, "transpose3",		transpose3);
			VF_LUA_ADD_TABLE_FIELD(L, "inverse",		inverse);
			VF_LUA_ADD_TABLE_FIELD(L, "determinant",	determinant);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string",		to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type",		to_type);
			//VF_LUA_ADD_TABLE_FIELD(L, "log", to_string);

			VF_LUA_NEW_META_TABLE(L, vufLuaStatic::g_mat4_meta_name.c_str());
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc",	destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index",	index);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex",new_index);
		}
		/** bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>);
		/** bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>& p_res_ptr)*/
		VF_LUA_GET_USER_DATA_PARAM(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>);
		/** create as reference of new user data and push to stack
		* vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)	*/
		VF_LUA_CREATE_USER_NEW_REF(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>);
		/** create mew user data and push to stack
		*  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)*/
		VF_LUA_CREATE_USER_DATA(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>);
	private:
		static int create(lua_State* L)
		{
			create_user_data(L);
			return 1;
		}
		VF_LUA_IMPLEMENT_COPY(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>,vufLuaMattrix4Wrapper<T>);
 		static int set(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 4)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " wrong number of arguments. Expect 3");
			}
			bool l_status;
			//read i,j
			int l_i, l_j;
			double  l_val;
			// read and check value
			VF_LUA_READ_NUMBER(L, -1, l_val, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " value has to be number");
			}
			// read and check row
			VF_LUA_READ_INTEGER(L, -2, l_j, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " row has to be number");
			}
			// read and check column
			VF_LUA_READ_INTEGER(L, -3, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " column has to be number");
			}
			// check indecies
			if (l_i < 0 || l_i >3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " index of column is out of range");
			}
			if (l_j < 0 || l_j >3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " index of row is out of range");
			}
			// matrix
			vufMatrix4<T>* l_matr_ptr;
			if (get_param(L, -4, &l_matr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " Failed tp get matrix4");
			}
			(*l_matr_ptr)[l_i][l_j] = l_val;
			return 0;
		}
		static int get(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " wrong number of arguments. Expect 2");
			}
			bool l_status;
			//read i,j
			int l_i, l_j;
			// read and check row
			VF_LUA_READ_INTEGER(L, -1, l_j, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " row has to be number");
			}
			// read and check column
			VF_LUA_READ_INTEGER(L, -2, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " column has to be number");
			}
			// check indecies
			if (l_i < 0 || l_i >3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " index of column is out of range");
			}
			if (l_j < 0 || l_j >3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " index of row is out of range");
			}
			// matrix
			vufMatrix4<T>* l_matr_ptr;
			if (get_param(L, -3, &l_matr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " Failed tp get matrix4");
			}
			T l_val = (*l_matr_ptr)[l_i][l_j];
			lua_pushnumber(L, l_val);
			return 1;
		}
		static int set_row(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " wrong number of arguments. Expect 2");
			}
			bool l_status;
			//read i,j
			int l_i;
			// read and check vector value
			vufVector4<T> *l_vec;
			if (vufLuaVector4<T>::get_param(L, -1, &l_vec) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " value has to be vector4");
			}
			// read and check row
			VF_LUA_READ_INTEGER(L, -2, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " row has to be number");
			}
			// check indecies
			if (l_i < 0 || l_i >3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " index of column is out of range");
			}
			
			// matrix
			vufMatrix4<T>* l_matr_ptr;
			if (get_param(L, -3, &l_matr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " Failed tp get matrix4");
			}
			l_matr_ptr->set_row( l_i, *l_vec);
			return 0;
		}
		static int get_row(lua_State* L)
		{
			bool l_status;
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " wrong number of arguments. Expect 1");
			}
			// read and check row
			int l_i;
			VF_LUA_READ_INTEGER(L, -1, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " row has to be number");
			}
			// matrix
			vufMatrix4<T>* l_matr_ptr;
			if (get_param(L, -2, &l_matr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " Failed tp get matrix4");
			}
			auto l_vec = l_matr_ptr->get_row( l_i );
			vufLuaVector4Wrapper<T>* l_wrap = vufLuaVector4<T>::create_user_data(L);
			l_wrap->set_data(l_vec);
			return 1;
		}
		static int numerate(lua_State* L)
		{
			auto l_matr_wrap_ptr = create_user_data(L);
			l_matr_wrap_ptr->set_data(vufMatrix4<T>::numerate_matrix());
			return 1;
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>,	get_transposed_4,	transpose);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>,	get_transposed_3,	transpose3);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>,	inverse,			inverse);

		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaStatic::g_mat4_meta_name, vufLuaMattrix4Wrapper<T>,	determinant,	determinant);

		static int to_string(lua_State* L)
		{
			vufMatrix4<T>* l_matr_ptr;
			if (get_param(L, -1, &l_matr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " Failed tp get matrix4.");
			}
			lua_pushstring(L, l_matr_ptr->to_string().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaStatic::g_mat4_tbl_name.c_str());
			return 1;
		}

		static int destroy(lua_State* L)
		{
			auto l_ptr = (vufLuaMattrix4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_mat4_meta_name.c_str());
			//std::cout << "MARIX DESTROY has to call destructor" << l_ptr->get_ref_count() <<  std::endl;
			l_ptr->~vufLuaMattrix4Wrapper<T>();
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>, sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>, unm);
		VF_LUA_IMPLEMENT_TYPE_MUL_TYPE(vufLuaStatic::g_mat4_meta_name, vufMatrix4<T>, vufLuaMattrix4Wrapper<T>,  mul);

		static int index(lua_State* L)
		{
			//std::cout << "MATR INDEX" << std::endl;
			// in lua m[0][0] rvalue or lvalue
			int l_number_of_arguments = lua_gettop(L);
			vufMatrix4<T>* l_matr_ptr;
			if (get_param( L, -2,  &l_matr_ptr) == false )
			{
				return luaL_error(L, "Failed to get matrix4 userdata");
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
			// in lua m[0] = v
			auto l_matr_wrap_ptr = (vufLuaMattrix4Wrapper<T>*)luaL_checkudata(L, -3, vufLuaStatic::g_mat4_meta_name.c_str());
			int l_index = (int)luaL_checkinteger(L, -2);
			if (l_index < 0 || l_index > 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " index is out of range");
			}
			vufVector4<T>* l_row_vec; 
			if (vufLuaVector4<T>::get_param(L, -1, &l_row_vec) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " Failed tp get vector4 as first argument");
			}
			l_matr_wrap_ptr->get_data().set_row(l_index, *l_row_vec);
			//vufLuaVector4Wrapper<T>* l_res = vufLuaVector4<T>::create_new_ref(L, &l_row);
			return 0;
		}
	};
}

#endif

