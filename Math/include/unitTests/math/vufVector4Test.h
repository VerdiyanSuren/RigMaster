#pragma once
#include <math/vufVector.h>

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
namespace vufMath
{
	template< typename T >
	class vufVector4Test
	{
	public:
		bool run(bool p_verbose = false)
		{	
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_") + typeid(vufVector4<T>).name())));

			if (test_serialization(p_verbose) == false)	l_status = false;
			if (test_math(p_verbose) == false)			l_status = false;;
			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_") + typeid(vufVector4<T>).name())));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_") + typeid(vufVector4<T>).name())));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
	private:
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			// start vectors are random 
			vufVector4<T> l_v_1 = vufVector4<T>::random(true);
			vufVector4<T> l_v_2 = vufVector4<T>::random(true);
			vufVector4<T> l_v_3 = vufVector4<T>::random(true);
			// convert them to string with default precision
			std::string str_1 = l_v_1.to_string();
			std::string str_2 = l_v_2.to_string();
			std::string str_3 = l_v_3.to_string();
			std::string str_4 = str_1 + str_2 + str_3;
			
			// get vectors back from strings
			vufVector4<T> l_res_1, l_res_2, l_res_3;
			uint64_t l_offset	= l_res_1.from_string(str_4);
			l_offset			= l_res_2.from_string(str_4, l_offset);
			l_offset			= l_res_3.from_string(str_4, l_offset);

			if (	l_res_1.is_equivalent( l_v_1, 0.00001 ) == false || 
					l_res_2.is_equivalent( l_v_2, 0.00001 ) == false || 
					l_res_3.is_equivalent( l_v_3, 0.00001 ) == false)
			{
				std::cout << l_v_1 << " " << l_res_1 << std::endl;
				std::cout << l_v_2 << " " << l_res_2 << std::endl;
				std::cout << l_v_3 << " " << l_res_3 << std::endl;
 				std::cout << "........Failed to/from string test" << std::endl;
				return false;
			}
			// convert them to string with maximum precision
			str_1 = l_v_1.to_string(64);
			str_2 = l_v_2.to_string(64);
			str_3 = l_v_3.to_string(64);
			str_4 = str_1 + str_2 + str_3;
			// get vector back from strings
			l_offset = l_res_1.from_string(str_4);
			l_offset = l_res_2.from_string(str_4, l_offset);
			l_offset = l_res_3.from_string(str_4, l_offset);
			if (l_res_1 != l_v_1 ||
				l_res_2 != l_v_2 ||
				l_res_3 != l_v_3)
			{
				std::cout << l_v_1 << " " << l_res_1 << std::endl;
				std::cout << l_v_2 << " " << l_res_2 << std::endl;
				std::cout << l_v_3 << " " << l_res_3 << std::endl;
				std::cout << "........Failed Serialization to/from string with maximum precision" << std::endl;
				return false;
			}

			std::cout << "........To/from string test passed successfully" << std::endl;


			// To\From binary
			std::vector<char> l_buff(1000);
			l_offset = l_v_1.to_binary(l_buff);
			l_offset = l_v_2.to_binary(l_buff, l_offset);
			l_offset = l_v_3.to_binary(l_buff, l_offset);
			vufVector4<T> l_res_11, l_res_21, l_res_31;
			l_offset = l_res_11.from_binary(l_buff);
			l_offset = l_res_21.from_binary(l_buff, l_offset);
			l_offset = l_res_31.from_binary(l_buff, l_offset);
			if ( l_res_11 != l_v_1 || l_res_21 != l_v_2 || l_res_31 != l_v_3 )
			{
				std::cout << "........Failed  to/from binary test" << std::endl;
				return false;
			}
			std::cout << "........To/from binary test passed successfully" << std::endl;

			// encode/decode to from buffer			
			l_offset = l_v_1.encode_to_buff(l_buff);
			l_offset = l_v_2.encode_to_buff(l_buff,l_offset);
			l_offset = l_v_3.encode_to_buff(l_buff,l_offset);
			vufVector4<T> l_res_12, l_res_22, l_res_32;
			l_offset = l_res_12.decode_from_buff(l_buff);
			l_offset = l_res_22.decode_from_buff(l_buff, l_offset);
			l_offset = l_res_32.decode_from_buff(l_buff, l_offset);
			if (l_res_12 != l_v_1 || l_res_22 != l_v_2 || l_res_32 != l_v_3)
			{
				std::cout << "........Failed Encode/decode to/from buffer test" << std::endl;
				return false;
			}
			std::cout << "........Encode/decode to/from buffer test passed successfully" << std::endl;
			std::cout << "....Serialization Test Passed Successfully" << std::endl;
			return true;
		}
		bool test_math(bool p_verbose = false)
		{
			std::cout << "....Math Test" << std::endl;

			vufVector4<T> l_dflt, l_res;
			vufVector4<T> l_cnstr(1., 2., 3., 4.);
			vufVector4<T> l_other(l_cnstr);
			vufMatrix4<T> l_matr;

			// constructors
			if (l_dflt.x != .0 || l_dflt.y != .0 || l_dflt.z != .0 || l_dflt.w != 1.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed default constructor"));
				return false;
			}
			if (l_cnstr.x != 1. || l_cnstr.y != 2. || l_cnstr.z != 3. || l_cnstr.w != 4.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed constructor parameters"));
				return false;
			}
			if (l_other.x != 1. || l_other.y != 2. || l_other.z != 3. || l_other.w != 4.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed copy constructor"));
				return false;
			}

			// operators
			if (l_cnstr(0) != 1. || l_cnstr(1) != 2. || l_cnstr(2) != 3. || l_cnstr(3) != 4.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator()"));
				return false;
			}
			if (l_cnstr[0] != 1. || l_cnstr[1] != 2. || l_cnstr[2] != 3. || l_cnstr[3] != 4.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator [] const"));
				return false;
			}

			l_other[0] = 11.; l_other[1] = 12.; l_other[2] = 13.; l_other[3] = 14.;
			if (l_other[0] != 11. || l_other[1] != 12. || l_other[2] != 13. || l_other[3] != 14.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator [] "));
				return false;
			}


			// operator *
			l_matr = vufMatrix4<T>::numerate(1);
			l_dflt.set(1., 2., 3., 4.);
			l_res = l_dflt * l_matr;
			if (l_res != vufVector4<T>(90, 100, 110, 120))
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator *(matrix)"));
				return false;
			}
			l_dflt *= l_matr;
			if (l_dflt != vufVector4<T>(90, 100, 110, 120))
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator *=(matrix)"));
				return false;
			}
			std::cout << "....Math Test Pass Successfully" << std::endl;
			return true;
			
		}
	};
}