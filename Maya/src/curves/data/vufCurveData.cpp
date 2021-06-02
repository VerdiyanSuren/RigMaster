#include <data/vufMayaDataList.h>

using namespace vufRM;
using namespace vufMath;

vufCurveData::vufCurveData()
{
	//m_curve_ptr = new curve math model
}
vufCurveData::~vufCurveData()
{}
MStatus 	vufCurveData::readASCII(const MArgList& p_args, unsigned int& p_last_element)
{
	return MS::kSuccess;
}
MStatus 	vufCurveData::readBinary(std::istream& p_in, unsigned int	p_length)
{
	return MS::kSuccess;
}
MStatus 	vufCurveData::writeASCII(std::ostream& p_out)
{
	return MS::kSuccess;
}
MStatus 	vufCurveData::writeBinary(std::ostream& p_out)
{
	return MS::kSuccess;
}
int			vufCurveData::get_type()	const
{
	return vufData::k_curve_data;
}
void		vufCurveData::log_me() const
{
	std::cout << "Curve data log " << std::endl;
}
