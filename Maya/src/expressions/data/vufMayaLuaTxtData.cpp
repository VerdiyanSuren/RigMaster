#include <data/vufMayaDataList.h>
#include <vufTxtSerializer.h>
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
	MString l_str	= p_args.asString(p_last_element++, &l_status);
	if (l_status != MS::kSuccess)
	{
		MGlobal::displayError("Failed to read Lua script");
		return MS::kFailure;
	}
	m_internal_data->set_txt(l_str.asChar());
	return MS::kSuccess;
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
		auto l_text = m_internal_data->get_script();
		if (l_text.empty() == false)
		{
			auto l_converted = vuf::txtSerializer::to_chars_string(l_text);
			p_out << l_converted << " ";
			return MS::kSuccess;
		}
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
	return vufData::k_txt_data;
}

void		vufMayaLuaTxtData::log_me() const
{
	std::cout << "Txt data log " << std::endl;
}

