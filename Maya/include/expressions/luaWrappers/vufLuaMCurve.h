#ifndef VF_LUA_MCURVE_H
#define VF_LUA_MCURVE_H

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
	class vufLuaMCurve
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mcurve_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "getCV",					get_cv);
			VF_LUA_ADD_TABLE_FIELD(L, "getCVs",					get_cvs);
			VF_LUA_ADD_TABLE_FIELD(L, "degree",					degree);
			VF_LUA_ADD_TABLE_FIELD(L, "numCVs",					num_cvs);
			VF_LUA_ADD_TABLE_FIELD(L, "numSpans",				num_spans);
			VF_LUA_ADD_TABLE_FIELD(L, "isPointOnCurve",			is_point_on_curve);
			VF_LUA_ADD_TABLE_FIELD(L, "getPointAtParam",		get_point_at_param);
			VF_LUA_ADD_TABLE_FIELD(L, "getParamAtPoint",		get_param_at_point);
			VF_LUA_ADD_TABLE_FIELD(L, "isParamOnCurve",			is_param_on_curve);
			VF_LUA_ADD_TABLE_FIELD(L, "normal",					normal);
			VF_LUA_ADD_TABLE_FIELD(L, "tangent",				tangent);
			VF_LUA_ADD_TABLE_FIELD(L, "getDerivativesAtParm",	get_derivatives_at_parm);
			VF_LUA_ADD_TABLE_FIELD(L, "isPlanar",				is_planar);
			VF_LUA_ADD_TABLE_FIELD(L, "closestPoint",			closest_point);
			VF_LUA_ADD_TABLE_FIELD(L, "distanceToPoint",		distance_to_point);
			VF_LUA_ADD_TABLE_FIELD(L, "area",					area);
			VF_LUA_ADD_TABLE_FIELD(L, "length",					length);
			VF_LUA_ADD_TABLE_FIELD(L, "findParamFromLength",	find_param_from_length);
			VF_LUA_ADD_TABLE_FIELD(L, "findLengthFromParam",	find_length_from_param);
			
			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mcurve_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
		}
		VF_LUA_SET_USER_DATA_GLOBAL(	vufLuaMayaStatic::g_mcurve_meta_name, MObject, vufLuaMObjectWrapper);
		VF_LUA_SET_USER_DATA_GLOBAL_REF(vufLuaMayaStatic::g_mcurve_meta_name, MObject, vufLuaMObjectWrapper);
		VF_LUA_GET_USER_DATA_GLOBAL(	vufLuaMayaStatic::g_mcurve_meta_name, MObject, vufLuaMObjectWrapper);
		VF_LUA_GET_USER_DATA_PARAM(		vufLuaMayaStatic::g_mcurve_meta_name, MObject, vufLuaMObjectWrapper);
		VF_LUA_CREATE_USER_NEW_REF(		vufLuaMayaStatic::g_mcurve_meta_name, MObject, vufLuaMObjectWrapper);
		VF_LUA_CREATE_USER_DATA(		vufLuaMayaStatic::g_mcurve_meta_name, MObject, vufLuaMObjectWrapper);

	private:
		static int create(lua_State* L)
		{
			create_user_data(L);
			return 1;
		}
		VF_LUA_IMPLEMENT_DESTROY(vufLuaMayaStatic::g_mcurve_meta_name, vufLuaMObjectWrapper);
		static int get_cv(lua_State* L);
		static int get_cvs(lua_State* L);
		static int degree(lua_State* L);
		static int num_cvs(lua_State* L);
		static int num_spans(lua_State* L);
		static int is_point_on_curve(lua_State* L);
		static int get_point_at_param(lua_State* L);
		static int get_param_at_point(lua_State* L);
		static int is_param_on_curve(lua_State* L);
		static int normal(lua_State* L);
		static int tangent(lua_State* L);
		
		static int get_derivatives_at_parm(lua_State* L);
		static int is_planar(lua_State* L);
		static int closest_point(lua_State* L);
		static int distance_to_point(lua_State* L);
		static int area(lua_State* L);
		static int length(lua_State* L);
		static int find_param_from_length(lua_State* L);
		static int find_length_from_param(lua_State* L);

		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key != nullptr)
			{
				lua_getglobal(L, vufLuaMayaStatic::g_mcurve_tbl_name);
				lua_pushstring(L, l_key);
				lua_rawget(L, -2);
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Unexpected MCurve error");
		}
	};
}
#endif // !VF_LUA_MCURVE_H
