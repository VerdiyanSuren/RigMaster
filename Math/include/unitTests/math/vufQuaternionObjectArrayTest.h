#pragma once

#include <math/vufQuaternion.h>

namespace vufMath
{
	template< typename T >
	class vufQuaternionObjectArrayTest
	{
	public:
		bool run()
		{
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			std::cout << "START_TESTS_FOR_TYPE_" << typeid(vufQuaternionArrayObject<T>).name() << std::endl << std::endl;

			if (test_serialization() == false) return false;

			std::cout << std::endl << "END_TESTS_FOR_TYPE_" << typeid(vufQuaternionArrayObject<T>).name() << std::endl;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;

			return true;
		}
	private:
		bool test_serialization()
		{
			std::cout << "....Serialization Test" << std::endl;
			vufQuaternionArrayObject<T> l_q_1;
			vufQuaternionArrayObject<T> l_q_2;
			vufQuaternionArrayObject<T> l_q_3;

			l_q_1.m_quaternion_array_v.resize(10);
			l_q_2.m_quaternion_array_v.resize(5);
			l_q_3.m_quaternion_array_v.resize(15);

			for (uint64_t i = 0; i < 10; ++i)
			{
				l_q_1.m_quaternion_array_v[i] = vufQuaternion<T>::random_quaternion();
			}
			for (uint64_t i = 0; i < 5; ++i)
			{
				l_q_2.m_quaternion_array_v[i] = vufQuaternion<T>::random_quaternion();
			}
			for (uint64_t i = 0; i < 15; ++i)
			{
				l_q_3.m_quaternion_array_v[i] = vufQuaternion<T>::random_quaternion();
			}

			vufQuaternionArrayObject<T> l_src_1(l_q_1);
			vufQuaternionArrayObject<T> l_src_2(l_q_2);
			vufQuaternionArrayObject<T> l_src_3(l_q_3);

			std::string str_1 = l_src_1.to_string();
			std::string str_2 = l_src_2.to_string();
			std::string str_3 = l_src_3.to_string();
			std::string str_4 = str_1 + str_2 + str_3;

			vufQuaternionArrayObject<T> l_res_1, l_res_2, l_res_3;
			uint64_t l_offset = 0;
			l_offset = l_res_1.from_string(str_4);
			l_offset = l_res_2.from_string(str_4, l_offset);
			l_offset = l_res_3.from_string(str_4, l_offset);

			//std::string str_11 = l_res_1.to_string();
			//std::string str_22 = l_res_2.to_string();
			if (l_q_1.m_quaternion_array_v != l_res_1.m_quaternion_array_v ||
				l_q_2.m_quaternion_array_v != l_res_2.m_quaternion_array_v ||
				l_q_3.m_quaternion_array_v != l_res_3.m_quaternion_array_v)
			{
				std::cout << "........Failed Serialization to/from string" << std::endl;
				return false;
			}
			std::cout << "........Serialization to/from string Successfully" << std::endl;
			std::vector<unsigned char> l_buff;
			l_q_1.to_binary(l_buff);
			l_q_2.to_binary(l_buff);
			l_q_3.to_binary(l_buff);
			vufQuaternionArrayObject<T> l_res_11, l_res_22, l_res_33;
			l_offset = 0;
			l_offset = l_res_11.from_binary(l_buff);
			l_offset = l_res_22.from_binary(l_buff, l_offset);
			l_offset = l_res_33.from_binary(l_buff, l_offset);

			if (l_q_1.m_quaternion_array_v != l_res_11.m_quaternion_array_v ||
				l_q_2.m_quaternion_array_v != l_res_22.m_quaternion_array_v ||
				l_q_3.m_quaternion_array_v != l_res_33.m_quaternion_array_v)
			{
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			//------------------------------------------------------------------------
			// Fn
			std::vector<vufQuaternion<T>> l_v_1, l_v_2, l_v_3;
			l_v_1.resize(10);
			l_v_2.resize(5);
			l_v_3.resize(15);

			for (uint64_t i = 0; i < 10; ++i)
			{
				l_v_1[i] = vufQuaternion<T>::random_quaternion();
			}
			for (uint64_t i = 0; i < 5; ++i)
			{
				l_v_2[i] = vufQuaternion<T>::random_quaternion();
			}
			for (uint64_t i = 0; i < 15; ++i)
			{
				l_v_3[i] = vufQuaternion<T>::random_quaternion();
			}
			vufObjectArrayFn<T, vufQuaternion> l_v_fn_1(l_v_1);
			vufObjectArrayFn<T, vufQuaternion> l_v_fn_2(l_v_2);
			vufObjectArrayFn<T, vufQuaternion> l_v_fn_3(l_v_3);
			str_1 = l_v_fn_1.to_string();
			str_2 = l_v_fn_2.to_string();
			str_3 = l_v_fn_3.to_string();
			str_4 = str_1 + str_2 + str_3;
			
			std::vector<vufQuaternion<T>> l_s_1, l_s_2, l_s_3;
			vufObjectArrayFn<T, vufQuaternion> l_s_fn_1(l_s_1);
			vufObjectArrayFn<T, vufQuaternion> l_s_fn_2(l_s_2);
			vufObjectArrayFn<T, vufQuaternion> l_s_fn_3(l_s_3);
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

			std::vector<vufQuaternion<T>> l_b_1, l_b_2, l_b_3;
			vufObjectArrayFn<T, vufQuaternion> l_b_fn_2(l_b_2);
			vufObjectArrayFn<T, vufQuaternion> l_b_fn_3(l_b_3);
			vufObjectArrayFn<T, vufQuaternion> l_b_fn_1(l_b_1);
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






			std::cout << "........Serialization to/from binary Successfully" << std::endl;
			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
	};
}