#ifndef VF_LUA_WRAP_UTIL_H
#define VF_LUA_WRAP_UTIL_H

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

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

// Implement method
// myType.method(MyType)->number
#define VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)\
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
#define VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_TYPE(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	int l_number_of_argiments = lua_gettop(L);														\
	if (l_number_of_argiments != 2)																	\
	{																								\
		VF_LUA_THROW_ERROR(L,META_NAME,"expects 1 argument");										\
	}																								\
	auto l_arg_ptr		= (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME.c_str());					\
	auto l_master_ptr	= (CLASS_NAME*)luaL_checkudata(L, -2, META_NAME.c_str());					\
	if (l_arg_ptr != nullptr && l_master_ptr != nullptr)											\
	{																								\
		auto l_res_ptr =	(CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));					\
		new (l_res_ptr)		CLASS_NAME();															\
		luaL_getmetatable(L, META_NAME.c_str());													\
		lua_setmetatable(L, -2);																	\
		l_res_ptr->set_data(l_master_ptr->get_data().CLASS_METHOD(l_arg_ptr->get_data()));			\
		return 1;					/*number of return  values*/									\
	}																								\
	VF_LUA_THROW_ERROR( L, META_NAME, " got null object.");																						\
}

// Implement method
// myType.method()->number
#define VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)\
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
#define VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE( META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto* l_arg_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME.c_str());						\
	if (l_arg_ptr == nullptr)																		\
	{																								\
		VF_LUA_THROW_ERROR(L,META_NAME," got a null");												\
	}																								\
	auto l_res_ptr = (CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));							\
	new (l_res_ptr) CLASS_NAME();																	\
	luaL_getmetatable(L, META_NAME.c_str());														\
	lua_setmetatable(L, -2);																		\
	l_res_ptr->set_data( l_arg_ptr->get_data().CLASS_METHOD());										\
	return 1;					/*number of return  values*/										\
}

// Implement metamethod
// myType (+,-,*,/) myType -> myType
#define VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(META_NAME,CLASS_NAME,LUA_METHOD_NAME)						\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto l_arg_1_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME.c_str());						\
	auto l_arg_2_ptr = (CLASS_NAME*)luaL_checkudata(L, -2, META_NAME.c_str());						\
	if (l_arg_1_ptr != nullptr && l_arg_2_ptr != nullptr)											\
	{																								\
		auto l_res_ptr = (CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));						\
		luaL_getmetatable(L, META_NAME.c_str());													\
		lua_setmetatable(L, -2);																	\
		l_res_ptr->set_data(l_arg_1_ptr->get_data() + l_arg_2_ptr->get_data());						\
		return 1;																					\
	}																								\
	VF_LUA_THROW_ERROR(L,META_NAME," got a null");													\
}

#define VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(META_NAME,CLASS_NAME,LUA_METHOD_NAME)						\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto l_arg_1_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME.c_str());						\
	auto l_arg_2_ptr = (CLASS_NAME*)luaL_checkudata(L, -2, META_NAME.c_str());						\
	if (l_arg_1_ptr != nullptr && l_arg_2_ptr != nullptr)											\
	{																								\
		auto l_res_ptr = (CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));						\
		luaL_getmetatable(L, META_NAME.c_str());													\
		lua_setmetatable(L, -2);																	\
		l_res_ptr->set_data(l_arg_2_ptr->get_data() - l_arg_1_ptr->get_data());						\
		return 1;																					\
	}																								\
	VF_LUA_THROW_ERROR(L,META_NAME," got a null");													\
}

#define VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(META_NAME,CLASS_NAME,LUA_METHOD_NAME)						\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto l_arg_1_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME.c_str());						\
	if (l_arg_1_ptr != nullptr )																	\
	{																								\
		auto l_res_ptr = (CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));						\
		luaL_getmetatable(L, META_NAME.c_str());													\
		lua_setmetatable(L, -2);																	\
		l_res_ptr->set_data( -l_arg_1_ptr->get_data());												\
		return 1;																					\
	}																								\
	VF_LUA_THROW_ERROR(L,META_NAME," got a null");													\
}

#endif // !VF_LUA_WRAP_UTIL_H
