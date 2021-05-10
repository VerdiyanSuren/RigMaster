#include <expressions/luaWrappers/vufMayaLuaPorts.h>
#include <utils/vufMayaUtils.h>
#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

#include <maya/MFnDependencyNode.h> 
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MVectorArray.h>
#include <maya/MIntArray.h>

using namespace vufRM;
#define VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(DATA_PORT,EXPR_PLUG,TYPE)		\
	uint16_t l_index = (uint16_t)DATA_PORT.size();			\
	std::string l_cmd = "cmds.connectAttr('";				\
	l_cmd += l_obj_name_ptr;								\
	l_cmd += "','";											\
	l_cmd += l_expr_node_name;								\
	l_cmd += ".";											\
	l_cmd += EXPR_PLUG;										\
	l_cmd += "[";											\
	l_cmd += std::to_string(l_index);						\
	l_cmd += "]',f = True)";								\
	VF_LOG_INFO(l_cmd);										\
	l_status = MGlobal::executePythonCommand(vufMayaUtils::str_2_mstr(l_cmd));	\
	if (l_status != MS::kSuccess)												\
	{																			\
		VF_LOG_ERR((std::string("Failed to execute command: ") + l_cmd));		\
		return luaL_error(L, "Cant evaluate connection command");				\
	}																			\
	vufLuaExpressionPort<TYPE> l_port;											\
	l_port.m_index = l_index;													\
	l_port.m_lua_var_name = l_var_name_ptr;										\
	DATA_PORT.push_back(l_port);												\
	l_data->m_var_names.insert(l_var_name_ptr);									\
	return 0;


void vufMayaLuaPorts::register_ports_for_editor(lua_State* L, std::string* p_node_name, vufMayaLuaPortInternalData* p_data)
{
	
	lua_pushlightuserdata(L, p_node_name);
	lua_pushlightuserdata(L, p_data);
	lua_pushcclosure(L, node_in, 2);
	lua_setglobal(L, "inputPort");

	lua_pushlightuserdata(L, p_node_name);
	lua_pushlightuserdata(L, p_data);
	lua_pushcclosure(L, node_out, 2);
	lua_setglobal(L, "outputPort");

	lua_pushcfunction(L, node_help);
	lua_setglobal(L, "help");	
}

