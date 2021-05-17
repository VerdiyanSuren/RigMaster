#ifndef VF_LUA_MVECTOR_H
#define VF_LUA_MVECTOR_H

#include <vufLuaWrapUtil.h>
#include <expressions/luaWrappers/vufLuaMayaStatic.h>
#include <expressions/luaWrappers/vufLuaMayaDataWrapper.h>
#include <coreUtils/vufStringUtils.h>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <sstream>
namespace vufRM
{
	class vufLuaMPoint;
	class vufLuaMVector
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mvec_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new",		create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",		copy);
			VF_LUA_ADD_TABLE_FIELD(L, "cross",		cross);
			VF_LUA_ADD_TABLE_FIELD(L, "dot",		dot);
			VF_LUA_ADD_TABLE_FIELD(L, "rotateBy",	rotate_by);
			VF_LUA_ADD_TABLE_FIELD(L, "rotateTo",	rotate_to);

			VF_LUA_ADD_TABLE_FIELD(L, "length",			length);
			VF_LUA_ADD_TABLE_FIELD(L, "normal",			normal);
			VF_LUA_ADD_TABLE_FIELD(L, "normalize",		normalize);
			VF_LUA_ADD_TABLE_FIELD(L, "angle",			angle);
			VF_LUA_ADD_TABLE_FIELD(L, "isEquivalent",	is_equivalent);
			VF_LUA_ADD_TABLE_FIELD(L, "isParallel",		is_parallel);
			VF_LUA_ADD_TABLE_FIELD(L, "transformAsNormal", transform_as_normal);
			VF_LUA_ADD_TABLE_FIELD(L, "orthoTo",		ortho_to);
			VF_LUA_ADD_TABLE_FIELD(L, "parallelTo",		parallel_to);
			VF_LUA_ADD_TABLE_FIELD(L, "distanceTo",		distance_to);
			

			VF_LUA_ADD_TABLE_FIELD(L, "to_string",		to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type",		to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mvec_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__div", div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", new_index);
		}
		VF_LUA_SET_USER_DATA_GLOBAL(	vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper);
		VF_LUA_SET_USER_DATA_GLOBAL_REF(vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper);
		VF_LUA_GET_USER_DATA_GLOBAL(	vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper);
		VF_LUA_GET_USER_DATA_PARAM(		vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper);
		VF_LUA_CREATE_USER_NEW_REF(		vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper);
		VF_LUA_CREATE_USER_DATA(		vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper);
	private:
		static int create(	lua_State* L);		
		VF_LUA_IMPLEMENT_COPY(vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper);
		static int cross(	lua_State* L);
		static int dot(		lua_State* L);
		static int rotate_by(lua_State* L);
		static int rotate_to(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaMayaStatic::g_mvec_meta_name, vufLuaMVectorWrapper, length, length);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper, normal, normal);
		static int normalize(		lua_State* L);
		static int is_equivalent(	lua_State* L);
		static int is_parallel(		lua_State* L);
		static int ortho_to(		lua_State* L);
		static int parallel_to(		lua_State* L);
		static int distance_to(		lua_State* L);
		static int transform_as_normal(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(vufLuaMayaStatic::g_mvec_meta_name, vufLuaMVectorWrapper, angle, angle);

		static int to_string(lua_State* L)
		{
			MVector* l_vec_ptr;
			if (get_param(L, -1, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed tp get MVector object.");
			}
			std::stringstream l_ss;
			l_ss << *l_vec_ptr;
			lua_pushstring(L, l_ss.str().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaMayaStatic::g_mvec_tbl_name);
			return 1;
		}
		
		VF_LUA_IMPLEMENT_DESTROY(		vufLuaMayaStatic::g_mvec_meta_name, vufLuaMVectorWrapper);
		//VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(	vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper, add);
		//VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(	vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper, sub);
		static int add(lua_State* L);
		static int sub(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(	vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper, unm);
		static int mul(lua_State* L);
		static int div(lua_State* L);
		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			MVector* l_vec_ptr;
			if (get_param(L, -2, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " got null object.");
			}
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_meta_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					lua_pushnumber(L, l_vec_ptr->x);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					lua_pushnumber(L, l_vec_ptr->y);
					return 1;
				}
				else if (l_key[0] == 'z')
				{
					lua_pushnumber(L, l_vec_ptr->z);
					return 1;
				}
			}
			lua_getglobal(L, vufLuaMayaStatic::g_mvec_tbl_name);
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
			auto l_wrapper_ptr = (vufLuaMVectorWrapper*)luaL_checkudata(L, -l_number_of_arguments, vufLuaMayaStatic::g_mvec_meta_name);
			if (l_wrapper_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " vector object is null.");
			}

			const char* l_key = luaL_checkstring(L, -l_number_of_arguments + 1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " unknown key");
			}
			MVector& l_vec = l_wrapper_ptr->get_data();
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					l_vec.x = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'y')
				{
					l_vec.y = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'z')
				{
					l_vec.z = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " unknown key");
		}
	};
}
#endif // !VF_LUA_MVECTOR_H
