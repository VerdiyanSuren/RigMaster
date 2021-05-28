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
#include <math/vufVector.h>

#include <iostream>

namespace vuf
{
	template <typename T>
	class vufLuaVector4
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaStatic::g_vec4_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new",				create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",				copy);
			VF_LUA_ADD_TABLE_FIELD(L, "set",				set);
			VF_LUA_ADD_TABLE_FIELD(L, "length",				length);
			VF_LUA_ADD_TABLE_FIELD(L, "normalized",			normalize);
			VF_LUA_ADD_TABLE_FIELD(L, "ortho_to",			ortho_to);
			VF_LUA_ADD_TABLE_FIELD(L, "parallel_to",		parallel_to);
			VF_LUA_ADD_TABLE_FIELD(L, "distance_to",		distance_to);
			VF_LUA_ADD_TABLE_FIELD(L, "dot",				dot);
			VF_LUA_ADD_TABLE_FIELD(L, "cross",				cross);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string",			to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type",			to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaStatic::g_vec4_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc",			destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add",			add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub",			sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm",			unm);			
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul",			mul);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__div",			div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index",		index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex",	new_index);
		}
		/** bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_SET_USER_DATA_GLOBAL(vufLuaStatic::g_vec4_meta_name, vufMath::vufVector4<T>, vufLuaVector4Wrapper<T>);
		/*
		static bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)
		{
			auto l_ptr = (vufLuaVector4Wrapper<T>*)lua_newuserdata(L, sizeof(vufLuaVector4Wrapper<T>));
			new (l_ptr) vufLuaVector4Wrapper<T>(p_res);
			luaL_getmetatable(L, vufLuaStatic::g_vec4_meta_name.c_str());
			lua_setmetatable(L, -2);
			lua_setglobal(L, p_var_name.c_str());
			return true;
		}
		*/
		/** bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaStatic::g_vec4_meta_name, vufMath::vufVector4<T>,vufLuaVector4Wrapper<T>);
		/** bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>& p_res_ptr)*/
		VF_LUA_GET_USER_DATA_PARAM(vufLuaStatic::g_vec4_meta_name, vufMath::vufVector4<T>, vufLuaVector4Wrapper<T>);
		/** create as reference of new user data and push to stack
		* vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)	*/
		VF_LUA_CREATE_USER_NEW_REF(vufLuaStatic::g_vec4_meta_name, vufMath::vufVector4<T>, vufLuaVector4Wrapper<T>);
		/** create mew user data and push to stack 
		*  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)*/
		VF_LUA_CREATE_USER_DATA(vufLuaStatic::g_vec4_meta_name, vufMath::vufVector4<T>, vufLuaVector4Wrapper<T>);
	private:		
		static int create(lua_State* L)
		{
			create_user_data(L);
			return 1;
		} 
		VF_LUA_IMPLEMENT_COPY(vufLuaStatic::g_vec4_meta_name, vufVector4<T>,  vufLuaVector4Wrapper<T>);
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
			vufVector4<T>* l_vec_ptr;
			if (get_param(L, -l_number_of_arguments, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " Failed tp get vector4.");
			}
			l_vec_ptr->set(l_x, l_y, l_z, l_w);
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaStatic::g_vec4_meta_name, vufVector4<T>, vufLuaVector4Wrapper<T>, get_normalized,	normalize);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(vufLuaStatic::g_vec4_meta_name, vufVector4<T>, vufLuaVector4Wrapper<T>, get_ortho_to,		ortho_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(vufLuaStatic::g_vec4_meta_name, vufVector4<T>, vufLuaVector4Wrapper<T>, get_parallel_to,	parallel_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(vufLuaStatic::g_vec4_meta_name, vufVector4<T>, vufLuaVector4Wrapper<T>, get_cross,		cross);

		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, length,		length);

		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, distance_to,	distance_to);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(vufLuaStatic::g_vec4_meta_name, vufLuaVector4Wrapper<T>, dot,			dot);
		
		static int to_string(lua_State* L)
		{
			vufVector4<T>* l_vec_ptr;
			if (get_param(L, -1, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " Failed tp get vector4.");
			}
			lua_pushstring(L, l_vec_ptr->to_string().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaStatic::g_vec4_tbl_name);
			return 1;
		}

		static int destroy(lua_State* L)
		{
			auto l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_vec4_meta_name);
			//std::cout << "VECTOR DESTROY has to call destructor: " << l_vector_ptr->get_ref_count() <<  std::endl;
			//if (l_vector_ptr->has_to_destroy() == true)
			//{
			l_vector_ptr->~vufLuaVector4Wrapper<T>();
			//}
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaStatic::g_vec4_meta_name, vufVector4<T>, vufLuaVector4Wrapper<T>, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaStatic::g_vec4_meta_name, vufVector4<T>, vufLuaVector4Wrapper<T>, sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(vufLuaStatic::g_vec4_meta_name, vufVector4<T>, vufLuaVector4Wrapper<T>, unm);
		static int mul(lua_State* L)
		{
			// double * vector
			if ( lua_isnumber(L,-2))
			{
				vufVector4<T>* l_vec_ptr;
				if ( get_param( L, -1, &l_vec_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
				}
				T l_val = (T)lua_tonumber(L, -2);
				vufLuaVector4Wrapper<T>* l_res_ptr = create_user_data(L);
				l_res_ptr->set_data( (*l_vec_ptr) * l_val);
				return 1;
			}
			//  vector * number
			if (lua_isnumber(L, -1))
			{
				T l_val = (T)lua_tonumber(L, -1);
				vufVector4<T>* l_vec_ptr;
				if (get_param(L, -2, &l_vec_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
				}
				vufLuaVector4Wrapper<T>* l_res_ptr = create_user_data(L);
				l_res_ptr->set_data((*l_vec_ptr) * l_val);
				return 1;
			}
			// vector * matrix
			if (lua_isuserdata(L, -1))
			{
				vufVector4<T>* l_vec_ptr;
				vufMatrix4<T>* l_mat_ptr;
				if (get_param(L, -2, &l_vec_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object. Expect vector");
				}
				if (vufLuaMatrix4<T>::get_param(L, -1, &l_mat_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_mat4_tbl_name, " got null object. Expect matrix");
				}
				vufLuaVector4Wrapper<T>* l_res_ptr = create_user_data(L);
				l_res_ptr->set_data((*l_vec_ptr) * (*l_mat_ptr));
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
				vufVector4<T>* l_vec_ptr;
				if (get_param(L, -2, &l_vec_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null vector object.");
				}
				vufLuaVector4Wrapper<T>* l_res_ptr = create_user_data(L);
				l_res_ptr->set_data((*l_vec_ptr) * l_val);
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " expecting  numerical  argument.");
		}
		static int index(lua_State* L)
		{
			//std::cout << "VECTOR INDEX" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			vufVector4<T>* l_vec_ptr;
			if (get_param(L, -2, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " got null object.");
			}			
			const char* l_key = luaL_checkstring(L,-1); 
			
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_vec4_tbl_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] ==  'x')
				{
					lua_pushnumber(L, l_vec_ptr->x);
					return 1;
				}
				else if (l_key[0] == 'y') 
				{
					lua_pushnumber(L, l_vec_ptr->y);
					return 1;
				}
				else if (l_key[0] ==  'z')
				{
					lua_pushnumber(L, l_vec_ptr->z);
					return 1;
				}
				else if (l_key[0] == 'w')
				{
					lua_pushnumber(L, l_vec_ptr->w);
					return 1;
				}
			}
			lua_getglobal(L, vufLuaStatic::g_vec4_tbl_name);
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
			auto* l_vector_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_vec4_meta_name);
			if (l_vector_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L,  vufLuaStatic::g_vec4_tbl_name, " vector object is null.");
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
}
#endif // !VF_MATH_VECTOR_LUA_H
