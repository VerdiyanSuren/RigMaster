#include <data/vufMayaDataList.h>
#include <vufTxtSerializer.h>
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
	
	// read version
	uint32_t l_version = 1;
	VF_READ_SERIALIZED_FROM_ASCII(l_version, uint32_t);

	if (l_version == 1)
	{
		// read script
		std::string l_script;
		VF_READ_SERIALIZED_FROM_ASCII(l_script, std::string);
		m_internal_data->set_script(l_script);
		return MS::kSuccess;
	}
	VF_LOG_ERR("Unknown version for vufMayaLuaTxtData");
	return MS::kFailure;
}
MStatus 	vufMayaLuaTxtData::readBinary(std::istream& p_in, unsigned int	p_length)
{
	// To Do
	// implement this

	return MS::kSuccess;
}
MStatus 	vufMayaLuaTxtData::writeASCII(std::ostream& p_out)
{	
	if (m_internal_data != nullptr)
	{
		std::string l_str;
		// data version
		uint32_t l_version = 1;
		p_out << vuf::txtSerializer::to_chars_string(&l_version, sizeof(l_version)) << " ";

		p_out << vuf::txtSerializer::to_chars_string(m_internal_data->get_script()) << std::endl;
		return MS::kSuccess;
	}
	
	return MS::kSuccess;
}
MStatus 	vufMayaLuaTxtData::writeBinary(std::ostream& p_out)
{
	// To Do 
	// Implement this

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

