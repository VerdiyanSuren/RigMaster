#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>
#include <expressions/luaWrappers/vufLuaMEulerRotation.h>


using namespace vufRM;
bool vufLuaMEulerRotation::rotate_order_from_str(lua_State* L, int p_index, MEulerRotation::RotationOrder& p_res)
{
	const char* l_char_ptr = luaL_checkstring(L, p_index);
	if (l_char_ptr == nullptr)
	{
		return false;
	}
	std::string l_str(l_char_ptr);
	if (l_char_ptr[0] == 'x')
	{
		if (l_str == "xyz")
		{
			p_res = MEulerRotation::kXYZ;
			return true;
		}
		if (l_str == "xzy")
		{
			p_res = MEulerRotation::kXZY;
			return true;
		}
	}
	if (l_char_ptr[0] == 'y')
	{
		if (l_str == "yxz")
		{
			p_res = MEulerRotation::kYXZ;
			return true;
		}
		if (l_str == "yzx")
		{
			p_res = MEulerRotation::kYZX;
			return true;
		}
	}
	if (l_char_ptr[0] == 'z')
	{
		if (l_str == "zxy")
		{
			p_res = MEulerRotation::kZXY;
			return true;
		}
		if (l_str == "zyx")
		{
			p_res = MEulerRotation::kZYX;
			return true;
		}
	}
	return false;
}
std::string vufLuaMEulerRotation::rotate_order_as_string(const MEulerRotation& p_rot)
{
	std::string l_str;
	switch (p_rot.order)
	{
	case MEulerRotation::kXYZ:
		return "xyz";
	case MEulerRotation::kXZY:
		return "xzy";
	case MEulerRotation::kYXZ:
		return "yxz"; 
	case MEulerRotation::kYZX:
			return "yzx";
	case MEulerRotation::kZXY:
		return "zxy";
	case MEulerRotation::kZYX:
		return "zyx";
	default:
		break;
	}
	return "xyz";
}
int vufLuaMEulerRotation::decompose(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// decompose (MMatrix  )
	if (l_number_of_arguments == 1)
	{
		MMatrix *l_m;
		if (vufLuaMMatrix::get_param(L, -1, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:decompose) to get first argument. expect MMatrix");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(MEulerRotation::decompose(*l_m, MEulerRotation::kXYZ));
		return 1;
	}
	// decompose (MMatrix , RotationOrder )
	if (l_number_of_arguments == 2)
	{
		MMatrix* l_m;
		MEulerRotation::RotationOrder l_rotate_order;
		if (rotate_order_from_str(L, -1, l_rotate_order) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:decompose) to get second argument. expect rjtation order");
		}
		if (vufLuaMMatrix::get_param(L, -2, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:decompose) to get first argument. expect MMatrix");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(MEulerRotation::decompose(*l_m, l_rotate_order));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::decompose ). Unexpected count of arguments.");

}
int vufLuaMEulerRotation::create(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// 	MEulerRotation ()
	if (l_number_of_arguments == 0)
	{
		create_user_data(L);
		return 1;
	}
	// MEulerRotation(double xx, double yy, double zz)
	if (l_number_of_arguments == 3)
	{
		bool l_status;
		double l_x, l_y, l_z;
		VF_LUA_READ_NUMBER(L, -3, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:new). Expect first argument as number");
		VF_LUA_READ_NUMBER(L, -2, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:new). Expect secons argument as number");
		VF_LUA_READ_NUMBER(L, -1, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:new). Expect third argument as number");
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(MEulerRotation(l_x, l_y, l_z));
		return 1;
	}
	// MEulerRotation(double xx, double yy, double zz, RotationOrder ord = kXYZ)
	if (l_number_of_arguments == 4)
	{
		bool l_status;
		double l_x, l_y, l_z;
		MEulerRotation::RotationOrder l_rotate_order = MEulerRotation::kXYZ;
		VF_LUA_READ_NUMBER(L, -4, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:new). Expect first argument as number");
		VF_LUA_READ_NUMBER(L, -3, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:new). Expect secons argument as number");
		VF_LUA_READ_NUMBER(L, -2, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:new). Expect third argument as number");
		if (rotate_order_from_str(L, -1, l_rotate_order) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name," Failed (MEulerRotation) to get rotation order.")
		}		
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(MEulerRotation(l_x,l_y,l_z, l_rotate_order));
		return 1;
	}
	// MEulerRotation (MVector)
	// MEulerRotation(MMatrix)
	// MEulerRotation(MQuaternion)
	if (l_number_of_arguments == 1)
	{
		MVector* l_vec;
		MMatrix* l_m;
		MQuaternion* l_q;
		if (vufLuaMVector::get_param(		L, -1, &l_vec) == true)
		{
			auto l_wrap = create_user_data(L);
			l_wrap->set_data(MEulerRotation(*l_vec));
			return 1;
		}
		if (vufLuaMMatrix::get_param(		L, -1, &l_m) == true)
		{
			MEulerRotation l_r;
			l_r = *l_m;
			auto l_wrap = create_user_data(L);
			l_wrap->set_data(MEulerRotation(l_r));
			return 1;

		}
		if (vufLuaMQuaternion::get_param(	L, -1, &l_q) == true)
		{
			MEulerRotation l_r;
			l_r = *l_q;
			auto l_wrap = create_user_data(L);
			l_wrap->set_data(MEulerRotation(l_r));
			return 1;
		}
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:new(1 arg) to create MEulerRotation. Wrong arguments.Should be MVector,MMatrix or Mquaternion");
	}
	// MEulerRotation (const MVector &v, RotationOrder ord=kXYZ)
	// 
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec;
		MEulerRotation::RotationOrder l_rotate_order = MEulerRotation::kXYZ;
		if (vufLuaMVector::get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:new). Expect MVector as first argument.");
		}
		if (rotate_order_from_str(L, -1, l_rotate_order) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:new) to get rotation order.")
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(MEulerRotation(*l_vec, l_rotate_order));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:new) to create MEulerRotation. Wrong arguments.");
}
int vufLuaMEulerRotation::set_value(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// setValue(const MVector & v)
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec;
		MEulerRotation* l_rot;
		if (vufLuaMVector::get_param(L, -1, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get first argument. expect MVector");
		}
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setValue(*l_vec));
		return 1;
	}
	// setValue (const MVector &v, RotationOrder ord=kXYZ)
	if (l_number_of_arguments == 3)
	{
		MVector* l_vec;
		MEulerRotation* l_rot;
		MEulerRotation::RotationOrder l_rotate_order = MEulerRotation::kXYZ;
		if (rotate_order_from_str(L, -1, l_rotate_order) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation) to get rotation order.")
		}
		if (vufLuaMVector::get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get first argument. expect MVector");
		}
		if (get_param(L, -3, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setValue(*l_vec, l_rotate_order));
		return 1;
	}
	// setValue (double xx, double yy, double zz)
	if (l_number_of_arguments == 4)
	{
		bool l_status;
		double l_x, l_y, l_z;
		MEulerRotation* l_rot;
		VF_LUA_READ_NUMBER(L, -3, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect first argument as number");
		VF_LUA_READ_NUMBER(L, -2, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect secons argument as number");
		VF_LUA_READ_NUMBER(L, -1, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect third argument as number");
		if (get_param(L, -4, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setValue(l_x,l_y,l_z));
		return 1;
	}
	// setValue (double xx, double yy, double zz, RotationOrder ord=kXYZ)
	if (l_number_of_arguments == 5)
	{ 
		bool l_status;
		double l_x, l_y, l_z;
		MEulerRotation::RotationOrder l_rotate_order = MEulerRotation::kXYZ;
		MEulerRotation* l_rot;
		if (rotate_order_from_str(L, -1, l_rotate_order) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation) to get rotation order.")
		}

		VF_LUA_READ_NUMBER(L, -4, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect first argument as number");
		VF_LUA_READ_NUMBER(L, -3, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect secons argument as number");
		VF_LUA_READ_NUMBER(L, -2, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect third argument as number");
		if (get_param(L, -5, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setValue(l_x, l_y, l_z, l_rotate_order));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to parse agruments.");
}
int vufLuaMEulerRotation::as_quaternion(lua_State* L)
{
	MEulerRotation* l_rot;
	if (get_param(L, -1, &l_rot) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get MEulerRotation object");
	}
	auto l_wrap = vufLuaMQuaternion::create_user_data(L);
	l_wrap->set_data(l_rot->asQuaternion());
	return 1;
}
int vufLuaMEulerRotation::as_matrix(lua_State* L)
{
	MEulerRotation* l_rot;
	if (get_param(L, -1, &l_rot) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get MEulerRotation object");
	}
	auto l_wrap = vufLuaMMatrix::create_user_data(L);
	l_wrap->set_data(l_rot->asMatrix());
	return 1;
}
int vufLuaMEulerRotation::as_vector(lua_State* L)
{
	MEulerRotation* l_rot;
	if (get_param(L, -1, &l_rot) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation) to get MEulerRotation object");
	}
	auto l_wrap = vufLuaMVector::create_user_data(L);
	l_wrap->set_data(l_rot->asVector());
	return 1;
}
int vufLuaMEulerRotation::is_equivalent(lua_State* L)
{
	double l_tol = kEulerRotationEpsilon;
	int l_number_of_arguments = lua_gettop(L);
	// isEquivalent (const MEulerRotation)
	if (l_number_of_arguments == 2)
	{
		MEulerRotation* l_rot, * l_rot_to;
		if (get_param(L, -1, &l_rot_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent). Expect first argument as MEulerRotation.");
		}
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent) to get MEulerRotation.");
		}

		bool l_res = l_rot->isEquivalent(*l_rot_to);
		lua_pushboolean(L, l_res);
		return 1;
	}
	// isEquivalent (const MEulerRotation other, double tolerance) 
	if (l_number_of_arguments == 3)
	{
		bool l_status;
		MEulerRotation* l_rot, * l_rot_to;
		if (get_param(L, -2, &l_rot_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent). Expect first argument as MEulerRotation.");
		}
		if (get_param(L, -3, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent) to get MEulerRotation.");
		}
		VF_LUA_READ_NUMBER(L, -1, l_tol, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent). Expect  third argument as numbers");
		bool l_res = l_rot->isEquivalent(*l_rot_to, l_tol);
		lua_pushboolean(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent). Unexpected count of arguments.");
}
int vufLuaMEulerRotation::is_zero(lua_State* L)
{
	double l_tol = kEulerRotationEpsilon;
	int l_number_of_arguments = lua_gettop(L);
	// isZero ()
	if (l_number_of_arguments == 1)
	{
		MEulerRotation* l_rot;
		if (get_param(L, -1, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent) to get MEulerRotation.");
		}
		bool l_res = l_rot->isZero();
		lua_pushboolean(L, l_res);
		return 1;
	}
	// isZero ( double tolerance) 
	if (l_number_of_arguments == 2)
	{
		bool l_status;
		MEulerRotation* l_rot;
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent) to get MEulerRotation.");
		}
		VF_LUA_READ_NUMBER(L, -1, l_tol, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isEquivalent). Expect  third argument as numbers");
		bool l_res = l_rot->isZero(l_tol);
		lua_pushboolean(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::isZero). Unexpected count of arguments.");
}
int vufLuaMEulerRotation::increment_rotate(lua_State* L)
{
	//incrementalRotateBy (MVector axis, double angle)
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 3)
	{
		bool l_status;
		double l_val;
		MVector* l_vec;
		MEulerRotation* l_rot;
		VF_LUA_READ_NUMBER(L, -1, l_val, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::incrementalRotateBy). Expect  second argument as numbers");
		if (vufLuaMVector::get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::incrementalRotateBy) Expect first argument as MVector.");
		}
		if (get_param(L, -3, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::incrementalRotateBy) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->incrementalRotateBy(*l_vec, l_val));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::incrementalRotateBy). Unexpected count of arguments.");
}
int vufLuaMEulerRotation::reorder(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 2)
	{
		MEulerRotation* l_rot;
		MEulerRotation::RotationOrder l_rotate_order = MEulerRotation::kXYZ;
		if (rotate_order_from_str(L, -1, l_rotate_order) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:reorder) to get rotation order.")
		}
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:reorder) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->reorder( l_rotate_order));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::reorder ). Unexpected count of arguments.");
}
int vufLuaMEulerRotation::reorder_it(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 2)
	{
		MEulerRotation* l_rot;
		MEulerRotation::RotationOrder l_rotate_order = MEulerRotation::kXYZ;
		if (rotate_order_from_str(L, -1, l_rotate_order) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation:reorderIt) to get rotation order.")
		}
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:reorderIt) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->reorderIt(l_rotate_order));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::reorderIt ). Unexpected count of arguments.");

}
int vufLuaMEulerRotation::bound_it(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// boundIt ()
	if (l_number_of_arguments == 1)
	{
		MEulerRotation* l_rot;
		if (get_param(L, -1, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:boundIt) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->boundIt());
		return 1;
	}
	// boundIt (MEulerRotation)
	if (l_number_of_arguments == 2)
	{
		MEulerRotation* l_rot, *l_rot_to;
		if (get_param(L, -2, &l_rot_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:boundIt) to get MEulerRotation as argument.");
		}
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:boundIt) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->boundIt(*l_rot_to));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::boundIt ). Unexpected count of arguments.");
}
int vufLuaMEulerRotation::set_alternate_solution(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// setToAlternateSolution ()
	if (l_number_of_arguments == 1)
	{
		MEulerRotation* l_rot;
		if (get_param(L, -1, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToAlternateSolution) to get MEulerRotation object");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setToAlternateSolution());
		return 1;
	}
	// setToAlternateSolution (MEulerRotation)
	if (l_number_of_arguments == 2)
	{
		MEulerRotation* l_rot, * l_rot_to;
		if (get_param(L, -1, &l_rot_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToAlternateSolution) to get MEulerRotation as argument.");
		}
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToAlternateSolution) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setToAlternateSolution(*l_rot_to));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::setToAlternateSolution ). Unexpected count of arguments.");
}
int vufLuaMEulerRotation::set_closest_solution(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// setToClosestSolution (MEulerRotation)
	if (l_number_of_arguments == 2)
	{
		MEulerRotation* l_rot, * l_rot_to;
		if (get_param(L, -1, &l_rot_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestSolution) to get MEulerRotation as argument.");
		}
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestSolution) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setToClosestSolution(*l_rot_to));
		return 1;
	}
	// setToClosestSolution ( MEulerRotation, MEulerRotation )
	if (l_number_of_arguments == 3)
	{
		MEulerRotation* l_rot, * l_rot_src, * l_rot_dst;
		if (get_param(L, -1, &l_rot_dst) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestSolution) to get MEulerRotation as argument.");
		}
		if (get_param(L, -2, &l_rot_src) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestSolution) to get MEulerRotation as argument.");
		}
		if (get_param(L, -3, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestSolution) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setToClosestSolution(*l_rot_src, *l_rot_dst));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::setToClosestSolution ). Unexpected count of arguments.");
}
int vufLuaMEulerRotation::set_closest_cut(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// setToClosestCut (MEulerRotation)
	if (l_number_of_arguments == 2)
	{
		MEulerRotation* l_rot, * l_rot_to;
		if (get_param(L, -1, &l_rot_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestCut) to get MEulerRotation as argument.");
		}
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestCut) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setToClosestCut(*l_rot_to));
		return 1;
	}
	// setToClosestSolution ( MEulerRotation, MEulerRotation )
	if (l_number_of_arguments == 3)
	{
		MEulerRotation* l_rot, * l_rot_src, * l_rot_dst;
		if (get_param(L, -1, &l_rot_dst) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestCut) to get MEulerRotation as argument.");
		}
		if (get_param(L, -2, &l_rot_src) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestCut) to get MEulerRotation as argument.");
		}
		if (get_param(L, -3, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:setToClosestCut) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_rot->setToClosestCut(*l_rot_src, *l_rot_dst));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::setToClosestCut ). Unexpected count of arguments.");
}
int vufLuaMEulerRotation::mul(lua_State* L)
{
	// rotation * double
	if (lua_isnumber(L, -1))
	{
		MEulerRotation *l_rot;
		double l_val = lua_tonumber(L, -1);
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:mul) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data((*l_rot) * l_val);
		return 1;
	}
	// double * rotaion
	if (lua_isnumber(L, -2))
	{
		MEulerRotation* l_rot;
		double l_val = lua_tonumber(L, -2);
		if (get_param(L, -1, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:mul) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data((*l_rot) * l_val);
		return 1;
	}
	// rotation * rotation
	//luaL_testudata(L, ud, tname);
	MEulerRotation *l_rot2;
	if (get_param(L, -1, &l_rot2) == true)
	{
		MEulerRotation* l_rot;
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:mul) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data((*l_rot) * (*l_rot2));
		return 1;
	}
	// rotation * quaternion
	MQuaternion* l_q;
	if (vufLuaMQuaternion::get_param(L, -1, &l_q) == true)
	{
		MEulerRotation* l_rot;
		if (get_param(L, -2, &l_rot) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, "  Failed (MEulerRotation:mul) to get MEulerRotation object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data((*l_rot) * (*l_q));
		return 1;
	}	
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation::mul ). Unexpected of arguments.");
}
