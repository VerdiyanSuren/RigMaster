#ifndef VF_LUA_MINT_ARRAY_H
#define VF_LUA_MINT_ARRAY_H

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
	class vufLuaMIntArray
	{
	public:
		static void registrator(lua_State* L)
		{
			VF_LUA_NEW_TABLE(L, vufLuaMayaStatic::g_mpoint_arr_tbl_name);
			VF_LUA_ADD_TABLE_FIELD(L, "new", create);
			VF_LUA_ADD_TABLE_FIELD(L, "copy", copy);
			VF_LUA_ADD_TABLE_FIELD(L, "setLength", set_length);
			VF_LUA_ADD_TABLE_FIELD(L, "length", length);
			VF_LUA_ADD_TABLE_FIELD(L, "set", set);
			VF_LUA_ADD_TABLE_FIELD(L, "get", get);
			VF_LUA_ADD_TABLE_FIELD(L, "clear", clear);
			VF_LUA_ADD_TABLE_FIELD(L, "remove", remove);
			VF_LUA_ADD_TABLE_FIELD(L, "insert", insert);
			VF_LUA_ADD_TABLE_FIELD(L, "append", append);

			VF_LUA_ADD_TABLE_FIELD(L, "to_string", to_string);
			VF_LUA_ADD_TABLE_FIELD(L, "to_type", to_type);

			VF_LUA_NEW_META_TABLE(L, vufLuaMayaStatic::g_mpoint_arr_meta_name);
			VF_LUA_ADD_META_TABLE_FIELD(L, "__gc", destroy);

			VF_LUA_ADD_META_TABLE_FIELD(L, "__index", index);
		}
	};
}
#endif //!VF_LUA_MINT_ARRAY_H