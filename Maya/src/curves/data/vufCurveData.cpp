#include <data/vufMayaDataList.h>

using namespace vufRM;
using namespace vufMath;

vufCurveContainerData_4d::vufCurveContainerData_4d()
{
	//m_curve_ptr = new curve math model
}
vufCurveContainerData_4d::~vufCurveContainerData_4d()
{}
MStatus 	vufCurveContainerData_4d::readASCII(const MArgList& p_args, unsigned int& p_last_element)
{
	return MS::kSuccess;
}
MStatus 	vufCurveContainerData_4d::readBinary(std::istream& p_in, unsigned int	p_length)
{
	return MS::kSuccess;
}
MStatus 	vufCurveContainerData_4d::writeASCII(std::ostream& p_out)
{
	return MS::kSuccess;
}
MStatus 	vufCurveContainerData_4d::writeBinary(std::ostream& p_out)
{
	return MS::kSuccess;
}
int			vufCurveContainerData_4d::get_type()	const
{
	return vufCurveContainerData_4d::k_curve_data;
}

void		vufCurveContainerData_4d::log_me() const
{
	std::cout << "Curve data log " << std::endl;
}
