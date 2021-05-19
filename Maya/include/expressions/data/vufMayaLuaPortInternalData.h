#ifndef VF_MAYA_LUA_PORT_INTRNL_DATA_H
#define VF_MAYA_LUA_PORT_INTRNL_DATA_H

#include <vector>
#include <string>
#include <set>
#include <vufLuaWrapper.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnNurbsSurface.h>

namespace vufRM
{
	struct vufLuaExpressionPort
	{
		std::string		m_lua_var_name	= "";
		uint16_t		m_index			= 0;;
	};
	class vufMayaLuaPortInternalData
	{
	public:
		vufMayaLuaPortInternalData() {}
		~vufMayaLuaPortInternalData() {}

		void clear_ports()
		{
			m_in_time_port.clear();
			m_in_double_port.clear();
			m_in_vector_port.clear();
			m_in_angle_port.clear();
			m_in_matrix_port.clear();
			m_in_mesh_port.clear();
			m_in_curve_port.clear();
			m_in_surf_port.clear();

			m_out_double_port.clear();

			m_var_names.clear();
		}
		//input ports
		std::vector< vufLuaExpressionPort>	m_in_time_port;
		std::vector< vufLuaExpressionPort>	m_in_double_port;
		std::vector< vufLuaExpressionPort>	m_in_angle_port;
		std::vector< vufLuaExpressionPort>	m_in_vector_port;
		std::vector< vufLuaExpressionPort>	m_in_matrix_port;
		std::vector< vufLuaExpressionPort>	m_in_mesh_port;
		std::vector< vufLuaExpressionPort>	m_in_curve_port;
		std::vector< vufLuaExpressionPort>	m_in_surf_port;

		//output_ports
		std::vector<vufLuaExpressionPort>	m_out_double_port;
		std::vector<vufLuaExpressionPort>	m_out_vector_port;
		std::vector<vufLuaExpressionPort>	m_out_matrix_port;
		//std::vector< vufLuaExpressionPort<double>	m_out_angle_port;

		std::set<std::string> m_var_names;
		bool m_need_lua_reset = true; // reset lua machine in exprerssion node
		vuf::vufTxt	m_lua_txt;
		uint64_t	m_hash;
	};
}
#endif // !VF_MAYA_LUA_PORT_INTRNL_DATA_H
