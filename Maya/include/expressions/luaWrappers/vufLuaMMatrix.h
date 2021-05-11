#ifndef VF_LUA_MMATRIX_H
#define VF_LUA_MMATRIX_H

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

namespace vufRM
{
	class vufLuaMMatrix
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_matrix_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new",			create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",			copy);
			VF_LUA_ADD_TABLE_FIELD(L, "get",			get);
			VF_LUA_ADD_TABLE_FIELD(L, "set",			set);
			VF_LUA_ADD_TABLE_FIELD(L, "transpose",		transpose);
			VF_LUA_ADD_TABLE_FIELD(L, "setToIdentity",	to_identity);
			VF_LUA_ADD_TABLE_FIELD(L, "inverse",		inverse);
			VF_LUA_ADD_TABLE_FIELD(L, "adjoint",		adjoint);
			VF_LUA_ADD_TABLE_FIELD(L, "homogenize",		homogenize);
			VF_LUA_ADD_TABLE_FIELD(L, "det4x4",			det4x4);
			VF_LUA_ADD_TABLE_FIELD(L, "det3x3",			det3x3);
			VF_LUA_ADD_TABLE_FIELD(L, "isSingular",		is_singular);

			//VF_LUA_ADD_TABLE_FIELD(L, "parallel_to", parallel_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "distance_to", distance_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "rotateBy", distance_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "rotateTo", distance_to);

			///VF_LUA_ADD_TABLE_FIELD(L, "dot", dot);
			//VF_LUA_ADD_TABLE_FIELD(L, "cross", cross);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_matrix_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div", div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", new_index);
		}
		/** bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_SET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_matrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		/** bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_matrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		/** bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>** p_res_ptr)*/
		VF_LUA_GET_USER_DATA_PARAM(	vufLuaMayaStatic::g_matrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		/** create as reference of new user data and push to stack
		* vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)	*/
		VF_LUA_CREATE_USER_NEW_REF(	vufLuaMayaStatic::g_matrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		/** create mew user data and push to stack
		*  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)*/
		VF_LUA_CREATE_USER_DATA(	vufLuaMayaStatic::g_matrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
	private:
		static int create(lua_State* L)
		{
			create_user_data(L);
			return 1;
		}
		VF_LUA_IMPLEMENT_COPY(vufLuaMayaStatic::g_matrix_meta_name, MMatrix, vufLuaMMattrixWrapper);

	};
}
#endif // !VF_LUA_MMATRIX_H
