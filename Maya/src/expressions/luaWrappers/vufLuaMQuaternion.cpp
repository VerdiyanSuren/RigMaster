#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMEulerRotation.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>

using namespace vufRM;

int vufLuaMQuaternion::create(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// MQuaternion ()
	if (l_number_of_arguments == 0)
	{
		create_user_data(L);
		return 1;
	}
	// 	MQuaternion (double, double, double, double )
	if (l_number_of_arguments == 4)
	{
		bool l_status = false;
		double l_x, l_y, l_z, l_w;
		VF_LUA_READ_NUMBER(L, -4, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion:new). Expect first argument as number");
		VF_LUA_READ_NUMBER(L, -3, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion:new). Expect secons argument as number");
		VF_LUA_READ_NUMBER(L, -2, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion:new). Expect third argument as number");
		VF_LUA_READ_NUMBER(L, -1, l_w, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion:new). Expect fourth argument as number");
		auto l_wrapper = create_user_data(L);
		MQuaternion& l_quat = l_wrapper->get_data();
		l_quat.x = l_x;
		l_quat.y = l_y;
		l_quat.z = l_z;
		l_quat.w = l_w;
		return 1;
	}
	// Constructor by 3 arguments. MQuaternion (const MVector &a, const MVector &b, double angleFactor)
	if (l_number_of_arguments == 3)
	{
		bool l_status = false;
		double l_angle;
		MVector* l_vec_1, * l_vec_2;
		if (vufLuaMVector::get_param(L, -3, &l_vec_1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion:new). Expect first argument as MVector. ");
		}
		if (vufLuaMVector::get_param(L, -2, &l_vec_2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion:new). Expect second argument as MVector. ");
		}
		VF_LUA_READ_NUMBER(L, -1, l_angle, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion:new). Expect fourth argument as number");
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(MQuaternion(*l_vec_1, *l_vec_2, l_angle));
		return 1;
	}
	// MQuaternion( MVector , MVector ) or
	// MQuaternion (double angle, const MVector &axisb)
	if (l_number_of_arguments == 2)
	{
		if (lua_isnumber(L, -2))
		{
			//  MQuaternion (double angle, const MVector &axisb)
			bool l_status = false;
			double l_angle;
			MVector* l_vec_1;
			VF_LUA_READ_NUMBER(L, -2, l_angle, l_status);
			if (vufLuaMVector::get_param(L, -1, &l_vec_1) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion:new). Expect second argument as MVector. ");
			}
			auto l_wrapper = create_user_data(L);
			l_wrapper->set_data(MQuaternion(l_angle, *l_vec_1));
			return 1;
		}
		// MQuaternion(const MVector & a, const MVector & b) 
		MVector* l_vec_1, * l_vec_2;
		if (vufLuaMVector::get_param(L, -1, &l_vec_2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion:new). Expect second argument as MVector. ");
		}
		if (vufLuaMVector::get_param(L, -2, &l_vec_1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion:new). Expect first argument as MVector. ");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(MQuaternion(*l_vec_1, *l_vec_2));
		return 1;
	}
	// MQuaternion (MMatrix)
	if(l_number_of_arguments == 1)
	{
		MEulerRotation* l_r; 
		MMatrix *l_m;
		if (vufLuaMMatrix::get_param(L, -1, &l_m) == true)
		{
			MQuaternion l_q;
			l_q = *l_m;
			auto l_wrap = create_user_data(L);
			l_wrap->set_data(l_q);			
			return 1;
		}
		if (vufLuaMEulerRotation::get_param(L, -1, &l_r) == true)
		{
			MQuaternion l_q;
			l_q = *l_r;
			auto l_wrap = create_user_data(L);
			l_wrap->set_data(l_q);
			return 1;
		}
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion:new). Expect single argument as MMatrtix or MEulerRotate. ");
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " Failed to create MQuaternon:new. Wrong arguments.");
}
int vufLuaMQuaternion::slerp_l(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 3)
	{
		MQuaternion *l_q1, *l_q2;
		double l_val;
		if (lua_isnumber(L, -1) == 0)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect last argument as number.");
		}
		l_val = (double)lua_tonumber(L, -1);
		if (get_param(L, -2, &l_q2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect second argument as MQuaternion.");
		}
		if (get_param(L, -3, &l_q1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect first argument as MQuaternion.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(slerp(*l_q1, *l_q2, l_val));		
		return 1;
	}
	if (l_number_of_arguments == 4)
	{
		MQuaternion* l_q1, * l_q2;
		double l_val;
		int l_spin;
		if (lua_isnumber(L, -1) == 0)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect last argument as number.");
		}
		l_spin = (int)lua_tonumber(L, -1);
		if (lua_isnumber(L, -2) == 0)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect thirth argument as number.");
		}
		l_val = (double)lua_tonumber(L, -2);
		if (get_param(L, -3, &l_q2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect second argument as MQuaternion.");
		}
		if (get_param(L, -4, &l_q1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect first argument as MQuaternion.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(slerp(*l_q1, *l_q2, l_val,l_spin));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " Failed to create MQuaternon:slerp. Wrong arguments.");
}
int vufLuaMQuaternion::dslerp_l(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 4)
	{
		MQuaternion* l_q1, * l_q2;
		double l_val;
		int l_dir;
		if (lua_isnumber(L, -1) == 0)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:dslerp) Expect last argument as number.");
		}
		l_dir = (int)lua_tonumber(L, -1);
		if (lua_isnumber(L, -2) == 0)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:dslerp) Expect thirth argument as number.");
		}
		l_val = (double)lua_tonumber(L, -2);
		if (get_param(L, -3, &l_q2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:dslerp) Expect second argument as MQuaternion.");
		}
		if (get_param(L, -4, &l_q1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:dslerp) Expect first argument as MQuaternion.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(dslerp(*l_q1, *l_q2, l_val, l_dir));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " Failed to create MQuaternon:dslerp. Wrong arguments.");
}
int vufLuaMQuaternion::squad_l(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 5)
	{
		MQuaternion* l_q1, *l_q2, *l_q3, *l_q4;
		double l_val;
		if (lua_isnumber(L, -1) == 0)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect last argument as number.");
		}
		l_val = (double)lua_tonumber(L, -1);
		if (get_param(L, -2, &l_q4) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect fourth argument as MQuaternion.");
		}
		if (get_param(L, -3, &l_q3) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect third argument as MQuaternion.");
		}
		if (get_param(L, -4, &l_q2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect second argument as MQuaternion.");
		}
		if (get_param(L, -5, &l_q1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect first argument as MQuaternion.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(squad(*l_q1, *l_q2,*l_q3, *l_q4, l_val));
		return 1;
	}
	if (l_number_of_arguments == 6)
	{
		MQuaternion* l_q1, * l_q2, * l_q3, * l_q4;
		double l_val;
		int l_spin;
		if (lua_isnumber(L, -1) == 0)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect last argument as number.");
		}
		l_spin = (int)lua_tonumber(L, -1);

		if (lua_isnumber(L, -2) == 0)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect fifth argument as number.");
		}
		l_val = (double)lua_tonumber(L, -2);
		if (get_param(L, -3, &l_q4) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect fourth argument as MQuaternion.");
		}
		if (get_param(L, -4, &l_q3) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect third argument as MQuaternion.");
		}
		if (get_param(L, -5, &l_q2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect second argument as MQuaternion.");
		}
		if (get_param(L, -6, &l_q1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:squad) Expect first argument as MQuaternion.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(squad(*l_q1, *l_q2, *l_q3, *l_q4, l_val, l_spin));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " Failed to create MQuaternon:squad. Wrong arguments.");
}
int vufLuaMQuaternion::squad_pt_l(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 3)
	{
		MQuaternion *l_q1, *l_q2, *l_q3;
		if (get_param(L, -1, &l_q3) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect last argument as MQuaternion.");
		}
		if (get_param(L, -2, &l_q2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect second argument as MQuaternion.");
		}
		if (get_param(L, -3, &l_q1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed(MQuaternion:slerp) Expect first argument as MQuaternion.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(squadPt(*l_q1, *l_q2,*l_q3));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " Failed to create MQuaternon:squadPt. Wrong arguments.");
}
int vufLuaMQuaternion::as_matrix(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 1)
	{
		MQuaternion* l_quat;
		if (get_param(L, -1, &l_quat) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternon:asMatrix) to get MQuaternion.");
		}
		auto l_matr_wrapper = vufLuaMMatrix::create_user_data(L);
		l_matr_wrapper->set_data(l_quat->asMatrix());
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternon:asMatrix) to parse arguments.");
}
int vufLuaMQuaternion::as_euler_rotation(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 1)
	{
		MQuaternion* l_quat;
		if (get_param(L, -1, &l_quat) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternon:asEulerRotation) to get MQuaternion.");
		}
		auto l_euler_wrapper =  vufLuaMEulerRotation::create_user_data(L);
		l_euler_wrapper->set_data(l_quat->asEulerRotation());
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternon:asEulerRotation) to parse arguments.");
}
int vufLuaMQuaternion::set_axis_angle(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 3)
	{
		MVector* l_v;
		MQuaternion* l_q;
		bool l_status;
		double l_angle;
		VF_LUA_READ_NUMBER(L, -1, l_angle, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternon:setAxisAngle). Expect last argument as number");
		if (vufLuaMVector::get_param(L, -2, &l_v) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternon:setAxisAngle). Expect MVector as first argument.");
		}
		if (get_param(L, -3, &l_q) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "  Failed (MQuaternon:setAxisAngle) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_q->setAxisAngle(*l_v, l_angle));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternon:setAxisAngle) to parse arguments.");

}
int vufLuaMQuaternion::get_axis_angle(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 1)
	{
		MQuaternion* l_q;
		MVector l_v;
		double l_angle;
		if (get_param(L, -1, &l_q) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "  Failed (MQuaternon:getAxisAngle) to get MQuaternion object");
		}		
		bool l_bool = l_q->getAxisAngle(l_v, l_angle);
		auto l_wrap = vufLuaMVector::create_user_data(L);
		l_wrap->set_data(l_v);
		lua_pushnumber(L, l_angle);
		lua_pushboolean(L, l_bool);
		return 3;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternon:getAxisAngle) to parse arguments.");
}
int vufLuaMQuaternion::set_axis_x(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 2)
	{
		MQuaternion* l_q;
		double l_angle;
		bool l_status;
		VF_LUA_READ_NUMBER(L, -1, l_angle, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion:setToXAxis). Expect argument as number");
		if (get_param(L, -2, &l_q) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "  Failed (MQuaternon:setToXAxis) to get MQuaternion object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_q->setToXAxis(l_angle));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternon:setToXAxis) to parse arguments.");
}
int vufLuaMQuaternion::set_axis_y(lua_State* L)
{
	MQuaternion* l_q;
	double l_angle;
	bool l_status;
	VF_LUA_READ_NUMBER(L, -1, l_angle, l_status);
	VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion:setToYAxis). Expect argument as number");
	if (get_param(L, -2, &l_q) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "  Failed (MQuaternon:setToYAxis) to get MQuaternion object");
	}
	auto l_wrap = create_user_data(L);
	l_wrap->set_data(l_q->setToYAxis(l_angle));
	return 1;
}
int vufLuaMQuaternion::set_axis_z(lua_State* L)
{
	MQuaternion* l_q;
	double l_angle;
	bool l_status;
	VF_LUA_READ_NUMBER(L, -1, l_angle, l_status);
	VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion:setToZAxis). Expect argument as number");
	if (get_param(L, -2, &l_q) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "  Failed (MQuaternon:setToZAxis) to get MQuaternion object");
	}
	auto l_wrap = create_user_data(L);
	l_wrap->set_data(l_q->setToZAxis(l_angle));
	return 1;
}
int vufLuaMQuaternion::is_equivalent(lua_State* L)
{
	double l_tol = kQuaternionEpsilon;
	int l_number_of_arguments = lua_gettop(L);
	// isEquivalent (const MEulerRotation)
	if (l_number_of_arguments == 2)
	{
		MQuaternion* l_q, * l_q_to;
		if (get_param(L, -1, &l_q_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MQuaternion::isEquivalent). Expect first argument as MQuaternion.");
		}
		if (get_param(L, -2, &l_q) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MQuaternion::isEquivalent) to get MQuaternion.");
		}

		bool l_res = l_q->isEquivalent(*l_q_to);
		lua_pushboolean(L, l_res);
		return 1;
	}
	// isEquivalent (const MEulerRotation other, double tolerance) 
	if (l_number_of_arguments == 3)
	{
		bool l_status;
		MQuaternion* l_q, * l_q_to;
		if (get_param(L, -2, &l_q_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MQuaternion::isEquivalent). Expect first argument as MQuaternion.");
		}
		if (get_param(L, -3, &l_q) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MQuaternion::isEquivalent) to get MQuaternion.");
		}
		VF_LUA_READ_NUMBER(L, -1, l_tol, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MQuaternion::isEquivalent). Expect  third argument as numbers");
		bool l_res = l_q->isEquivalent(*l_q_to, l_tol);
		lua_pushboolean(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MQuaternion::isEquivalent). Unexpected count of arguments.");
}
int vufLuaMQuaternion::mul(lua_State* L)
{
	// quaternion * double
	if (lua_isnumber(L, -1))
	{
		MQuaternion* l_q;
		double l_val = lua_tonumber(L, -1);
		if (get_param(L, -2, &l_q) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MQuaternion:mul) to get MQuaternion object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_val *(*l_q));
		return 1;
	}
	// double * quaternion
	if (lua_isnumber(L, -2))
	{
		MQuaternion* l_q;
		double l_val = lua_tonumber(L, -2);
		if (get_param(L, -1, &l_q) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MQuaternion:mul) to get MQuaternion object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_val * (*l_q) );
		return 1;
	}
	// quaternion * quaternion
	//luaL_testudata(L, ud, tname);
	MQuaternion* l_q2;
	if (get_param(L, -1, &l_q2) == true)
	{
		MQuaternion* l_q;
		if (get_param(L, -2, &l_q) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MQuaternion:mul) to get MQuaternion object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data((*l_q) * (*l_q2));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MQuaternion::mul ). Unexpected of arguments.");
}
