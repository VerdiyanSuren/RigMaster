#ifndef VF_LUA_MPOINT_ARRAY_H
#define VF_LUA_MPOINT_ARRAY_H

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
	class vufLuaMPointArray
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mpoint_arr_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new",		create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy",		copy);
			VF_LUA_ADD_TABLE_FIELD(L, "setLength",	set_length);
			VF_LUA_ADD_TABLE_FIELD(L, "length",		length);
			VF_LUA_ADD_TABLE_FIELD(L, "set",		set);
			VF_LUA_ADD_TABLE_FIELD(L, "get",		get);
			VF_LUA_ADD_TABLE_FIELD(L, "clear",		clear);
			VF_LUA_ADD_TABLE_FIELD(L, "remove",		remove);
			VF_LUA_ADD_TABLE_FIELD(L, "insert",		insert);
			VF_LUA_ADD_TABLE_FIELD(L, "append",		append);

			VF_LUA_ADD_TABLE_FIELD(L, "to_string",	to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type",	to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mpoint_arr_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
		}
		/// bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)	
		VF_LUA_SET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_mpoint_arr_meta_name, MPointArray, vufLuaMPointArrayWrapper);
		/// bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_mpoint_arr_meta_name, MPointArray, vufLuaMPointArrayWrapper);
		/// bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>& p_res_ptr)
		VF_LUA_GET_USER_DATA_PARAM(	vufLuaMayaStatic::g_mpoint_arr_meta_name, MPointArray, vufLuaMPointArrayWrapper);
		/// create as reference of new user data and push to stack
		/// vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)
		VF_LUA_CREATE_USER_NEW_REF(	vufLuaMayaStatic::g_mpoint_arr_meta_name, MPointArray, vufLuaMPointArrayWrapper);
		/// create mew user data and push to stack
		///  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)
		VF_LUA_CREATE_USER_DATA(	vufLuaMayaStatic::g_mpoint_arr_meta_name, MPointArray, vufLuaMPointArrayWrapper);
	private:
		static int create(lua_State* L)
		{
			create_user_data(L);
			return 1;
		}
		VF_LUA_IMPLEMENT_COPY_CONSTRUCTOR(vufLuaMayaStatic::g_mpoint_arr_meta_name, MPointArray, vufLuaMPointArrayWrapper);
		VF_LUA_IMPLEMENT_DESTROY(vufLuaMayaStatic::g_mpoint_arr_meta_name, vufLuaMPointArrayWrapper);
		static int set(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " Failed(MPointArray:set) wrong number of arguments. Expect 2");
			}
			//read i
			int l_i;
			MPoint* l_pt;
			// read and check value
			if (vufLuaMPoint::get_param(L, -2, &l_pt) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " MPointArray:set expect MPoint as argument.");
			}
			// read and check row
			l_i = luaL_checkinteger(L, -1);
			MPointArray* l_pnt_ptr;
			if (get_param(L, -3, &l_pnt_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " MPointArray:set to get array object");
			}
			// check indecies
			if (l_i < 0 || l_i >= (int)l_pnt_ptr->length())
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " index is out of range");
			}
			(*l_pnt_ptr)[l_i] = *l_pt;
			return 0;
		}
		static int get(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " Failed(MPointArray:get) wrong number of arguments. Expect 1");
			}
			//read i
			int l_i;
			// read and check row
			l_i = (int)luaL_checkinteger(L, -1);
			MPointArray* l_pnt_arr_ptr;
			if (get_param(L, -2, &l_pnt_arr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_arr_tbl_name, " FailedFailed(MPointArray:get) tp get MVectorArray");
			}
			if (l_i < 0 || l_i >= (int)l_pnt_arr_ptr->length())
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_tbl_name, " index of column is out of range");
			}
			auto l_vec_wrapper = vufLuaMVector::create_user_data(L);
			l_vec_wrapper->set_data((*l_pnt_arr_ptr)[l_i]);
			return 1;
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaMayaStatic::g_mpoint_arr_meta_name, vufLuaMPointArrayWrapper, length, length);
		static int set_length(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				return luaL_error(L, "expect exactly  1 numerical  arguments");
			}
			int l_size;
			l_size = (int)luaL_checkinteger(L, -1);
			MPointArray* l_res_ptr;
			if (get_param(L, -2, &l_res_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, "Failed(MPointArray:setLength) to get MPointArray object.");
			}
			l_res_ptr->setLength(l_size);
			return 0;
		}
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_VOID(vufLuaMayaStatic::g_mpoint_arr_meta_name, vufLuaMPointArrayWrapper, clear, clear);
		static int remove(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 1)
			{
				return luaL_error(L, "expect exactly  1 numerical  arguments");
			}
			int l_index = (int)luaL_checkinteger(L, -1);
			
			MPointArray* l_res_ptr;
			if (get_param(L, -2, &l_res_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, "failed(MPointArray:remove) to get MPointArray object.");
			}
			l_res_ptr->remove(l_index);
			return 0;
		}
		static int insert(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 3)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " wrong number of arguments. Expect 2");
			}
			//read i
			int l_i;
			MPoint* l_pnt;
			// read and check value
			if (vufLuaMPoint::get_param(L, -2, &l_pnt) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " expect MPoint as argument.");
			}
			// read and check row
			l_i = (int)luaL_checkinteger(L, -1);
			// matrix
			MPointArray* l_pnt_ptr;
			if (get_param(L, -3, &l_pnt_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " Failed tp get array object");
			}

			// check indecies
			if (l_i < 0 || l_i >= (int)l_pnt_ptr->length())
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " index is out of range");
			}
			l_pnt_ptr->insert(*l_pnt, l_i);
			return 0;
		}
		static int append(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments != 2)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " wrong number of arguments. Expect 1");
			}
			MPoint* l_pnt;
			// read and check value
			if (vufLuaMPoint::get_param(L, -2, &l_pnt) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " expect MPoint as argument.");
			}

			MPointArray* l_pnt_arr_ptr;
			if (get_param(L, -2, &l_pnt_arr_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " Failed tp get MVectorArray");
			}
			l_pnt_arr_ptr->append(*l_pnt);
			return 0;
		}
		static int to_string(lua_State* L)
		{
			MPointArray* l_pnt_ptr;
			if (get_param(L, -1, &l_pnt_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mpoint_arr_meta_name, " Failed tp get vector4.");
			}
			std::stringstream l_ss;
			l_ss << "length: " << l_pnt_ptr->length() << "[";
			for (uint32_t i = 0; i < l_pnt_ptr->length(); i++)
			{
				l_ss << l_pnt_ptr->operator[](i) << " ";				
			}
			l_ss << "]";
			lua_pushstring(L, l_ss.str().c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaMayaStatic::g_mpoint_arr_tbl_name);
			return 1;
		}		

		static int index(lua_State* L)
		{
			const char* l_key = luaL_checkstring(L, -1);
			lua_getglobal(L, vufLuaMayaStatic::g_mpoint_arr_tbl_name);
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}

	};
}
#endif // !VF_LUA_MPOINT_ARRAY_H
