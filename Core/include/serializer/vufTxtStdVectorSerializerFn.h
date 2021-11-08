#ifndef VF_CORE_TXT_VECTOR_NUMBER_SERIALIZER_H
#define VF_CORE_TXT_VECTOR_NUMBER_SERIALIZER_H

#include <serializer/vufTxtSerializer.h>
#include <vector>
#include <sstream>
#include <string>

namespace vuf
{
	template<typename T>
	class VF_API txtStdVectorSerializerFn
	{
		txtStdVectorSerializerFn() = delete;
	public:
		txtStdVectorSerializerFn(std::vector<T>& p_vec) :m_numeric_array_v(p_vec) {};
		txtStdVectorSerializerFn(const txtStdVectorSerializerFn& p_other) :m_numeric_array_v(p_other.m_numeric_array_v) {}
		/// method to convert to string for log system 
		std::string to_string(int p_precision = -1, uint32_t p_tab_count = 0, char p_tab_symbol = '_')	const
		{
			std::stringstream l_ss;
			if (p_precision > 0)
			{
				if (p_precision > 64)
				{
					l_ss.precision(64);
				}
				else
				{
					l_ss.precision(p_precision);
				}
				l_ss << std::fixed;
			}
			if (p_tab_count > 0)
			{
				for (uint32_t i = 0; i < p_tab_count; ++i)
				{
					l_ss << p_tab_symbol << p_tab_symbol << p_tab_symbol << p_tab_symbol;
				}
			}
			uint64_t l_sz = m_numeric_array_v.size();
			l_ss << "[(" << l_sz << ")";
			if (l_sz > 0)
			{
				l_ss << m_numeric_array_v[0];
			}
			for (uint64_t i = 1; i < l_sz; ++i)
			{
				l_ss << ", " << m_numeric_array_v[i];
			}
			l_ss << ']';
			return l_ss.str();
		}		
		
		uint64_t	encode_to_buff(std::vector<char>& p_buff, uint64_t p_offset = 0)	const
		{
			return txtSerializer::encode_std_vector_to_buff(m_numeric_array_v, p_buff, p_offset);
		}
		uint64_t	decode_from_buff(const std::vector<char>& p_buff, uint64_t p_offset = 0)
		{			
			return txtSerializer::decode_std_vector_from_buff( p_buff, m_numeric_array_v, p_offset);
		}
		std::vector<T>& m_numeric_array_v;
	};
}
#endif !VF_CORE_TXT_VECTOR_NUMBER_SERIALIZER_H
