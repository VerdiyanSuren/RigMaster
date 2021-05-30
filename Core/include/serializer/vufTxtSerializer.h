#ifndef VF_CORE_TXT_SERIALIZER_H
#define VF_CORE_TXT_SERIALIZER_H

#include <vufCoreInclude.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
/**
This class designed to compact writing/reading to/from txt format all data include non text data.
You should do 2 things before use this class in your application
1. Define static variables by macro
	VF_TXT_WRITER_DEFINE_STATIC_VARS();
2. Call init method. It's not critical if your call init more than one time
	vuf::txtSerializer::init();
3. Call release when finish working with serializer
	vuf::txtSerializer::release();

How to use. Examples

	//write
	double l_dbl = 0.122132;
	std::string l_str = "fddfgdsfds";
	std::vector l_buff(1024);
	uint64_t l_offset	= vuf::txtSerializer::encode_number_to_buff(l_dbl,l_buff);
	l_offset			= vuf::txtSerializer::encode_string_to_buff(l_str,l_buff,l_offset);

	//read
	l_offset	= txtSerializer::decode_number_from_buff(l_dbl, l_buff);
	l_offset	= txtSerializer::decode_string_from_buff(l_str, l_buff, l_offset);
	
*/
#define VF_TXT_WRITER_DEFINE_STATIC_VARS()								\
std::map<uint16_t, uint8_t>		vuf::txtSerializer::g_char_to_byte_m;	\
std::vector<uint16_t>			vuf::txtSerializer::g_byte_to_char_v;

#define VF_TXT_SERIALIZER_RELEAZE()	\
vuf::txtSerializer::release();


namespace vuf
{
	class VF_API txtSerializer
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
					g_char_to_byte_m[l_res] = (uint8_t)i;
					//std::cout << "i= " << i << " l_res: " << std::hex  << l_res << " " << l_hi << ", " << l_low << std::dec <<" " <<  l_pattern[l_hi] << l_pattern[l_low]<< std::endl;
				}
			}
			//check system little emndian
			// To Do replace this impimenetation
		}
		static void release()
		{
			g_char_to_byte_m.clear();
			g_byte_to_char_v.clear();
		}

		// binary -> ASCII(encoded)
		static uint64_t encode_to_buff( const void* p_bytes_ptr, uint64_t p_size, std::vector<char>& p_buff, uint64_t p_offset = 0)
		{
			uint64_t l_buff_size				= p_buff.size();
			const unsigned char* l_p_bytes_ptr	= (unsigned char*)p_bytes_ptr;
			
			if (p_offset + p_size * 2 > l_buff_size)
			{
				p_buff.resize(p_offset + p_size * 2);
			}
			
			for (uint64_t i = 0; i < p_size; ++i)
			{
				uint16_t l_value = g_byte_to_char_v[ l_p_bytes_ptr[i] ];
				uint16_t l_hi = l_value & 0xff00;
				l_hi = l_hi >> 8;
				uint16_t l_low = l_value & 0xff;

				p_buff[p_offset + i * 2] = (char)l_hi;
				p_buff[p_offset + i * 2 + 1] = (char)l_low;
			}
			return p_offset + p_size * 2;
		}
		template<typename T>
		static uint64_t encode_std_vector_to_buff( const std::vector<T>& p_src, std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			uint64_t l_vec_size		= p_src.size();
			p_offset = encode_to_buff(&l_vec_size, sizeof(l_vec_size), p_buff, p_offset);
			if (l_vec_size > 0)
			{
				p_offset = encode_to_buff(p_src.data(), sizeof(T) * l_vec_size, p_buff, p_offset);
			}
			return p_offset;
		}
		template<typename T> 
		static uint64_t encode_number_to_buff(const T& p_src, std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			p_offset = encode_to_buff(&p_src, sizeof(p_src), p_buff, p_offset);
			return p_offset;
		}
		static uint64_t encode_string_to_buff(const std::string& p_src, std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			uint64_t l_str_size = p_src.size();
			p_offset = encode_to_buff(&l_str_size, sizeof(l_str_size), p_buff, p_offset);
			if (l_str_size > 0)
			{
				p_offset = encode_to_buff(p_src.data(), p_src.size(), p_buff, p_offset);
			}
			return p_offset;
		}


		// ASCII -> to binary(decoded)
		static uint64_t decode_from_buff( void* p_bytes_ptr, uint64_t p_size, const std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			uint64_t l_buff_size = p_buff.size();
			unsigned char* l_p_bytes_ptr = (unsigned  char*)p_bytes_ptr;

			if (p_offset + p_size * 2 > l_buff_size)
			{
				//error
				return 0;
			}

			for (uint64_t i = 0; i < p_size; i++)
			{
				int16_t l_index = p_buff[p_offset + i * 2];
				l_index = l_index << 8;
				l_index = l_index | p_buff[p_offset + i * 2 + 1];
				l_p_bytes_ptr[i] = g_char_to_byte_m[ l_index ];
			}
			return p_offset + p_size * 2;
		}
		template<typename T>
		static uint64_t decode_std_vector_from_buff( std::vector<T>& p_dist, const std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			uint64_t l_vec_size;
			p_offset = decode_from_buff(&l_vec_size, sizeof(l_vec_size), p_buff, p_offset);
			p_dist.resize(l_vec_size);
			if (l_vec_size > 0)
			{
				p_offset = decode_from_buff(p_dist.data(), sizeof(T) * l_vec_size, p_buff, p_offset);
			}
			return p_offset;			
		}
		template<typename T>
		static uint64_t decode_number_from_buff( T& p_src, std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			p_offset = decode_from_buff(&p_src, sizeof(p_src), p_buff, p_offset);
			return p_offset;
		}
		static uint64_t decode_string_from_buff(std::string& p_src, std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			std::vector<char> l_char_vec;
			p_offset = decode_std_vector_from_buff(l_char_vec, p_buff, p_offset);
			p_src = std::string(l_char_vec.begin(), l_char_vec.end());
			return p_offset;
		}

		
		// Convertors
		template<typename T>
		static T convert_bytes_to_value(const std::vector<unsigned char>& p_bytes, uint64_t* p_offset_ptr = nullptr)
		{
			uint64_t l_offset = 0;
			if (p_offset_ptr != nullptr) l_offset = *p_offset_ptr; 
			T l_res = *( (const T*)(&p_bytes[l_offset]) );
			if (p_offset_ptr != nullptr) *p_offset_ptr += sizeof(T);
			return l_res;
		}
		
		template<>
		static std::string convert_bytes_to_value<std::string>(const std::vector<unsigned char>& p_bytes, uint64_t* p_offset_ptr)
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
		static std::map<uint16_t,uint8_t>	g_char_to_byte_m;	// example AF -> 0x28
		static std::vector<uint16_t>		g_byte_to_char_v;	// example 0x28 -> AF
	};	
}
#endif // !VF_CORE_TXT_SERIALIZER_H
