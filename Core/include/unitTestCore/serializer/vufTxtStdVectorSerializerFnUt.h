#pragma once


#include <serializer/vufTxtSerializer.h>
#include <serializer/vufTxtStdVectorSerializerFn.h>
#include <iostream>
#include <vufLog.h>
namespace vuf
{
	class txtStdVectorSerializerFnUt
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			vuf::txtSerializer::init();
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO(vufStringUtils::string_padding(std::string("START_TESTS_FOR_txtStdVectorSerializerFn")));

			if (test_serialization(p_verbose) == false)	l_status = false;

			if (l_status == true)
			{
				VF_LOG_INFO(vufStringUtils::string_padding(std::string("END_TESTS_FOR_txtStdVectorSerializerFn")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR(vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_txtStdVectorSerializerFn")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
	private:
		bool test_serialization(bool p_verbose = false)
		{
			if (test_encode_decode<int>(p_verbose) == true)
			{
				std::cout << "___int serialization succeed" << std::endl;
			}
			else
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("___int seriualization failed")));
				return false;
			}
			if (test_encode_decode<double>(p_verbose) == true)
			{
				std::cout << "___double serialization succeed" << std::endl;
			}
			else
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("___double seriualization failed")));
				return false;
			}

			return true;
		}
		template <typename T>
		bool test_encode_decode(bool p_verbose = false)
		{
			std::vector<T> l_vec_1(10), l_vec_2, l_vec_3(5);
			for (uint64_t i = 0; i < l_vec_1.size(); ++i)
			{
				l_vec_1[i] = ((T)rand() - RAND_MAX / 2) + ((T)rand() - RAND_MAX) / ((T)RAND_MAX);
			}
			for (uint64_t i = 0; i < l_vec_2.size(); ++i)
			{
				l_vec_2[i] = ((T)rand() - RAND_MAX / 2) + ((T)rand() - RAND_MAX) / ((T)RAND_MAX);
			}
			for (uint64_t i = 0; i < l_vec_3.size(); ++i)
			{
				l_vec_3[i] = ((T)rand() - RAND_MAX / 2) + ((T)rand() - RAND_MAX) / ((T)RAND_MAX);
			}
			
			std::vector<char> l_buff;
			txtStdVectorSerializerFn<T> l_fn_1(l_vec_1);
			txtStdVectorSerializerFn<T> l_fn_2(l_vec_2);
			txtStdVectorSerializerFn<T> l_fn_3(l_vec_3);
			l_fn_1.m_numeric_array_v[0] = 1000;

			uint64_t l_offset	= l_fn_1.encode_to_buff( l_buff);
			l_offset			= l_fn_2.encode_to_buff( l_buff, l_offset);
			l_offset			= l_fn_3.encode_to_buff( l_buff, l_offset);

			std::vector<T> l_vec_11, l_vec_21, l_vec_31;
			txtStdVectorSerializerFn<T> l_fn_11(l_vec_11);
			txtStdVectorSerializerFn<T> l_fn_21(l_vec_21);
			txtStdVectorSerializerFn<T> l_fn_31(l_vec_31);
			l_offset = l_fn_11.decode_from_buff(l_buff);
			l_offset = l_fn_21.decode_from_buff(l_buff, l_offset);
			l_offset = l_fn_31.decode_from_buff(l_buff, l_offset);

			if (p_verbose == true)
			{
				std::string l_str(l_buff.data(), l_buff.size());
				std::cout << "____---------------------------------------------------------------------" << std::endl;
				std::cout << "____encode txtStdVectorSerializerFn::encode_to_buff" << std::endl;
				std::cout << "____initial " << l_fn_1.to_string() << std::endl;
				std::cout << "____initial " << l_fn_2.to_string() << std::endl;
				std::cout << "____initial " << l_fn_3.to_string() << std::endl;
				std::cout << std::endl;
				std::cout << "____encoded " << l_str << std::endl;
				std::cout << std::endl;
				std::cout << "____decoded txtStdVectorSerializerFn::decode_from_buff" << std::endl;
				std::cout << "____decoded " << l_fn_11.to_string() << std::endl;
				std::cout << "____decoded " << l_fn_21.to_string() << std::endl;
				std::cout << "____decoded " << l_fn_31.to_string() << std::endl;
			}
			if (l_vec_1 != l_vec_11 || l_vec_2 != l_vec_21 || l_vec_3 != l_vec_31) 
			{
				VF_LOG_ERR("____Failed txtStdVectorSerializerFn::encode_to_buff");
				return false;
			}

			return true;
		}
	};
}
