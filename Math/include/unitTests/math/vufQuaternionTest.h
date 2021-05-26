#pragma once


#include <math/vufVector.h>
#include <math/vufMatrix.h>
#include <math/vufQuaternion.h>

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

#include <iomanip>


#include <iomanip>
namespace vufMath
{
	template< typename T >
	class vufQuaternionTest
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_") + typeid(vufQuaternion<T>).name())));

			if (test_serialization(p_verbose) == false)	l_status = false;

			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_") + typeid(vufQuaternion<T>).name())));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_") + typeid(vufQuaternion<T>).name())));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			// start quaternions are random 
			vufQuaternion<T> l_q_1 = vufQuaternion<T>::random_quaternion(true);
			vufQuaternion<T> l_q_2 = vufQuaternion<T>::random_quaternion(true);
			vufQuaternion<T> l_q_3 = vufQuaternion<T>::random_quaternion(true);
			if (p_verbose == true)
			{
				std::cout << "........Example Quaternions" << std::endl;
				std::cout << "............" << l_q_1 << std::endl;
				std::cout << "............" << l_q_2 << std::endl;
				std::cout << "............" << l_q_3 << std::endl;
			}
			
			// convert them to string with default precision
			std::string str_1 = l_q_1.to_string();
			std::string str_2 = l_q_2.to_string();
			std::string str_3 = l_q_3.to_string();
			std::string str_4 = str_1 + str_2 + str_3;
			if (p_verbose == true)
			{
				std::cout << "........Converted_to_string" << std::endl;
				std::cout << "............" << str_1 << std::endl;
				std::cout << "............" << str_2 << std::endl;
				std::cout << "............" << str_3 << std::endl;
				std::cout << "............" << str_4 << std::endl;
			}
			
			// get quaternions back from strings
			vufQuaternion<T> l_res_1, l_res_2, l_res_3;
			uint64_t l_offset	= l_res_1.from_string(str_4);
			l_offset			= l_res_2.from_string(str_4, l_offset);
			l_offset			= l_res_3.from_string(str_4, l_offset);
			// check them / error has to be less than 0.00001
			if (	l_res_1.is_equivalent( l_q_1, 0.00001) == false || 
					l_res_2.is_equivalent( l_q_2, 0.00001) == false|| 
					l_res_3.is_equivalent( l_q_3, 0.00001) == false)
			{
				std::cout << l_q_1 << " " << l_res_1 << std::endl;
				std::cout << l_q_2 << " " << l_res_2 << std::endl;
				std::cout << l_q_3 << " " << l_res_3 << std::endl;
				std::cout << "........Failed Serialization to/from string with default precision" << std::endl;
				return false;
			}
			// convert them to string with maximum precision
			str_1 = l_q_1.to_string(64);
			str_2 = l_q_2.to_string(64);
			str_3 = l_q_3.to_string(64);
			str_4 = str_1 + str_2 + str_3;
			// get quaternions back from strings
			l_offset = l_res_1.from_string(str_4);
			l_offset = l_res_2.from_string(str_4, l_offset);
			l_offset = l_res_3.from_string(str_4, l_offset);
			if (l_res_1 != l_q_1 ||
				l_res_2 != l_q_2 ||
				l_res_3 != l_q_3 )
			{
				std::cout << l_q_1 << " " << l_res_1 << std::endl;
				std::cout << l_q_2 << " " << l_res_2 << std::endl;
				std::cout << l_q_3 << " " << l_res_3 << std::endl;
				std::cout << "........Failed Serialization to/from string with maximum precision" << std::endl;
				return false;
			}
			std::cout << "........Serialization  to/from string Successfully" << std::endl;
			// convert quaternions to byte array
			std::vector<unsigned char> l_buff;
			l_q_1.to_binary(l_buff);
			l_q_2.to_binary(l_buff);
			l_q_3.to_binary(l_buff);
			vufQuaternion<T> l_res_11, l_res_22, l_res_33;
			l_offset = l_res_11.from_binary(l_buff);
			l_offset = l_res_22.from_binary(l_buff, l_offset);
			l_offset = l_res_33.from_binary(l_buff, l_offset);
			if (l_res_11 != l_q_1 || l_res_22 != l_q_2 || l_res_33 != l_q_3)
			{
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			std::cout << "........Serialization  to/from binary Successfully" << std::endl;
			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
	};
}
