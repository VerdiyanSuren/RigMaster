#pragma once

#include <vufNumericArrayObject.h>

namespace vufMath
{
	template< typename T >
	class vufNumericArrayObjectTest
	{
	public:
		bool run()
		{
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			std::cout << "START_TESTS_FOR_TYPE_" << typeid(vufNumericArrayObject<T>).name() << std::endl << std::endl;

			if (test_serialization() == false)	return false;

			std::cout << std::endl << "END_TESTS_FOR_TYPE_" << typeid(vufNumericArrayObject<T>).name() << std::endl;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return true;
		}
	private:
		bool test_serialization()
		{
			std::cout << "....Serialization Test" << std::endl;
			vufNumericArrayObject<T> l_n_1, l_n_2, l_n_3;
			l_n_1.m_numeric_array_v.resize(5);
			l_n_2.m_numeric_array_v.resize(10);
			l_n_3.m_numeric_array_v.resize(15);
			for (int i = 0; i < 5; ++i)
			{
				l_n_1.m_numeric_array_v[i] = (T)i;
			}
			for (int i = 0; i < 10; ++i)
			{
				l_n_2.m_numeric_array_v[i] = (T)(i+10);
			}
			for (int i = 0; i < 15; ++i)
			{
				l_n_3.m_numeric_array_v[i] = (T)(i - 20);
			}

			vufNumericArrayObject<T> l_src_1(l_n_1), l_src_2(l_n_2), l_src_3(l_n_3);
			std::string str_1 = l_src_1.to_string();
			std::string str_2 = l_src_2.to_string();
			std::string str_3 = l_src_3.to_string();
			std::string str_4 = str_1 + str_2 + str_3;

			//std::cout << "src: " << str_1 << std::endl;
			//std::cout << "src: " << str_2 << std::endl;
			//std::cout << "src: " << str_3 << std::endl;

			vufNumericArrayObject<T> l_res_1;
			vufNumericArrayObject<T> l_res_2;
			vufNumericArrayObject<T> l_res_3;

			uint64_t l_offset = 0;
			l_offset = l_res_1.from_string(str_4);
			l_offset = l_res_2.from_string(str_4, l_offset);
			l_offset = l_res_3.from_string(str_4, l_offset);

			if (l_res_1.m_numeric_array_v != l_src_1.m_numeric_array_v || 
				l_res_2.m_numeric_array_v != l_src_2.m_numeric_array_v || 
				l_res_3.m_numeric_array_v != l_src_3.m_numeric_array_v)
			{
				std::cout << "........Failed Serialization to/from string" << std::endl;
				return false;
			}
			std::cout << "........Serialization  to/from string Successfully" << std::endl;
			if (l_src_1.m_numeric_array_v != l_n_1.m_numeric_array_v || 
				l_src_2.m_numeric_array_v != l_n_2.m_numeric_array_v || 
				l_src_3.m_numeric_array_v != l_n_3.m_numeric_array_v)
			{
				std::cout << "........Failed Serialization 2 to/from string" << std::endl;
				return false;
			}
			std::cout << "........Serialization 2 to/from string Successfully" << std::endl;

			//std::cout << l_res_1.to_string() << std::endl;
			//std::cout << l_res_2.to_string() << std::endl;
			//std::cout << l_res_3.to_string() << std::endl;
			std::vector<unsigned char> buff;
			l_src_1.to_binary(buff);
			l_src_2.to_binary(buff);
			l_src_3.to_binary(buff);
			vufNumericArrayObject<T> l_res_11, l_res_22, l_res_33;

			l_offset = l_res_11.from_binary(buff);
			l_offset = l_res_22.from_binary(buff, l_offset);
			l_offset = l_res_33.from_binary(buff, l_offset);
			//std::cout << std::endl;
			//std::cout << l_res_11.to_string() << std::endl;
			//std::cout << l_res_22.to_string() << std::endl;
			//return false;
			//std::cout << l_res_33.to_string() << std::endl;

			if (l_res_11.m_numeric_array_v != l_src_1.m_numeric_array_v || 
				l_res_22.m_numeric_array_v != l_src_2.m_numeric_array_v || 
				l_res_33.m_numeric_array_v != l_src_3.m_numeric_array_v)
			{
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			std::cout << "........Serialization  to/from binary Successfully" << std::endl;
			if (l_src_1.m_numeric_array_v != l_n_1.m_numeric_array_v || 
				l_src_2.m_numeric_array_v != l_n_2.m_numeric_array_v || 
				l_src_3.m_numeric_array_v != l_n_3.m_numeric_array_v)
			{
				std::cout << "........Failed Serialization 2 to/from binary" << std::endl;
				return false;
			}
			std::cout << "........Serialization 2 to/from binary Successfully" << std::endl;

			std::vector<T> l_v_1, l_v_2, l_v_3;
			std::vector<T> l_r_1, l_r_2, l_r_3;
			l_v_1.resize(5);
			l_v_2.resize(10);
			l_v_3.resize(15);
			for (int i = 0; i < 5; ++i)
			{
				l_v_1[i] = (T)i;
			}
			for (int i = 0; i < 10; ++i)
			{
				l_v_2[i] = (T)(i + 10);
			}
			for (int i = 0; i < 15; ++i)
			{
				l_v_3[i] = (T)(i - 20);
			}

			vufNumericArrayFn<T>  l_v_fn_1(l_v_1);
			vufNumericArrayFn<T>  l_v_fn_2(l_v_2);
			vufNumericArrayFn<T>  l_v_fn_3(l_v_3);
			str_1 = l_v_fn_1.to_string();
			str_2 = l_v_fn_2.to_string();
			str_3 = l_v_fn_3.to_string();
			str_4 = str_1 + str_2 + str_3;
			
			vufNumericArrayFn<T>  l_r_fn_1(l_r_1);
			vufNumericArrayFn<T>  l_r_fn_2(l_r_2);
			vufNumericArrayFn<T>  l_r_fn_3(l_r_3);
			l_offset = l_r_fn_1.from_string(str_4);
			l_offset = l_r_fn_2.from_string(str_4,l_offset);
			l_offset = l_r_fn_3.from_string(str_4,l_offset);

			if (l_v_1 != l_r_1 ||
				l_v_2 != l_r_2 ||
				l_v_3 != l_r_3)
			{
				std::cout << "........Failed Fn Serialization to/from string" << std::endl;
				return false;
			}
			std::cout << "........Serialization  Fn to/from string Successfully" << std::endl;

			buff.clear();
			l_v_fn_1.to_binary(buff);
			l_v_fn_2.to_binary(buff);
			l_v_fn_3.to_binary(buff);
			std::vector<T> l_b_1, l_b_2, l_b_3;
			vufNumericArrayFn<T>  l_b_fn_1(l_b_1);
			vufNumericArrayFn<T>  l_b_fn_2(l_b_2);
			vufNumericArrayFn<T>  l_b_fn_3(l_b_3);
			l_offset = l_b_fn_1.from_binary(buff);
			l_offset = l_b_fn_2.from_binary(buff, l_offset);
			l_offset = l_b_fn_3.from_binary(buff, l_offset);
			if (l_v_1 != l_b_1 ||
				l_v_2 != l_b_2 ||
				l_v_3 != l_b_3)
			{
				std::cout << "........Failed Fn Serialization to/from binary" << std::endl;
				return false;
			}
			std::cout << "........Serialization  Fn to/from binary Successfully" << std::endl;

			//return false;
			/*
			*/
			std::cout << "....Serialization Test Pass Successfully" << std::endl;

			return true;
		}
	};
}
