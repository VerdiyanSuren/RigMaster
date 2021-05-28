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
			vufPolinomCoeff<T, 0> l_polinom_01; 
			if (p_verbose == true)
			{
				std::cout << l_polinom_01 << std::endl;
			}

			// degree 1
			vufPolinomCoeff<T, 5> l_polinom_11 = vufPolinomCoeff<T, 5>::random_polinom();
			vufPolinomCoeff<T, 1> l_polinom_12 = vufPolinomCoeff<T, 1>::random_polinom();
			vufPolinomCoeff<T, 1> l_polinom_13 = vufPolinomCoeff<T, 1>::random_polinom();
			l_polinom_11.a[1] = 0;
			if (p_verbose == true)
			{
				std::cout << l_polinom_11.to_string(-1,2) << std::endl;
				std::cout << l_polinom_12 << std::endl;
				std::cout << l_polinom_13 << std::endl;
			}
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