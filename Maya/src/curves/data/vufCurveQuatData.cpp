#include <data/vufMayaDataList.h>
#include <utils/vufMayaUtils.h>

using namespace vuf;
using namespace vufRM;
using namespace vufMath;

vufCurveQuatData::vufCurveQuatData() {}
vufCurveQuatData::~vufCurveQuatData() {}
MStatus 	vufCurveQuatData::readASCII(const MArgList& p_args, unsigned int& p_last_element)
{
	MStatus l_status;
	int l_type;
	l_type = p_args.asInt(p_last_element++, &l_status);
	m_internal_data = vufCurveQuaternionFn_4d::create(vufCurveQuatFnType(l_type));
	if (m_internal_data != nullptr)
	{
		uint64_t l_size;
		l_size = p_args.asInt(p_last_element++, &l_status);
		if (l_size == 0)
		{
			return MS::kSuccess;
		}
		MString l_mstr = p_args.asString(p_last_element++, &l_status);
		std::string l_str = vufMayaUtils::mstr_2_str(l_mstr);
		
		std::vector<char> l_zipped(l_str.begin(), l_str.end());
		std::vector<char> l_unzipped;
		txtSerializer::unzip_to_buff(l_zipped, l_unzipped);
		m_internal_data->decode_from_buff(l_unzipped);
	}
	return MS::kSuccess;
}
MStatus 	vufCurveQuatData::readBinary(std::istream& p_in, unsigned int	p_length)
{
	int l_type;
	p_in.read((char*)(&l_type), sizeof(l_type));
	m_internal_data = vufCurveQuaternionFn_4d::create(vufCurveQuatFnType(l_type));
	if (m_internal_data != nullptr)
	{
		uint64_t l_size;
		uint32_t l_version;
		p_in.read((char*)(&l_size), sizeof(uint64_t));
		if (l_size == 0)
		{
			return MS::kSuccess;
		}
		std::vector<char> l_buff(l_size);
		p_in.read(&(l_buff[0]), l_size);
		m_internal_data->from_binary(l_buff, l_version);

	}
	return MS::kSuccess;
}
MStatus 	vufCurveQuatData::writeASCII(std::ostream& p_out)
{
	if (m_internal_data == nullptr)
	{
		int l_type = (int)vufCurveQuatFnType::k_none;
		p_out << l_type << std::endl;
		return MS::kSuccess;
	}
	int l_type = (int)m_internal_data->get_type();
	p_out << l_type << std::endl;

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
MStatus 	vufCurveQuatData::writeBinary(std::ostream& p_out)
{
	if (m_internal_data == nullptr)
	{
		int l_type = (int)vufCurveQuatFnType::k_none;
		p_out.write((char*)&l_type, sizeof(l_type));
		return MS::kSuccess;
	}
	int l_type = (int)m_internal_data->get_type();
	p_out.write((char*)&l_type, sizeof(l_type));

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
int			vufCurveQuatData::get_type()	const
{
	return vufData::k_curve_quat_data;
}
void		vufCurveQuatData::log_me() const
{
	std::cout << "Curve Quat data log " << std::endl;
}