int vufMayaLuaPorts::node_in(lua_State* L)// syntax rule    maya_node.attribute, var_name
{
	//int l_number_of_argiments = lua_gettop(L);
	//read upvalues
	std::string& l_expr_node_name		= *(std::string*)lua_touserdata(L, lua_upvalueindex(1));
	vufMayaLuaPortInternalData* l_data	= (vufMayaLuaPortInternalData*)lua_touserdata(L, lua_upvalueindex(2));
	// read parameters
	const char* l_obj_name_ptr	= luaL_checkstring(L, -2);
	const char* l_var_name_ptr	= luaL_checkstring(L, -1);
	
	


	if (l_obj_name_ptr != nullptr && l_var_name_ptr != nullptr)
	{
		// Check is variable name empty
		if (!isalpha(l_var_name_ptr[0]))
		{
			VF_LOG_ERR((std::string("This variable hasd to start by letter: ") + l_var_name_ptr));
			return luaL_error(L, "This variable hasd to start by letter");

		}
		// Check if variable with this name already exists
		if (l_data->m_var_names.find(l_var_name_ptr) != l_data->m_var_names.end())
		{
			VF_LOG_ERR((std::string("This variable already exists: ") + l_var_name_ptr));
			return luaL_error(L, "This variable already exists");
		}

		MStatus l_status;
		std::vector<std::string> l_splited_v = vuf::vufStringUtils::string_split_by(l_obj_name_ptr,".");
		if (l_splited_v.size() < 2)
		{			
			return luaL_error(L, "Failed to split first argument by . Expect obj_name.attr_name");
		}
		MObject l_maya_obj = vufMayaUtils::get_object_by_name(vufMayaUtils::str_2_mstr( l_splited_v[0]));
		if (l_maya_obj.isNull() == true)
		{
			return luaL_error(L, "Cant find object");
		}
		MFnDependencyNode l_dn(l_maya_obj);
		MPlug l_plug = l_dn.findPlug(vufMayaUtils::str_2_mstr(l_splited_v[1]), true, &l_status);
		if (l_status != MS::kSuccess)
		{
			return luaL_error(L, "Cant find plug");
		}
		
		

		// If could be connected to time
		if (vufMayaUtils::is_plug_connectable_to_time(l_plug) == true)
		{
			VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(l_data->m_in_time_port, "inTime", double);
		}
		// If could be connected to angle
		if (vufMayaUtils::is_plug_connectable_to_angle(l_plug) == true)
		{
			VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(l_data->m_in_angle_port, "inAngle", double);
		}
		// If could be connected to number
		if (vufMayaUtils::is_plug_connectable_to_number(l_plug) == true)
		{
			VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(l_data->m_in_double_port, "inNumber", double);
		}
		// If could be connected to Vector
		if (vufMayaUtils::is_plug_connectable_to_vector(l_plug) == true)
		{
			VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(l_data->m_in_vector_port, "inVector", MVector);
		}
		// If could be connected to Matrix
		if (vufMayaUtils::is_plug_connectable_to_matrix(l_plug) == true)
		{
			VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(l_data->m_in_matrix_port, "inMatrix", MMatrix);
		}
		// If could be connected to Mesh
		if (vufMayaUtils::is_plug_connectable_to_mesh(l_plug) == true)
		{
			VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(l_data->m_in_mesh_port, "inMesh", MFnMesh);
		}
		// If could be connected to Curve
		if (vufMayaUtils::is_plug_connectable_to_curve(l_plug) == true)
		{
			VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(l_data->m_in_curve_port, "inCurve", MFnNurbsCurve);
		}
		// If could be connected to Surface 
		if (vufMayaUtils::is_plug_connectable_to_surface(l_plug) == true)
		{
			VF_LUA_CONNECT_IN_COMMAND_AND_RETURN(l_data->m_in_surf_port, "inSurface", MFnNurbsSurface);
		}

		
		//auto l_plug_type = vufMayaUtils::get_plug_type(l_plug);
		//VF_LOG_INFO(l_plug_type);
		return luaL_error(L, "Unxpected input plug type");
	}
	return luaL_error(L, "Failed to get arguments. Expect two strings: obj_name.attr_name and lua variable name.");
}
int vufMayaLuaPorts::node_out(lua_State* L)// syntax rule    var_name, maya_node.attribute,id
{
	std::string& l_expr_node_name		= *(std::string*)lua_touserdata(L, lua_upvalueindex(1));
	vufMayaLuaPortInternalData* l_data	= (vufMayaLuaPortInternalData*)lua_touserdata(L, lua_upvalueindex(2));
	const char* l_obj_name_ptr = luaL_checkstring(L, -2);
	const char* l_var_name_ptr = luaL_checkstring(L, -1);

	if (l_obj_name_ptr != nullptr && l_var_name_ptr != nullptr)
	{
		// Check is variable name empty
		if (!isalpha(l_var_name_ptr[0]))
		{
			VF_LOG_ERR((std::string("This variable hasd to start by letter: ") + l_var_name_ptr));
			return luaL_error(L, "This variable hasd to start by letter");

		}
		// Check if varible with this name already exists
		if (l_data->m_var_names.find(l_var_name_ptr) != l_data->m_var_names.end())
		{
			VF_LOG_ERR((std::string("This variable already exists: ") + l_var_name_ptr));
			return luaL_error(L, "This variable already exists");
		}

		MStatus l_status;
		std::vector<std::string> l_splited_v = vuf::vufStringUtils::string_split_by(l_obj_name_ptr, ".");
		if (l_splited_v.size() < 2)
		{
			return luaL_error(L, "Failed to split first argument by . Expect obj_name.attr_name");
		}
		MObject l_maya_obj = vufMayaUtils::get_object_by_name(vufMayaUtils::str_2_mstr(l_splited_v[0]));
		if (l_maya_obj.isNull() == true)
		{
			return luaL_error(L, "Cant find object");
		}
		MFnDependencyNode l_dn(l_maya_obj);
		MPlug l_plug = l_dn.findPlug(vufMayaUtils::str_2_mstr(l_splited_v[1]), true, &l_status);
		if (l_status != MS::kSuccess)
		{
			return luaL_error(L, "Cant find plug");
		}
		
		// If could be connected to number
		if (vufMayaUtils::is_plug_connectable_to_number(l_plug) == true)
		{
			uint16_t l_index = (uint16_t)l_data->m_out_double_port.size();
			std::string l_cmd = "cmds.connectAttr('";
			l_cmd += l_expr_node_name;
			l_cmd += ".outNumber[";
			l_cmd += std::to_string(l_index);
			l_cmd += "]','";
			l_cmd += l_obj_name_ptr;
			l_cmd += "',f = True)";

			VF_LOG_INFO(l_cmd);
			
			l_status = MGlobal::executePythonCommand(vufMayaUtils::str_2_mstr(l_cmd));
			if (l_status != MS::kSuccess)
			{
				VF_LOG_ERR((std::string("Failed to execute command: ") + l_cmd));
				return luaL_error(L, "Cant execute connection command");
			}
			vufLuaExpressionPort<double> l_port;
			l_port.m_index = l_index;
			l_port.m_lua_var_name = l_var_name_ptr;
			l_data->m_out_double_port.push_back(l_port);
			return 0;
		}

		return luaL_error(L, "Unxpected output plug type");
	}
	return luaL_error(L, "Failed to get arguments. Expect two strings: obj_name.attr_name and lua variable name.");

}
int vufMayaLuaPorts::node_help(lua_State* L)
{
	return 0;
}

#undef VF_LUA_CONNECT_COMMAND
