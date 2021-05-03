#ifndef VF_LUA_WRAP_UTIL_H
#define VF_LUA_WRAP_UTIL_H

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

/*
* VF_LUA_NEW_TABLE(L,TABLE_NAME)
* VF_LUA_NEW_META_TABLE(L,TABLE_NAME) 
* VF_LUA_ADD_TABLE_FIELD(L,NAME,FUNCTION)
* 
* VF_LUA_THROW_ERROR(L,TBL_NAME, ERROR_CHAR_PTR)
* 
* VF_LUA_GET_USER_DATA_GLOBAL(META_NAME, DATA_TYPE, WRAPPER_TYPE)
* VF_LUA_GET_USER_DATA_PARAM(META_NAME, DATA_TYPE, WRAPPER_TYPE)
* VF_LUA_CREATE_USER_NEW_REF(META_NAME, DATA_TYPE, WRAPPER_TYPE)
* VF_LUA_CREATE_USER_DATA(META_NAME,DATA_TYPE, WRAPPER_TYPE)
* 
* VF_LUA_IMPLEMENT_COPY(META_NAME,DATA_TYPE,WRAPPER_TYPE)
* VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)
* VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(META_NAME,DATA_TYPE, WRAPPER_TYPE,CLASS_METHOD,LUA_METHOD_NAME)
* VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)
* VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( META_NAME,DATA_TYPE,WRAPPER_TYPE,CLASS_METHOD,LUA_METHOD_NAME)
* VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(META_NAME, DATA_TYPE, WRAPPER_TYPE, LUA_METHOD_NAME)
* VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(META_NAME,DATA_TYPE,WRAPPER_TYPE, LUA_METHOD_NAME)
* VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(META_NAME,DATA_TYPE, WRAPPER_TYPE,LUA_METHOD_NAME)
*/
//---------------------------------------------------------------------------
// Declare new table to add class metods
#define VF_LUA_NEW_TABLE(L,TABLE_NAME)				\
	lua_newtable(L);								\
	lua_pushvalue(L, -1);							\
	lua_setglobal(L, TABLE_NAME);

// Declare new metatable to implement methamethos
#define VF_LUA_NEW_META_TABLE(L,TABLE_NAME)			\
	luaL_newmetatable(L, TABLE_NAME);

// Add class methods to tabel
#define VF_LUA_ADD_TABLE_FIELD(L,NAME,FUNCTION)		\
	lua_pushcfunction(L, FUNCTION);					\
	lua_setfield(L, -2, NAME);

// Add methathods
#define VF_LUA_ADD_META_TABLE_FIELD(L,NAME,FUNCTION)\
	lua_pushstring(L, NAME);						\
	lua_pushcfunction(L, FUNCTION);					\
	lua_settable(L, -3);

// Throw error
#define VF_LUA_THROW_ERROR(L,TBL_NAME, ERROR_CHAR_PTR)			\
	return luaL_error( L, (vufStringUtils::string_padding(TBL_NAME +  ERROR_CHAR_PTR).c_str()) );
//---------------------------------------------------------------------------
//  Read Save params
#define VF_LUA_READ_NUMBER(L, INDEX, NUMBER, BOOL_RES)	\
BOOL_RES = false;										\
if ( lua_isnumber(L,INDEX))								\
{														\
	NUMBER = (double)lua_tonumber(L, INDEX);			\
	BOOL_RES = true;									\
}

#define VF_LUA_READ_INTEGER(L, INDEX, NUMBER, BOOL_RES)	\
BOOL_RES = false;										\
if ( lua_isnumber(L,INDEX))								\
{														\
	NUMBER = (int)lua_tonumber(L, INDEX);				\
	BOOL_RES = true;									\
}

// Implement method
// Get global variable of user
#define VF_LUA_GET_USER_DATA_GLOBAL(META_NAME, DATA_TYPE, WRAPPER_TYPE )				\
static bool	get_global(lua_State * L, const std::string & p_var_name, DATA_TYPE& p_res)	\
{																						\
	lua_getglobal(L, p_var_name.c_str());												\
	auto l_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -1, META_NAME.c_str());				\
	if (l_ptr == nullptr)																\
	{																					\
		return false;																	\
	}																					\
	p_res = l_ptr->get_data();															\
	return true;																		\
}

