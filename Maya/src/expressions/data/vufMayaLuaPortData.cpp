#include <data/vufMayaDataList.h>
#include <vufTxtSerializer.h>


using namespace vufRM;

vufMayaLuaPortData::vufMayaLuaPortData()
{
}
vufMayaLuaPortData::~vufMayaLuaPortData()
{}
MStatus 	vufMayaLuaPortData::readASCII(const MArgList& p_args, unsigned int& p_last_element)
{
	MStatus l_status;
	return MS::kSuccess;
}
MStatus 	vufMayaLuaPortData::readBinary(std::istream& p_in, unsigned int	p_length)
{
	// To Do
	// implement this

	return MS::kSuccess;
}
MStatus 	vufMayaLuaPortData::writeASCII(std::ostream& p_out)
{
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
	return vufData::k_txt_data;
}

void		vufMayaLuaPortData::log_me() const
{
	std::cout << "Txt port data log " << std::endl;
}
