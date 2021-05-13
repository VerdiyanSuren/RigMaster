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
			VF_LUA_ADD_TABLE_FIELD(L, "new", create);
			VF_LUA_ADD_TABLE_FIELD(L, "decompose", decompose);
			VF_LUA_ADD_TABLE_FIELD(L, "copy", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "setValue", set_value);
			VF_LUA_ADD_TABLE_FIELD(L, "asQuaternion", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "asMatrix", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "asVector", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "isEquivalent", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "isZero", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "incrementalRotateBy", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "inverse", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "invertIt", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "reorder", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "reorderIt", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "bound", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "boundIt", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "alternateSolution", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "setToAlternateSolution", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "closestSolution", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "setToClosestSolution", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "closestCut ", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "setToClosestCut", copy);

			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mquat_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);
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
	private:
		static int create(lua_State* L);
		VF_LUA_IMPLEMENT_COPY(vufLuaMayaStatic::g_meuler_meta_name, MEulerRotation, vufLuaMEulerRotaionWrapper);
		VF_LUA_IMPLEMENT_DESTROY(vufLuaMayaStatic::g_meuler_meta_name, vufLuaMEulerRotaionWrapper);
		static int set_value(lua_State* L);
	};
}
#endif // !VF_LUA_MEULER_ROTATTION_H
