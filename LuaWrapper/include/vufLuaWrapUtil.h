#ifndef VF_LUA_WRAP_UTIL_H
#define VF_LUA_WRAP_UTIL_H

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

// Implement method
// myType.method(MyType)->number
#define VF_LUA_IMPLEMENT_TYPE_OF_TYPE_TO_NUMBER(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	int l_number_of_argiments = lua_gettop(L);														\
	if (l_number_of_argiments != 2)																	\
	{																								\
		return luaL_error(L, "expecting exactly 1 argument");										\
	}																								\
	auto* l_arg_ptr		= (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME);							\
	auto* l_master_ptr = (CLASS_NAME*)luaL_checkudata(L, -2, META_NAME);							\
	if (l_arg_ptr != nullptr && l_master_ptr != nullptr)											\
	{																								\
		auto l_dist = l_master_ptr->CLASS_METHOD(*l_arg_ptr);										\
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
		return luaL_error(L, "expecting exactly 1 argument");										\
	}																								\
	auto* l_arg_ptr		= (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME);							\
	auto* l_master_ptr	= (CLASS_NAME*)luaL_checkudata(L, -2, META_NAME);							\
	if (l_arg_ptr != nullptr && l_master_ptr != nullptr)											\
	{																								\
		auto l_res_ptr =	(CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));					\
		new (l_res_ptr)		CLASS_NAME();															\
		luaL_getmetatable(L, META_NAME);															\
		lua_setmetatable(L, -2);																	\
		*l_res_ptr = l_master_ptr->CLASS_METHOD(*l_arg_ptr);										\
		return 1;					/*number of return  values*/									\
	}																								\
	return 0;																						\
}

// Implement method
// myType.method()->number
#define VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_NUMBER(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto* l_arg_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME);								\
	if (l_arg_ptr == nullptr)																		\
	{																								\
		return 0;																					\
	}																								\
	T l_res = l_arg_ptr->CLASS_METHOD();															\
	lua_pushnumber(L, l_res);	/* set result of function*/											\
	return 1;					/*number of return  values*/										\
}

// Implement method
// myType.method()->myType
#define VF_LUA_IMPLEMENT_TYPE_OF_VOID_TO_TYPE(META_NAME,CLASS_NAME,CLASS_METHOD,LUA_METHOD_NAME)	\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto* l_arg_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME);								\
	if (l_arg_ptr == nullptr)																		\
	{																								\
		return 0;																					\
	}																								\
	auto l_res_ptr = (CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));							\
	new (l_res_ptr) CLASS_NAME();																	\
	luaL_getmetatable(L, META_NAME);																\
	lua_setmetatable(L, -2);																		\
	*l_res_ptr = l_arg_ptr->CLASS_METHOD();															\
	return 1;					/*number of return  values*/										\
}

// Implement metamethod
// myType (+,-,*,/) myType -> myType
#define VF_LUA_IMPLEMENT_TYPE_ADD_TYPE(META_NAME,CLASS_NAME,LUA_METHOD_NAME)						\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto l_arg_1_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME);								\
	auto l_arg_2_ptr = (CLASS_NAME*)luaL_checkudata(L, -2, META_NAME);								\
	if (l_arg_1_ptr != nullptr && l_arg_2_ptr != nullptr)											\
	{																								\
		auto l_res_ptr = (CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));						\
		luaL_getmetatable(L, META_NAME);															\
		lua_setmetatable(L, -2);																	\
		*l_res_ptr = *l_arg_1_ptr + *l_arg_2_ptr;													\
		return 1;																					\
	}																								\
	return 0;																						\
}

#define VF_LUA_IMPLEMENT_TYPE_SUB_TYPE(META_NAME,CLASS_NAME,LUA_METHOD_NAME)						\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto l_arg_1_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME);								\
	auto l_arg_2_ptr = (CLASS_NAME*)luaL_checkudata(L, -2, META_NAME);								\
	if (l_arg_1_ptr != nullptr && l_arg_2_ptr != nullptr)											\
	{																								\
		auto l_res_ptr = (CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));						\
		luaL_getmetatable(L, META_NAME);															\
		lua_setmetatable(L, -2);																	\
		*l_res_ptr = *l_arg_1_ptr - *l_arg_2_ptr;													\
		return 1;																					\
	}																								\
	return 0;																						\
}

#define VF_LUA_IMPLEMENT_TYPE_UNM_TYPE(META_NAME,CLASS_NAME,LUA_METHOD_NAME)						\
static int LUA_METHOD_NAME(lua_State* L)															\
{																									\
	auto l_arg_1_ptr = (CLASS_NAME*)luaL_checkudata(L, -1, META_NAME);								\
	if (l_arg_1_ptr != nullptr )																	\
	{																								\
		auto l_res_ptr = (CLASS_NAME*)lua_newuserdata(L, sizeof(CLASS_NAME));						\
		luaL_getmetatable(L, META_NAME);															\
		lua_setmetatable(L, -2);																	\
		*l_res_ptr =  -*l_arg_1_ptr;																\
		return 1;																					\
	}																								\
	return 0;																						\
}

#endif // !VF_LUA_WRAP_UTIL_H
