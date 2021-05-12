#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>


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
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Wrong number of arguments.");
	}
	MVector* l_v_1, * l_v_2;
	if (get_param(L, -2, &l_v_1) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed to get MVector.");
	}
	if (get_param(L, -1, &l_v_2) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed. Expect first argumen as MVector.");
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
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed to get MVector.");
	}
	if (get_param(L, -1, &l_v_2) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed. Expect first argumen as MVector.");
	}
	double l_val = ((*l_v_1) * (*l_v_2));
	lua_pushnumber(L, l_val);
	return 1;
}

