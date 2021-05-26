#pragma once
#include <math/vufVector.h>

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

namespace vufMath
{
	template<	typename T,
				template<typename> class VECTOR,
				template<typename, template<typename> class> class MArray = vufVectorArrayObject >
	class vufVectorObjectArrayTest
	{
	public:
		bool run()
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_") + typeid(MArray<T, VECTOR>).name())));

			std::cout << "START_TESTS_FOR_TYPE_" << typeid(MArray<T, VECTOR>).name() << std::endl << std::endl;

			if (test_serialization() == false) l_status = false;

			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_") + typeid(MArray<T, VECTOR>).name())));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_") + typeid(MArray<T, VECTOR>).name())));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
	private:
		bool test_serialization()
		{
			std::cout << "....Serialization Test" << std::endl;
			MArray<T, VECTOR> l_vv_1;
			MArray<T, VECTOR> l_vv_2;
			MArray<T, VECTOR> l_vv_3;

			l_vv_1.m_vector_array_v.resize(10);
			l_vv_2.m_vector_array_v.resize(5);
			l_vv_3.m_vector_array_v.resize(15);
			T l_counter = 0;
			for (auto& i : l_vv_1.m_vector_array_v)
			{
				i = VECTOR<T>::random_vector(true);
			}
			for (auto& i : l_vv_2.m_vector_array_v)
			{
				i = VECTOR<T>::random_vector(true);
			}
			for (auto& i : l_vv_3.m_vector_array_v)
			{
				i = VECTOR<T>::random_vector();
			}

			std::string str_1 = l_vv_1.to_string();
			std::string str_2 = l_vv_2.to_string();
			std::string str_3 = l_vv_3.to_string();
			std::string str_4 = str_1 + str_2 + str_3;

			MArray<T, VECTOR> l_res_1, l_res_2,l_res_3;
			uint64_t l_offset = 0;
			l_offset = l_res_1.from_string(str_4);
			l_offset = l_res_2.from_string(str_4, l_offset);
			l_offset = l_res_3.from_string(str_4, l_offset);

			if (l_vv_1.m_vector_array_v != l_res_1.m_vector_array_v || 
				l_vv_2.m_vector_array_v != l_res_2.m_vector_array_v ||
				l_vv_3.m_vector_array_v != l_res_3.m_vector_array_v )
			{
				std::cout << "........Failed Serialization to/from string" << std::endl;
				return false;
			}
			std::cout << "........Serialization to/from string Successfully" << std::endl;
			
			
			std::vector<unsigned char> l_buff;
			l_vv_1.to_binary(l_buff);
			l_vv_2.to_binary(l_buff);
			l_vv_3.to_binary(l_buff);
			MArray<T, VECTOR> l_res_11, l_res_22, l_res_33;
			l_offset = 0;
			l_offset = l_res_11.from_binary(l_buff);
			l_offset = l_res_22.from_binary(l_buff, l_offset);
			l_offset = l_res_33.from_binary(l_buff, l_offset);
			if (l_vv_1.m_vector_array_v != l_res_11.m_vector_array_v || 
				l_vv_2.m_vector_array_v != l_res_22.m_vector_array_v ||
				l_vv_3.m_vector_array_v != l_res_33.m_vector_array_v )
			{
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			std::cout << "........Serialization to/from binary Successfully" << std::endl;

			//------------------------------------------------------------------------------
			// Fn
			std::vector<VECTOR<T>> l_v_1, l_v_2, l_v_3;
			l_v_1.resize(2);
			l_v_2.resize(5);
			l_v_3.resize(15);

			for (uint64_t i = 0; i < 2; ++i)
			{
				l_v_1[i] = VECTOR<T>::random_vector();
			}
			for (uint64_t i = 0; i < 5; ++i)
			{
				l_v_2[i] = VECTOR<T>::random_vector();
			}
			for (uint64_t i = 0; i < 15; ++i)
			{
				l_v_3[i] = VECTOR<T>::random_vector();
			}

			vufObjectArrayFn<T, VECTOR> l_v_fn_1(l_v_1);
			vufObjectArrayFn<T, VECTOR> l_v_fn_2(l_v_2);
			vufObjectArrayFn<T, VECTOR> l_v_fn_3(l_v_3);
			str_1 = l_v_fn_1.to_string();
			str_2 = l_v_fn_2.to_string();
			str_3 = l_v_fn_3.to_string();
			str_4 = str_1 + str_2 + str_3;
			std::vector<VECTOR<T>> l_s_1, l_s_2, l_s_3;
			vufObjectArrayFn<T, VECTOR> l_s_fn_1(l_s_1);
			vufObjectArrayFn<T, VECTOR> l_s_fn_2(l_s_2);
			vufObjectArrayFn<T, VECTOR> l_s_fn_3(l_s_3);
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

			std::vector<VECTOR<T>> l_b_1, l_b_2, l_b_3;
			vufObjectArrayFn<T, VECTOR> l_b_fn_2(l_b_2);
			vufObjectArrayFn<T, VECTOR> l_b_fn_3(l_b_3);
			vufObjectArrayFn<T, VECTOR> l_b_fn_1(l_b_1);
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