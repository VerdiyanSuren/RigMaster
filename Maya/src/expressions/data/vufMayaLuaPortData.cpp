#include <data/vufMayaDataList.h>
#include <expressions/data/vufMayaLuaPortInternalData.h>
#include <utils/vufMayaUtils.h>
#include <vufMayaGlobalIncludes.h>
#include <serializer/vufTxtSerializer.h>


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
	m_internal_data		= std::shared_ptr<vufMayaLuaPortInternalData>(new vufMayaLuaPortInternalData());
	
	MString l_maya_str	= p_args.asString(p_last_element++, &l_status); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	std::string l_str	= vufMayaUtils::mstr_2_str(l_maya_str);	
	std::vector<char>	l_buff(l_str.begin(), l_str.end());
	// read version
	uint32_t l_version;
	uint64_t l_offset = vuf::txtSerializer::decode_number_from_buff(l_version, l_buff);

	if (l_version == 1)
	{
		// --------------------------------------------------------------------------------
		// read input ports
		//in time ports
		{
			uint16_t l_time_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_time_sz, l_buff, l_offset);
			m_internal_data->m_in_time_port.resize(l_time_sz);
			for (uint16_t i = 0; i < l_time_sz; ++i)
			{
				std::string l_var_name; 
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_time_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_time_port[i].m_index, l_buff, l_offset);
			}
		}
		//in double ports
		{
			uint16_t l_double_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_double_sz, l_buff, l_offset);
			m_internal_data->m_in_double_port.resize(l_double_sz);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_double_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_double_port[i].m_index, l_buff, l_offset);
			}
		}
		//in vector ports
		{
			uint16_t l_vector_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_vector_sz, l_buff, l_offset);
			m_internal_data->m_in_vector_port.resize(l_vector_sz);
			for (uint16_t i = 0; i < l_vector_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_vector_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_vector_port[i].m_index, l_buff, l_offset);
			}
		}
		//in angle ports
		{
			uint16_t l_angle_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_angle_sz, l_buff, l_offset);
			m_internal_data->m_in_angle_port.resize(l_angle_sz);
			for (uint16_t i = 0; i < l_angle_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_angle_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_angle_port[i].m_index, l_buff, l_offset);
			}
		}
		//in matrix ports
		{
			uint16_t l_matrix_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_matrix_sz, l_buff, l_offset);
			m_internal_data->m_in_matrix_port.resize(l_matrix_sz);
			for (uint16_t i = 0; i < l_matrix_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_matrix_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_matrix_port[i].m_index, l_buff, l_offset);
			}
		}

		//in mesh ports
		{
			uint16_t l_mesh_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_mesh_sz, l_buff, l_offset);
			m_internal_data->m_in_mesh_port.resize(l_mesh_sz);
			for (uint16_t i = 0; i < l_mesh_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_mesh_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_mesh_port[i].m_index, l_buff, l_offset);
			}
		}
		//in curve ports
		{
			uint16_t l_curve_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_curve_sz, l_buff, l_offset);
			m_internal_data->m_in_curve_port.resize(l_curve_sz);
			for (uint16_t i = 0; i < l_curve_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_curve_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_curve_port[i].m_index, l_buff, l_offset);
			}
		}
		//in surf ports
		{
			uint16_t l_surf_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_surf_sz, l_buff, l_offset);
			m_internal_data->m_in_surf_port.resize(l_surf_sz);
			for (uint16_t i = 0; i < l_surf_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_surf_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_surf_port[i].m_index, l_buff, l_offset);
			}
		}
		// --------------------------------------------------------------------------------
		// read output ports
		
		// out double
		{
			uint16_t l_double_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_double_sz, l_buff, l_offset);
			m_internal_data->m_out_double_port.resize(l_double_sz);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_out_double_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_out_double_port[i].m_index, l_buff, l_offset);
			}
		}
		// read script
		std::string l_script;
		l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_script, l_buff, l_offset);
		m_internal_data->m_lua_txt.set_script(l_script);

		return MS::kSuccess;
	}
	VF_LOG_ERR("Unknown version for vufMayaLuaPortData");
	return MS::kFailure;
}
MStatus 	vufMayaLuaPortData::readBinary(std::istream& p_in, unsigned int	p_length)
{
	m_internal_data = std::shared_ptr<vufMayaLuaPortInternalData>(new vufMayaLuaPortInternalData());

	uint64_t l_buff_size;
	p_in.read((char*)&l_buff_size, sizeof(l_buff_size));
	std::vector<char> l_buff(l_buff_size);
	p_in.read(&l_buff[0], l_buff_size);

	uint32_t l_version;
	uint64_t l_offset = vuf::txtSerializer::decode_number_from_buff(l_version, l_buff);

	if (l_version == 1)
	{
		// --------------------------------------------------------------------------------
		// read input ports
		//in time ports
		{
			uint16_t l_time_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_time_sz, l_buff, l_offset);
			m_internal_data->m_in_time_port.resize(l_time_sz);
			for (uint16_t i = 0; i < l_time_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_time_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_time_port[i].m_index, l_buff, l_offset);
			}
		}
		//in double ports
		{
			uint16_t l_double_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_double_sz, l_buff, l_offset);
			m_internal_data->m_in_double_port.resize(l_double_sz);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_double_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_double_port[i].m_index, l_buff, l_offset);
			}
		}
		//in vector ports
		{
			uint16_t l_vector_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_vector_sz, l_buff, l_offset);
			m_internal_data->m_in_vector_port.resize(l_vector_sz);
			for (uint16_t i = 0; i < l_vector_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_vector_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_vector_port[i].m_index, l_buff, l_offset);
			}
		}
		//in angle ports
		{
			uint16_t l_angle_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_angle_sz, l_buff, l_offset);
			m_internal_data->m_in_angle_port.resize(l_angle_sz);
			for (uint16_t i = 0; i < l_angle_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_angle_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_angle_port[i].m_index, l_buff, l_offset);
			}
		}
		//in matrix ports
		{
			uint16_t l_matrix_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_matrix_sz, l_buff, l_offset);
			m_internal_data->m_in_matrix_port.resize(l_matrix_sz);
			for (uint16_t i = 0; i < l_matrix_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_matrix_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_matrix_port[i].m_index, l_buff, l_offset);
			}
		}

		//in mesh ports
		{
			uint16_t l_mesh_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_mesh_sz, l_buff, l_offset);
			m_internal_data->m_in_mesh_port.resize(l_mesh_sz);
			for (uint16_t i = 0; i < l_mesh_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_mesh_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_mesh_port[i].m_index, l_buff, l_offset);
			}
		}
		//in curve ports
		{
			uint16_t l_curve_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_curve_sz, l_buff, l_offset);
			m_internal_data->m_in_curve_port.resize(l_curve_sz);
			for (uint16_t i = 0; i < l_curve_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_curve_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_curve_port[i].m_index, l_buff, l_offset);
			}
		}
		//in surf ports
		{
			uint16_t l_surf_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_surf_sz, l_buff, l_offset);
			m_internal_data->m_in_surf_port.resize(l_surf_sz);
			for (uint16_t i = 0; i < l_surf_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_in_surf_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_in_surf_port[i].m_index, l_buff, l_offset);
			}
		}
		// --------------------------------------------------------------------------------
		// read output ports

		// out double
		{
			uint16_t l_double_sz;
			l_offset = vuf::txtSerializer::decode_number_from_buff(l_double_sz, l_buff, l_offset);
			m_internal_data->m_out_double_port.resize(l_double_sz);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				std::string l_var_name;
				l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_var_name, l_buff, l_offset);
				m_internal_data->m_out_double_port[i].m_lua_var_name = l_var_name;
				l_offset = vuf::txtSerializer::decode_number_from_buff(m_internal_data->m_out_double_port[i].m_index, l_buff, l_offset);
			}
		}
		// read script
		std::string l_script;
		l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_script, l_buff, l_offset);
		m_internal_data->m_lua_txt.set_script(l_script);

		return MS::kSuccess;
	}
	VF_LOG_ERR("Unknown version for vufMayaLuaPortData");
	return MS::kFailure;
}
MStatus 	vufMayaLuaPortData::writeASCII(std::ostream& p_out)
{	
	if (m_internal_data != nullptr)
	{
		// data version
		uint32_t l_version = 1;
		std::vector<char> l_buff;
		uint64_t l_offset = vuf::txtSerializer::encode_number_to_buff(&l_version, l_buff); 
		// --------------------------------------------------------------------------------
		// input ports serialization

		//in time ports
		{
			uint16_t l_time_sz = (uint16_t)m_internal_data->m_in_time_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(&l_time_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_time_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_in_time_port[i].m_lua_var_name,	l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_in_time_port[i].m_index,			l_buff, l_offset);													
			}
		}
		// in double ports
		{
			uint16_t l_double_sz = (uint16_t)m_internal_data->m_in_double_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_double_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_in_double_port[i].m_lua_var_name,	l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_in_double_port[i].m_index,			l_buff, l_offset);
			}
		}
		// in vector ports
		{
			uint16_t l_vector_sz = (uint16_t)m_internal_data->m_in_vector_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_vector_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_vector_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_in_vector_port[i].m_lua_var_name,	l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_in_vector_port[i].m_index,			l_buff, l_offset);
			}
		}
		// in angle ports
		{
			uint16_t l_angle_sz = (uint16_t)m_internal_data->m_in_angle_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_angle_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_angle_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_in_angle_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_in_angle_port[i].m_index,		l_buff, l_offset);
			}
		}
		// in matrix ports
		{
			uint16_t l_matrix_sz = (uint16_t)m_internal_data->m_in_matrix_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_matrix_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_matrix_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_in_matrix_port[i].m_lua_var_name,	l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_in_matrix_port[i].m_index,			l_buff, l_offset);

			}
		}
		// in mesh ports
		{
			uint16_t l_mesh_sz = (uint16_t)m_internal_data->m_in_mesh_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_mesh_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_mesh_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_in_mesh_port[i].m_lua_var_name,	l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_in_mesh_port[i].m_index,			l_buff, l_offset);
			}
		}
		// in curve ports
		{
			uint16_t l_curve_sz = (uint16_t)m_internal_data->m_in_curve_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_curve_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_curve_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_in_curve_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_in_curve_port[i].m_index,		l_buff, l_offset);
			}
		}
		// in srv ports
		{
			uint16_t l_surf_sz = (uint16_t)m_internal_data->m_in_surf_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_surf_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_surf_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_in_surf_port[i].m_lua_var_name,	l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_in_surf_port[i].m_index,			l_buff, l_offset);
			}
		}
		// --------------------------------------------------------------------------------
		// output ports serialization
		// out double
		{
			uint16_t l_double_sz = (uint16_t)m_internal_data->m_out_double_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_double_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(	m_internal_data->m_out_double_port[i].m_lua_var_name,	l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(		m_internal_data->m_out_double_port[i].m_index,			l_buff, l_offset);
			}
		}
		// --------------------------------------------------------------------------------
		// script serialization
		l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_lua_txt.get_script(), l_buff, l_offset);
		std::string l_str(l_buff.begin(), l_buff.end());
		p_out << l_str << std::endl;
	}
	return MS::kSuccess;
}
MStatus 	vufMayaLuaPortData::writeBinary(std::ostream& p_out)
{
	if (m_internal_data != nullptr)
	{	
		// data version
		uint32_t l_version = 1;
		std::vector<char> l_buff;
		uint64_t l_offset = vuf::txtSerializer::encode_number_to_buff(&l_version, l_buff);
		// --------------------------------------------------------------------------------
		// input ports serialization

		//in time ports
		{
			uint16_t l_time_sz = (uint16_t)m_internal_data->m_in_time_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(&l_time_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_time_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_in_time_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_in_time_port[i].m_index, l_buff, l_offset);
			}
		}
		// in double ports
		{
			uint16_t l_double_sz = (uint16_t)m_internal_data->m_in_double_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_double_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_in_double_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_in_double_port[i].m_index, l_buff, l_offset);
			}
		}
		// in vector ports
		{
			uint16_t l_vector_sz = (uint16_t)m_internal_data->m_in_vector_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_vector_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_vector_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_in_vector_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_in_vector_port[i].m_index, l_buff, l_offset);
			}
		}
		// in angle ports
		{
			uint16_t l_angle_sz = (uint16_t)m_internal_data->m_in_angle_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_angle_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_angle_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_in_angle_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_in_angle_port[i].m_index, l_buff, l_offset);
			}
		}
		// in matrix ports
		{
			uint16_t l_matrix_sz = (uint16_t)m_internal_data->m_in_matrix_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_matrix_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_matrix_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_in_matrix_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_in_matrix_port[i].m_index, l_buff, l_offset);

			}
		}
		// in mesh ports
		{
			uint16_t l_mesh_sz = (uint16_t)m_internal_data->m_in_mesh_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_mesh_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_mesh_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_in_mesh_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_in_mesh_port[i].m_index, l_buff, l_offset);
			}
		}
		// in curve ports
		{
			uint16_t l_curve_sz = (uint16_t)m_internal_data->m_in_curve_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_curve_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_curve_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_in_curve_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_in_curve_port[i].m_index, l_buff, l_offset);
			}
		}
		// in srv ports
		{
			uint16_t l_surf_sz = (uint16_t)m_internal_data->m_in_surf_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_surf_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_surf_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_in_surf_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_in_surf_port[i].m_index, l_buff, l_offset);
			}
		}
		// --------------------------------------------------------------------------------
		// output ports serialization
		// out double
		{
			uint16_t l_double_sz = (uint16_t)m_internal_data->m_out_double_port.size();
			l_offset = vuf::txtSerializer::encode_number_to_buff(l_double_sz, l_buff, l_offset);
			for (uint16_t i = 0; i < l_double_sz; ++i)
			{
				l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_out_double_port[i].m_lua_var_name, l_buff, l_offset);
				l_offset = vuf::txtSerializer::encode_number_to_buff(m_internal_data->m_out_double_port[i].m_index, l_buff, l_offset);
			}
		}
		// --------------------------------------------------------------------------------
		// script serialization
		l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->m_lua_txt.get_script(), l_buff, l_offset);

		p_out.write((char*)&l_offset, sizeof(l_offset));
		p_out.write(&l_buff[0], l_offset);
	}
	return MS::kSuccess;
}
