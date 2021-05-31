#include "quatCurves/vufCurveRebuildData.h"

using namespace vufTP;
using namespace vufMath;

vufCurveRebuildData_4d::vufCurveRebuildData_4d()
{
	//m_curve_ptr = new curve math model
}
vufCurveRebuildData_4d::~vufCurveRebuildData_4d()
{}
MStatus 	vufCurveRebuildData_4d::readASCII(const MArgList& p_args, unsigned int& p_last_element)
{
	return MS::kSuccess;
}
MStatus 	vufCurveRebuildData_4d::readBinary(std::istream& p_in, unsigned int	p_length)
{
	return MS::kSuccess;
}
MStatus 	vufCurveRebuildData_4d::writeASCII(std::ostream& p_out)
{
	return MS::kSuccess;
}
MStatus 	vufCurveRebuildData_4d::writeBinary(std::ostream& p_out)
{
	return MS::kSuccess;
}
int			vufCurveRebuildData_4d::get_type()	const
{
	return vufCurveRebuildData_4d::k_curve_data;
}

void		vufCurveRebuildData_4d::log_me() const
{
	std::cout << "Curve Rebuil data log " << std::endl;
}
