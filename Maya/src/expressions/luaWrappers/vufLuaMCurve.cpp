#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPointArray.h>
#include <expressions/luaWrappers/vufLuaMCurve.h>
#include <maya/MFnNurbsCurve.h>

using namespace vufRM;

int vufLuaMCurve::get_cv(lua_State* L)
{
	int l_index;
	MPoint* l_pt;
	MObject* l_object;
	int l_world;
	
	l_world = (int)luaL_checkinteger(L, -1);
	if (vufLuaMPoint::get_param(L, -2, &l_pt) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getCV) to get -2 arg as MPoint.")
	}
	l_index = (int)luaL_checkinteger(L, -3);
	if (get_param(L,-4, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getCV) to get -4 arg as MCurve.");
	}
	
	MFnNurbsCurve l_fn(*l_object);
	MStatus l_status = l_fn.getCV(l_index, *l_pt, l_world == 0 ? MSpace::kObject : MSpace::kWorld);
	l_status == MS::kSuccess ? lua_pushboolean(L, 1) : lua_pushboolean(L, 0);
	return 1;
}
int vufLuaMCurve::get_cvs(lua_State* L)
{
	MPointArray* l_pt;
	MObject* l_object;
	int l_world;

	l_world = (int)luaL_checkinteger(L, -1);
	if (vufLuaMPointArray::get_param(L, -2, &l_pt) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getCVs) to get -2 arg as MPointArray.")
	}
	if (get_param(L, -3, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getCVs) to get -3 arg as MCurve.");
	}

	MFnNurbsCurve l_fn(*l_object);
	MStatus l_status = l_fn.getCVs(*l_pt, l_world == 0 ? MSpace::kObject : MSpace::kWorld);
	l_status == MS::kSuccess ? lua_pushboolean(L, 1) : lua_pushboolean(L, 0);
	return 1;
}
int vufLuaMCurve::degree(lua_State* L)
{
	int l_degree;
	MObject* l_object;
	if (get_param(L, -1, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:degree) to get -1 arg as MCurve.");
	}
	MFnNurbsCurve l_fn(*l_object);
	l_degree = l_fn.degree();
	lua_pushinteger(L, l_degree);
	return 1;
}
int vufLuaMCurve::num_cvs(lua_State* L)
{
	int l_degree;
	MObject* l_object;
	if (get_param(L, -1, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:numCVs) to get -1 arg as MCurve.");
	}
	MFnNurbsCurve l_fn(*l_object);
	l_degree = l_fn.numCVs();
	lua_pushinteger(L, l_degree);
	return 1;
}
int vufLuaMCurve::num_spans(lua_State* L)
{
	int l_degree;
	MObject* l_object;
	if (get_param(L, -1, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:numSpans) to get -1 arg as MCurve.");
	}
	MFnNurbsCurve l_fn(*l_object);
	l_degree = l_fn.numSpans();
	lua_pushinteger(L, l_degree);
	return 1;
}
int vufLuaMCurve::is_point_on_curve(lua_State* L)
{
	int l_num_arguuments = lua_gettop(L);
	MPoint* l_pt;
	MObject* l_object;
	int l_world;
	double l_epsillon = kMFnNurbsEpsilon;
	l_world = (int)luaL_checkinteger(L, -1);
	if (l_num_arguuments == 4)
	{
		l_epsillon = (double)luaL_checknumber(L, -2);
		if(vufLuaMPoint::get_param(L, -3, &l_pt) == false)
		{
			VF_LUA_THROW_ERROR(L,vufLuaMayaStatic::g_mcurve_tbl_name, " Failed to get -3. Expect MPoint.");
		}
		if (get_param(L, -4, &l_object) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:isPointOnCurve) to get -4. Expect MCurve.");
		}
		MFnNurbsCurve l_fn(*l_object);
		bool l_res = l_fn.isPointOnCurve(*l_pt, l_epsillon, l_world == l_world == 0 ? MSpace::kObject : MSpace::kWorld);
		lua_pushboolean(L, (int)l_res);
		return 1;
	}
	if (l_num_arguuments == 3)
	{
		if (vufLuaMPoint::get_param(L, -2, &l_pt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:isPointOnCurve) to get -2. Expect MPoint.");
		}
		if (get_param(L, -3, &l_object) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:isPointOnCurve) to get -3. Expect MCurve.");
		}
		MFnNurbsCurve l_fn(*l_object);
		bool l_res = l_fn.isPointOnCurve(*l_pt, l_epsillon, l_world == l_world == 0 ? MSpace::kObject : MSpace::kWorld);
		lua_pushboolean(L, (int)l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name," Failed(MCurve:isPointOnCurve) Unexpected parsing of arguments.")
}
int vufLuaMCurve::get_point_at_param(lua_State* L)
{
	MPoint* l_pt;
	MObject* l_object;
	int l_world;
	double l_param;
	l_world = (int)luaL_checkinteger(L, -1);
	l_param = (double)luaL_checknumber(L, -2);
	if (vufLuaMPoint::get_param(L, -3, &l_pt) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getPointAtParam) to get -3. Expect MPoint.");
	}
	if (get_param(L, -4, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getPointAtParam) to get -4. Expect MCurve.");
	}
	MFnNurbsCurve l_fn(*l_object);
	bool l_res = l_fn.getPointAtParam( l_param, *l_pt, l_world == l_world == 0 ? MSpace::kObject : MSpace::kWorld);
	lua_pushboolean(L, (int)l_res);
	return 1;
}
int vufLuaMCurve::get_param_at_point(lua_State* L)
{
	int l_num_of_arguments = lua_gettop(L);
	MPoint* l_pt;
	MObject* l_object;
	int l_world;
	double l_param;
	l_world = (int)luaL_checkinteger(L, -1);
	if (l_num_of_arguments == 5)
	{
		double l_toolerance = (double)luaL_checknumber(L, -2);
		l_param = (double)luaL_checknumber(L, -3);
		if (vufLuaMPoint::get_param(L, -4, &l_pt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getParamAtPoint) to get -4. Expect MPoint.");
		}
		if (get_param(L, -5, &l_object) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getParamAtPoint) to get -5. Expect MCurve.");
		}
		MFnNurbsCurve l_fn(*l_object);
		MStatus l_status = l_fn.getParamAtPoint(*l_pt, l_param, l_toolerance, l_world == l_world == 0 ? MSpace::kObject : MSpace::kWorld);
		bool l_res = l_status == MS::kSuccess ? true: false;
		lua_pushboolean(L, (int)l_res);
		return 1;
	}
	if (l_num_of_arguments == 4)
	{
		l_param = (double)luaL_checknumber(L, -2);
		if (vufLuaMPoint::get_param(L, -3, &l_pt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getParamAtPoint) to get -3. Expect MPoint.");
		}
		if (get_param(L, -4, &l_object) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getParamAtPoint) to get -4. Expect MCurve.");
		}
		MFnNurbsCurve l_fn(*l_object);
		MStatus l_status = l_fn.getParamAtPoint(*l_pt, l_param, l_world == l_world == 0 ? MSpace::kObject : MSpace::kWorld);
		bool l_res = l_status == MS::kSuccess ? true : false;
		lua_pushboolean(L, (int)l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getParamAtPoint) Unexpected parsing of arguments.")
}
int vufLuaMCurve::is_param_on_curve(lua_State* L)
{	
	MObject* l_object;
	double l_param = (double)luaL_checknumber(L, -1);
	if (get_param(L, -2, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:isParamOnCurve) to get -2. Expect MCurve.");
	}
	MFnNurbsCurve l_fn(*l_object);
	bool l_res = l_fn.isParamOnCurve( l_param );
	lua_pushboolean(L, (int)l_res);
	return 1;
}
int vufLuaMCurve::normal(lua_State* L)
{
	MObject* l_object;
	int l_world;
	l_world = (int)luaL_checkinteger(L, -1);
	double l_param = (double)luaL_checknumber(L, -2);
	if (get_param(L, -3, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:normal) to get -3. Expect MCurve.");
	}
	MFnNurbsCurve l_fn(*l_object);
	auto l_wrap = vufLuaMVector::create_user_data(L);
	l_wrap->set_data(l_fn.normal(l_param, l_world == 0 ? MSpace::kObject : MSpace::kWorld));
	return 1;
}
int vufLuaMCurve::tangent(lua_State* L)
{
	MObject* l_object;
	int l_world;
	l_world = (int)luaL_checkinteger(L, -1);
	double l_param = (double)luaL_checknumber(L, -2);
	if (get_param(L, -3, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:tangent) to get -3. Expect MCurve.");
	}
	MFnNurbsCurve l_fn(*l_object);
	auto l_wrap = vufLuaMVector::create_user_data(L);
	l_wrap->set_data(l_fn.tangent(l_param, l_world == 0 ? MSpace::kObject : MSpace::kWorld));
	return 1;
}
int vufLuaMCurve::get_derivatives_at_parm(lua_State* L)
{
	MObject* l_object;
	int l_world;
	MPoint* l_pt;
	MVector* l_vec;
	double l_param;
	//getDerivativesAtParm
	l_world = (int)luaL_checkinteger(L, -1);
	if (vufLuaMVector::get_param(L, -2, &l_vec) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getDerivativesAtParm) to get -2. Expect MVector.");
	}
	if (vufLuaMPoint::get_param(L, -3, &l_pt) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getDerivativesAtParm) to get -3. Expect MPoint.");
	}
	l_param = (double)luaL_checknumber(L, -4);
	if (get_param(L, -5, &l_object) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:getDerivativesAtParm) to get -5. Expect MCurve.");
	}
	MFnNurbsCurve l_fn(*l_object);
	MStatus l_status = l_fn.getDerivativesAtParm(l_param ,*l_pt, *l_vec, l_world == l_world == 0 ? MSpace::kObject : MSpace::kWorld);
	bool l_res = l_status == MS::kSuccess ? true : false;
	lua_pushboolean(L, (int)l_res);
	return 1;
}
int vufLuaMCurve::is_planar(lua_State* L)
{
	MObject* l_object;
	int l_num_arguments = lua_gettop(L);
	if (l_num_arguments == 1)
	{
		if (get_param(L, -1, &l_object) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:isPlanar) to get -1. Expect MCurve.");
		}
		MFnNurbsCurve l_fn(*l_object);
		bool l_res = l_fn.isPlanar();
		lua_pushboolean(L, (int)l_res);
		return 1;
	}
	if (l_num_arguments == 2)
	{
		MVector* l_vec;
		if (vufLuaMVector::get_param(L, -1, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:isPlanar) to get -1. Expect MVector.");
		}
		if (get_param(L, -1, &l_object) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:isPlanar) to get -2. Expect MCurve.");
		}
		MFnNurbsCurve l_fn(*l_object);
		bool l_res = l_fn.isPlanar(l_vec);
		lua_pushboolean(L, (int)l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mcurve_tbl_name, " Failed(MCurve:isPlanar) Unexpected parsing of arguments.")
}
int vufLuaMCurve::closest_point(lua_State* L)
{
	return 0;
}
int vufLuaMCurve::distance_to_point(lua_State* L)
{
	return 0;
}
int vufLuaMCurve::area(lua_State* L)
{
	return 0;
}
int vufLuaMCurve::length(lua_State* L)
{
	return 0;
}
int vufLuaMCurve::find_param_from_length(lua_State* L)
{
	return 0;
}
int vufLuaMCurve::find_length_from_param(lua_State* L)
{
	return 0;
}
