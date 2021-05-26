#pragma once

#include <math/vufMatrix.h>

namespace vufMath
{
	template<	typename T, 
				template<typename> class MATRIX,
				template<typename ,template<typename> class> class MArray = vufMatrixArrayObject>
	class vufMatrixObjectArrayTest
	{
	public:
		bool run()
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_") + typeid(MArray<T, MATRIX>).name())));

			if (test_serialization() == false) return false;

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
		bool test_serialization()
		{
			std::cout << "....Serialization Test" << std::endl;
			MArray<T, MATRIX> l_m_1;
			MArray<T, MATRIX> l_m_2;
			MArray<T, MATRIX> l_m_3;

			MArray<T, MATRIX> l_e_1;
			MArray<T, MATRIX> l_e_2;
			MArray<T, MATRIX> l_e_3;

			l_m_1.m_matrix_array_v.resize(2);
			l_m_2.m_matrix_array_v.resize(5);
			l_m_3.m_matrix_array_v.resize(15);

			for (uint64_t i = 0; i < 2; ++i)
			{
				l_m_1.m_matrix_array_v[i] = MATRIX<T>::random_matrix();
			}
			for (uint64_t i = 0; i < 5; ++i)
			{
				l_m_2.m_matrix_array_v[i] = MATRIX<T>::random_matrix();
			}
			for (uint64_t i = 0; i < 15; ++i)
			{
				l_m_3.m_matrix_array_v[i] = MATRIX<T>::random_matrix();
			}
			
			MArray<T, MATRIX> l_src_1(l_m_1);
			MArray<T, MATRIX> l_src_2(l_m_2);
			MArray<T, MATRIX> l_src_3(l_m_3);
			
			std::string str_1 = l_src_1.to_string();
			std::string str_2 = l_src_2.to_string();
			std::string str_3 = l_src_3.to_string();
			std::string str_4 = str_1 + str_2 + str_3;

			//std::cout << l_src_1.to_string() <<std::endl;
			//std::cout << std::endl;

			MArray<T, MATRIX> l_res_1, l_res_2, l_res_3,l_res_4;
			uint64_t l_offset = 0;
			l_offset = l_res_1.from_string(str_4);
			l_offset = l_res_2.from_string(str_4, l_offset);
			l_offset = l_res_3.from_string(str_4, l_offset);

			//std::string str_11 = l_res_1.to_string();
			//std::string str_22 = l_res_2.to_string();
			if (l_m_1.m_matrix_array_v != l_res_1.m_matrix_array_v )
			{
				std::cout << "........Failed Serialization to/from string" << std::endl;
				return false;
			}
			if (l_m_2.m_matrix_array_v != l_res_2.m_matrix_array_v)
			{
				std::cout << "........Failed Serialization 2 to/from string" << std::endl;
				return false;
			}	
			if (l_m_3.m_matrix_array_v != l_res_3.m_matrix_array_v)
			{
				std::cout << "........Failed Serialization 3 to/from string" << std::endl;
				return false;
			}
			// zero length array mix to non zero
			str_1 = l_e_1.to_string() +  l_e_2.to_string() + l_e_3.to_string() + l_m_1.to_string();
			l_offset = l_res_1.from_string(str_1);
			l_offset = l_res_2.from_string(str_1, l_offset);
			l_offset = l_res_3.from_string(str_1, l_offset);
			l_res_4.from_string(str_1, l_offset);
			if (l_res_4.m_matrix_array_v != l_m_1.m_matrix_array_v)
			{
				std::cout << "........Failed Serialization of mixed zero/nonzero length array to/from string" << std::endl;
				return false;
			}


			std::cout << "........Serialization to/from string Successfully" << std::endl;
			std::vector<unsigned char> l_buff;
			l_m_1.to_binary(l_buff);
			l_m_2.to_binary(l_buff);
			l_m_3.to_binary(l_buff);
			MArray<T, MATRIX> l_res_11, l_res_22, l_res_33;
			l_offset = 0;
			l_offset = l_res_11.from_binary(l_buff);
			l_offset = l_res_22.from_binary(l_buff, l_offset);
			l_offset = l_res_33.from_binary(l_buff, l_offset);
			
			if ( l_m_1.m_matrix_array_v != l_res_11.m_matrix_array_v )
			{
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			if ( l_m_2.m_matrix_array_v != l_res_22.m_matrix_array_v)
			{
				std::cout << "........Failed Serialization 2 to/from binary" << std::endl;
				return false;
			}
			if ( l_m_3.m_matrix_array_v != l_res_33.m_matrix_array_v)
			{
				std::cout << "........Failed Serialization 2 to/from binary" << std::endl;
				return false;
			}
			std::cout << "........Serialization to/from binary Successfully" << std::endl;
			//------------------------------------------------------------------------------
			// Fn
			std::vector<MATRIX<T>> l_v_1, l_v_2, l_v_3;
			l_v_1.resize(2);
			l_v_2.resize(5);
			l_v_3.resize(15);

			for (uint64_t i = 0; i < 2; ++i)
			{
				l_v_1[i] = MATRIX<T>::random_matrix();
			}
			for (uint64_t i = 0; i < 5; ++i)
			{
				l_v_2[i] = MATRIX<T>::random_matrix();
			}
			for (uint64_t i = 0; i < 15; ++i)
			{
				l_v_3[i] = MATRIX<T>::random_matrix();
			}
			
			vufObjectArrayFn<T,MATRIX> l_v_fn_1(l_v_1);
			vufObjectArrayFn<T,MATRIX> l_v_fn_2(l_v_2);
			vufObjectArrayFn<T,MATRIX> l_v_fn_3(l_v_3);
			str_1 = l_v_fn_1.to_string();
			str_2 = l_v_fn_2.to_string();
			str_3 = l_v_fn_3.to_string();
			str_4 = str_1 + str_2 + str_3;
			std::vector<MATRIX<T>> l_s_1, l_s_2, l_s_3;
			vufObjectArrayFn<T, MATRIX> l_s_fn_1(l_s_1);
			vufObjectArrayFn<T, MATRIX> l_s_fn_2(l_s_2);
			vufObjectArrayFn<T, MATRIX> l_s_fn_3(l_s_3);
			l_offset = l_s_fn_1.from_string(str_4);
			l_offset = l_s_fn_2.from_string(str_4, l_offset);
			l_offset = l_s_fn_3.from_string(str_4, l_offset);

			if (l_v_1 != l_s_1 ||
				l_v_2 != l_s_2 ||
				l_v_3 != l_s_3)
			{
				std::cout << "........Failed Fn Serialization to/from string" << std::endl;
				return false;
			}
			std::cout << "........Serialization Fn to/from string Successfully" << std::endl;

			l_buff.clear();
			l_v_fn_1.to_binary(l_buff);
			l_v_fn_2.to_binary(l_buff);
			l_v_fn_3.to_binary(l_buff);

			std::vector<MATRIX<T>> l_b_1, l_b_2, l_b_3;
			vufObjectArrayFn<T, MATRIX> l_b_fn_2(l_b_2);
			vufObjectArrayFn<T, MATRIX> l_b_fn_3(l_b_3);
			vufObjectArrayFn<T, MATRIX> l_b_fn_1(l_b_1);
			l_offset = l_b_fn_1.from_binary(l_buff);
			l_offset = l_b_fn_2.from_binary(l_buff, l_offset);
			l_offset = l_b_fn_3.from_binary(l_buff, l_offset);

			if (l_v_1 != l_b_1 ||
				l_v_2 != l_b_2 ||
				l_v_3 != l_b_3)
			{
				std::cout << "........Failed Fn Serialization to/from binary" << std::endl;
				return false;
			}
			std::cout << "........Serialization Fn to/from binary Successfully" << std::endl;

			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
	};
}