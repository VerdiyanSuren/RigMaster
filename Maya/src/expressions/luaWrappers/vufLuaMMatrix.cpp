#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMEulerRotation.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>

using namespace vufRM;

int vufLuaMMatrix::set(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments != 4)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "Fail(MMatrix:set) wrong number of arguments. Expect 3");
	}
	//read i,j
	int l_i, l_j;
	double  l_val;
	// read and check value
	l_val	= (double)luaL_checknumber(	L, -1);
	l_j		= (int)luaL_checkinteger(	L, -2 );
	l_i		= (int)luaL_checkinteger(	L, -3);
	if (l_i < 0 || l_i >3)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " index of column is out of range");
	}
	if (l_j < 0 || l_j >3)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " index of row is out of range");
	}
	// matrix
	MMatrix* l_matr_ptr;
	if (get_param(L, -4, &l_matr_ptr) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed(MMatrix:get) to get MMatrix");
	}
	(*l_matr_ptr)[l_i][l_j] = l_val;
	return 0;
}
int vufLuaMMatrix::get(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments != 3)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " wrong number of arguments. Expect 2");
	}
	//read i,j
	int l_i, l_j;
	// read and check row
	l_j = (int)luaL_checkinteger(L, -1);
	// read and check column
	l_i = (int)luaL_checkinteger(L, -2);
	// check indecies
	if (l_i < 0 || l_i >3)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " index of column is out of range");
	}
	if (l_j < 0 || l_j >3)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " index of row is out of range");
	}
	// matrix
	MMatrix* l_matr_ptr;
	if (get_param(L, -3, &l_matr_ptr) == false)
	{
		VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed tp get matrix4");
	}
	double l_val = (*l_matr_ptr)[l_i][l_j];
	lua_pushnumber(L, l_val);
	return 1;
}
int vufLuaMMatrix::set_to_product(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	if (l_number_of_arguments == 3)
	{
		MMatrix* l_m1, * l_m2, * l_m;
		if (get_param(L, -1, &l_m2) == false)
		{
			VF_LUA_THROW_ERROR(L,vufLuaMayaStatic::g_mmatrix_tbl_name, "Failed(MMatrix:setToProduct) to get MMatrix as second arg.");
		}
		if (get_param(L, -2, &l_m1) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "Failed(MMatrix:setToProduct) to get MMatrix as first arg.");
		}
		if (get_param(L, -3, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "Failed(MMatrix:setToProduct) to get MMatrix object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_m->setToProduct(*l_m1, *l_m2));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "Failed(MMatrix:setToProduct) wrong number of args.")
}
int vufLuaMMatrix::is_equivalent(lua_State* L)
{
	double l_tol = MMatrix_kTol;
	int l_number_of_arguments = lua_gettop(L);
	// isEquivalent (const MEulerRotation)
	if (l_number_of_arguments == 2)
	{
		MMatrix* l_m, * l_m_to;
		if (get_param(L, -1, &l_m_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MMatrix::isEquivalent). Expect first argument as MMatrix.");
		}
		if (get_param(L, -2, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MMatrix::isEquivalent) to get MMatrix.");
		}

		bool l_res = l_m->isEquivalent(*l_m_to);
		lua_pushboolean(L, l_res);
		return 1;
	}
	// isEquivalent (const MEulerRotation other, double tolerance) 
	if (l_number_of_arguments == 3)
	{
		bool l_status;
		MMatrix* l_m, * l_m_to;
		if (get_param(L, -2, &l_m_to) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MMatrix::isEquivalent). Expect first argument as MMatrix.");
		}
		if (get_param(L, -3, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MMatrix::isEquivalent) to get MMatrix.");
		}
		VF_LUA_READ_NUMBER(L, -1, l_tol, l_status);
		VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MQuaternion::isEquivalent). Expect  third argument as numbers");
		bool l_res = l_m->isEquivalent(*l_m_to, l_tol);
		lua_pushboolean(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MMatrix::isEquivalent). Unexpected count of arguments.");
}
int vufLuaMMatrix::is_singular(lua_State* L)
{
	int l_number_of_arguments = lua_gettop(L);
	// isEquivalent (const MEulerRotation)
	if (l_number_of_arguments == 1)
	{
		MMatrix* l_m;
		if (get_param(L, -1, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MMatrix::isSingular) to get MMatrix.");
		}
		bool l_res = l_m->isSingular();
		lua_pushboolean(L, l_res);
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MMatrix::isSingular). Unexpected count of arguments.");
}
int vufLuaMMatrix::mul(lua_State* L)
{
	// matrix * double
	if (lua_isnumber(L, -1))
	{
		MMatrix* l_m;
		double l_val = (double)lua_tonumber(L, -1);
		if (get_param(L, -2, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "  Failed (MMutrix:mul) to get MMutrix object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_val * (*l_m));
		return 1;
	}
	// double * matrix
	if (lua_isnumber(L, -2))
	{
		MMatrix* l_m;
		double l_val = lua_tonumber(L, -2);
		if (get_param(L, -1, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "  Failed (MMutrix:mul) to get MMutrix object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data(l_val * (*l_m));
		return 1;
	}
	// matrix * matrix
	MMatrix* l_m2;
	if (get_param(L, -1, &l_m2) == true)
	{
		MMatrix* l_m;
		if (get_param(L, -2, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "  Failed (MMutrix:mul) to get MMutrix object.");
		}
		auto l_wrap = create_user_data(L);
		l_wrap->set_data((*l_m) * (*l_m2));
		return 1;
	}
	MPoint* l_p;
	if (vufLuaMPoint::get_param(L, -1, &l_p) == true)
	{
		MMatrix* l_m;
		if (get_param(L, -2, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "  Failed (MMutrix:mul) to get MMutrix object.");
		}
		auto l_wrap = vufLuaMPoint::create_user_data(L);
		l_wrap->set_data( (*l_p) * (*l_m) );
		return 1;
	}
	MVector* l_v;
	if (vufLuaMVector::get_param(L, -1, &l_v) == true)
	{
		MMatrix* l_m;
		if (get_param(L, -2, &l_m) == false)
		{
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, "  Failed (MMutrix:mul) to get MMutrix object.");
		}
		auto l_wrap = vufLuaMVector::create_user_data(L);
		l_wrap->set_data((*l_v) * (*l_m));
		return 1;
	}
	VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed (MMatrix::mul). Unexpected  of arguments.");
}
