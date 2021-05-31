#include <data/vufMayaDataList.h>
#include <serializer/vufTxtSerializer.h>
#include <vufMayaGlobalIncludes.h>
#include <utils/vufMayaUtils.h>
#include <vufLuaWrapper.h>
#include <maya/MGlobal.h>


using namespace vufRM;

 
vufMayaLuaTxtData::vufMayaLuaTxtData()
{
}
vufMayaLuaTxtData::~vufMayaLuaTxtData()
{}
MStatus 	vufMayaLuaTxtData::readASCII(const MArgList& p_args, unsigned int& p_last_element)
{
	MStatus l_status;
	m_internal_data = std::shared_ptr<vuf::vufTxt>(new vuf::vufTxt());
	MString l_maya_str = p_args.asString(p_last_element++, &l_status); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	std::string l_str = vufMayaUtils::mstr_2_str(l_maya_str);
	std::vector<char> l_buff(l_str.begin(), l_str.end());


	// read version
	uint32_t l_version;
	uint64_t l_offset = vuf::txtSerializer::encode_number_to_buff(&l_version, l_buff);
	// read version

	if (l_version == 1)
	{
		// read script
		std::string l_script;
		l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_script, l_buff, l_offset);
		m_internal_data->set_script(l_script);
		return MS::kSuccess;
	}
	VF_LOG_ERR("Unknown version for vufMayaLuaTxtData");
	return MS::kFailure;
}
MStatus 	vufMayaLuaTxtData::readBinary(std::istream& p_in, unsigned int	p_length)
{	
	m_internal_data = std::shared_ptr<vuf::vufTxt>(new vuf::vufTxt());
	
	uint64_t l_buff_size;
	p_in.read((char*)&l_buff_size, sizeof(l_buff_size));
	std::vector<char> l_buff(l_buff_size);
	p_in.read(&l_buff[0], l_buff_size);
	uint32_t l_version;
	uint64_t l_offset = vuf::txtSerializer::decode_number_from_buff(l_version, l_buff);
	if (l_version == 1)
	{
		// read script
		std::string l_script;
		l_offset = vuf::txtSerializer::decode_std_string_from_buff(l_script, l_buff, l_offset);
		m_internal_data->set_script(l_script);
		return MS::kSuccess;
	}
	
	VF_LOG_ERR("Unknown version for vufMayaLuaTxtData");
	return MS::kFailure;
}
MStatus 	vufMayaLuaTxtData::writeASCII(std::ostream& p_out)
{	
	if (m_internal_data != nullptr)
	{
		// data version
		uint32_t l_version = 1;
		std::vector<char> l_buff;
		uint64_t l_offset	= vuf::txtSerializer::encode_number_to_buff(&l_version, l_buff);
		l_offset			= vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->get_script(), l_buff, l_offset); 
		
		std::string l_str(l_buff.begin(), l_buff.end());
		p_out << l_str << std::endl;
		return MS::kSuccess;
	}
	
	return MS::kSuccess;
}
MStatus 	vufMayaLuaTxtData::writeBinary(std::ostream& p_out)
{
	if (m_internal_data != nullptr)
	{
		uint32_t l_version = 1;
		std::vector<char> l_buff;
		uint64_t l_offset = vuf::txtSerializer::encode_number_to_buff(&l_version, l_buff);
		l_offset = vuf::txtSerializer::encode_std_string_to_buff(m_internal_data->get_script(), l_buff, l_offset);
		p_out.write((char*)&l_offset, sizeof(l_offset));
		p_out.write(&l_buff[0], l_offset);
	}
	return MS::kSuccess;
}
int			vufMayaLuaTxtData::get_type()	const
{
	return vufData::k_lua_script_data;
}

void		vufMayaLuaTxtData::log_me() const
{
	std::cout << "Txt data log " << std::endl;
}

