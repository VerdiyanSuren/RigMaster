#ifndef  VF_LUA_MVECTOR_ARRAS_H
#define VF_LUA_MVECTOR_ARRAS_H

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
	class vufLuaMVectorArray
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mvec_arr_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new", create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "setLength",	set_length);
			VF_LUA_ADD_TABLE_FIELD(L, "length",		length);
			VF_LUA_ADD_TABLE_FIELD(L, "set", set);
			VF_LUA_ADD_TABLE_FIELD(L, "get", get);
			VF_LUA_ADD_TABLE_FIELD(L, "clear", clear);
			VF_LUA_ADD_TABLE_FIELD(L, "remove", remove);
			VF_LUA_ADD_TABLE_FIELD(L, "insert", insert);
			VF_LUA_ADD_TABLE_FIELD(L, "append", append);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mvec_arr_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
		}
		VF_LUA_SET_USER_DATA_GLOBAL(	vufLuaMayaStatic::g_mvec_arr_meta_name, MVectorArray, vufLuaMVectorArrayWrapper);
		VF_LUA_SET_USER_DATA_GLOBAL_REF(vufLuaMayaStatic::g_mvec_arr_meta_name, MVectorArray, vufLuaMVectorArrayWrapper);
		VF_LUA_GET_USER_DATA_GLOBAL(	vufLuaMayaStatic::g_mvec_arr_meta_name, MVectorArray, vufLuaMVectorArrayWrapper);
		VF_LUA_GET_USER_DATA_PARAM(		vufLuaMayaStatic::g_mvec_arr_meta_name,	MVectorArray, vufLuaMVectorArrayWrapper);
		VF_LUA_CREATE_USER_NEW_REF(		vufLuaMayaStatic::g_mvec_arr_meta_name,	MVectorArray, vufLuaMVectorArrayWrapper);
		VF_LUA_CREATE_USER_DATA(		vufLuaMayaStatic::g_mvec_arr_meta_name,	MVectorArray, vufLuaMVectorArrayWrapper);
	private:
		static int create(lua_State* L)
		{
			create_user_data(L);
			return 1;
		}
		VF_LUA_IMPLEMENT_COPY_CONSTRUCTOR(	vufLuaMayaStatic::g_mvec_arr_meta_name, MVectorArray, vufLuaMVectorArrayWrapper);
		VF_LUA_IMPLEMENT_DESTROY(			vufLuaMayaStatic::g_mvec_arr_meta_name, vufLuaMVectorArrayWrapper);
		
		static int set(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " wrong number of arguments. Expect 2");
			}
			bool l_status;
			//read i
			int l_i;
			MVector*  l_vec;
			// read and check value
			if (vufLuaMVector::get_param(L, -2, &l_vec) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " expect MVectoras argument.");
			}
			// read and check row
			VF_LUA_READ_INTEGER(L, -1, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " index has to be integer");
			}
			// matrix
			MVectorArray* l_vec_ptr;
			if (get_param(L, -3, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed tp get array object");
			}

			// check indecies
			if (l_i < 0 || l_i >= (int)l_vec_ptr->length())
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " index is out of range");
			}
			(*l_vec_ptr)[l_i] = *l_vec;
			return 0;
		}
		static int get(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " wrong number of arguments. Expect 1");
			}
			bool l_status;
			//read i
			int l_i;
			// read and check row
			VF_LUA_READ_INTEGER(L, -1, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " index has to be integer");
			}
			// read and check column
			// check indecies
			// matrix
			MVectorArray *l_vec_arr_ptr;
			if (get_param(L, -2, &l_vec_arr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed tp get MVectorArray");
			}
			if (l_i < 0 || l_i >= (int)l_vec_arr_ptr->length())
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " index of column is out of range");
			}
			
			auto l_vec_wrapper = vufLuaMVector::create_user_data(L);
			l_vec_wrapper->set_data((*l_vec_arr_ptr)[l_i]);
			return 1;
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaMayaStatic::g_mvec_arr_meta_name, vufLuaMVectorArrayWrapper, length, length);
		static int set_length(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				return luaL_error(L, "expect exactly  1 numerical  arguments");
			}
			bool l_status;
			int l_size;
			VF_LUA_READ_INTEGER(L, -1, l_size, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_meta_name, " expect integer as new length argument.");
			}
			MVectorArray* l_res_ptr;
			if (get_param(L, -2, &l_res_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_meta_name, "failed to get MVectorArray object.");
			}
			l_res_ptr->setLength(l_size);
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_VOID(vufLuaMayaStatic::g_mvec_arr_meta_name, vufLuaMVectorArrayWrapper, clear, clear);
		static int remove(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 1)
			{
				return luaL_error(L, "expect exactly  1 numerical  arguments");
			}
			bool l_status;
			int l_index;
			VF_LUA_READ_INTEGER(L, -1, l_index, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_meta_name, " expect integer as new length argument.");
			}
			MVectorArray* l_res_ptr;
			if (get_param(L, -2, &l_res_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_meta_name, "failed to get MVectorArray object.");
			}
			l_res_ptr->remove(l_index);
			return 0;
		}
		static int insert(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " wrong number of arguments. Expect 2");
			}
			bool l_status;
			//read i
			int l_i;
			MVector* l_vec;
			// read and check value
			if (vufLuaMVector::get_param(L, -2, &l_vec) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " expect MVector as argument.");
			}
			// read and check row
			VF_LUA_READ_INTEGER(L, -1, l_i, l_status);
			if (l_status == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " index has to be integer");
			}
			// matrix
			MVectorArray* l_vec_ptr;
			if (get_param(L, -3, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed tp get array object");
			}

			// check indecies
			if (l_i < 0 || l_i >= (int)l_vec_ptr->length())
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " index is out of range");
			}
			l_vec_ptr->insert(*l_vec, l_i);
			return 0;
		}
		static int append(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " wrong number of arguments. Expect 1");
			}			
			MVector* l_vec;
			// read and check value
			if (vufLuaMVector::get_param(L, -2, &l_vec) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " expect MVector as argument.");
			}

			MVectorArray* l_vec_arr_ptr;
			if (get_param(L, -2, &l_vec_arr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed tp get MVectorArray");
			}
			l_vec_arr_ptr->append(*l_vec);
			return 0;
		}
		static int to_string(lua_State* L)
		{
			MVectorArray* l_vec_ptr;
			if (get_param(L, -1, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed tp get vector4.");
			}
			std::stringstream l_ss;
			l_ss << "length: " << l_vec_ptr->length() << "[";
			for (uint32_t i = 0; i < l_vec_ptr->length(); i++)
			{
				auto& l_vec = l_vec_ptr->operator[](i);
				l_ss << "<" << l_vec.x << "," << l_vec.y << "," << l_vec.z << ">";
			}
			l_ss << "length: " << l_vec_ptr->length() << "]";
			lua_pushstring(L, l_ss.str().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaMayaStatic::g_mvec_arr_meta_name);
			return 1;
		}

		static int index(lua_State* L)
		{
			const char* l_key = luaL_checkstring(L, -1);
			lua_getglobal(L, vufLuaMayaStatic::g_mvec_arr_tbl_name);
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
	};
}
#endif // ! VF_LUA_MVECTOR_ARRAS_H
