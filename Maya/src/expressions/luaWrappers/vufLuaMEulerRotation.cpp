#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>
#include <expressions/luaWrappers/vufLuaMEulerRotaion.h>


using namespace vufRM;

int vufLuaMEulerRotation::create(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// 	MEulerRotation ()
	if (l_number_of_arguments == 0)
	{
		create_user_data(L);
		return 1;
	}
	// MEulerRotation(double xx, double yy, double zz, RotationOrder ord = kXYZ)
	if (l_number_of_arguments == 4)
	{
		bool l_status;
		double l_x, l_y, l_z;
		VF_LUA_READ_NUMBER(L, -4, l_x, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect first argument as number");
		VF_LUA_READ_NUMBER(L, -3, l_y, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect secons argument as number");
		VF_LUA_READ_NUMBER(L, -2, l_z, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect third argument as number");
		const char* l_char_ptr = luaL_checkstring(L, -1);
		if (l_char_ptr == nullptr)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect string as rotate order.");
		}
		std::string l_str(l_char_ptr);
		if (l_char_ptr[0] == 'x')
		{
			if (l_str == "xyz")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(l_x,l_y,l_z, MEulerRotation::kXYZ));
				return 1;
			}
			if (l_str == "xzy")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(l_x, l_y, l_z, MEulerRotation::kXZY));
				return 1;
			}
		}
		if (l_char_ptr[0] == 'y')
		{
			if (l_str == "yxz")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(l_x, l_y, l_z, MEulerRotation::kYXZ));
				return 1;
			}
			if (l_str == "yzx")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(l_x, l_y, l_z, MEulerRotation::kYZX));
				return 1;
			}
		}
		if (l_char_ptr[0] == 'z')
		{
			if (l_str == "zxy")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(l_x, l_y, l_z, MEulerRotation::kZXY));
				return 1;
			}
			if (l_str == "zyx")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(l_x, l_y, l_z, MEulerRotation::kZYX));
				return 1;
			}
		}
	}
	// MEulerRotation (const MVector &v, RotationOrder ord=kXYZ)
	if (l_number_of_arguments == 2)
	{
		MVector* l_vec;
		if (vufLuaMVector::get_param(L, -2, &l_vec) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect MVector as first argument.");
		}
		const char* l_char_ptr = luaL_checkstring(L, -1);
		if (l_char_ptr == nullptr)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed (MEulerRotation). Expect string as rotate order.");
		}
		std::string l_str(l_char_ptr);
		if (l_char_ptr[0] == 'x')
		{
			if (l_str == "xyz")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(*l_vec, MEulerRotation::kXYZ));
				return 1;
			}
			if (l_str == "xzy")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(*l_vec, MEulerRotation::kXZY));
				return 1;
			}
		}
		if (l_char_ptr[0] == 'y')
		{
			if (l_str == "yxz")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(*l_vec, MEulerRotation::kYXZ));
				return 1;
			}
			if (l_str == "yzx")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(*l_vec, MEulerRotation::kYZX));
				return 1;
			}
		}
		if (l_char_ptr[0] == 'z')
		{
			if (l_str == "zxy")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(*l_vec, MEulerRotation::kZXY));
				return 1;
			}
			if (l_str == "zyx")
			{
				auto l_wrap = create_user_data(L);
				l_wrap->set_data(MEulerRotation(*l_vec, MEulerRotation::kZYX));
				return 1;
			}
		}
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_meuler_tbl_name, " Failed to create MEulerRotatipm. Wrong arguments.");

}
int vufLuaMEulerRotation::set_value(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
}