// Get user data as new param by index in stack 
#define VF_LUA_GET_USER_DATA_PARAM(META_NAME, DATA_TYPE, WRAPPER_TYPE)					\
static bool	get_param(lua_State * L, int p_lua_index, DATA_TYPE** p_res_ptr)			\
{																						\
	auto l_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, p_lua_index, META_NAME.c_str());		\
	if (l_ptr == nullptr)																\
	{																					\
		*p_res_ptr = nullptr;															\
		return false;																	\
	}																					\
	*p_res_ptr = &l_ptr->get_data();													\
	return true;																		\
}
// Create new reference 
#define VF_LUA_CREATE_USER_NEW_REF(META_NAME, DATA_TYPE, WRAPPER_TYPE)				\
static WRAPPER_TYPE*	create_new_ref(lua_State* L,DATA_TYPE* l_ref_data)			\
{																					\
	auto  l_ptr = (WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));			\
	new (l_ptr) WRAPPER_TYPE(*l_ref_data, l_ref_data);								\
	luaL_getmetatable(L, META_NAME.c_str());										\
	lua_setmetatable(L, -2);														\
	return l_ptr;																	\
}

#define VF_LUA_CREATE_USER_DATA(META_NAME,DATA_TYPE, WRAPPER_TYPE)					\
static WRAPPER_TYPE* create_user_data(lua_State* L)									\
{																					\
	auto l_ptr = (WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));			\
	new (l_ptr) WRAPPER_TYPE(DATA_TYPE());											\
	luaL_getmetatable(L, META_NAME.c_str());										\
	lua_setmetatable(L, -2);														\
	return l_ptr;																	\
}

// myType.copy()->myType
#define VF_LUA_IMPLEMENT_COPY(META_NAME,DATA_TYPE,WRAPPER_TYPE)						\
static int copy(lua_State* L)														\
{																					\
	auto l_data_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -1, META_NAME.c_str());		\
	if (l_data_ptr == nullptr)														\
	{																				\
		VF_LUA_THROW_ERROR(L,META_NAME, " got null object.");						\
	}																				\
	auto l_res_ptr = (WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));		\
	new (l_res_ptr) WRAPPER_TYPE( DATA_TYPE() );									\
	luaL_getmetatable(L, META_NAME.c_str());										\
	lua_setmetatable(L, -2);														\
	l_res_ptr->set_data(l_data_ptr->get_data());									\
	return 1;																		\
}

// myType.method(MyType)->number
#define VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	int l_number_of_argiments = lua_gettop(L);														\
	if (l_number_of_argiments != 2)																	\
	{																								\
		VF_LUA_THROW_ERROR(L,META_NAME,"expects 1 argument");										\
	}																								\
	auto l_arg_ptr		= (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME.c_str());					\
	auto l_master_ptr	=  (CLASS_NAME*)luaL_checkudata(L, -2, META_NAME.c_str());					\
	if (l_arg_ptr != nullptr && l_master_ptr != nullptr)											\
	{																								\
		auto l_dist = l_master_ptr->get_data().CLASS_METHOD(l_arg_ptr->get_data());					\
		lua_pushnumber(L, l_dist);																	\
		return 1;					/*number of return  values	*/									\
	}																								\
	return 0;																						\
}

// Implement method
// myType.method(myType)->MyType
#define VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(META_NAME,DATA_TYPE, WRAPPER_TYPE,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)																		\
{																												\
	int l_number_of_argiments = lua_gettop(L);																	\
	if (l_number_of_argiments != 2)																				\
	{																											\
		VF_LUA_THROW_ERROR(L,META_NAME,"expects 1 argument");													\
	}																											\
	auto l_arg_ptr		= (WRAPPER_TYPE*)luaL_checkudata(L, -1, META_NAME.c_str());								\
	auto l_master_ptr	= (WRAPPER_TYPE*)luaL_checkudata(L, -2, META_NAME.c_str());								\
	if (l_arg_ptr != nullptr && l_master_ptr != nullptr)														\
	{																											\
		auto l_res_ptr =	(WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));							\
		new (l_res_ptr)		WRAPPER_TYPE(DATA_TYPE());															\
		luaL_getmetatable(L, META_NAME.c_str());																\
		lua_setmetatable(L, -2);																				\
		l_res_ptr->set_data( l_master_ptr->get_data().CLASS_METHOD(l_arg_ptr->get_data()));						\
		return 1;					/*number of return  values*/												\
	}																											\
	VF_LUA_THROW_ERROR( L, META_NAME, " got null object.");														\
}

// Implement method
// myType.method()->number
#define VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto* l_arg_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME.c_str());						\
	if (l_arg_ptr == nullptr)																		\
	{																								\
		VF_LUA_THROW_ERROR(L, META_NAME, " got null");												\
	}																								\
	T l_res = l_arg_ptr->get_data().CLASS_METHOD();													\
	lua_pushnumber(L, l_res);	/* set result of function*/											\
	return 1;					/*number of return  values*/										\
}

