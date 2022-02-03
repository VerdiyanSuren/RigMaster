#include <data/vufMayaDataList.h>
#include <utils/vufMayaUtils.h>
using namespace vuf;
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
	m_internal_data = vufCurveContainer_4d::create();

	MStatus l_status;
	uint64_t l_size;
	l_size = p_args.asInt(p_last_element++, &l_status);
	if (l_size > 0)
	{
		MString l_mstr = p_args.asString(p_last_element++, &l_status);
		std::string l_str = vufMayaUtils::mstr_2_str(l_mstr);
		std::vector<char> l_zipped(l_str.begin(),l_str.end());
		std::vector<char> l_unzipped;
		txtSerializer::unzip_to_buff(l_zipped, l_unzipped);
		m_internal_data->decode_from_buff(l_unzipped);
	}
	return MS::kSuccess;
}
MStatus 	vufCurveData::readBinary(std::istream& p_in, unsigned int	p_length)
{
	m_internal_data = vufCurveContainer_4d::create();

	uint64_t l_size;
	p_in.read((char*)(&l_size), sizeof(uint64_t));
	if (l_size == 0)
	{
		return MS::kSuccess;
	}
	std::vector<char> l_buff(l_size);
	p_in.read(&(l_buff[0]), l_size);
	m_internal_data->from_binary(l_buff);
	return MS::kSuccess;
}
MStatus 	vufCurveData::writeASCII(std::ostream& p_out)
{
	if (m_internal_data == nullptr)
	{
		return MS::kSuccess;
	}
	std::vector<char> l_encoded;
	std::vector<char> l_zipped;
	m_internal_data->encode_to_buff(l_encoded);
	txtSerializer::zip_to_buff(l_encoded, l_zipped);
	int l_size = (int)l_zipped.size();
	std::string l_str(l_zipped.begin(), l_zipped.end());
	p_out << l_size;
	if (l_size > 0)
	{
		p_out << " " << l_str;
	}
	p_out << std::endl;
	return MS::kSuccess;
}
MStatus 	vufCurveData::writeBinary(std::ostream& p_out)
{
	if (m_internal_data == nullptr)
	{
		uint64_t l_size = 0;
		p_out.write((char*)&l_size, sizeof(l_size));
		return MS::kSuccess;
	}
	std::vector<char> l_buff;
	m_internal_data->to_binary(l_buff);
	uint64_t l_size = (int)l_buff.size();
	p_out.write((char*)l_size, sizeof(l_size));
	if (l_size > 0)
	{
		p_out.write(l_buff.data(), l_buff.size());
	}
	return MS::kSuccess;
}