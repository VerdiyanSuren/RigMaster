#ifndef VF_MATH_QUATERNION_LUA_H
#define VF_MATH_QUATERNION_LUA_H

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
#include <vufQuaternion.h>
#include <vufVector.h>
#include <vufLuaStatic.h>

namespace vuf
{
	template <typename T>	class vufLuaVector4;	

	template <typename T>
	class vufLuaQuaternion
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaStatic::g_quat_tbl_name.c_str());
			VF_LUA_ADD_TABLE_FIELD(L, "new",					create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",					copy);

			VF_LUA_ADD_TABLE_FIELD(L, "set",					set);
			VF_LUA_ADD_TABLE_FIELD(L, "set_by_angle_and_axis",	set_by_angle);

			VF_LUA_ADD_TABLE_FIELD(L, "normalize",		normalize);
			VF_LUA_ADD_TABLE_FIELD(L, "conjugate",		conjugated);
			VF_LUA_ADD_TABLE_FIELD(L, "invert",			invert);
			
			VF_LUA_ADD_TABLE_FIELD(L, "length",		length);
			VF_LUA_ADD_TABLE_FIELD(L, "angle",		angle);
			VF_LUA_ADD_TABLE_FIELD(L, "axis",		axis);
			//VF_LUA_ADD_TABLE_FIELD(L, "rotate_vector",	rotate_vec);
			//VF_LUA_ADD_TABLE_FIELD(L, "slerp",			slerp);


			//VF_LUA_ADD_TABLE_FIELD(L, "get_parallel_to",quat_get_parallel_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "distance_to",	quat_distance_to);
			VF_LUA_ADD_TABLE_FIELD(L, "dot",			dot);
			//VF_LUA_ADD_TABLE_FIELD(L, "cross",			quat_cross);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_string",		quat_to_string);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_type",		quat_to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaStatic::g_quat_meta_name.c_str());
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc",		destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add",		add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub",		sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm",		unm);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul",		mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div",		quat_div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index",	index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex",new_index);
		}
		static bool	get_global(lua_State* L, const std::string& p_var_name, vufQuaternion<T>& p_res)
		{
			lua_getglobal(L, p_var_name.c_str());
			auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_quat_meta_name.c_str());
			if (l_q_ptr == nullptr)
			{
				return false;
			}
			p_res = l_q_ptr->get_data();
			return true;
		}
		// create as new user data
		static vufLuaQuaternionWrapper<T>* create_new_ref(lua_State* L, vufQuaternion<T>* p_ref_quat)
		{
			vufLuaQuaternionWrapper<T>* l_q_ptr = (vufLuaQuaternionWrapper<T>*)lua_newuserdata(L, sizeof(vufLuaQuaternionWrapper<T>));
			new (l_q_ptr) vufLuaQuaternionWrapper<T>(l_q_ptr, p_ref_quat);
			luaL_getmetatable(L, vufLuaStatic::g_quat_meta_name.c_str());
			lua_setmetatable(L, -2);
			return l_q_ptr;
		}


	private:
		static int create(lua_State* L)
		{
			void* l_quat_ptr = lua_newuserdata(L, sizeof(vufLuaQuaternionWrapper<T>));
			new (l_quat_ptr) vufLuaQuaternionWrapper<T>();
			luaL_getmetatable(L, vufLuaStatic::g_quat_meta_name.c_str());
			lua_setmetatable(L, -2);
			return 1;
		}
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

			auto* l_quat_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_quat_meta_name.c_str());
			if (l_quat_ptr != nullptr)
			{
				l_quat_ptr->get_data().set(l_x, l_y, l_z, l_w);
			}
			return 0;
		}
		VF_LUA_IMPLEMENT_COPY(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>);
		static int set_by_angle(lua_State* L)
		{			
			int l_number_of_arguments = lua_gettop(L);
			auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_quat_meta_name.c_str());
			if (l_q_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " got null object. expect quaternion");
			}
			auto l_v_ptr = (vufLuaVector4Wrapper<T>*)luaL_checkudata(L, -l_number_of_arguments + 2, vufLuaStatic::g_vec4_meta_name.c_str());
			if (l_v_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " got null object. expect vector");
			}
			if (lua_isnumber(L, -l_number_of_arguments + 1))
			{
				T l_angle = (T)lua_tonumber(L, -l_number_of_arguments + 1);
				l_q_ptr->get_data().set_by_angle_and_axis_in_place(l_angle, l_v_ptr->get_data());
				return 0;
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " got null object.");
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, normalize_in_place,	normalize);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, conjugate_in_place,	conjugated);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, invert_in_place,		invert);

		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, length,				length);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, get_angle,			angle);
		static int axis(lua_State* L)
		{
			auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -2, vufLuaStatic::g_quat_meta_name.c_str());
			if (l_q_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " got null object. ");
			}
			vufVector4<T> l_vec = l_q_ptr->get_data().get_axis_as_v4();
			return 0;
		}

		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, dot,				dot);
		
		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"quatMeta",	vufQuaternion<T>, get_parallel_to, vector4_get_parallel_to);
		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"quatMeta",	vufQuaternion<T>, get_cross, vector4_cross);

		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER("quatMeta", vufQuaternion<T>, distance_to, vector4_distance_to);
		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, unm);
		
		static int destroy(lua_State* L)
		{
			auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_quat_meta_name.c_str());
			//std::cout << "VECTOR DESTROY has to call destructor" << l_vector_ptr->has_to_destroy() <<  std::endl;
			//if (l_vector_ptr->has_to_destroy() == true)
			//{
			l_q_ptr->~vufLuaQuaternionWrapper<T>();
			return 0;
		}
		static int mul(lua_State* L)
		{
			//  doubhle *quaternion 
			if (lua_isnumber(L, -2))
			{
				auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_quat_meta_name.c_str());
				if (l_q_ptr == nullptr)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " got null object.");
				}
				T l_val = (T)lua_tonumber(L, -2);
				auto l_res_ptr = (vufLuaQuaternionWrapper<T>*)lua_newuserdata(L, sizeof(vufLuaQuaternionWrapper<T>));
				luaL_getmetatable(L, vufLuaStatic::g_quat_meta_name.c_str());
				lua_setmetatable(L, -2);
				l_res_ptr->set_data(l_q_ptr->get_data() * l_val);
				return 1;
			}
			// number * quaternion
			if (lua_isnumber(L, -1))
			{
				T l_val = (T)lua_tonumber(L, -1);
				auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -2, vufLuaStatic::g_quat_meta_name.c_str());
				if (l_q_ptr == nullptr)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " got null object.");
				}
				auto l_res_ptr = (vufLuaQuaternionWrapper<T>*)lua_newuserdata(L, sizeof(vufLuaQuaternionWrapper<T>));
				luaL_getmetatable(L, vufLuaStatic::g_quat_meta_name.c_str());
				lua_setmetatable(L, -2);
				l_res_ptr->set_data(l_q_ptr->get_data() * l_val);
				return 1;
			}
			// quaternion * quaternion
			if (lua_isuserdata(L, -1) && lua_isuserdata(L, -2))
			{
				auto l_q_1_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -2, vufLuaStatic::g_quat_meta_name.c_str());
				auto l_q_2_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_quat_meta_name.c_str());
				if (l_q_1_ptr != nullptr && l_q_2_ptr != nullptr)
				{
					auto l_res_ptr = (vufLuaQuaternionWrapper<T>*)lua_newuserdata(L, sizeof(vufLuaQuaternionWrapper<T>));
						luaL_getmetatable(L, vufLuaStatic::g_quat_meta_name.c_str());
						lua_setmetatable(L, -2);
						l_res_ptr->set_data( l_q_1_ptr->get_data() * l_q_2_ptr->get_data() );
						return 1;
				}
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " got null object.");
		}
		
		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_quat_meta_name.c_str());
			if (l_q_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " got null object.");
			}
			vufQuaternion<T>& l_q = l_q_ptr->get_data();
			if (lua_isinteger(L, -1))
			{
				int32_t l_index = (int32_t)lua_tonumber(L, -l_number_of_arguments + 1);
				if (l_index < 0 || l_index > 3)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " index is out of range.");
				}
				lua_pushnumber(L, l_q[l_index]);
				return 1;
			}

			const char* l_key = luaL_checkstring(L, -1);

			if (l_key == nullptr || l_q_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					lua_pushnumber(L, l_q.x);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					lua_pushnumber(L, l_q.y);
					return 1;
				}
				else if (l_key[0] == 'z')
				{
					lua_pushnumber(L, l_q.z);
					return 1;
				}
				else if (l_key[0] == 'w')
				{
					lua_pushnumber(L, l_q.w);
					return 1;
				}
			}
			lua_getglobal(L, vufLuaStatic::g_quat_tbl_name.c_str());
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
		static int new_index(lua_State* L)
		{
			//std::cout << "VECTOR NEW_INDEX" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			auto* l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_quat_meta_name.c_str());
			if (l_q_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " quaternion object is null.");
			}

			if (lua_isnumber(L, -l_number_of_arguments + 1) == 1)
			{
				int32_t l_index = (int32_t)lua_tonumber(L, -l_number_of_arguments + 1);
				if (l_index < 0 || l_index > 3)
				{
					//return luaL_error(L, "dfdsafdsfdsfds");
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " index is out of range.");
				}
				if (lua_isnumber(L, -l_number_of_arguments + 2) != 1)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " expect number.");
				}
				T l_val = (T)lua_tonumber(L, -l_number_of_arguments + 2);
				l_q_ptr->get_data()[l_index] = l_val;
				return 0;
			}

			const char* l_key = luaL_checkstring(L, -l_number_of_arguments + 1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " unknown key");
			}
			vufQuaternion<T>& l_q = l_q_ptr->get_data();
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					l_q.x = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'y')
				{
					l_q.y = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'z')
				{
					l_q.z = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'w')
				{
					l_q.w = (T)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " unknown key");
		}		
	};

}
#endif // !VF_MATH_QUATERNION_LUA_H
