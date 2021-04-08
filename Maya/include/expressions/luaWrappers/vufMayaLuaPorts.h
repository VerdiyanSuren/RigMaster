#ifndef VF_MAYA_LUA_PORT_WRAPPER
#define VF_MAYA_LUA_PORT_WRAPPER

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <new>
#include <vufLuaWrapUtil.h>
#include <expressions/data/vufMayaLuaPortInternalData.h>

namespace vufRM
{
	class vufMayaLuaPorts
	{
	public:
		static void register_ports_for_editor(lua_State* p_L, std::string* p_node_name, vufMayaLuaPortInternalData* p_data);

	private:
		static int node_in(lua_State* L);	// syntax rule    var_name, maya_node, attribute, 
		static int node_out(lua_State* L);	// syntax rule    var_name, maya_node, attribute, 
		static int node_help(lua_State* L);	// syntax rule    nothing

	};
}
#endif // !VF_MAYA_LUA_PORT_WRAPPER
