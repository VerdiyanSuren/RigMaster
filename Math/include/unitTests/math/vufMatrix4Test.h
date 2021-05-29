#pragma once

#include <math/vufVector.h>
#include <math/vufMatrix.h>

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

#include <iomanip>
namespace vufMath
{
	template< typename T >
	class vufMatrix4Test
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_") + typeid(vufMatrix4<T>).name())));

			if (test_serialization() == false)	l_status = false;
			if (test_math() == false)			l_status = false;;

			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_") + typeid(vufMatrix4<T>).name())));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_") + typeid(vufMatrix4<T>).name())));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
	private:
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			// start matricies are random 
			vufMatrix4<T> l_m_1 = vufMatrix4<T>::random_matrix();
			vufMatrix4<T> l_m_2 = vufMatrix4<T>::random_matrix();
			vufMatrix4<T> l_m_3 = vufMatrix4<T>::random_matrix();
			vufMatrix4<T> l_src_1(l_m_1);
			vufMatrix4<T> l_src_2(l_m_2);
			vufMatrix4<T> l_src_3(l_m_3);
			// convert them to string with default precision
			std::string str_1 = l_src_1.to_string();
			std::string str_2 = l_src_2.to_string();
			std::string str_3 = l_src_3.to_string();
			std::string str_4 = str_1 + str_2 + str_3;

			vufMatrix4<T> l_res_1;
			vufMatrix4<T> l_res_2;
			vufMatrix4<T> l_res_3;
			uint64_t l_offset = 0;
			// get matricies back from strings
			l_offset = l_res_1.from_string(str_4);
			l_offset = l_res_2.from_string(str_4, l_offset);
			l_offset = l_res_3.from_string(str_4, l_offset);
			if (l_res_1.is_equivalent(l_src_1, 0.00001) == false ||
				l_res_2.is_equivalent(l_src_2, 0.00001) == false ||
				l_res_3.is_equivalent(l_src_3, 0.00001) == false)
			{
				std::cout << "........Failed Serialization to/from string test 1" << std::endl;
				return false;
			}
			// convert them to string with maximum precision
			str_1 = l_src_1.to_string(64);
			str_2 = l_src_2.to_string(64);
			str_3 = l_src_3.to_string(64);
			str_4 = str_1 + str_2 + str_3;
			// get matricies back from strings
			l_offset = l_res_1.from_string(str_4);
			l_offset = l_res_2.from_string(str_4, l_offset);
			l_offset = l_res_3.from_string(str_4, l_offset);
			l_offset = l_res_1.from_string(str_4);

			if (l_res_1 != l_src_1 || l_res_2 != l_src_2 || l_res_3 != l_src_3)
			{
				std::cout << "........Failed Serialization to/from string test 2" << std::endl;
				return false;
			}
			std::cout << "........To/from string test passed successfully" << std::endl;
			if (l_src_1 != l_m_1 || l_src_2 != l_m_2 || l_src_3 != l_m_3)
			{
				std::cout << "........Failed to/from string test 2" << std::endl;
				return false;
			}
			std::cout << "........To/from string test 2 passed successfully" << std::endl;

			std::vector< char> buff;
			l_offset = l_src_1.to_binary(buff);
			l_offset = l_src_2.to_binary(buff, l_offset);
			l_offset = l_src_3.to_binary(buff, l_offset);
			vufMatrix4<T> l_res_11, l_res_21, l_res_31;

			l_offset = l_res_11.from_binary(buff);
			l_offset = l_res_21.from_binary(buff, l_offset);
			l_offset = l_res_31.from_binary(buff, l_offset);

			if (l_res_11 != l_src_1 || l_res_21 != l_src_2 || l_res_31 != l_src_3)
			{
				std::cout << "........Failed To/from binary test" << std::endl;
				return false;
			}
			std::cout << "........To/from binary test passed saccessfully" << std::endl;
			if (l_src_1 != l_m_1 || l_src_2 != l_m_2 || l_src_3 != l_m_3)
			{
				std::cout << "........Failed to/from binary test 2" << std::endl;
				return false;
			}
			std::cout << "........To/from binary test 2 passed successfully" << std::endl;

			// encode decod
			l_offset = l_src_1.encode_to_buff(buff);
			l_offset = l_src_2.encode_to_buff(buff, l_offset);
			l_offset = l_src_3.encode_to_buff(buff, l_offset);
			vufMatrix4<T> l_res_12, l_res_22, l_res_32;

			l_offset = l_res_12.decode_from_buff(buff);
			l_offset = l_res_22.decode_from_buff(buff, l_offset);
			l_offset = l_res_32.decode_from_buff(buff, l_offset);
			if (l_res_12 != l_src_1 || l_res_22 != l_src_2 || l_res_32 != l_src_3)
			{
				std::cout << "........Failed encode/decode to/from buffer test" << std::endl;
				return false;
			}
			std::cout << "........Encode/decode to/from buffer passed successfully" << std::endl;
			std::cout << "....Serialization Test Pass Successfully" << std::endl;

			return true;
		}
		bool test_math(bool p_verbose = false)
		{
			std::cout << "....Math Test" << std::endl;
			std::cout << std::setprecision(12) << std::fixed;
			T l_ptr[4][4] = { { 1.0, 2.0, 3.0, 4.0 },
								{ 5.0, 6.0, 7.0, 8.0 },
								{ 9.0, 10., 11., 12. },
								{ 13., 14., 15., 16. } };
			T l_ptr_2[16] = { 1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15.,16. };
			vufMatrix4<T> l_dflt;
			vufMatrix4<T> l_ident;
			vufMatrix4<T> l_num;
			vufMatrix4<T> l_by_raws(l_ptr);
			vufMatrix4<T> l_by_arr(l_ptr_2);
			vufMatrix4<T> l_other(l_by_arr);
			// constructors
			{
				if (l_dflt.m_ptr[0][0] != 1.0 || l_dflt.m_ptr[0][1] != 0.0 || l_dflt.m_ptr[0][2] != 0.0 || l_dflt.m_ptr[0][3] != 0.0 ||
					l_dflt.m_ptr[1][0] != 0.0 || l_dflt.m_ptr[1][1] != 1.0 || l_dflt.m_ptr[1][2] != 0.0 || l_dflt.m_ptr[1][3] != 0.0 ||
					l_dflt.m_ptr[2][0] != 0.0 || l_dflt.m_ptr[2][1] != 0.0 || l_dflt.m_ptr[2][2] != 1.0 || l_dflt.m_ptr[2][3] != 0.0 ||
					l_dflt.m_ptr[3][0] != 0.0 || l_dflt.m_ptr[3][1] != 0.0 || l_dflt.m_ptr[3][2] != 0.0 || l_dflt.m_ptr[3][3] != 1.0)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed default constructor"));
					return false;
				}

				if (l_by_raws.m_ptr[0][0] != 1.0 || l_by_raws.m_ptr[0][1] != 2.0 || l_by_raws.m_ptr[0][2] != 3.0 || l_by_raws.m_ptr[0][3] != 4.0 ||
					l_by_raws.m_ptr[1][0] != 5.0 || l_by_raws.m_ptr[1][1] != 6.0 || l_by_raws.m_ptr[1][2] != 7.0 || l_by_raws.m_ptr[1][3] != 8.0 ||
					l_by_raws.m_ptr[2][0] != 9.0 || l_by_raws.m_ptr[2][1] != 10. || l_by_raws.m_ptr[2][2] != 11. || l_by_raws.m_ptr[2][3] != 12. ||
					l_by_raws.m_ptr[3][0] != 13. || l_by_raws.m_ptr[3][1] != 14. || l_by_raws.m_ptr[3][2] != 15. || l_by_raws.m_ptr[3][3] != 16.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by rows constructor"));
					return false;
				}

				if (l_by_arr.m_ptr[0][0] != 1.0 || l_by_arr.m_ptr[0][1] != 2.0 || l_by_arr.m_ptr[0][2] != 3.0 || l_by_arr.m_ptr[0][3] != 4.0 ||
					l_by_arr.m_ptr[1][0] != 5.0 || l_by_arr.m_ptr[1][1] != 6.0 || l_by_arr.m_ptr[1][2] != 7.0 || l_by_arr.m_ptr[1][3] != 8.0 ||
					l_by_arr.m_ptr[2][0] != 9.0 || l_by_arr.m_ptr[2][1] != 10. || l_by_arr.m_ptr[2][2] != 11. || l_by_arr.m_ptr[2][3] != 12. ||
					l_by_arr.m_ptr[3][0] != 13. || l_by_arr.m_ptr[3][1] != 14. || l_by_arr.m_ptr[3][2] != 15. || l_by_arr.m_ptr[3][3] != 16.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by array constructor"));
					return false;
				}

				if (l_other.m_ptr[0][0] != 1.0 || l_other.m_ptr[0][1] != 2.0 || l_other.m_ptr[0][2] != 3.0 || l_other.m_ptr[0][3] != 4.0 ||
					l_other.m_ptr[1][0] != 5.0 || l_other.m_ptr[1][1] != 6.0 || l_other.m_ptr[1][2] != 7.0 || l_other.m_ptr[1][3] != 8.0 ||
					l_other.m_ptr[2][0] != 9.0 || l_other.m_ptr[2][1] != 10. || l_other.m_ptr[2][2] != 11. || l_other.m_ptr[2][3] != 12. ||
					l_other.m_ptr[3][0] != 13. || l_other.m_ptr[3][1] != 14. || l_other.m_ptr[3][2] != 15. || l_other.m_ptr[3][3] != 16.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by array constructor"));
					return false;
				}
			}
			// operators
			// compare
			{
				if (l_by_arr != l_other)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed != operator"));
					return false;
				}
				if (l_by_arr == l_other)
				{
				}
				else
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed == operator"));
					return false;
				}
			}
			// sum
			{
				vufMatrix4<T> l_sum(l_other);
				l_sum += l_other;
				if (l_sum.m_ptr[0][0] != 2.0 || l_sum.m_ptr[0][1] != 4.0 || l_sum.m_ptr[0][2] != 6.0 || l_sum.m_ptr[0][3] != 8.0 ||
					l_sum.m_ptr[1][0] != 10. || l_sum.m_ptr[1][1] != 12. || l_sum.m_ptr[1][2] != 14. || l_sum.m_ptr[1][3] != 16. ||
					l_sum.m_ptr[2][0] != 18. || l_sum.m_ptr[2][1] != 20. || l_sum.m_ptr[2][2] != 22. || l_sum.m_ptr[2][3] != 24. ||
					l_sum.m_ptr[3][0] != 26. || l_sum.m_ptr[3][1] != 28. || l_sum.m_ptr[3][2] != 30. || l_sum.m_ptr[3][3] != 32.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by += operator"));
					return false;
				}

				l_sum = l_other;
				if (l_sum.m_ptr[0][0] != 1.0 || l_sum.m_ptr[0][1] != 2.0 || l_sum.m_ptr[0][2] != 3.0 || l_sum.m_ptr[0][3] != 4.0 ||
					l_sum.m_ptr[1][0] != 5.0 || l_sum.m_ptr[1][1] != 6.0 || l_sum.m_ptr[1][2] != 7.0 || l_sum.m_ptr[1][3] != 8.0 ||
					l_sum.m_ptr[2][0] != 9.0 || l_sum.m_ptr[2][1] != 10. || l_sum.m_ptr[2][2] != 11. || l_sum.m_ptr[2][3] != 12. ||
					l_sum.m_ptr[3][0] != 13. || l_sum.m_ptr[3][1] != 14. || l_sum.m_ptr[3][2] != 15. || l_sum.m_ptr[3][3] != 16.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by = operator"));
					return false;
				}

				l_sum = l_other + l_by_raws;
				if (l_sum.m_ptr[0][0] != 2.0 || l_sum.m_ptr[0][1] != 4.0 || l_sum.m_ptr[0][2] != 6.0 || l_sum.m_ptr[0][3] != 8.0 ||
					l_sum.m_ptr[1][0] != 10. || l_sum.m_ptr[1][1] != 12. || l_sum.m_ptr[1][2] != 14. || l_sum.m_ptr[1][3] != 16. ||
					l_sum.m_ptr[2][0] != 18. || l_sum.m_ptr[2][1] != 20. || l_sum.m_ptr[2][2] != 22. || l_sum.m_ptr[2][3] != 24. ||
					l_sum.m_ptr[3][0] != 26. || l_sum.m_ptr[3][1] != 28. || l_sum.m_ptr[3][2] != 30. || l_sum.m_ptr[3][3] != 32.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by + operator"));
					return false;
				}
			}
			// minus
			{
				vufMatrix4<T> l_minus(l_other);
				l_minus -= l_other;
				if (l_minus.m_ptr[0][0] != .0 || l_minus.m_ptr[0][1] != .0 || l_minus.m_ptr[0][2] != .0 || l_minus.m_ptr[0][3] != .0 ||
					l_minus.m_ptr[1][0] != .0 || l_minus.m_ptr[1][1] != .0 || l_minus.m_ptr[1][2] != .0 || l_minus.m_ptr[1][3] != .0 ||
					l_minus.m_ptr[2][0] != .0 || l_minus.m_ptr[2][1] != .0 || l_minus.m_ptr[2][2] != .0 || l_minus.m_ptr[2][3] != .0 ||
					l_minus.m_ptr[3][0] != .0 || l_minus.m_ptr[3][1] != .0 || l_minus.m_ptr[3][2] != .0 || l_minus.m_ptr[3][3] != .0)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by -= operator"));
					return false;
				}
				l_minus = l_other - l_by_arr;
				if (l_minus.m_ptr[0][0] != .0 || l_minus.m_ptr[0][1] != .0 || l_minus.m_ptr[0][2] != .0 || l_minus.m_ptr[0][3] != .0 ||
					l_minus.m_ptr[1][0] != .0 || l_minus.m_ptr[1][1] != .0 || l_minus.m_ptr[1][2] != .0 || l_minus.m_ptr[1][3] != .0 ||
					l_minus.m_ptr[2][0] != .0 || l_minus.m_ptr[2][1] != .0 || l_minus.m_ptr[2][2] != .0 || l_minus.m_ptr[2][3] != .0 ||
					l_minus.m_ptr[3][0] != .0 || l_minus.m_ptr[3][1] != .0 || l_minus.m_ptr[3][2] != .0 || l_minus.m_ptr[3][3] != .0)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by - operator"));
					return false;
				}
			}
			// multiplications
			{
				vufMatrix4<T> l_mult(l_other);
				l_mult *= l_other;
				if (l_mult.m_ptr[0][0] != 90.0 || l_mult.m_ptr[0][1] != 100. || l_mult.m_ptr[0][2] != 110. || l_mult.m_ptr[0][3] != 120. ||
					l_mult.m_ptr[1][0] != 202. || l_mult.m_ptr[1][1] != 228. || l_mult.m_ptr[1][2] != 254. || l_mult.m_ptr[1][3] != 280. ||
					l_mult.m_ptr[2][0] != 314. || l_mult.m_ptr[2][1] != 356. || l_mult.m_ptr[2][2] != 398. || l_mult.m_ptr[2][3] != 440. ||
					l_mult.m_ptr[3][0] != 426. || l_mult.m_ptr[3][1] != 484. || l_mult.m_ptr[3][2] != 542. || l_mult.m_ptr[3][3] != 600.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by *=(matrix) operator"));
					return false;
				}

				l_mult = l_other * l_other;
				if (l_mult.m_ptr[0][0] != 90.0 || l_mult.m_ptr[0][1] != 100. || l_mult.m_ptr[0][2] != 110. || l_mult.m_ptr[0][3] != 120. ||
					l_mult.m_ptr[1][0] != 202. || l_mult.m_ptr[1][1] != 228. || l_mult.m_ptr[1][2] != 254. || l_mult.m_ptr[1][3] != 280. ||
					l_mult.m_ptr[2][0] != 314. || l_mult.m_ptr[2][1] != 356. || l_mult.m_ptr[2][2] != 398. || l_mult.m_ptr[2][3] != 440. ||
					l_mult.m_ptr[3][0] != 426. || l_mult.m_ptr[3][1] != 484. || l_mult.m_ptr[3][2] != 542. || l_mult.m_ptr[3][3] != 600.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by *(matrix) operator"));
					return false;
				}

				l_mult = l_other;
				l_mult *= 2.0;
				if (l_mult != (l_other + l_other))
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by *=(T) operator"));
					return false;
				}

				l_mult = l_other * 2;
				if (l_mult != (l_other + l_other))
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by *(T) operator"));
					return false;
				}

				l_mult = 2. * l_other;
				if (l_mult != (l_other + l_other))
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by (T)* operator"));
					return false;
				}
			}
			// operator ()
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (l_other(i, j) != l_other.m_ptr[i][j])
					{
						VF_LOG_ERR(vufStringUtils::string_padding("Failed by () operator"));
						return false;
					}
				}
			}
			// operator []
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (l_other[i][j] != l_other.m_ptr[i][j])
					{
						VF_LOG_ERR(vufStringUtils::string_padding("Failed by [] operator"));
						return false;
					}
				}
			}
			// get row
			for (int i = 0; i < 4; ++i)
			{
				vufVector4<T> l_vec = l_other.get_row(i);
				vufVector4<T> l_res(l_other(i, 0), l_other(i, 1), l_other(i, 2), l_other(i, 3));
				if (l_vec != l_res)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed by get_rowr"));
					return false;
				}
			}

			// Numerator
			{
				l_num = vufMatrix4<T>::numerate_matrix();
				if (l_num.m_ptr[0][0] != 0.0 || l_num.m_ptr[0][1] != 1.0 || l_num.m_ptr[0][2] != 2.0 || l_num.m_ptr[0][3] != 3.0 ||
					l_num.m_ptr[1][0] != 4.0 || l_num.m_ptr[1][1] != 5.0 || l_num.m_ptr[1][2] != 6.0 || l_num.m_ptr[1][3] != 7.0 ||
					l_num.m_ptr[2][0] != 8.0 || l_num.m_ptr[2][1] != 9.0 || l_num.m_ptr[2][2] != 10. || l_num.m_ptr[2][3] != 11. ||
					l_num.m_ptr[3][0] != 12. || l_num.m_ptr[3][1] != 13. || l_num.m_ptr[3][2] != 14. || l_num.m_ptr[3][3] != 15.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  numerate_matrix"));
					return false;
				}
			}
			// transpose get_4
			{
				vufMatrix4<T> l_tps = l_num.get_transposed_4();
				if (l_tps.m_ptr[0][0] != 0.0 || l_tps.m_ptr[1][0] != 1.0 || l_tps.m_ptr[2][0] != 2.0 || l_tps.m_ptr[3][0] != 3.0 ||
					l_tps.m_ptr[0][1] != 4.0 || l_tps.m_ptr[1][1] != 5.0 || l_tps.m_ptr[2][1] != 6.0 || l_tps.m_ptr[3][1] != 7.0 ||
					l_tps.m_ptr[0][2] != 8.0 || l_tps.m_ptr[1][2] != 9.0 || l_tps.m_ptr[2][2] != 10. || l_tps.m_ptr[3][2] != 11. ||
					l_tps.m_ptr[0][3] != 12. || l_tps.m_ptr[1][3] != 13. || l_tps.m_ptr[2][3] != 14. || l_tps.m_ptr[3][3] != 15.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  get_transposed_4"));
					return false;
				}

				l_num.transpose_4_in_place();
				if (l_num.m_ptr[0][0] != 0.0 || l_num.m_ptr[1][0] != 1.0 || l_num.m_ptr[2][0] != 2.0 || l_num.m_ptr[3][0] != 3.0 ||
					l_num.m_ptr[0][1] != 4.0 || l_num.m_ptr[1][1] != 5.0 || l_num.m_ptr[2][1] != 6.0 || l_num.m_ptr[3][1] != 7.0 ||
					l_num.m_ptr[0][2] != 8.0 || l_num.m_ptr[1][2] != 9.0 || l_num.m_ptr[2][2] != 10. || l_num.m_ptr[3][2] != 11. ||
					l_num.m_ptr[0][3] != 12. || l_num.m_ptr[1][3] != 13. || l_num.m_ptr[2][3] != 14. || l_num.m_ptr[3][3] != 15.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  transpose_4_in_place"));
					return false;
				}
				l_num.transpose_4_in_place();

				l_tps = l_num.get_transposed_3();
				if (l_tps.m_ptr[0][0] != 0.0 || l_tps.m_ptr[0][1] != 4.0 || l_tps.m_ptr[0][2] != 8.0 || l_tps.m_ptr[0][3] != 3.0 ||
					l_tps.m_ptr[1][0] != 1.0 || l_tps.m_ptr[1][1] != 5.0 || l_tps.m_ptr[1][2] != 9.0 || l_tps.m_ptr[1][3] != 7.0 ||
					l_tps.m_ptr[2][0] != 2.0 || l_tps.m_ptr[2][1] != 6.0 || l_tps.m_ptr[2][2] != 10. || l_tps.m_ptr[2][3] != 11. ||
					l_tps.m_ptr[3][0] != 12. || l_tps.m_ptr[3][1] != 13. || l_tps.m_ptr[3][2] != 14. || l_tps.m_ptr[3][3] != 15.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  get_transposed_3"));
					return false;
				}
				l_num.transpose_3_in_place();
				if (l_num.m_ptr[0][0] != 0.0 || l_num.m_ptr[0][1] != 4.0 || l_num.m_ptr[0][2] != 8.0 || l_num.m_ptr[0][3] != 3.0 ||
					l_num.m_ptr[1][0] != 1.0 || l_num.m_ptr[1][1] != 5.0 || l_num.m_ptr[1][2] != 9.0 || l_num.m_ptr[1][3] != 7.0 ||
					l_num.m_ptr[2][0] != 2.0 || l_num.m_ptr[2][1] != 6.0 || l_num.m_ptr[2][2] != 10. || l_num.m_ptr[2][3] != 11. ||
					l_num.m_ptr[3][0] != 12. || l_num.m_ptr[3][1] != 13. || l_num.m_ptr[3][2] != 14. || l_num.m_ptr[3][3] != 15.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  transpose_3_in_place"));
					return false;
				}
			}
			// set to identity
			{
				l_num.set_to_identity_in_place();
				if (l_num != l_ident)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  set_to_identity_in_place"));
					return false;
				}
			}
			// determinant 3
			{
				l_ident = l_by_raws;
				l_ident[0][0] = -l_ident[0][0];
				l_ident[1][1] = -l_ident[1][1];
				l_ident[2][2] = -l_ident[2][2];
				l_ident[3][3] = -l_ident[3][3];
				if (l_ident.determinant3() != 552)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  determinant3"));
					return false;
				}
			}
			// determinant 4
			{
				if (l_by_raws.determinant() != 0)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  determinant"));
					return false;
				}
				l_ident = l_by_raws;
				l_ident[0][0] = -l_ident[0][0];
				l_ident[1][1] = -l_ident[1][1];
				l_ident[2][2] = -l_ident[2][2];
				l_ident[3][3] = -l_ident[3][3];
				if (l_ident.determinant() != -36416.)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  determinant"));
					return false;
				}
			}
			// orthogonalized 4
			{
				vufMatrix4<T> l_ortho = l_ident.get_orthogonalized_4();
				vufVector4<T> l_row_x = l_ortho.get_row(0);
				vufVector4<T> l_row_y = l_ortho.get_row(1);
				vufVector4<T> l_row_z = l_ortho.get_row(2);
				vufVector4<T> l_row_w = l_ortho.get_row(3);

				T l_x = sqrt(l_row_x.x * l_row_x.x + l_row_x.y * l_row_x.y + l_row_x.z * l_row_x.z + l_row_x.w * l_row_x.w);
				T l_y = sqrt(l_row_y.x * l_row_y.x + l_row_y.y * l_row_y.y + l_row_y.z * l_row_y.z + l_row_y.w * l_row_y.w);
				T l_z = sqrt(l_row_z.x * l_row_z.x + l_row_z.y * l_row_z.y + l_row_z.z * l_row_z.z + l_row_z.w * l_row_z.w);
				T l_w = sqrt(l_row_w.x * l_row_w.x + l_row_w.y * l_row_w.y + l_row_w.z * l_row_w.z + l_row_w.w * l_row_w.w);

				T l_dx_dy = l_row_x.x * l_row_y.x + l_row_x.y * l_row_y.y + l_row_x.z * l_row_y.z + l_row_x.w * l_row_y.w;
				T l_dx_dz = l_row_x.x * l_row_z.x + l_row_x.y * l_row_z.y + l_row_x.z * l_row_z.z + l_row_x.w * l_row_z.w;
				T l_dx_dw = l_row_x.x * l_row_w.x + l_row_x.y * l_row_w.y + l_row_x.z * l_row_w.z + l_row_x.w * l_row_w.w;
				T l_dy_dz = l_row_y.x * l_row_z.x + l_row_y.y * l_row_z.y + l_row_y.z * l_row_z.z + l_row_y.w * l_row_z.w;
				T l_dy_dw = l_row_y.x * l_row_w.x + l_row_y.y * l_row_w.y + l_row_y.z * l_row_w.z + l_row_y.w * l_row_w.w;
				T l_dz_dw = l_row_z.x * l_row_w.x + l_row_z.y * l_row_w.y + l_row_z.z * l_row_w.z + l_row_z.w * l_row_w.w;

				if (std::abs(l_x - 1.) > VF_MATH_EPSILON ||
					std::abs(l_y - 1.) > VF_MATH_EPSILON ||
					std::abs(l_z - 1.) > VF_MATH_EPSILON ||
					std::abs(l_w - 1.) > VF_MATH_EPSILON)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  get_orthogonalized_4 i"));
					return false;
				}
				if (std::abs(l_dx_dy) > VF_MATH_EPSILON ||
					std::abs(l_dx_dz) > VF_MATH_EPSILON ||
					std::abs(l_dx_dw) > VF_MATH_EPSILON ||
					std::abs(l_dy_dz) > VF_MATH_EPSILON ||
					std::abs(l_dy_dw) > VF_MATH_EPSILON ||
					std::abs(l_dz_dw) > VF_MATH_EPSILON)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  get_orthogonalized_3 ii"));
					return false;
				}
			}
			// orthogonalized 3
			{
				l_ident[0][3] = 0.;
				l_ident[1][3] = 0.;
				l_ident[2][3] = 0.;
				l_ident[3][3] = 1.;
				auto l_ortho = l_ident.get_orthogonalized_3();
				auto l_row_x = l_ortho.get_row(0);
				auto l_row_y = l_ortho.get_row(1);
				auto l_row_z = l_ortho.get_row(2);

				if (std::abs(l_row_x.length() - 1.) > VF_MATH_EPSILON ||
					std::abs(l_row_y.length() - 1.) > VF_MATH_EPSILON ||
					std::abs(l_row_z.length() - 1.) > VF_MATH_EPSILON)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  get_orthogonalized_3 i"));
					return false;
				}
				if (std::abs(l_row_x.dot(l_row_y)) > VF_MATH_EPSILON ||
					std::abs(l_row_y.dot(l_row_z)) > VF_MATH_EPSILON ||
					std::abs(l_row_x.dot(l_row_z)) > VF_MATH_EPSILON)
				{
					VF_LOG_ERR(vufStringUtils::string_padding("Failed  get_orthogonalized_3 ii"));
					return false;
				}
			}
			std::cout << "....Math Test Pass Successfully" << std::endl;
			return true;
		}
	};
}
