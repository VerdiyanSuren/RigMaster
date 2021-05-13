#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>
#include <vufVector.h>

using namespace vufRM;

int vufLuaMVector::create(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	//  constructor MVector()
	if (l_number_of_arguments == 0)
	{
		create_user_data(L);
		return 1;
	}
	// constructor MVector(double, double, double)
	if (l_number_of_arguments == 3)
	{
		bool l_status = false;
		double l_x, l_y, l_z;
		VF_LUA_READ_NUMBER(L, -3, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mvec_tbl_name, " Failed. Expect  first argument of MVector as numbers");
		VF_LUA_READ_NUMBER(L, -2, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mvec_tbl_name, " Failed. Expect  second argument of MVector as numbers");
		VF_LUA_READ_NUMBER(L, -1, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mvec_tbl_name, " Failed. Expect  third argument of MVector as numbers");
		auto l_wrapper = create_user_data(L);
		MVector& l_vec = l_wrapper->get_data();
		l_vec.x = l_x;
		l_vec.y = l_y;
		l_vec.z = l_z;
		return 1;
	}
	// constructor MVector(MPoint)
	if (l_number_of_arguments == 1)
	{
		MPoint* l_pnt;
		if (vufLuaMPoint::get_param(L, -1, &l_pnt) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed. Expect  argument of MVector as MPoint");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(MVector(*l_pnt));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to create MVector. Wrong arguments.");
}
int vufLuaMVector::cross(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments != 2)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Wrong number of arguments.");
	}
	MVector* l_v_1, * l_v_2;
	if (get_param(L, -2, &l_v_1) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to get MVector.");
	}
	if (get_param(L, -1, &l_v_2) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed. Expect first argumen as MVector.");
	}
	auto l_wrap = create_user_data(L);
	l_wrap->set_data((*l_v_1) ^ (*l_v_2));
	return 1;
}
int vufLuaMVector::dot(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments != 2)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Wrong number of arguments.");
	}
	MVector* l_v_1, * l_v_2;
	if (get_param(L, -2, &l_v_1) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to get MVector.");
	}
	if (get_param(L, -1, &l_v_2) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed. Expect first argumen as MVector.");
	}
	double l_val = ((*l_v_1) * (*l_v_2));
	lua_pushnumber(L, l_val);
	return 1;
}
int vufLuaMVector::rotate_by(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// rotate_by(MQuaternion) or rotate_by(MEuler_Rotation)
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec;
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed to get MVector.");
		}
		MQuaternion *l_quat;
		if (vufLuaMQuaternion::get_param(L, -1, &l_quat) == false)
		{
			// check is this MEuler rotation
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to get parameter.HAs to be MQuaternion");
		}
		// rotate by quaternion
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(l_vec->rotateBy(*l_quat));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to rotateBy. Wrong arguments.");
}
int vufLuaMVector::rotate_to(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec, * l_vec_to;
		if (get_param(L, -1, &l_vec_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to get MVector.");
		}
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed. Expect first argument as MVector.");
		}
		auto l_quat_wrap = vufLuaMQuaternion::create_user_data(L);
		l_quat_wrap->set_data(l_vec->rotateTo(*l_vec_to));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to rotateTo. Wrong arguments.");
}
int vufLuaMVector::normalize(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 1)
	{
		MVector* l_vec;
		if (get_param(L, -1, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to get MVector.");
		}
		MStatus l_status;
		l_status = l_vec->normalize();
		bool l_res = l_status == MS::kSuccess ? true : false;
		lua_pushboolean(L,l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed to rotateTo. Wrong arguments.");
}
int vufLuaMVector::is_equivalent(lua_State* L)
{
	double l_tol = MVector_kTol;
	int l_number_of_arguments = lua_gettop(L);
	// isEquivalent (const MVector)
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec,*l_vec_to;
		if (get_param(L, -1, &l_vec_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::isEquivalent). EXpect first argument as MVector.");
		}
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::isEquivalent) to get MVector.");
		}

		bool l_res = l_vec->isEquivalent(*l_vec_to);
		lua_pushboolean(L, l_res);
		return 1;
	}
	// isEquivalent (const MVector other, double tolerance) 
	if (l_number_of_arguments == 3)
	{
		bool l_status;
		MVector* l_vec, * l_vec_to;
		if (get_param(L, -2, &l_vec_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::isEquivalent). Expect first argument as MVector.");
		}
		if (get_param(L, -3, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::isEquivalent) to get MVector.");
		}
		VF_LUA_READ_NUMBER(L, -1, l_tol, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::isEquivalent). Expect  third argument as numbers");
		bool l_res = l_vec->isEquivalent(*l_vec_to, l_tol);
		lua_pushboolean(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::isEquivalent). Unexpected count of arguments.");
}
int vufLuaMVector::is_parallel(lua_State* L)
{
	double l_tol = MVector_kTol;
	int l_number_of_arguments = lua_gettop(L);
	// isParallel (const MVector other)
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec, * l_vec_to;
		if (get_param(L, -1, &l_vec_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::is_parallel). EXpect first argument as MVector.");
		}
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::is_parallel) to get MVector.");
		}
		bool l_res = l_vec->isParallel(*l_vec_to);
		lua_pushboolean(L, l_res);
		return 1;
	}
	// isParallel(const MVector & other, double tolerance )
	if (l_number_of_arguments == 3)
	{
		bool l_status;
		MVector* l_vec, * l_vec_to;
		if (get_param(L, -2, &l_vec_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::is_parallel). Expect first argument as MVector.");
		}
		if (get_param(L, -3, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::is_parallel) to get MVector.");
		}
		VF_LUA_READ_NUMBER(L, -1, l_tol, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::is_parallel). Expect  third argument as numbers");
		bool l_res = l_vec->isParallel(*l_vec_to, l_tol);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::is_parallel). Unexpected count of arguments.");
}
int vufLuaMVector::ortho_to(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec, * l_vec_to;
		if (get_param(L, -1, &l_vec_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::orthoTo). Expect first argument as MVector.");
		}
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::orthoTo) to get MVector.");
		}
		vuf::vufVector4<double> l_v1(l_vec->x, l_vec->y, l_vec->z), l_v2(l_vec_to->x, l_vec_to->y, l_vec_to->z);
		l_v1.make_ortho_to_in_place(l_v2);
		MVector l_res(l_v1.x, l_v1.y, l_v1.z);
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::orthoTo). Unexpected count of arguments.");
}
int vufLuaMVector::parallel_to(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec, * l_vec_to;
		if (get_param(L, -1, &l_vec_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::parallelTo). Expect first argument as MVector.");
		}
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::parallelTo) to get MVector.");
		}
		vuf::vufVector4<double> l_v1(l_vec->x, l_vec->y, l_vec->z), l_v2(l_vec_to->x, l_vec_to->y, l_vec_to->z);
		l_v1.make_parallel_to_in_place(l_v2);
		MVector l_res(l_v1.x, l_v1.y, l_v1.z);
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::parallelTo). Unexpected count of arguments.");

}
int vufLuaMVector::distance_to(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec, * l_vec_to;
		if (get_param(L, -1, &l_vec_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::distanceTo). Expect first argument as MVector.");
		}
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::distanceTo) to get MVector.");
		}
		vuf::vufVector4<double> l_v1(l_vec->x, l_vec->y, l_vec->z), l_v2(l_vec_to->x, l_vec_to->y, l_vec_to->z);
		double l_res = l_v1.distance_to(l_v2);
		lua_pushnumber(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::distanceTo). Unexpected count of arguments.");
}
int vufLuaMVector::transform_as_normal(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec;
		MMatrix* l_mat;
		if (vufLuaMMatrix::get_param(L, -1, &l_mat) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::transformAsNormal). Expect first argument as MMatrix.");

		}
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::transformAsNormal) to get MVector.");

		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(l_vec->transformAsNormal(*l_mat));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::transformAsNormal). Unexpected count of arguments.");
}
int vufLuaMVector::mul(lua_State* L)
{
	// double * vector
	if (lua_isnumber(L, -2))
	{
		MVector* l_vec;
		if (get_param(L, -1, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed double * MVector mul.");
		}
		double l_val = (double)lua_tonumber(L, -2);
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data((*l_vec) * l_val);
		return 1;
	}
	//  vector * number
	if (lua_isnumber(L, -1))
	{
		double l_val = (double)lua_tonumber(L, -1);
		MVector* l_vec;
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed MVector * double  mul.");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data((*l_vec) * l_val);
		return 1;
	}
	//  vector * matrix
	MMatrix* l_mat;
	if (vufLuaMMatrix::get_param(L, -1,&l_mat ) == true)
	{
		MVector* l_vec;
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed MVector * MMatrix mul.");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data( (*l_vec) * (*l_mat));
		return 1;
	}
	// matrix * vecotor
	if (vufLuaMMatrix::get_param(L, -2, &l_mat) == true)
	{
		MVector* l_vec;
		if (get_param(L, -1, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed MMatrix * MVector  mul.");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data((*l_mat) * (*l_vec));
		return 1;
	}
	// matrix * vector
	MVector* l_vec2;
	if (get_param(L, -2, &l_vec2) == true)
	{
		MVector* l_vec;
		if (get_param(L, -1, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed MVector * MVector  mul.");
		}
		double l_res = (*l_vec2) * (*l_vec);
		lua_pushnumber(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::mul). Unexpected of arguments.");
}
int vufLuaMVector::div(lua_State* L)
{
	if (lua_isnumber(L, -1))
	{
		double l_val = (double)lua_tonumber(L, -1);
		if (l_val == 0.)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Error dividing by zero.");
		}
		l_val = 1. / l_val;
		MVector* l_vec;
		if (get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, "Failed MVector div. Got null MVector object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data((*l_vec) * l_val);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed (MVector::div). Unexpected of arguments.");

}