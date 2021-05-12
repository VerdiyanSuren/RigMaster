#ifndef VF_LUA_MMATRIX_H
#define VF_LUA_MMATRIX_H

#include <vufLuaWrapUtil.h>
#include <expressions/luaWrappers/vufLuaMayaStatic.h>
#include <expressions/luaWrappers/vufLuaMayaDataWrapper.h>
#include <coreUtils/vufStringUtils.h>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <sstream>


namespace vufRM
{
	class vufLuaMMatrix
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mmatrix_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new",			create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",			copy);
			VF_LUA_ADD_TABLE_FIELD(L, "get",			get);
			VF_LUA_ADD_TABLE_FIELD(L, "set",			set);
			VF_LUA_ADD_TABLE_FIELD(L, "transpose",		transpose);
			VF_LUA_ADD_TABLE_FIELD(L, "setToIdentity",	to_identity);
			VF_LUA_ADD_TABLE_FIELD(L, "inverse",		inverse);
			VF_LUA_ADD_TABLE_FIELD(L, "adjoint",		adjoint);
			VF_LUA_ADD_TABLE_FIELD(L, "homogenize",		homogenize);
			VF_LUA_ADD_TABLE_FIELD(L, "det4x4",			det4x4);
			VF_LUA_ADD_TABLE_FIELD(L, "det3x3",			det3x3);
			VF_LUA_ADD_TABLE_FIELD(L, "isSingular",		is_singular);

			//VF_LUA_ADD_TABLE_FIELD(L, "parallel_to", parallel_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "distance_to", distance_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "rotateBy", distance_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "rotateTo", distance_to);

			///VF_LUA_ADD_TABLE_FIELD(L, "dot", dot);
			//VF_LUA_ADD_TABLE_FIELD(L, "cross", cross);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mmatrix_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div", div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", new_index);
		}
		/** bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_SET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		/** bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		/** bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>** p_res_ptr)*/
		VF_LUA_GET_USER_DATA_PARAM(	vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		/** create as reference of new user data and push to stack
		* vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)	*/
		VF_LUA_CREATE_USER_NEW_REF(	vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		/** create mew user data and push to stack
		*  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)*/
		VF_LUA_CREATE_USER_DATA(	vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
	private:
		static int create(lua_State* L)
		{
			create_user_data(L);
			return 1;
		}
		VF_LUA_IMPLEMENT_COPY(vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper);
		VF_LUA_IMPLEMENT_DESTROY(vufLuaMayaStatic::g_mmatrix_meta_name, vufLuaMMattrixWrapper);

		static int set(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 4)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " wrong number of arguments. Expect 3");
			}
			bool l_status;
			//read i,j
			int l_i, l_j;
			double  l_val;
			// read and check value
			VF_LUA_READ_NUMBER(L, -3, l_val, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " value has to be number");
			}
			// read and check row
			VF_LUA_READ_INTEGER(L, -2, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " row has to be number");
			}
			// read and check column
			VF_LUA_READ_INTEGER(L, -1, l_j, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " column has to be number");
			}
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
			if (get_param(L, -4, &l_matr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed tp get matrix4");
			}
			(*l_matr_ptr)[l_i][l_j] = l_val;
			return 0;
		}
		static int get(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " wrong number of arguments. Expect 2");
			}
			bool l_status;
			//read i,j
			int l_i, l_j;
			// read and check row
			VF_LUA_READ_INTEGER(L, -1, l_j, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " row has to be number");
			}
			// read and check column
			VF_LUA_READ_INTEGER(L, -2, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " column has to be number");
			}
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

		static int to_string(lua_State* L)
		{
			MMatrix* l_mat_ptr;
			if (get_param(L, -1, &l_mat_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed tp get MMatrix object.");
			}
			std::stringstream l_ss;
			l_ss << "[";
			for (int i = 0; i < 4; ++i)
			{
				l_ss << "[";
				for (int j = 0; j < 3; ++j)
				{
					l_ss << (*l_mat_ptr)[i][j] << ",";
				}
				l_ss << (*l_mat_ptr)[i][3];
				l_ss << "]";
			}
			l_ss << "]";
			lua_pushstring(L, l_ss.str().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaMayaStatic::g_mmatrix_tbl_name);
			return 1;
		}

		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix,	vufLuaMMattrixWrapper, transpose,		transpose);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_VOID(	vufLuaMayaStatic::g_mmatrix_meta_name,			vufLuaMMattrixWrapper, setToIdentity,	to_identity );
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_VOID(	vufLuaMayaStatic::g_mmatrix_meta_name,			vufLuaMMattrixWrapper, inverse,			inverse);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix,	vufLuaMMattrixWrapper, adjoint,			adjoint);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix,	vufLuaMMattrixWrapper, homogenize,		homogenize);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaMayaStatic::g_mmatrix_meta_name,			vufLuaMMattrixWrapper, det4x4,			det4x4);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaMayaStatic::g_mmatrix_meta_name,			vufLuaMMattrixWrapper, det3x3,			det3x3);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaMayaStatic::g_mmatrix_meta_name,			vufLuaMMattrixWrapper, isSingular,		is_singular);

		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper, sub);
		static int unm(lua_State* L)
		{
			auto l_arg_ptr = (vufLuaMMattrixWrapper*)luaL_checkudata(L, -1, vufLuaMayaStatic::g_mmatrix_meta_name);
			if (l_arg_ptr != nullptr)
			{
				auto l_wrapper = create_user_data(L);
				MMatrix l_matr = l_arg_ptr->get_data();
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						l_matr[i][j] = -l_matr[i][j];
					}
				}
				l_wrapper->set_data(l_matr);
				return 1;
			}
			VF_LUA_THROW_ERROR(L,vufLuaMayaStatic::g_mmatrix_tbl_name, " faile to get MMatrix object");
		}
		VF_LUA_IMPLEMENT_TYPE_MUL_TYPE(vufLuaMayaStatic::g_mmatrix_meta_name, MMatrix, vufLuaMMattrixWrapper, mul);

		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			MMatrix* l_matr_ptr;
			if (get_param(L, -2, &l_matr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Failed to get matrix4 userdata");
			}
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key != nullptr)
			{
				lua_getglobal(L, vufLuaMayaStatic::g_mmatrix_tbl_name);
				lua_pushstring(L, l_key);
				lua_rawget(L, -2);
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mmatrix_tbl_name, " Unexpected MMatrix error");
		}
	};
}
#endif // !VF_LUA_MMATRIX_H
