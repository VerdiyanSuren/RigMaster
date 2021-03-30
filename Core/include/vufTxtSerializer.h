#ifndef VF_MATH_TXT_SERIALIZER_H
#define VF_MATH_TXT_SERIALIZER_H

#include <map>
#include <vector>
#include <string>

/**
This class designed to compact writing/reading to/from txt format.
You should do 2 thinks before use this class in your application
1. Define static variables by macro
	VF_TXT_WRITER_DEFIEN_STATIC_VARS();
2. Call init method. It's not critiacal if your call init more than one time
	vuf::txtSerializer::init();
3. Call release when finish working with serializer
	vuf::txtSerializer::release();

How to use. Examples
	double l_dbl = 0.122132;
	auto l_txt_vector = txtSerializer::to_chars(&l_dbl,sizeof(l_dbl));
	auto l_string_txt = txtSerializer::to_chars_string(&l_dbl,sizeof(l_dbl));

	auto l_bytes_vector = txtSerializer::to_bytes(l_string_txt);
	double l_read_dbl = txtSerializer::convert_bytes_to_value<double>(l_bytes_vector);
	
*/
#define VF_TXT_WRITER_DEFIEN_STATIC_VARS()								\
std::map<uint16_t, uint8_t>		vuf::txtSerializer::g_char_to_byte_v;	\
std::vector<uint16_t>			vuf::txtSerializer::g_byte_to_char_v;	

namespace vuf
{
	class txtSerializer
	{
	public:
		txtSerializer()
		{}
		static void init()
		{
			if (g_byte_to_char_v.size() != 256)
			{				
				g_byte_to_char_v.resize(256);
				std::vector<unsigned char> l_pattern = {'Q','W','E','R','T','Y','U','I','O','P',
														'A','S','D','F','G','H','J','K','L',
														'Z','X','C','V','B','N','M' };
				for (uint16_t i = 0; i < 256; ++i)
				{
					uint16_t l_hi	= i & 0xf0;
					l_hi = l_hi >> 4;
					uint16_t l_low	= i & 0xf;
					uint16_t l_res	= (l_pattern[l_hi] << 8) | l_pattern[l_low];
					g_byte_to_char_v[i] = l_res;
					g_char_to_byte_v[l_res] = (uint8_t)i;
					//std::cout << "i= " << i << " l_res: " << std::hex  << l_res << " " << l_hi << ", " << l_low << std::dec <<" " <<  l_pattern[l_hi] << l_pattern[l_low]<< std::endl;
				}
			}
		}
		static void release()
		{
			g_char_to_byte_v.clear();
			g_byte_to_char_v.clear();
		}
		// binary -> ASCII(encoded)
		static std::vector<unsigned char>	to_chars(const unsigned char* p_bytes_ptr, uint64_t p_size)
		{
			std::vector<unsigned char> l_res;
			l_res.resize(p_size * 2);
			for (uint64_t i = 0; i < p_size; ++i)
			{
				uint16_t l_value = g_byte_to_char_v[p_bytes_ptr[i]];
				uint16_t l_hi = l_value & 0xff00;
				l_hi = l_hi >> 8;
				uint16_t l_low = l_value & 0xff;

				l_res[i*2]		= (unsigned char)l_hi;
				l_res[i*2 + 1]	= (unsigned char)l_low;
			}
			return l_res;
		}
		template<typename T>
		static std::vector<unsigned char>	to_chars(const T& p_value)
		{
			unsigned char* l_ptr = (unsigned char*)&p_value;
			uint64_t l_sz = sizeof(T);
			return to_chars(l_ptr, l_sz);
		}
		static std::vector<unsigned char>	to_chars(const std::string p_str)
		{
			uint64_t l_sz = p_str.length();
			auto l_sz_v = to_chars((const unsigned char*)&l_sz, sizeof(l_sz));
			auto l_str_v = to_chars((const unsigned char*)p_str.c_str(),p_str.length());
			l_sz_v.insert(l_sz_v.end(), l_str_v.begin(), l_str_v.end());
			return l_sz_v;
		}
		
		
		static std::string					to_chars_string(const unsigned char* p_bytes_ptr, uint64_t p_size)
		{
			auto l_res = to_chars(p_bytes_ptr, p_size);
			return std::string((const char*)l_res.data(), l_res.size());
		}
		template<typename T>
		static std::string					to_chars_string(const T& p_value)
		{
			const unsigned char* l_ptr = (const unsigned char*)&p_value;
			uint64_t l_sz = sizeof(T);
			return to_chars_string(l_ptr, l_sz);
		}
		static std::string					to_chars_string(const std::string& p_str)
		{
			uint64_t l_sz = p_str.length();
			auto l_sz_v		= to_chars_string((const unsigned char*)&l_sz, sizeof(l_sz));
			auto l_string	=  to_chars_string((const unsigned char*)p_str.c_str(), p_str.length());
			return l_sz_v + l_string;
		}


		// ASCII -> to binary(decoded)
		static std::vector<unsigned char>	to_bytes(const unsigned char* p_char_ptr, uint64_t p_size)
		{
			std::vector<unsigned char> l_res;
			l_res.resize(p_size / 2);
			for (uint64_t i = 0; i < p_size; i += 2)
			{
				uint16_t l_index = p_char_ptr[i];
				l_index = l_index << 8;
				l_index = l_index | p_char_ptr[i + 1];
				l_res[i/2] = g_char_to_byte_v[l_index];
			}
			return l_res;
		}
		static std::vector<unsigned char>	to_bytes(const std::string& p_str)
		{
			return to_bytes((const unsigned char*)p_str.c_str(), p_str.length());
		}


		// Convertors
		template<typename T>
		T convert_bytes_to_value(const std::vector<unsigned char>& p_bytes, uint64_t* p_offset_ptr = nullptr)
		{
			uint64_t l_offset = 0;
			if (p_offset_ptr != nullptr) l_offset = *p_offset_ptr; 
			T l_res = *( (const T*)(&p_bytes[l_offset]) );
			if (p_offset_ptr != nullptr) *p_offset_ptr += sizeof(T);
			return l_res;
		}
		
		template<>
		std::string convert_bytes_to_value<std::string>(const std::vector<unsigned char>& p_bytes, uint64_t* p_offset_ptr)
		{
			uint64_t l_offset = 0;
			if (p_offset_ptr != nullptr) l_offset = *p_offset_ptr;

			uint64_t	l_sz	= convert_bytes_to_value<uint64_t>(p_bytes, &l_offset);
			std::string l_str	=  std::string((const char*)&p_bytes[l_offset], l_sz);
			l_offset += l_sz;
			if (p_offset_ptr != nullptr) *p_offset_ptr += l_offset;

			return l_str;
		}
		
	private:
		static std::map<uint16_t,uint8_t>	g_char_to_byte_v;	// example AF -> 0x28
		static std::vector<uint16_t>		g_byte_to_char_v;	// example 0x28 -> AF
	};	
}
#endif // !VF_MATH_WRITER_H
