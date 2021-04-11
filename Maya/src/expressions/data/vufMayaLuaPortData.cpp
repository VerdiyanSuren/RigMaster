#include <data/vufMayaDataList.h>
#include <expressions/data/vufMayaLuaPortInternalData.h>
#include <utils/vufMayaUtils.h>
#include <vufMayaGlobalIncludes.h>
#include <vufTxtSerializer.h>


using namespace vufRM;
using namespace vuf;

vufMayaLuaPortData::vufMayaLuaPortData():vufData()
{
}
vufMayaLuaPortData::~vufMayaLuaPortData()
{}
MStatus 	vufMayaLuaPortData::readASCII(const MArgList& p_args, unsigned int& p_last_element)
{
	MStatus l_status;
	std::cout << "vufMayaLuaPortData read ascII" << std::endl;
	VF_LOG_INFO("TRY TO LOAD DATA");

	m_internal_data = std::shared_ptr<vufMayaLuaPortInternalData>(new vufMayaLuaPortInternalData());
	
	// read version
	uint32_t l_version = 1;
	MString l_str;
	std::vector<unsigned char> l_bytes_vector;
	VF_READ_SERIALIZED_FROM_ASCII(l_version, uint32_t);

	if (l_version == 1)
	{
		// --------------------------------------------------------------------------------
		// read input ports

		//in time ports
		{
			uint16_t l_time_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_time_sz, uint16_t);
			m_internal_data->m_in_time_port.resize(l_time_sz);
			for (uint16_t i = 0; i < l_time_sz; ++i)
			{
				std::string l_var_name; 
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_in_time_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_in_time_port[i].m_index, uint16_t);
			}
		}

		//in double ports
		{
			uint16_t l_double_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_double_sz, uint16_t);
			m_internal_data->m_in_double_port.resize(l_double_sz);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				std::string l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_in_double_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_in_double_port[i].m_index, uint16_t);
			}
		}

		//in vector ports
		{
			uint16_t l_vector_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_vector_sz, uint16_t);
			m_internal_data->m_in_vector_port.resize(l_vector_sz);
			for (uint16_t i = 0; i < l_vector_sz; ++i)
			{
				std::string l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_in_vector_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_in_vector_port[i].m_index, uint16_t);
			}
		}

		//in angle ports
		{
			uint16_t l_angle_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_angle_sz, uint16_t);
			m_internal_data->m_in_angle_port.resize(l_angle_sz);
			for (uint16_t i = 0; i < l_angle_sz; ++i)
			{
				std::string l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_in_angle_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_in_angle_port[i].m_index, uint16_t);
			}
		}

		//in matrix ports
		{
			uint16_t l_matrix_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_matrix_sz, uint16_t);
			m_internal_data->m_in_matrix_port.resize(l_matrix_sz);
			for (uint16_t i = 0; i < l_matrix_sz; ++i)
			{
				std::string l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_in_matrix_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_in_matrix_port[i].m_index, uint16_t);
			}
		}

		//in mesh ports
		{
			uint16_t l_mesh_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_mesh_sz, uint16_t);
			m_internal_data->m_in_mesh_port.resize(l_mesh_sz);
			for (uint16_t i = 0; i < l_mesh_sz; ++i)
			{
				std::string l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_in_mesh_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_in_mesh_port[i].m_index, uint16_t);
			}
		}

		//in curve ports
		{
			uint16_t l_curve_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_curve_sz, uint16_t);
			m_internal_data->m_in_curve_port.resize(l_curve_sz);
			for (uint16_t i = 0; i < l_curve_sz; ++i)
			{
				std::string l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_in_curve_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_in_curve_port[i].m_index, uint16_t);
			}
		}

		//in surf ports
		{
			uint16_t l_surf_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_surf_sz, uint16_t);
			m_internal_data->m_in_surf_port.resize(l_surf_sz);
			for (uint16_t i = 0; i < l_surf_sz; ++i)
			{
				std::string l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_in_surf_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_in_surf_port[i].m_index, uint16_t);
			}
		}
		// --------------------------------------------------------------------------------
		// read output ports
		
		// out double
		{
			uint16_t l_double_sz;
			VF_READ_SERIALIZED_FROM_ASCII(l_double_sz, uint16_t);
			m_internal_data->m_out_double_port.resize(l_double_sz);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				std::string l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(l_var_name, std::string);
				m_internal_data->m_out_double_port[i].m_lua_var_name = l_var_name;
				VF_READ_SERIALIZED_FROM_ASCII(m_internal_data->m_out_double_port[i].m_index, uint16_t);
			}
		}

		// read script
		std::string l_script;
		VF_READ_SERIALIZED_FROM_ASCII(l_script, std::string);
		m_internal_data->m_lua_txt.set_script(l_script);

		return MS::kSuccess;
	}
	VF_LOG_ERR("Unknown version for vufMayaLuaPortData");

	return MS::kFailure;
}
MStatus 	vufMayaLuaPortData::readBinary(std::istream& p_in, unsigned int	p_length)
{
	// To Do
	// implement this

	return MS::kSuccess;
}
MStatus 	vufMayaLuaPortData::writeASCII(std::ostream& p_out)
{	
	std::cout << "vufMayaLuaPortData write ascII" << std::endl;
	VF_LOG_INFO("TRY TO SAVE DATA");
	if (m_internal_data != nullptr)
	{
		std::string l_str;
		// data version
		uint32_t l_version = 1;
		p_out << vuf::txtSerializer::to_chars_string(&l_version, sizeof(l_version)) << " ";
		// --------------------------------------------------------------------------------
		// input ports serialization

		//in time ports
		{
			uint16_t l_time_sz = (uint16_t)m_internal_data->m_in_time_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_time_sz, sizeof(l_time_sz)) << " ";
			for (uint16_t i = 0; i < l_time_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(	   m_internal_data->m_in_time_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string( &m_internal_data->m_in_time_port[i].m_index,
														sizeof(m_internal_data->m_in_time_port[i].m_index)) << " ";
			}
		}

		// in double ports
		{
			uint16_t l_double_sz = (uint16_t)m_internal_data->m_in_double_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_double_sz, sizeof(l_double_sz)) << " ";
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(		m_internal_data->m_in_double_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string(  &m_internal_data->m_in_double_port[i].m_index,
														sizeof(	m_internal_data->m_in_double_port[i].m_index)) << " ";
			}
		}

		// in vector ports
		{
			uint16_t l_vector_sz = (uint16_t)m_internal_data->m_in_vector_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_vector_sz, sizeof(l_vector_sz)) << " ";
			for (uint16_t i = 0; i < l_vector_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(		m_internal_data->m_in_vector_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string(  &m_internal_data->m_in_vector_port[i].m_index,
														sizeof( m_internal_data->m_in_vector_port[i].m_index)) << " ";
			}
		}

		// in angle ports
		{
			uint16_t l_angle_sz = (uint16_t)m_internal_data->m_in_angle_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_angle_sz, sizeof(l_angle_sz)) << " ";
			for (uint16_t i = 0; i < l_angle_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(		m_internal_data->m_in_angle_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string(  &m_internal_data->m_in_angle_port[i].m_index,
														sizeof( m_internal_data->m_in_angle_port[i].m_index)) << " ";
			}
		}

		// in matrix ports
		{
			uint16_t l_matrix_sz = (uint16_t)m_internal_data->m_in_matrix_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_matrix_sz, sizeof(l_matrix_sz)) << " ";
			for (uint16_t i = 0; i < l_matrix_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(		m_internal_data->m_in_matrix_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string(  &m_internal_data->m_in_matrix_port[i].m_index,
														sizeof( m_internal_data->m_in_matrix_port[i].m_index)) << " ";
			}
		}

		// in mesh ports
		{
			uint16_t l_mesh_sz = (uint16_t)m_internal_data->m_in_mesh_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_mesh_sz, sizeof(l_mesh_sz)) << " ";
			for (uint16_t i = 0; i < l_mesh_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(		m_internal_data->m_in_mesh_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string(  &m_internal_data->m_in_mesh_port[i].m_index,
														sizeof( m_internal_data->m_in_mesh_port[i].m_index)) << " ";
			}
		}

		// in curve ports
		{
			uint16_t l_curve_sz = (uint16_t)m_internal_data->m_in_curve_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_curve_sz, sizeof(l_curve_sz)) << " ";
			for (uint16_t i = 0; i < l_curve_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(		m_internal_data->m_in_curve_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string(  &m_internal_data->m_in_curve_port[i].m_index,
														sizeof( m_internal_data->m_in_curve_port[i].m_index)) << " ";
			}
		}

		// in srv ports
		{
			uint16_t l_surf_sz = (uint16_t)m_internal_data->m_in_surf_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_surf_sz, sizeof(l_surf_sz)) << " ";
			for (uint16_t i = 0; i < l_surf_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(		m_internal_data->m_in_surf_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string(  &m_internal_data->m_in_surf_port[i].m_index,
														sizeof( m_internal_data->m_in_surf_port[i].m_index)) << " ";
			}
		}

		// --------------------------------------------------------------------------------
		// output ports serialization
		
		// out double
		{
			uint16_t l_double_sz = (uint16_t)m_internal_data->m_out_double_port.size();
			p_out << vuf::txtSerializer::to_chars_string(&l_double_sz, sizeof(l_double_sz)) << " ";
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				p_out << txtSerializer::to_chars_string(		m_internal_data->m_out_double_port[i].m_lua_var_name) << " ";
				p_out << vuf::txtSerializer::to_chars_string(  &m_internal_data->m_out_double_port[i].m_index,
														sizeof( m_internal_data->m_out_double_port[i].m_index)) << " ";
			}
		}

		// --------------------------------------------------------------------------------
		// script serialization
		p_out << txtSerializer::to_chars_string(m_internal_data->m_lua_txt.get_script()) << std::endl;

	}
	return MS::kSuccess;
}
MStatus 	vufMayaLuaPortData::writeBinary(std::ostream& p_out)
{
	// To Do 
	// Implement this

	return MS::kSuccess;
}
int			vufMayaLuaPortData::get_type()	const
{
	return vufData::k_lua_port_data;
}

void		vufMayaLuaPortData::log_me() const
{
	std::cout << "Txt port data log " << std::endl;
}