// Implement method
// myType.method()->myType
#define VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( META_NAME,DATA_TYPE,WRAPPER_TYPE,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)																		\
{																												\
	auto l_arg_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -1, META_NAME.c_str());									\
	if (l_arg_ptr == nullptr)																					\
	{																											\
		VF_LUA_THROW_ERROR(L,META_NAME," got a null");															\
	}																											\
	auto l_res_ptr = (WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));									\
	new (l_res_ptr) WRAPPER_TYPE(DATA_TYPE());																	\
	luaL_getmetatable(L, META_NAME.c_str());																	\
	lua_setmetatable(L, -2);																					\
	l_res_ptr->set_data( l_arg_ptr->get_data().CLASS_METHOD());													\
	return 1;					/*number of return  values*/													\
}

//----------------------------------------------------------------------------------------------------------------
// Ariphmetic
// Implement metamethod
// myType (+,-,*,/) myType -> myType
#define VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(META_NAME, DATA_TYPE, WRAPPER_TYPE, LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)													\
{																							\
	auto l_arg_1_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -1, META_NAME.c_str());			\
	auto l_arg_2_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -2, META_NAME.c_str());			\
	if (l_arg_1_ptr != nullptr && l_arg_2_ptr != nullptr)									\
	{																						\
		auto l_res_ptr = (WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));			\
		new (l_res_ptr) WRAPPER_TYPE(DATA_TYPE());											\
		luaL_getmetatable(L, META_NAME.c_str());											\
		lua_setmetatable(L, -2);															\
		l_res_ptr->set_data(l_arg_1_ptr->get_data() + l_arg_2_ptr->get_data());				\
		return 1;																			\
	}																						\
	VF_LUA_THROW_ERROR(L,META_NAME," got a null");											\
}

#define VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(META_NAME,DATA_TYPE,WRAPPER_TYPE, LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)													\
{																							\
	auto l_arg_1_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -1, META_NAME.c_str());			\
	auto l_arg_2_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -2, META_NAME.c_str());			\
	if (l_arg_1_ptr != nullptr && l_arg_2_ptr != nullptr)									\
	{																						\
		auto l_res_ptr = (WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));			\
		new (l_res_ptr) WRAPPER_TYPE(DATA_TYPE());											\
		luaL_getmetatable(L, META_NAME.c_str());											\
		lua_setmetatable(L, -2);															\
		l_res_ptr->set_data(l_arg_2_ptr->get_data() - l_arg_1_ptr->get_data());				\
		return 1;																			\
	}																						\
	VF_LUA_THROW_ERROR(L,META_NAME," got a null");											\
}

#define VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(META_NAME,DATA_TYPE, WRAPPER_TYPE,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)													\
{																							\
	auto l_arg_1_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -1, META_NAME.c_str());			\
	if (l_arg_1_ptr != nullptr )															\
	{																						\
		auto l_res_ptr = (WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));			\
		new (l_res_ptr) WRAPPER_TYPE(DATA_TYPE());											\
		luaL_getmetatable(L, META_NAME.c_str());											\
		lua_setmetatable(L, -2);															\
		l_res_ptr->set_data( -l_arg_1_ptr->get_data());										\
		return 1;																			\
	}																						\
	VF_LUA_THROW_ERROR(L,META_NAME," got a null");											\
}

#define VF_LUA_IMPLEMENT_TYPE_MUL_TYPE(META_NAME,DATA_TYPE,WRAPPER_TYPE, LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)													\
{																							\
	auto l_arg_1_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -1, META_NAME.c_str());			\
	auto l_arg_2_ptr = (WRAPPER_TYPE*)luaL_checkudata(L, -2, META_NAME.c_str());			\
	if (l_arg_1_ptr != nullptr && l_arg_2_ptr != nullptr)									\
	{																						\
		auto l_res_ptr = (WRAPPER_TYPE*)lua_newuserdata(L, sizeof(WRAPPER_TYPE));			\
		new (l_res_ptr) WRAPPER_TYPE(DATA_TYPE());											\
		luaL_getmetatable(L, META_NAME.c_str());											\
		lua_setmetatable(L, -2);															\
		l_res_ptr->set_data(l_arg_2_ptr->get_data() * l_arg_1_ptr->get_data());				\
		return 1;																			\
	}																						\
	VF_LUA_THROW_ERROR(L,META_NAME," got a null");											\
}

#endif // !VF_LUA_WRAP_UTIL_H
