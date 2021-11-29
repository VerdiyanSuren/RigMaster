#pragma once

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
#include <vufObjectArrayFn.h>

#include <iostream>

namespace vufMath
{
	template <typename T>
	class vufObjectArrayFnUT
	{
		template< bool cond, typename U >
		using resolvedType = typename std::enable_if_t<cond, U >;
		static constexpr bool p_state = std::is_arithmetic_v<T> == true;
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_vufObjectArrayFn<") + typeid(T).name() + ">")));
			if (test_to_from_string(p_verbose) == false) l_status = false;

			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_vufObjectArrayFn<") + typeid(T).name() + ">")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_vufObjectArrayFn<") + typeid(T).name() + ">")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
	private:
		// arithmetic
		template <typename  U = T >
		bool test_to_from_string(bool p_verose = false, resolvedType<p_state, U>* = nullptr)
		{
			std::cout << "....To/From String Test" << std::endl;
			std::vector<T> l_vec_1(10);
			for (auto& l_item : l_vec_1)
			{
				l_item = T(static_cast <float> (rand()) / static_cast <float> (1000.));
			}
			vufArrayFn<T> l_fn_source(l_vec_1);
			std::string l_str = l_fn_source.to_string(64,2,true);
			if (p_verose == true)
			{
				std::cout << "Generate array" << std::endl;
				std::cout << l_str << std::endl << std::endl;
			}
			std::vector<T> l_vec_2(10);
			vufArrayFn<T> l_fn_dist(l_vec_2);
			l_fn_dist.from_string(l_str);
			if (l_vec_2 != l_vec_1)
			{
				std::cout << "........To String This:   " << l_str << std::endl;
				std::cout << "........From String This: " << l_fn_dist.to_string() << std::endl;
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}

			std::cout << "........To/From String Successfully" << std::endl;
			return true;
		}
		template <typename  U = T >

		// obnject
		bool test_to_from_string(bool p_verose = false, resolvedType<!p_state, U>* = nullptr)
		{
			std::cout << "....To/From String Test" << std::endl;
			std::vector<T> l_vec_1(10);
			for (auto& l_item : l_vec_1)
			{
				l_item = l_item.random();
			}
			vufArrayFn<T> l_fn_source(l_vec_1);
			std::string l_str = l_fn_source.to_string(64, 2, true);
			if (p_verose == true)
			{
				std::cout << "Generate array" << std::endl;
				std::cout << l_str << std::endl << std::endl;
			}
			std::vector<T> l_vec_2(10);
			vufArrayFn<T> l_fn_dist(l_vec_2);
			l_fn_dist.from_string(l_str);
			if (l_vec_2 != l_vec_1)
			{
				std::cout << "........To String This:\n" << l_str << std::endl;
				std::cout << "........From String This:\n " << l_fn_dist.to_string() << std::endl;
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}

			std::cout << "........To/From String Successfully" << std::endl;
			return true;
		}
	};
}