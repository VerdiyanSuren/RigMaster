#include <data/vufData.h>
#include <data/vufMayaDataList.h>
#include <vufLuaWrapper.h>



using namespace vufRM;
std::shared_ptr<vufData> vufData::create_data(int p_type)
{
	if (p_type == k_lua_port_data)		return std::shared_ptr<vufMayaLuaPortData>(new vufMayaLuaPortData());
	if (p_type == k_lua_script_data)	return std::shared_ptr<vufMayaLuaTxtData>(new vufMayaLuaTxtData());
	//if (p_type == k_curve_data) return std::shared_ptr<vufCurveContainerData_4d>(new vufCurveContainerData_4d());

	return nullptr;
}

#pragma region DATA_TEST
vufDataTest::vufDataTest():vufData()
{
	//std::cout << "Data constructor" << std::endl;
}
vufDataTest::~vufDataTest() 
{
	//std::cout << "Data destructor m_value"  << std::endl;
}
MStatus 	vufDataTest::readASCII(const MArgList& p_args, unsigned int& p_last_element)
{
	//std::cout << "readASCII" << std::endl;
	MStatus l_status;
	m_value		= p_args.asDouble(p_last_element++, &l_status);
	m_random	= p_args.asInt(p_last_element++, &l_status);
	m_type		= p_args.asInt(p_last_element++, &l_status);
	return l_status;
}
MStatus 	vufDataTest::readBinary(std::istream& p_in, unsigned int	p_length)
{
	//std::cout << "readBinary" << std::endl;
	p_in.read((char*)&m_random, sizeof(int));
	p_in.read((char*)&m_value,	sizeof(double));
	p_in.read((char*)&m_type,	sizeof(int));
	return MS::kSuccess;
}
MStatus 	vufDataTest::writeASCII(std::ostream& p_out)
{
	//std::cout << "writeASCII" << std::endl;
	p_out << m_value << " " << m_random << " " << m_type << " \n";
	return MS::kSuccess;
}
MStatus 	vufDataTest::writeBinary(std::ostream& p_out)
{
	//std::cout << "writeBinary" << std::endl;
	p_out.write((char*)&m_random,	sizeof(int));
	p_out.write((char*)&m_value,	sizeof(double));
	p_out.write((char*)&m_type,		sizeof(int));
	return MS::kSuccess;
}
int			vufDataTest::get_type()	const
{
	return vufData::k_test_data;
}

void vufDataTest::log_me() const
{
	std::cout << " Test data value is" << m_value << std::endl;
}
#pragma endregion DATA_TEST