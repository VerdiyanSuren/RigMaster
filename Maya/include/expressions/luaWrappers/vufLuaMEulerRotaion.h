#ifndef VF_LUA_MEULER_ROTATTION_H
#define VF_LUA_MEULER_ROTATTION_H

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
	class vufLuaMEulerRotation
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_meuler_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new",				create);
			//VF_LUA_ADD_TABLE_FIELD(L, "decompose", decompose);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",				copy);
			VF_LUA_ADD_TABLE_FIELD(L, "setValue",			set_value);
			VF_LUA_ADD_TABLE_FIELD(L, "asQuaternion",		as_quaternion);
			VF_LUA_ADD_TABLE_FIELD(L, "asMatrix",			as_matrix);
			VF_LUA_ADD_TABLE_FIELD(L, "asVector",			as_vector);
			VF_LUA_ADD_TABLE_FIELD(L, "isEquivalent",		is_equivalent);
			VF_LUA_ADD_TABLE_FIELD(L, "isZero",				is_zero);
			VF_LUA_ADD_TABLE_FIELD(L, "incrementalRotateBy",increment_rotate);
			VF_LUA_ADD_TABLE_FIELD(L, "inverse",			inverse);
			VF_LUA_ADD_TABLE_FIELD(L, "invertIt",			inverse_it);
			VF_LUA_ADD_TABLE_FIELD(L, "reorder",			reorder);
			VF_LUA_ADD_TABLE_FIELD(L, "reorderIt",			reorder_it);
			VF_LUA_ADD_TABLE_FIELD(L, "bound",				bound);
			VF_LUA_ADD_TABLE_FIELD(L, "boundIt",			bound_it );
			VF_LUA_ADD_TABLE_FIELD(L, "alternateSolution",	alternate_solution);
			VF_LUA_ADD_TABLE_FIELD(L, "setToAlternateSolution", set_alternate_solution);
			VF_LUA_ADD_TABLE_FIELD(L, "closestSolution",		closest_solution);
			VF_LUA_ADD_TABLE_FIELD(L, "setToClosestSolution",	set_closest_solution);
			VF_LUA_ADD_TABLE_FIELD(L, "closestCut ",			closest_cut);
			VF_LUA_ADD_TABLE_FIELD(L, "setToClosestCut",		set_closest_cut);

			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_meuler_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div", div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", new_index);
		}
		/** bool	set_global(lua_State* L, const std::string& p_var_name, MEulerRotation& p_res)	*/
		VF_LUA_SET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper);
		/** bool	get_global(lua_State * L, const std::string & p_var_name, MEulerRotation& p_res)	*/
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper);
		/** bool	static bool	get_param(lua_State * L, int p_lua_index, MEulerRotation** p_res_ptr)*/
		VF_LUA_GET_USER_DATA_PARAM(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper);
		/** create as reference of new user data and push to stack
		* vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, MEulerRotation* l_ref_vector)	*/
		VF_LUA_CREATE_USER_NEW_REF(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper);
		/** create mew user data and push to stack
		*  vufLuaMEulerRotaionWrapper* create_user_data(lua_State* L)*/
		VF_LUA_CREATE_USER_DATA(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper);
		static bool rotate_order_from_str(lua_State* L, int index, MEulerRotation::RotationOrder& p_res);
		static std::string rotate_order_as_string( const MEulerRotation& p_rot);
	private:
		static int create(lua_State* L);
		VF_LUA_IMPLEMENT_COPY(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper);
		VF_LUA_IMPLEMENT_DESTROY(vufLuaMayaStatic::g_meuler_meta_name, vufLuaMEulerRotaionWrapper);
		static int set_value(lua_State* L);
		static int as_quaternion(lua_State* L);
		static int as_matrix(lua_State* L);
		static int as_vector(lua_State* L);
		static int is_equivalent(lua_State* L);
		static int is_zero(lua_State* L);
		static int increment_rotate(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, inverse, inverse);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, invertIt, inverse_it);
		static int reorder(lua_State* L);
		static int reorder_it(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, bound, bound);
		static int bound_it(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, alternateSolution, alternate_solution);
		static int set_alternate_solution(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, closestSolution, closest_solution);
		static int set_closest_solution(lua_State* L);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, closestCut, closest_cut);
		static int set_closest_cut(lua_State* L);

		static int to_string(lua_State* L)
		{
			MEulerRotation* l_rot_ptr;
			if (get_param(L, -1, &l_rot_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed tp get MEulerRotation object.");
			}
			std::stringstream l_ss;
			l_ss << *l_rot_ptr;
			lua_pushstring(L, l_ss.str().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaMayaStatic::g_meuler_tbl_name);
			return 1;
		}

		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper, unm);
		static int mul(lua_State* L);

		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			MEulerRotation* l_rot;
			if (get_param(L, -2, &l_rot) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " got null object.");
			}
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					lua_pushnumber(L, l_rot->x);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					lua_pushnumber(L, l_rot->y);
					return 1;
				}
				else if (l_key[0] == 'z')
				{
					lua_pushnumber(L, l_rot->z);
					return 1;
				}
				else if (l_key[0] == 'r')
				{
					std::string l_str = rotate_order_as_string(*l_rot);
					lua_pushstring(L, l_str.c_str());
					return 1;
				}
			}
			lua_getglobal(L, vufLuaMayaStatic::g_meuler_tbl_name);
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
		static int new_index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			MEulerRotation* l_rot;
			MEulerRotation::RotationOrder l_order;
			if (get_param(L, -l_number_of_arguments, &l_rot) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " got null object.");
			}
			if (rotate_order_from_str(L, -l_number_of_arguments + 1, l_order) == true)
			{
				l_rot->order = l_order;
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " unknown key");
		}
	};
}
#endif // !VF_LUA_MEULER_ROTATTION_H
