#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>

using namespace vufRM;

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