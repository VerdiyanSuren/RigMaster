#ifndef VF_CORE_TXT_VECTOR_NUMBER_SERIALIZER_H
#define VF_CORE_TXT_VECTOR_NUMBER_SERIALIZER_H

#include <serializer/vufTxtSerializer.h>
#include <vector>
#include <sstream>
#include <string>

namespace vuf
{
	template<typename T>
	class VF_API stdVectorNumberSerializerFn
	{
		stdVectorNumberSerializerFn() = delete;
	public:
		stdVectorNumberSerializerFn(std::vector<T>& p_vec) :m_numeric_array_v(p_vec) {};
		stdVectorNumberSerializerFn(const stdVectorNumberSerializerFn& p_other) :m_numeric_array_v(p_other.m_numeric_array_v) {}
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
		
		uint64_t	to_binary(std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)	const
		{
			uint64_t l_sz			= m_numeric_array_v.size();					// size of source array
			uint64_t l_data_sz		= sizeof(uint64_t) + l_sz * sizeof(T);		// size of data + sizeof uint64_t(length of array)
			if (p_offset == 0)
			{
				// write to end of buffer.
				// resize it
				p_offset = p_buff.size();										// size of old binary buffer to store data
			}
			if (p_buff.size() < p_offset + l_data_sz)
			{
				p_buff.resize(p_offset + l_data_sz);							// resize buffer to have enougth space for array data
			}
			/*write size*/
			uint64_t l_little_sz = txtSerializer::to_little_endian<uint64_t>(l_sz);
			std::memcpy((void*)&p_buff[p_offset], (const void*)&l_little_sz, sizeof(uint64_t));
			p_offset += sizeof(uint64_t);
			/*write vectors*/
			if (l_sz != 0)
			{
				for (uint64_t i = 0; i < l_sz; ++i)	
				{
					T l_val = m_numeric_array_v[i];
					l_val = txtSerializer::to_little_endian<T>(l_val);
					
					unsigned char* l_ptr = (unsigned char*)&l_val;
					for (uint64_t j = 0; j < sizeof(T); ++j)
					{
						p_buff[p_offset++] = l_ptr[j];
					}
				}
			}
			return p_offset;
		}
		uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)
		{
			uint64_t l_sz;
			uint64_t l_buff_offset = p_offset;
			/* Read size of vector */
			std::memcpy((void*)&l_sz, (const void*)&p_buff[l_buff_offset], sizeof(uint64_t));
			l_sz = txtSerializer::to_little_endian<uint64_t>(l_sz);
			l_buff_offset += sizeof(uint64_t);
			m_numeric_array_v.resize(l_sz);
			if (l_sz == 0)
			{
				return l_buff_offset;
			}
			for (uint64_t i = 0; i < l_sz; ++i)
			{
				T l_val;
				unsigned char* l_ptr = (unsigned char*)&l_val;
				for (uint64_t j = 0; j < sizeof(T); ++j)
				{
					l_ptr[j] = p_buff[l_buff_offset++];
				}
				m_numeric_array_v[i] = txtSerializer::to_little_endian<T>(l_val);
			}
			return l_buff_offset;
		}
		std::vector<T>& m_numeric_array_v;
	};
}
#endif !VF_CORE_TXT_VECTOR_NUMBER_SERIALIZER_H
