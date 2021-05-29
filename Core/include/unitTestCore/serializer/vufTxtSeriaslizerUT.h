#pragma once


#include <serializer/vufTxtSerializer.h>
#include <iostream>
#include <vufLog.h>
namespace vuf
{
	class txtSeriaslizerUT
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			vuf::txtSerializer::init();
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO(vufStringUtils::string_padding(std::string("START_TESTS_FOR_TxtSeriaslizer")));

			if (test_serialization(p_verbose) == false)	l_status = false;

			if (l_status == true)
			{
				VF_LOG_INFO(vufStringUtils::string_padding(std::string("END_TESTS_FOR_TxtSeriaslizer")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR(vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TxtSeriaslizer")));
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
			//-----------------------------------------------------------------------------------------------------------------------------
			// encode_to_buff
			// decode_from_buff

			// encode
			T l_val_1 = (T)-50676870.122132;
			T l_val_2 = (T)-368687687.122132;
			T l_val_3 = (T)-798698689.122132;
			constexpr char* l_char_ptr = R"(
				this is a 
				multiline 
				text example
				)";
			std::string l_str_1(l_char_ptr);
			std::vector<T> l_val_arr_1(10);
			std::vector<T> l_val_arr_2(10);
			for (uint64_t i = 0; i < 10; ++i)
			{
				l_val_arr_1[i] = ((T)rand() - RAND_MAX/2) + ((T)rand() - RAND_MAX) / ((T)RAND_MAX);
				l_val_arr_2[i] = ((T)rand() - RAND_MAX/2) + ((T)rand() - RAND_MAX) / ((T)RAND_MAX);
			}

			std::vector<char> l_buff;
			uint64_t l_offset	= txtSerializer::encode_number_to_buff(l_val_1,l_buff);
			l_offset			= txtSerializer::encode_number_to_buff(l_val_2, l_buff, l_offset);
			l_offset			= txtSerializer::encode_number_to_buff(l_val_3, l_buff, l_offset);
			l_offset			= txtSerializer::encode_std_vector_to_buff(l_val_arr_1, l_buff, l_offset);
			l_offset			= txtSerializer::encode_std_vector_to_buff(l_val_arr_2, l_buff, l_offset);
			l_offset			= txtSerializer::encode_string_to_buff(l_str_1, l_buff, l_offset);
			// decode
			T l_val_11;
			T l_val_21;
			T l_val_31;
			std::vector<T>		l_val_arr_11;
			std::vector<T>		l_val_arr_21;
			std::string			l_str_11;
			l_offset = txtSerializer::decode_number_from_buff(l_val_11, l_buff);
			l_offset = txtSerializer::decode_number_from_buff(l_val_21, l_buff, l_offset);
			l_offset = txtSerializer::decode_number_from_buff(l_val_31, l_buff, l_offset);
			l_offset = txtSerializer::decode_std_vector_from_buff(l_val_arr_11, l_buff, l_offset);
			l_offset = txtSerializer::decode_std_vector_from_buff(l_val_arr_21, l_buff, l_offset);
			l_offset = txtSerializer::decode_string_from_buff(l_str_11, l_buff, l_offset);

			if (p_verbose == true)
			{
				std::string l_str(l_buff.data(), l_buff.size());
				std::cout << "____---------------------------------------------------------------------" << std::endl;
				std::cout << "____encode txtSerializer::encode_to_buff" << std::endl;
				std::cout << "____initial " << l_val_1 << " " << l_val_2 << " " << l_val_3 << std::endl;
				std::cout << "____initial (" << l_val_arr_1.size() << ")[" << l_val_arr_1[0];
				for (uint64_t i = 0; i < l_val_arr_1.size(); ++i)
				{
					std::cout << "," << l_val_arr_1[i];
				}
				std::cout << "]" << std::endl;
				std::cout << "____initial (" << l_val_arr_2.size() << ")[" << l_val_arr_2[0];
				for (uint64_t i = 0; i < l_val_arr_2.size(); ++i)
				{
					std::cout << "," << l_val_arr_2[i];
				}
				std::cout << "]" << std::endl;
				std::cout << "____initial " << l_str_1 << std::endl << std::endl;

				std::cout << "____encoded " << l_str << std::endl;
				std::cout << std::endl;


				std::cout << "____decoded txtSerializer::decode_from_buff" << std::endl;
				std::cout << "____decoded " << l_val_11 << " " << l_val_21 << " " << l_val_31 << std::endl;
				
				std::cout << "____decoded (" << l_val_arr_11.size() << ")[" << l_val_arr_11[0];
				for (uint64_t i = 0; i < l_val_arr_11.size(); ++i)
				{
					std::cout << "," << l_val_arr_11[i];
				}
				std::cout << "]" << std::endl;
				std::cout << "____decoded (" << l_val_arr_21.size() << ")[" << l_val_arr_21[0];
				for (uint64_t i = 0; i < l_val_arr_21.size(); ++i)
				{
					std::cout << "," << l_val_arr_21[i];
				}
				std::cout << "]" << std::endl;
				std::cout << "____decoded " << l_str_11 << std::endl << std::endl;
				std::cout << "____---------------------------------------------------------------------" << std::endl;
			}
			if (l_val_1 != l_val_11			|| l_val_2 != l_val_21			|| l_val_3 != l_val_31 || 
				l_val_arr_1 != l_val_arr_11 || l_val_arr_2 != l_val_arr_21	|| l_str_1 != l_str_11)
			{
				VF_LOG_ERR("____Failed txtSerializer::encode_to_buff");
				return false;
			}
			//-----------------------------------------------------------------------------------------------------------------------------
			// encode_to_chars
			return true;
		}
	};
}

