#ifndef VF_LUA_MPOINT_H
#define VF_LUA_MPOINT_H

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
	class vufLuaMVector;
	class vufLuaMPoint
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mpoint_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new", create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy", copy);
			//VF_LUA_ADD_TABLE_FIELD(L, "MVector", to_mvector);


			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mpoint_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div", div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", new_index);
		}
		/** bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_SET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_mpoint_meta_name,	MPoint, vufLuaMPointWrapper);
		/** bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_mpoint_meta_name,	MPoint, vufLuaMPointWrapper);
		/** bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>** p_res_ptr)*/
		VF_LUA_GET_USER_DATA_PARAM(vufLuaMayaStatic::g_mpoint_meta_name,	MPoint, vufLuaMPointWrapper);
		/** create as reference of new user data and push to stack
		* vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)	*/
		VF_LUA_CREATE_USER_NEW_REF(vufLuaMayaStatic::g_mpoint_meta_name,	MPoint, vufLuaMPointWrapper);
		/** create mew user data and push to stack
		*  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)*/
		VF_LUA_CREATE_USER_DATA(vufLuaMayaStatic::g_mpoint_meta_name,		MPoint, vufLuaMPointWrapper);
	private:
		static int create(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments == 0)
			{
				create_user_data(L);
				return 1;
			}
			if (l_number_of_arguments == 4)
			{
				bool l_status = false;
				double l_x, l_y, l_z,l_w;
				VF_LUA_READ_NUMBER(L, -4, l_x, l_status);
				VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_meta_name, " first arg has to be a number");
				VF_LUA_READ_NUMBER(L, -3, l_y, l_status);
				VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_meta_name, " second arg has to be a number");
				VF_LUA_READ_NUMBER(L, -2, l_z, l_status);
				VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_meta_name, " third  arg has to be a number");
				VF_LUA_READ_NUMBER(L, -1, l_w, l_status);
				VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_meta_name, " fourth arg has to be a number");

				auto l_wrapper = create_user_data(L);
				MPoint& l_pnt = l_wrapper->get_data();
				l_pnt.x = l_x;
				l_pnt.y = l_y;
				l_pnt.z = l_z;
				l_pnt.w = l_w;
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_meta_name, " Fauled to create MPoint. Wrong arguments count. Expect  0 or 4 numbers");
		}
		VF_LUA_IMPLEMENT_COPY(		vufLuaMayaStatic::g_mpoint_meta_name, MPoint, vufLuaMPointWrapper);
		VF_LUA_IMPLEMENT_DESTROY(	vufLuaMayaStatic::g_mpoint_meta_name, vufLuaMPointWrapper);
		/*
		static int to_mvector(lua_State* L)
		{
			MPoint* l_pnt;
			if (get_param(L, -1, &l_pnt) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, "Failed to get Mpoint object");
			}
			auto l_vec_wrap = vufLuaMVector::create_user_data(L);
			auto& l_vec =  l_vec_wrap->get_data();
			l_vec.x = l_pnt->x;
			l_vec.y = l_pnt->y;
			l_vec.z = l_pnt->z;
			return 1;
		}
		*/
		static int to_string(lua_State* L)
		{
			MPoint* l_pnt_ptr;
			if (get_param(L, -1, &l_pnt_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed tp get MPoint object.");
			}
			std::stringstream l_ss;
			l_ss << "[";
			l_ss << l_pnt_ptr->x << ", ";
			l_ss << l_pnt_ptr->y << ", ";
			l_ss << l_pnt_ptr->z << ",";
			l_ss << l_pnt_ptr->w << "]";
			lua_pushstring(L, l_ss.str().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaMayaStatic::g_mpoint_tbl_name);
			return 1;
		}

		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaMayaStatic::g_mpoint_meta_name, MPoint, vufLuaMPointWrapper, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaMayaStatic::g_mpoint_meta_name, MPoint, vufLuaMPointWrapper, sub);
		//VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(vufLuaMayaStatic::g_mpoint_meta_name, MPoint, vufLuaMPointWrapper, unm);

		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			MPoint* l_pnt_ptr;
			if (get_param(L, -2, &l_pnt_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_meta_name, " got null object.");
			}
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_meta_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					lua_pushnumber(L, l_pnt_ptr->x);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					lua_pushnumber(L, l_pnt_ptr->y);
					return 1;
				}
				else if (l_key[0] == 'z')
				{
					lua_pushnumber(L, l_pnt_ptr->z);
					return 1;
				}
				else if (l_key[0] == 'w')
				{
					lua_pushnumber(L, l_pnt_ptr->w);
					return 1;
				}
			}
			lua_getglobal(L, vufLuaMayaStatic::g_mpoint_tbl_name);
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
		static int new_index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			MPoint* l_pnt_ptr;
			if (get_param(L, -l_number_of_arguments, &l_pnt_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " got null object.");
			}

			const char* l_key = luaL_checkstring(L, -l_number_of_arguments + 1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_meta_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					l_pnt_ptr->x = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'y')
				{
					l_pnt_ptr->y = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'z')
				{
					l_pnt_ptr->z = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'w')
				{
					l_pnt_ptr->w = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " unknown key");
		}
	};
}
#endif // !VF_LUA_MPOINT_H
