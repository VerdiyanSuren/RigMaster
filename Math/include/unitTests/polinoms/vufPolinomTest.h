#pragma once

#include <math/vufPolinom.h>
#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

namespace vufMath
{
	template< typename T >
	class vufPolinomTest
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_POLINOM"))));

			if (test_roots() == false)	l_status = false;

			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_") + typeid(vufPolinomTest<T>).name())));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_") + typeid(vufPolinomTest<T>).name())));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
	private:
		bool test_roots(bool p_verbose = false)
		{
			std::cout << "....Solve Roots Test" << std::endl;
			vufPolinomCoeff<T, 1> l_p1;
			vufPolinomCoeff<T, 2> l_p2;
			vufPolinomCoeff<T, 3> l_p3;
			vufPolinomCoeff<T, 4> l_p4;
			vufPolinomCoeff<T, 5> l_p5;
			vufPolinomCoeff<T, 6> l_p6;
			vufPolinomCoeff<T, 7> l_p7;
			vufPolinomCoeff<T, 8> l_p8;
			vufPolinomCoeff<T, 9> l_p9;
			// degree 1
			// degree 2
			// degree 3
			T p_arr3[] = { 1., 1., -3., 1. };
			T p_slv3[3];
			l_p3.set(p_arr3);
			if (l_p3.find_root_on_interval(-1, 3, p_slv3) != 3)
			{
				std::cout << "........Failed Solve Roots Test For Polinom degree 3" << std::endl;
			}
			std::cout << "....Solve Roots Test Have Been Passed Successfully" << std::endl;
			return true;
		}
	};
}
