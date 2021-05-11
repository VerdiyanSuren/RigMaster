#ifndef VF_LUA_MVECTOR_H
#define VF_LUA_MVECTOR_H

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
#include <maya/MVector.h>


namespace vufRM
{
	class vufLuaMVector
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mvec_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new", create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "length", length);
			VF_LUA_ADD_TABLE_FIELD(L, "normalize", normalize);
			//VF_LUA_ADD_TABLE_FIELD(L, "normal", normal);
			//VF_LUA_ADD_TABLE_FIELD(L, "ortho_to", ortho_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "parallel_to", parallel_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "distance_to", distance_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "rotateBy", distance_to);
			//VF_LUA_ADD_TABLE_FIELD(L, "rotateTo", distance_to);
			
			VF_LUA_ADD_TABLE_FIELD(L, "angle", angle);
			///VF_LUA_ADD_TABLE_FIELD(L, "dot", dot);
			//VF_LUA_ADD_TABLE_FIELD(L, "cross", cross);
			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mvec_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__add", add);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__sub", sub);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__unm", unm);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__mul", mul);
			//VF_LUA_ADD_META_TABLE_FIELD(L, "__div", div);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__newindex", new_index);
		}
		/** bool	set_global(lua_State* L, const std::string& p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_SET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper);
		/** bool	get_global(lua_State * L, const std::string & p_var_name, vufVector4<T>& p_res)	*/
		VF_LUA_GET_USER_DATA_GLOBAL(vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper);
		/** bool	static bool	get_param(lua_State * L, int p_lua_index, vufVector4<T>** p_res_ptr)*/
		VF_LUA_GET_USER_DATA_PARAM(	vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper);
		/** create as reference of new user data and push to stack
		* vufLuaVector4Wrapper<T>*	create_new_ref(lua_State* L, vufVector4<T>* l_ref_vector)	*/
		VF_LUA_CREATE_USER_NEW_REF(	vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper);
		/** create mew user data and push to stack
		*  vufLuaVector4Wrapper<T>* create_user_data(lua_State* L)*/
		VF_LUA_CREATE_USER_DATA(	vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper);
	private:
		static int create(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			if (l_number_of_arguments == 0)
			{
				create_user_data(L);
				return 1;
			}
			if (l_number_of_arguments == 3)
			{
				bool l_status = false;
				double l_x, l_y, l_z;
				VF_LUA_READ_NUMBER(L, -3, l_x, l_status);
				VF_LUA_THROW_ERROR_BY_BOOL(L,l_status, vufLuaMayaStatic::g_mvec_tbl_name, "wrong nunber of arguments. Expect  o or 3 numbers");
				VF_LUA_READ_NUMBER(L, -2, l_y, l_status);
				VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mvec_tbl_name, "wrong nunber of arguments. Expect  o or 3 numbers");
				VF_LUA_READ_NUMBER(L, -1, l_z, l_status);
				VF_LUA_THROW_ERROR_BY_BOOL(L, l_status, vufLuaMayaStatic::g_mvec_tbl_name, "wrong nunber of arguments. Expect  o or 3 numbers");
				auto l_wrapper = create_user_data(L);
				MVector& l_vec = l_wrapper->get_data();
				l_vec.x = l_x; 
				l_vec.y = l_y; 
				l_vec.z = l_z;
				return 1;
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Fauled to create MVector. Wrong arguments.");
		}
		VF_LUA_IMPLEMENT_COPY(vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(vufLuaMayaStatic::g_mvec_meta_name, vufLuaMVectorWrapper, length, length);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_VOID(	vufLuaMayaStatic::g_mvec_meta_name, vufLuaMVectorWrapper, normalize, normalize);
		VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(	vufLuaMayaStatic::g_mvec_meta_name,	MVector, vufLuaMVectorWrapper, normal, normal);
		VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(vufLuaMayaStatic::g_mvec_meta_name, vufLuaMVectorWrapper, angle, angle);


		static int to_string(lua_State* L)
		{
			MVector* l_vec_ptr;
			if (get_param(L, -1, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " Failed tp get MVector object.");
			}
			std::string l_str = "[";
			l_str += l_vec_ptr->x; l_str += ", ";
			l_str += l_vec_ptr->y; l_str += ", ";
			l_str += l_vec_ptr->z; l_str += "]";
			lua_pushstring(L, l_str.c_str());
			return 1;
		}
		static int to_type(lua_State* L)
		{
			lua_pushstring(L, vufLuaMayaStatic::g_mvec_tbl_name);
			return 1;
		}
		
		VF_LUA_IMPLEMENT_DESTROY(		vufLuaMayaStatic::g_mvec_meta_name, vufLuaMVectorWrapper);
		VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(	vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper, add);
		VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(	vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper, sub);
		VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(	vufLuaMayaStatic::g_mvec_meta_name, MVector, vufLuaMVectorWrapper, unm);

		static int index(lua_State* L)
		{
			int l_number_of_arguments = lua_gettop(L);
			MVector* l_vec_ptr;
			if (get_param(L, -2, &l_vec_ptr) == false)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " got null object.");
			}
			const char* l_key = luaL_checkstring(L, -1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_meta_name, " unknown key");
			}
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					lua_pushnumber(L, l_vec_ptr->x);
					return 1;
				}
				else if (l_key[0] == 'y')
				{
					lua_pushnumber(L, l_vec_ptr->y);
					return 1;
				}
				else if (l_key[0] == 'z')
				{
					lua_pushnumber(L, l_vec_ptr->z);
					return 1;
				}
			}
			lua_getglobal(L, vufLuaMayaStatic::g_mvec_tbl_name);
			lua_pushstring(L, l_key);
			lua_rawget(L, -2);
			return 1;
		}
		static int new_index(lua_State* L)
		{
			//std::cout << "VECTOR NEW_INDEX" << std::endl;
			int l_number_of_arguments = lua_gettop(L);
			// in lua a[0] = 5
			// a.x = 5
			auto l_wrapper_ptr = (vufLuaMVectorWrapper*)luaL_checkudata(L, -l_number_of_arguments, vufLuaMayaStatic::g_mvec_meta_name);
			if (l_wrapper_ptr == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " vector object is null.");
			}

			const char* l_key = luaL_checkstring(L, -l_number_of_arguments + 1);
			if (l_key == nullptr)
			{
				VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " unknown key");
			}
			MVector& l_vec = l_wrapper_ptr->get_data();
			if (l_key[1] == '\0')
			{
				if (l_key[0] == 'x')
				{
					l_vec.x = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'y')
				{
					l_vec.y = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
				else if (l_key[0] == 'z')
				{
					l_vec.z = (double)luaL_checknumber(L, -l_number_of_arguments + 2);
					return 0;
				}
			}
			VF_LUA_THROW_ERROR(L, vufLuaMayaStatic::g_mvec_tbl_name, " unknown key");
		}

	};
}
#endif // !VF_LUA_MVECTOR_H
