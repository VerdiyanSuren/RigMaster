#ifndef VF_LUA_MQUATERNION_H
#define VF_LUA_MQUATERNION_H
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
	class vufLuaMQuaternion
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mquat_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new",				create);
			VF_LUA_ADD_TABLE_FIELD(L, "slerp",				slerp_l);
			VF_LUA_ADD_TABLE_FIELD(L, "dslerp",				dslerp_l);
			VF_LUA_ADD_TABLE_FIELD(L, "squad",				squad_l);
			VF_LUA_ADD_TABLE_FIELD(L, "squadPt",			squad_pt_l);

			VF_LUA_ADD_TABLE_FIELD(L, "copy",				copy);
			VF_LUA_ADD_TABLE_FIELD(L, "asMatrix",			as_matrix);
			VF_LUA_ADD_TABLE_FIELD(L, "asEulerRotation",	as_euler_rotation);
			VF_LUA_ADD_TABLE_FIELD(L, "setAxisAngle",		set_axis_angle);
			VF_LUA_ADD_TABLE_FIELD(L, "getAxisAngle",		get_axis_angle);
			VF_LUA_ADD_TABLE_FIELD(L, "setToXAxis",			set_axis_x);
			VF_LUA_ADD_TABLE_FIELD(L, "setToYAxis",			set_axis_y);
			VF_LUA_ADD_TABLE_FIELD(L, "setToZAxis",			set_axis_z);
			VF_LUA_ADD_TABLE_FIELD(L, "negateIt",			negate_it);
			VF_LUA_ADD_TABLE_FIELD(L, "isEquivalent",		is_equivalent);
			VF_LUA_ADD_TABLE_FIELD(L, "scaleIt",			scale_it);
			VF_LUA_ADD_TABLE_FIELD(L, "normal",				normal);
			VF_LUA_ADD_TABLE_FIELD(L, "normalizeIt",		normalize_it);
			VF_LUA_ADD_TABLE_FIELD(L, "conjugate",			conjugate);
			VF_LUA_ADD_TABLE_FIELD(L, "conjugateIt",		conjugate_it);
			VF_LUA_ADD_TABLE_FIELD(L, "inverse",			inverse);
			VF_LUA_ADD_TABLE_FIELD(L, "invertIt",			invert_it);
			VF_LUA_ADD_TABLE_FIELD(L, "log", log_l);
			VF_LUA_ADD_TABLE_FIELD(L, "exp", exp_l);

			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mquat_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div", div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", new_index);
		}
		VF_LUA_SET_USER_DATA_GLOBAL(	vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper);
		VF_LUA_SET_USER_DATA_GLOBAL_REF(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper);
		VF_LUA_GET_USER_DATA_GLOBAL(	vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper);
		VF_LUA_GET_USER_DATA_PARAM(		vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper);
		VF_LUA_CREATE_USER_NEW_REF(		vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper);
		VF_LUA_CREATE_USER_DATA(		vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper);
	private:
		static int create(lua_State* L);
		static int slerp_l(lua_State* L);
		static int dslerp_l(lua_State* L);
		static int squad_l(lua_State* L);
		static int squad_pt_l(lua_State* L);
		VF_LUA_IMPLEMENT_COPY(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper);
		VF_LUA_IMPLEMENT_DESTROY(vufLuaMayaStatic::g_mquat_meta_name, vufLuaMQuaternionWrapper);
		static int as_matrix(lua_State* L);
		static int as_euler_rotation(lua_State* L);
		static int set_axis_angle(lua_State* L);
		static int get_axis_angle(lua_State* L);
		static int set_axis_x(lua_State* L);
		static int set_axis_y(lua_State* L);
		static int set_axis_z(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	negateIt,	negate_it);
		static int is_equivalent(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_NUMBER_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper, scaleIt,	scale_it);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	normal,		normal);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	normalizeIt,normalize_it);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	conjugate,	conjugate);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	conjugateIt,conjugate_it);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	inverse,	inverse);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	invertIt,	invert_it);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	log,		log_l);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper,	exp,		exp_l);
		
		static int to_string(lua_State* L)
		{
			MQuaternion* l_quat_ptr;
			if (get_param(L, -1, &l_quat_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed tp get MQuaternion object.");
			}
			std::stringstream l_ss;
			l_ss << *l_quat_ptr;
			lua_pushstring(L, l_ss.str().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaMayaStatic::g_mquat_tbl_name);
			return 1;
		}

		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper, sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(vufLuaMayaStatic::g_mquat_meta_name, MQuaternion, vufLuaMQuaternionWrapper, unm);
		static int mul(lua_State* L);
		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			MQuaternion* l_quat_ptr;
			if (get_param(L, -2, &l_quat_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " got null object.");
			}
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					lua_pushnumber(L, l_quat_ptr->x);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					lua_pushnumber(L, l_quat_ptr->y);
					return 1;
				}
				else if (l_key[0] == 'z')
				{
					lua_pushnumber(L, l_quat_ptr->z);
					return 1;
				}
				else if (l_key[0] == 'w')
				{
					lua_pushnumber(L, l_quat_ptr->w);
					return 1;
				}
			}
			lua_getglobal(L, vufLuaMayaStatic::g_mquat_tbl_name);
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
			MQuaternion* l_quat_ptr;
			if (get_param(L, -l_number_of_arguments, &l_quat_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " got null object.");
			}

			const char* l_key = luaL_checkstring(L, -l_number_of_arguments + 1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					l_quat_ptr->x = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'y')
				{
					l_quat_ptr->y = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'z')
				{
					l_quat_ptr->z = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'w')
				{
					l_quat_ptr->w = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " unknown key");
		}
	};
}



#endif // !VF_LUA_MQUATERNION_H
