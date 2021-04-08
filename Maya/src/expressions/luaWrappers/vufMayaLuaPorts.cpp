#include <expressions/luaWrappers/vufMayaLuaPorts.h>
#include <utils/vufMayaUtils.h>
#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

#include <maya/MFnDependencyNode.h> 
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>

using namespace vufRM;

void vufMayaLuaPorts::register_ports_for_editor(lua_State* L, std::string* p_node_name, vufMayaLuaPortInternalData* p_data)
{
	
	lua_pushlightuserdata(L, p_node_name);
	lua_pushlightuserdata(L, p_data);
	lua_pushcclosure(L, node_in, 2);
	lua_setglobal(L, "inputPort");

	lua_pushlightuserdata(L, p_node_name);
	lua_pushlightuserdata(L, p_data);
	lua_pushcclosure(L, node_out, 1);
	lua_setglobal(L, "outputPort");

	lua_pushcfunction(L, node_help);
	lua_setglobal(L, "help");	
}

int vufMayaLuaPorts::node_in(lua_State* L)// syntax rule    maya_node.attribute, var_name
{
	int l_number_of_argiments = lua_gettop(L);
	//read upvalues
	std::string& l_name					= *(std::string*)lua_touserdata(L, lua_upvalueindex(1));
	vufMayaLuaPortInternalData* l_data	= (vufMayaLuaPortInternalData*)lua_touserdata(L, lua_upvalueindex(2));
	// read parameters
	const char* l_obj_name_ptr	= luaL_checkstring(L, -2);
	const char* l_var_name_ptr	= luaL_checkstring(L, -1);
	
	if (l_obj_name_ptr != nullptr && l_var_name_ptr != nullptr)
	{
		MStatus l_status;
		std::vector<std::string> l_splited_v = vuf::vufStringUtils::string_split_by(l_obj_name_ptr,".");
		if (l_splited_v.size() < 2)
		{			
			return luaL_error(L, "Failed to split first argument by . Expect obj_name.attr_name");
		}
		MObject l_maya_obj = vufMayaUtils::get_object_by_name(vufMayaUtils::str_2_mstr( l_splited_v[0]));
		if (l_maya_obj.isNull() == true)
		{
			VF_LOG_INFO(l_splited_v[0]);

			return luaL_error(L, "Cant find object");
		}
		MFnDependencyNode l_dn(l_maya_obj);
		MPlug l_plug = l_dn.findPlug(vufMayaUtils::str_2_mstr(l_splited_v[1]), true, &l_status);
		if (l_status != MS::kSuccess)
		{
			return luaL_error(L, "Cant find plug");
		}
		auto l_plug_type = vufMayaUtils::get_plug_type(l_plug);
		VF_LOG_INFO(l_plug_type);

	}
	//const char* l_attr		= luaL_checkstring(L, -3);
	//const char* l_attr		= luaL_checkstring(L, -2);
	//const char* l_attr		= luaL_checkstring(L, -1);

	//VF_LOG_INFO((std::string("Number of arguments ") + std::to_string(l_number_of_argiments) ));
	return 0;
}
int vufMayaLuaPorts::node_out(lua_State* L)// syntax rule    var_name, maya_node.attribute,id
{
	int l_number_of_argiments = lua_gettop(L);
	const char* l_obj_name = luaL_checkstring(L, -4);
	//const char* l_attr = luaL_checkstring(L, -3);
	//const char* l_attr = luaL_checkstring(L, -2);
	//const char* l_attr = luaL_checkstring(L, -1);

	VF_LOG_INFO((std::string("Number of arguments ") + std::to_string(l_number_of_argiments)));
	return 0;
}
int vufMayaLuaPorts::node_help(lua_State* L)
{
	return 0;
}
