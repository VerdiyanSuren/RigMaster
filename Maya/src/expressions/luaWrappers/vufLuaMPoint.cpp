#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>


using namespace vufRM;

int vufLuaMPoint::create(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	//  constructor MPoint()
	if (l_number_of_arguments == 0)
	{
		create_user_data(L);
		return 1;
	}
	// constructor MVector(double, double, double, double)
	if (l_number_of_arguments == 4)
	{
		bool l_status = false;
		double l_x, l_y, l_z, l_w;
		VF_LUA_READ_NUMBER(L, -4, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint.new()). Expect  first argument of MPoint as numbers");
		VF_LUA_READ_NUMBER(L, -3, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint.new()). Expect  second argument of MPoint as numbers");
		VF_LUA_READ_NUMBER(L, -2, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint.new()). Expect  third argument of MPoint as numbers");
		VF_LUA_READ_NUMBER(L, -1, l_w, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint.new()). Expect  fourth argument of MPoint as numbers");

		auto l_wrapper = create_user_data(L);
		MPoint& l_pnt = l_wrapper->get_data();
		l_pnt.x = l_x;
		l_pnt.y = l_y;
		l_pnt.z = l_z;
		l_pnt.w = l_w;
		return 1;
	}
	// constructor MPoint(MVector)
	if (l_number_of_arguments == 1)
	{
		MVector* l_vec;
		if (vufLuaMVector::get_param(L, -1, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint.new(MVector)). Expect  argument of MPoint as MVector");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(MPoint(*l_vec));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed to create MPoint. Wrong arguments.");
}
int vufLuaMPoint::is_equivalent(lua_State* L)
{
	double l_tol = MVector_kTol;
	int l_number_of_arguments = lua_gettop(L);
	// isEquivalent (const MVector)
	if (l_number_of_arguments == 2)
	{
		MPoint* l_pnt, * l_pnt_to;
		if (get_param(L, -1, &l_pnt_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::isEquivalent). Expect first argument as MPoint.");
		}
		if (get_param(L, -2, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::isEquivalent) to get MPoint.");
		}

		bool l_res = l_pnt->isEquivalent(*l_pnt_to);
		lua_pushboolean(L, l_res);
		return 1;
	}
	// isEquivalent (const MVector other, double tolerance) 
	if (l_number_of_arguments == 3)
	{
		bool l_status;
		MPoint* l_pnt, * l_pnt_to;
		if (get_param(L, -2, &l_pnt_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::isEquivalent). Expect first argument as MPoint.");
		}
		if (get_param(L, -3, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::isEquivalent) to get MPoint.");
		}
		VF_LUA_READ_NUMBER(L, -1, l_tol, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::isEquivalent). Expect  third argument as numbers");
		bool l_res = l_pnt->isEquivalent(*l_pnt_to, l_tol);
		lua_pushboolean(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::isEquivalent). Unexpected count of arguments.");
}
int vufLuaMPoint::add(lua_State* L)
{
	MPoint* l_p1;
	//mpoint + *
	if (get_param(L, -2, &l_p1) == true)
	{
		MPoint* l_p2;
		if (get_param(L, -1, &l_p2) == true)
		{
			auto l_wrap = create_user_data(L);
			l_wrap->set_data((*l_p1) + (*l_p2));
			return 1;
		}
		MVector* l_v;
		if (vufLuaMVector::get_param(L, -1, &l_v) == true)
		{
			auto l_wrap = create_user_data(L);
			l_wrap->set_data((*l_p1) + (*l_v));
			return 1;
		}
	}
	// vector + point
	MVector* l_v;
	if (vufLuaMVector::get_param(L, -2, &l_v) == true)
	{
		MPoint* l_p2;
		if (get_param(L, -1, &l_p2) == true)
		{
			auto l_wrap = create_user_data(L);
			l_wrap->set_data((*l_v) + (*l_p2));
			return 1;
		}
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::add). Unexpected of arguments.");
}
int vufLuaMPoint::sub(lua_State* L)
{
	MPoint* l_p1;
	//mpoint - *
	if (get_param(L, -2, &l_p1) == true)
	{
		MPoint* l_p2;
		if (get_param(L, -1, &l_p2) == true)
		{
			auto l_wrap = create_user_data(L);
			l_wrap->set_data((*l_p1) - (*l_p2));
			return 1;
		}
		MVector* l_v;
		if (vufLuaMVector::get_param(L, -1, &l_v) == true)
		{
			auto l_wrap = create_user_data(L);
			l_wrap->set_data((*l_p1) - (*l_v));
			return 1;
		}
	}
	MVector* l_v;
	if (vufLuaMVector::get_param(L, -2, &l_v) == true)
	{
		MPoint* l_p2;
		if (get_param(L, -1, &l_p2) == true)
		{
			auto l_wrap = create_user_data(L);
			l_wrap->set_data((*l_v) - (*l_p2));
			return 1;
		}
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::sub). Unexpected of arguments.");
}
int vufLuaMPoint::unm(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 1)
	{
		MPoint* l_pnt;
		if (get_param(L, -1, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::unm) to get MPoint.");
		}
		MPoint l_p;
		l_p.x = l_pnt->x;
		l_p.y = l_pnt->y;
		l_p.z = l_pnt->z;
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_p);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::unnm). Unexpected count of arguments.");
}
int vufLuaMPoint::mul(lua_State* L)
{
	// double * mpoint
	if (lua_isnumber(L, -2))
	{
		MPoint* l_pnt;
		if (get_param(L, -1, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed double * MPoint mul.");
		}
		double l_val = (double)lua_tonumber(L, -2);
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data((*l_pnt) * l_val);
		return 1;
	}
	//  mpoint * number
	if (lua_isnumber(L, -1))
	{
		double l_val = (double)lua_tonumber(L, -1);
		MPoint* l_pnt;
		if (get_param(L, -2, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed MPoint * double  mul.");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data((*l_pnt) * l_val);
		return 1;
	}
	//  mpooint * matrix
	MMatrix* l_mat;
	if (vufLuaMMatrix::get_param(L, -1, &l_mat) == true)
	{
		MPoint* l_pnt;
		if (get_param(L, -2, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed MPoint * MMatrix mul.");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data((*l_pnt) * (*l_mat));
		return 1;
	}
	// matrix * mpoint
	if (vufLuaMMatrix::get_param(L, -2, &l_mat) == true)
	{
		MPoint* l_pnt;
		if (get_param(L, -1, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed MMatrix * MPoint  mul.");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data((*l_mat) * (*l_pnt));
		return 1;
	}

	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MPoint::mul). Unexpected of arguments.");
}
int vufLuaMPoint::div(lua_State* L)
{
	if (lua_isnumber(L, -1))
	{
		double l_val = (double)lua_tonumber(L, -1);
		if (l_val == 0.)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Error dividing by zero.");
		}
		l_val = 1. / l_val;
		MPoint* l_pnt;
		if (get_param(L, -2, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, "Failed MPoint div. Got null MVector object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data((*l_pnt) * l_val);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_tbl_name, " Failed (MPoint::div). Unexpected of arguments.");
}
