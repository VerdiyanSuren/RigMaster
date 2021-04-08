#ifndef VF_MAYA_LUA_PORT_INTRNL_DATA_H
#define VF_MAYA_LUA_PORT_INTRNL_DATA_H

#include <vector>
#include <string>
#include <vufLuaWrapper.h>

namespace vufRM
{
	template<typename T>
	struct vufLuaExpressionPort
	{
		std::string		m_obj_source_name;
		std::string		m_plug_source_name;
		std::string		m_lua_var_name;
		uint16_t		m_type;
		uint16_t		m_index;
		T				m_data;
	};
	class vufMayaLuaPortInternalData
	{
	public:
		vufMayaLuaPortInternalData() {}
		~vufMayaLuaPortInternalData() {}
		//input ports
		std::vector< vufLuaExpressionPort<double>> m_in_time_port;
		std::vector< vufLuaExpressionPort<double>> m_in_double_port;
		//std::vector< vufLuaExpressionPort<MVector>> m_in_vector_port;
		//std::vector< vufLuaExpressionPort<double>> m_in_angle_port;
		//output_ports
		std::vector< vufLuaExpressionPort<double>>	m_out_double_port;
		//std::vector< vufLuaExpressionPort<MVector>> m_out_vector_port;
		//std::vector< vufLuaExpressionPort<double>	m_out_angle_port;

		vuf::vufTxt	m_lua_txt;
		uint64_t	m_hash;
	};
}
#endif // !VF_MAYA_LUA_PORT_INTRNL_DATA_H
