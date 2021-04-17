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
#include <vufQuaternion.h>

namespace vuf
{
	template <typename T>
	class vufLuaQuaternion_4
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, "quat");
			VF_LUA_ADD_TABLE_FIELD(L, "new",			quat_create);
			VF_LUA_ADD_TABLE_FIELD(L, "set",			quat_set);
			VF_LUA_ADD_TABLE_FIELD(L, "normalize",		quat_normalize);
			VF_LUA_ADD_TABLE_FIELD(L, "conjugate",		quat_conjugated);
			VF_LUA_ADD_TABLE_FIELD(L, "invert",			quat_invert);
			
			VF_LUA_ADD_TABLE_FIELD(L, "length",			quat_length);
			VF_LUA_ADD_TABLE_FIELD(L, "get_angle",		quat_angle);
			//VF_LUA_ADD_TABLE_FIELD(L, "get_axis",		quat_axis);

			//VF_LUA_ADD_TABLE_FIELD(L, "get_parallel_to",quat_get_parallel_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "distance_to",	quat_distance_to);
			VF_LUA_ADD_TABLE_FIELD(L, "dot",			quat_dot);
			//VF_LUA_ADD_TABLE_FIELD(L, "cross",			quat_cross);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_string",		quat_to_string);
			//VF_LUA_ADD_TABLE_FIELD(L, "to_type",		quat_to_type);

			VF_LUA_NEW_META_TABLE(L, "quatMeta");
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc",		quat_destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add",		quat_add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub",		quat_sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm",		quat_unm);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul",		quat_mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div",		quat_div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index",	quat_index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex",quat_new_index);
		}
	private:
		static int quat_create(lua_State* L)
		{
			void* l_quat_ptr = lua_newuserdata(L, sizeof(vufQuaternion<T>));
			new (l_quat_ptr) vufQuaternion<T>();
			luaL_getmetatable(L, "quatMeta");
			lua_setmetatable(L, -2);
			return 1;
		}
		static int quat_set(lua_State* L)
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

			auto* l_quat_ptr = (vufQuaternion<T>*)luaL_checkudata(L, -l_number_of_argiments, "quatMeta");
			if (l_quat_ptr != nullptr)
			{
				l_quat_ptr->set(l_x, l_y, l_z, l_w);
			}
			return 0;
		}
		
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	"quatMeta",	vufQuaternion<T>, normalize_in_place,	quat_normalize);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	"quatMeta", vufQuaternion<T>, conjugate_in_place,	quat_conjugated);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	"quatMeta",	vufQuaternion<T>, invert_in_place,		quat_invert);

		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER("quatMeta", vufQuaternion<T>, length,				quat_length);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER("quatMeta", vufQuaternion<T>, get_angle,			quat_angle);

		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER("quatMeta", vufQuaternion<T>, dot,					quat_dot);
		
		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"quatMeta",	vufQuaternion<T>, get_parallel_to, vector4_get_parallel_to);
		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(	"quatMeta",	vufQuaternion<T>, get_cross, vector4_cross);

		//VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER("quatMeta", vufQuaternion<T>, distance_to, vector4_distance_to);
		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE("quatMeta", vufQuaternion<T>, quat_add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE("quatMeta", vufQuaternion<T>, quat_sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE("quatMeta", vufQuaternion<T>, quat_unm);
		static int quat_mul(lua_State* L)
		{
			//  quaternion * number
			if (lua_isnumber(L, -1))
			{
				T l_val = (T)lua_tonumber(L, -1);
				auto l_quat_ptr = (vufQuaternion<T>*)luaL_checkudata(L, -2, "quatMeta");
				if (l_quat_ptr == nullptr)	return 0;
				auto l_res_ptr = (vufQuaternion<T>*)lua_newuserdata(L, sizeof(vufQuaternion<T>));
				luaL_getmetatable(L, "quatMeta");
				lua_setmetatable(L, -2);
				*l_res_ptr = (*l_quat_ptr) * l_val;
				return 1;
			}
			// number * quaternion
			if (lua_isnumber(L, -2))
			{
				T l_val = (T)lua_tonumber(L, -2);
				auto l_quat_ptr = (vufQuaternion<T>*)luaL_checkudata(L, -1, "quatMeta");
				if (l_quat_ptr == nullptr)	return 0;
				auto l_res_ptr = (vufQuaternion<T>*)lua_newuserdata(L, sizeof(vufQuaternion<T>));
				luaL_getmetatable(L, "quatMeta");
				lua_setmetatable(L, -2);
				*l_res_ptr = (*l_quat_ptr) * l_val;
				return 1;
			}
			// quaternion * quaternion
			auto l_q_1_ptr = (vufQuaternion<T>*)luaL_checkudata(L, -2, "quatMeta");
			auto l_q_2_ptr = (vufQuaternion<T>*)luaL_checkudata(L, -1, "quatMeta");
			if (l_q_1_ptr != nullptr && l_q_2_ptr != nullptr)
			{
				auto l_res_ptr = (vufQuaternion<T>*)lua_newuserdata(L, sizeof(vufQuaternion<T>));
					luaL_getmetatable(L, "quatMeta");
					lua_setmetatable(L, -2);
					*l_res_ptr = (*l_q_1_ptr) * (*l_q_2_ptr);
					return 1;
			}
			return 0;

		}

		static int quat_destroy(lua_State* L)
		{
			auto l_quat_ptr = (vufQuaternion<T>*)luaL_checkudata(L, -1, "quatMeta");
			l_quat_ptr->~vufQuaternion<T>();
			return 0;
		}
		static int quat_index(lua_State* L)
		{
			int l_number_of_argiments = lua_gettop(L);
			const char* l_key = luaL_checkstring(L, -1);
			auto* l_vector_ptr = (vufQuaternion<T>*)luaL_checkudata(L, -2, "quatMeta");

			if (l_key == nullptr || l_vector_ptr == nullptr)
			{
				return 0;
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					lua_pushnumber(L, l_vector_ptr->x);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					lua_pushnumber(L, l_vector_ptr->y);
					return 1;
				}
				else if (l_key[0] == 'z')
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
			lua_getglobal(L, "quat");
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
		static int quat_new_index(lua_State* L)
		{
			//std::cout << "new_index()" << std::endl;
			//int l_number_of_argiments = lua_gettop(L);
			auto* l_quat_ptr = (vufQuaternion<T>*)luaL_checkudata(L, -3, "quatMeta");
			const char* l_key = luaL_checkstring(L, -2);
			if (l_key == nullptr || l_quat_ptr == nullptr)
			{
				return 0;
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					l_quat_ptr->x = (T)luaL_checknumber(L, -1);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					l_quat_ptr->y = (T)luaL_checknumber(L, -1);
					return 1;
				}
				else if (l_key[0] == 'z')
				{
					l_quat_ptr->z = (T)luaL_checknumber(L, -1);
					return 1;
				}
				else if (l_key[0] == 'w')
				{
					l_quat_ptr->w = (T)luaL_checknumber(L, -1);
					return 1;
				}
			}
			return 0;
		}
	};

}
#endif // !VF_MATH_QUATERNION_LUA_H
