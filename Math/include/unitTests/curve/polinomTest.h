#pragma once

#include <curves/vufCurve.h>

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

namespace vufMath
{
	template<typename T>
	class vufPolinomCoeffTest
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_vufPolinomCoeff<") +typeid(T).name() + ">")));

			if (test_serialization(p_verbose) == false)	l_status = false;
			if (test_math(p_verbose) == false)			l_status = false;;
			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_vufPolinomCoeff<") + typeid(T).name() + ">")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_vufPolinomCoeff<") + typeid(T).name() + ">")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			// degree 0
			vufPolinomCoeff<T, 0> l_polinom_00;
			if (p_verbose == true)
			{
				std::cout << l_polinom_00 << std::endl;
			}
			vufPolinomCoeff<T, 1> l_polinom_01;
			if (p_verbose == true)
			{
				std::cout << l_polinom_01 << std::endl;
			}

			// degree 1
			vufPolinomCoeff<T, 5> l_polinom_1 = vufPolinomCoeff<T, 5>::random_polinom();
			vufPolinomCoeff<T, 5> l_polinom_2 = vufPolinomCoeff<T, 5>::random_polinom();
			vufPolinomCoeff<T, 5> l_polinom_3 = vufPolinomCoeff<T, 5>::random_polinom();
			l_polinom_1.a[1] = 0;
			if (p_verbose == true)
			{
				std::cout << l_polinom_1.to_string(-1, 2) << std::endl;
				std::cout << l_polinom_2 << std::endl;
				std::cout << l_polinom_3 << std::endl;
			}
			std::vector<char> l_buff;
			uint64_t l_offset;
			l_offset = l_polinom_1.to_binary(l_buff);
			l_offset = l_polinom_2.to_binary(l_buff, l_offset);
			l_offset = l_polinom_3.to_binary(l_buff, l_offset);

			vufPolinomCoeff<T, 5> l_polinom_11, l_polinom_21, l_polinom_31;
			l_offset = l_polinom_11.from_binary(l_buff);
			l_offset = l_polinom_21.from_binary(l_buff, l_offset);
			l_offset = l_polinom_31.from_binary(l_buff, l_offset);

			if (l_polinom_1 != l_polinom_11 ||
				l_polinom_2 != l_polinom_21 ||
				l_polinom_3 != l_polinom_31)
			{
				VF_LOG_ERR("........Failed To/from binary test");
				std::cout << l_polinom_1 << " " << l_polinom_11 <<std::endl;
				std::cout << l_polinom_2 << " " << l_polinom_21 <<std::endl;
				std::cout << l_polinom_3 << " " << l_polinom_31 <<std::endl;
				return false;
			}
			std::cout << "........To/from binary test passed successfully" << std::endl;
			
			// encode/decode
			l_offset = l_polinom_1.encode_to_buff(l_buff);
			l_offset = l_polinom_2.encode_to_buff(l_buff, l_offset);
			l_offset = l_polinom_3.encode_to_buff(l_buff, l_offset);
			vufPolinomCoeff<T, 5> l_polinom_12, l_polinom_22, l_polinom_32;
			l_offset = l_polinom_12.decode_from_buff(l_buff);
			l_offset = l_polinom_22.decode_from_buff(l_buff, l_offset);
			l_offset = l_polinom_32.decode_from_buff(l_buff, l_offset);
			if (l_polinom_1 != l_polinom_12 ||
				l_polinom_2 != l_polinom_22 ||
				l_polinom_3 != l_polinom_32)
			{
				VF_LOG_ERR("........Failed Encode/decode to/from buffer test");
				return false;
			}
			std::cout << "........Encode/decode to/from buffer test passed successfully" << std::endl;
			 
			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
		bool test_math(bool p_verbose = false)
		{
			std::cout << "....Math Test" << std::endl;

			std::cout << "....Math Test Pass Successfully" << std::endl;
			return true;
		}
	};
}