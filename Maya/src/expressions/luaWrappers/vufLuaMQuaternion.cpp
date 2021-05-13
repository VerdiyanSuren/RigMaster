#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>

using namespace vufRM;

int vufLuaMQuaternion::create(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// Just a simple constructor. No params. 	MQuaternion ()
	if (l_number_of_arguments == 0)
	{
		create_user_data(L);
		return 1;
	}
	// Constructor by 4 doubles. MQuaternion (double xx, double yy, double zz, double ww)
	if (l_number_of_arguments == 4)
	{
		bool l_status = false;
		double l_x, l_y, l_z, l_w;
		VF_LUA_READ_NUMBER(L, -4, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion). Expect first argument as number");
		VF_LUA_READ_NUMBER(L, -3, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion). Expect secons argument as number");
		VF_LUA_READ_NUMBER(L, -2, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion). Expect third argument as number");
		VF_LUA_READ_NUMBER(L, -1, l_w, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion). Expect fourth argument as number");
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
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion). Expect first argument as MVector. ");
		}
		if (vufLuaMVector::get_param(L, -2, &l_vec_2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion). Expect second argument as MVector. ");
		}
		VF_LUA_READ_NUMBER(L, -1, l_angle, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mquat_tbl_name, " Failed (MQuaternion). Expect fourth argument as number");
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(MQuaternion(*l_vec_1, *l_vec_2, l_angle));
		return 1;
	}
	// MQuaternion(const MVector & a, const MVector & b) or
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
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion). Expect second argument as MVector. ");
			}
			auto l_wrapper = create_user_data(L);
			l_wrapper->set_data(MQuaternion(l_angle, *l_vec_1));
			return 1;
		}
		// MQuaternion(const MVector & a, const MVector & b) 
		MVector* l_vec_1, * l_vec_2;
		if (vufLuaMVector::get_param(L, -1, &l_vec_2) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion). Expect second argument as MVector. ");
		}
		if (vufLuaMVector::get_param(L, -2, &l_vec_1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed (MQuaternion). Expect first argument as MVector. ");
		}
		auto l_wrapper = create_user_data(L);
		l_wrapper->set_data(MQuaternion(*l_vec_1, *l_vec_2));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, " Failed to create MQuaternon. Wrong arguments.");
}
int vufLuaMQuaternion::as_matrix(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 1)
	{
		MQuaternion* l_quat;
		if (get_param(L, -1, &l_quat) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed to get MQuaternion.");
		}
		MMatrix l_m = l_quat->asMatrix();
		auto l_matr_wrapper = vufLuaMMatrix::create_user_data(L);
		l_matr_wrapper->set_data(l_m);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mquat_tbl_name, "Failed to parse arguments.");
}
