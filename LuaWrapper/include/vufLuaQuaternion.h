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
/**
* Lua Examples
* q_1 = quat.new()								-- create quaternion
* q_1 = q_2										-- both point to the same lua cell
* q_2 = q_1:copy()								-- both point to different cells
* q_1:set( 0.2, .0.3, 0.4, 1)					-- set q_1 value
* q_1 = quat:by_axis_and_angle(vec4:new(),60)	-- create quaternion by axis(vecotor4) and angle in degree
*/
namespace vuf
{
	template <typename T>	class vufLuaVector4;	

	template <typename T>
	class vufLuaQuaternion
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaStatic::g_quat_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new",				create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",				copy);

			VF_LUA_ADD_TABLE_FIELD(L, "set",				set);
			VF_LUA_ADD_TABLE_FIELD(L, "by_axis_and_angle",	by_angle);

			VF_LUA_ADD_TABLE_FIELD(L, "normalized",	normalize);
			VF_LUA_ADD_TABLE_FIELD(L, "conjugated",	conjugated);
			VF_LUA_ADD_TABLE_FIELD(L, "inverted",		invert);
			
			VF_LUA_ADD_TABLE_FIELD(L, "length",		length);
			VF_LUA_ADD_TABLE_FIELD(L, "angle",		angle);
			VF_LUA_ADD_TABLE_FIELD(L, "axis",		axis);
			//VF_LUA_ADD_TABLE_FIELD(L, "rotate_vector",	rotate_vec);
			//VF_LUA_ADD_TABLE_FIELD(L, "slerp",			slerp);


			//VF_LUA_ADD_TABLE_FIELD(L, "get_parallel_to",quat_get_parallel_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "distance_to",	quat_distance_to);
			VF_LUA_ADD_TABLE_FIELD(L, "dot",			dot);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string",		to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type",		to_type);  

			VF_LUA_NEW_META_TABLE(L, vufLuaStatic::g_quat_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc",		destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add",		add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub",		sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm",		unm);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul",		mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div",		quat_div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index",	index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex",new_index);
		}
		/** bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_SET_USER_DATA_GLOBAL(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>);
		/** bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>);
		/** bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>& p_res_ptr)*/
		VF_LUA_GET_USER_DATA_PARAM(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>);
		/** create as reference of new user data and push to stack
		* vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)	*/
		VF_LUA_CREATE_USER_NEW_REF(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>);
		/** create mew user data and push to stack
		*  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)*/
		VF_LUA_CREATE_USER_DATA(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>);
	private:
		static int create(lua_State* L)
		{
			create_user_data(L);
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
			vufQuaternion<T>* l_q_ptr;
			if (get_param(L, -l_number_of_arguments, &l_q_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " Failed tp get quaternion.");
			}
			l_q_ptr->set(l_x, l_y, l_z, l_w);
			return 0;
		}
		VF_LUA_IMPLEMENT_COPY(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>);
		static int by_angle(lua_State* L)
		{			
			int l_number_of_arguments = lua_gettop(L);
			vufVector4<T> *l_vec;
			if (vufLuaVector4<T>::get_param(L, -2, &l_vec) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " Expect vec4 as first param.");
			}
			if (lua_isnumber(L, -1))
			{
				T l_angle = (T)lua_tonumber(L, -1);
				vufQuaternion<T> l_q;
				l_q = vufQuaternion<T>::set_by_angle_and_axis(l_angle, *l_vec);
				auto* l_wrapper = create_user_data(L);
				l_wrapper->set_data(l_q);
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " got null object.");
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>, get_normalized,	normalize);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>, get_conjugated,	conjugated);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>, get_inverted,		invert);

		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, length,				length);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, get_angle,			angle);
		static int axis(lua_State* L)
		{
			vufQuaternion<T>* l_q;
			if (get_param( L, -1, &l_q ) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " got null object. ");
			}
			auto l_vec_wrapper = vufLuaVector4<T>::create_user_data(L);
			l_vec_wrapper->set_data(l_q->get_axis_as_v4());
			return 1;
		}

		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(vufLuaStatic::g_quat_meta_name, vufLuaQuaternionWrapper<T>, dot,				dot);
		
		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"quatMeta",	vufQuaternion<T>, get_parallel_to, vector4_get_parallel_to);
		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"quatMeta",	vufQuaternion<T>, get_cross, vector4_cross);

		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER("quatMeta", vufQuaternion<T>, distance_to, vector4_distance_to);
		static int to_string(lua_State* L)
		{
			vufQuaternion<T>* l_q_ptr;
			if (get_param(L, -1, &l_q_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " Failed tp get vector4.");
			}
			lua_pushstring(L, l_q_ptr->to_string().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaStatic::g_quat_tbl_name);
			return 1;
		}

		static int destroy(lua_State* L)
		{
			auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -1, vufLuaStatic::g_quat_meta_name);
			l_q_ptr->~vufLuaQuaternionWrapper<T>();
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>, sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(vufLuaStatic::g_quat_meta_name, vufQuaternion<T>, vufLuaQuaternionWrapper<T>, unm);
		static int mul(lua_State* L)
		{
			//  doubhle *quaternion 
			vufQuaternion<T>* l_q_ptr;
			if (lua_isnumber(L, -2))
			{
				if (get_param(L, -1, &l_q_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " got null object.");
				}
				T l_val = (T)lua_tonumber(L, -2);

				auto l_res_ptr = create_user_data(L);
				l_res_ptr->set_data( (*l_q_ptr) * l_val);
				return 1;
			}
			// number * quaternion
			if (lua_isnumber(L, -1))
			{
				T l_val = (T)lua_tonumber(L, -1);
				if (get_param(L, -2, &l_q_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " got null object.");
				}
				auto l_res_ptr = create_user_data(L);
				l_res_ptr->set_data((*l_q_ptr) * l_val);
				return 1;
			}
			// quaternion * quaternion
			if (lua_isuserdata(L, -1) && lua_isuserdata(L, -2))
			{
				vufQuaternion<T>* l_q_2_ptr;
				if (get_param(L, -2, &l_q_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " got null object.");
				}
				if (get_param(L, -1, &l_q_2_ptr) == false)
				{
					VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " got null object.");
				}
				auto l_res_ptr = create_user_data(L);
				l_res_ptr->set_data((*l_q_ptr) * (*l_q_2_ptr));
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_meta_name, " got null object.");
		}
		
		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			
			auto l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_quat_meta_name);
			if (l_q_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " got null object.");
			}
			vufQuaternion<T>& l_q = l_q_ptr->get_data();
			
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
			lua_getglobal(L, vufLuaStatic::g_quat_tbl_name);
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
		static int new_index(lua_State* L)
		{
			//std::cout << "VECTOR NEW_INDEX" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			auto* l_q_ptr = (vufLuaQuaternionWrapper<T>*)luaL_checkudata(L, -l_number_of_arguments, vufLuaStatic::g_quat_meta_name);
			if (l_q_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaStatic::g_quat_tbl_name, " quaternion object is null.");
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
