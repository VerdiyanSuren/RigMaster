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
			if (test_to_from_binary(p_verbose) == false) l_status = false;
			if (test_encode_decode(p_verbose) == false)	l_status = false;
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
			std::vector<T> l_vec_2(5);
			for (auto& l_item : l_vec_1)
			{
				l_item = T(VF_RAND(10));
			}
			for (auto& l_item : l_vec_2)
			{
				l_item = T(VF_RAND(10));
			}

			vufArrayFn<T> l_fn_source_1(l_vec_1);
			vufArrayFn<T> l_fn_source_2(l_vec_2);
			std::string l_str = l_fn_source_1.to_string(64, 2, true);
			l_str += l_fn_source_2.to_string(64, 2, true);
			
			std::vector<T> l_vec_1r;
			std::vector<T> l_vec_2r;
			vufArrayFn<T> l_fn_dist_1(l_vec_1r);
			vufArrayFn<T> l_fn_dist_2(l_vec_2r);

			uint64_t l_offset = l_fn_dist_1.from_string(l_str);
			l_fn_dist_2.from_string(l_str, l_offset);
			if (l_vec_1 != l_vec_1r)
			{
				std::cout << "........To String This:   " << l_fn_source_1.to_string(3, 2, true) << std::endl;
				std::cout << "........From String This: " << l_fn_dist_1.to_string(3,2,true) << std::endl;
				std::cout << "........Failed Serialization to/from string" << std::endl;
				return false;
			}
			if (l_vec_2 != l_vec_2r)
			{
				std::cout << "........To String This:   " << l_fn_source_2.to_string(3, 2, true) << std::endl;
				std::cout << "........From String This: " << l_fn_dist_2.to_string(3, 2, true) << std::endl;
				std::cout << "........Failed Serialization to/from string" << std::endl;
				return false;
			}
			std::cout << "....To/From String Successfully" << std::endl;
			return true;
		}
		template <typename  U = T >
		bool test_to_from_binary(bool p_verbose = false, resolvedType<p_state, U>* = nullptr)
		{
			std::cout << "....To/From Binary Test" << std::endl;
			std::vector<T> l_vec_1(10);
			std::vector<T> l_vec_2(5);
			for (auto& l_item : l_vec_1)
			{
				l_item = T(VF_RAND(10));
			}
			for (auto& l_item : l_vec_2)
			{
				l_item = T(VF_RAND(10));
			}
			vufArrayFn<T> l_fn_source_1(l_vec_1);
			vufArrayFn<T> l_fn_source_2(l_vec_2);
			std::vector<char> l_buff;
			/*if (p_verbose == true)
			{
				std::cout << "Generate array 1" << std::endl;
				std::cout << l_fn_source_1.to_string(3, 2, true) << std::endl;
				std::cout << "Generate array 2" << std::endl;
				std::cout << l_fn_source_2.to_string(3, 2, true) << std::endl << std::endl;
			}*/
			auto l_offset	= l_fn_source_1.to_binary(l_buff);
			l_offset		= l_fn_source_2.to_binary(l_buff, l_offset);
			// read 
			std::vector<T> l_vec_1r;
			std::vector<T> l_vec_2r;
			vufArrayFn<T> l_fn_dist_1(l_vec_1r);
			vufArrayFn<T> l_fn_dist_2(l_vec_2r);
			l_offset = l_fn_dist_1.from_binary(l_buff, 0);
			l_offset = l_fn_dist_2.from_binary(l_buff, l_offset);
			if (l_vec_1r != l_vec_1)
			{
				std::cout << "........To Binary This:   " << l_fn_source_1.to_string(3, 2, true) << std::endl;
				std::cout << "........From Binary This: " << l_fn_dist_1.to_string(3, 2, true) << std::endl;;
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			if (l_vec_2r != l_vec_2)
			{
				std::cout << "........To Binary This:   " << l_fn_source_2.to_string(3, 2, true) << std::endl;
				std::cout << "........From Binary This: " << l_fn_dist_2.to_string(3, 2, true) << std::endl;;
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			std::cout << "....To/From Binary Successfully" << std::endl;
			return true;
		}
		template <typename  U = T >
		bool test_encode_decode(bool p_verbose, resolvedType<p_state, U>* = nullptr)
		{
			std::cout << "....Encode/Decode  Test" << std::endl;
			std::vector<T> l_vec_1(10);
			std::vector<T> l_vec_2(5);
			for (auto& l_item : l_vec_1)
			{
				l_item = T(VF_RAND(10));
			}
			for (auto& l_item : l_vec_2)
			{
				l_item = T(VF_RAND(10));
			}

			vufArrayFn<T> l_fn_source_1(l_vec_1);
			vufArrayFn<T> l_fn_source_2(l_vec_2);

			std::vector< char> l_buff;

			auto l_offset = l_fn_source_1.encode_to_buff(l_buff);
			l_fn_source_2.encode_to_buff(l_buff,l_offset);
			
			std::vector<T> l_vec_1r;
			std::vector<T> l_vec_2r;
			vufArrayFn<T> l_fn_dist_1(l_vec_1r);
			vufArrayFn<T> l_fn_dist_2(l_vec_2r);
			l_offset = l_fn_dist_1.decode_from_buff(l_buff);
			 l_fn_dist_2.decode_from_buff(l_buff, l_offset);

			 if (l_vec_1 != l_vec_1r)
			 {
				 std::cout << "........To Encode This:   " << l_fn_source_1.to_string(3, 2, true) << std::endl;
				 std::cout << "........From Decode This: " << l_fn_dist_1.to_string(3, 2, true) << std::endl;
				 std::cout << "........Failed Serialization Encode/Decode string" << std::endl;
				 return false;
			 }
			 if (l_vec_2 != l_vec_2r)
			 {
				 std::cout << "........To Encode This:   " << l_fn_source_2.to_string(3, 2, true) << std::endl;
				 std::cout << "........From Decode This: " << l_fn_dist_2.to_string(3, 2, true) << std::endl;
				 std::cout << "........Failed Serialization Encode/Decode string" << std::endl;
				 return false;
			 }
			std::cout << "....Encode/Decode Test Successfully" << std::endl;
			return true;
		}
		// object
		template <typename  U = T >
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
			/*if (p_verose == true)
			{
				std::cout << "Generate array" << std::endl;
				std::cout << l_fn_source.to_string(3, 2, true) << std::endl << std::endl;
			}*/
			std::vector<T> l_vec_2(10);
			vufArrayFn<T> l_fn_dist(l_vec_2);
			l_fn_dist.from_string(l_str);
			if (l_vec_2 != l_vec_1)
			{
				std::cout << "........To String This:\n" << l_fn_source.to_string(3, 2, true) << std::endl;
				std::cout << "........From String This:\n " << l_fn_dist.to_string(3,2,true) << std::endl;
				std::cout << "........Failed Serialization to/from string" << std::endl;
				return false;
			}

			std::cout << "....To/From String Successfully" << std::endl;
			return true;
		}
		template <typename  U = T >
		bool test_to_from_binary(bool p_verbose = false, resolvedType<!p_state, U>* = nullptr)
		{
			std::cout << "....To/From Binary Test" << std::endl;
			std::vector<T> l_vec_1(10);
			std::vector<T> l_vec_2(5);
			for (auto& l_item : l_vec_1)
			{
				l_item = l_item.random();
			}
			for (auto& l_item : l_vec_2)
			{
				l_item = l_item.random();
			}
			vufArrayFn<T> l_fn_source_1(l_vec_1);
			vufArrayFn<T> l_fn_source_2(l_vec_2);
			std::vector<char> l_buff;
			/*if (p_verbose == true)
			{
				std::cout << "Generate array 1" << std::endl;
				std::cout << l_fn_source_1.to_string(3, 2, true) << std::endl;
				std::cout << "Generate array 2" << std::endl;
				std::cout << l_fn_source_2.to_string(3, 2, true) << std::endl << std::endl;
			}*/
			auto l_offset = l_fn_source_1.to_binary(l_buff);
			l_offset = l_fn_source_2.to_binary(l_buff, l_offset);

			// read
			std::vector<T> l_vec_1r;
			std::vector<T> l_vec_2r;
			vufArrayFn<T> l_fn_dist_1(l_vec_1r);
			vufArrayFn<T> l_fn_dist_2(l_vec_2r);
			l_offset = l_fn_dist_1.from_binary(l_buff, 0);
			l_offset = l_fn_dist_2.from_binary(l_buff, l_offset);
			if (l_vec_1r != l_vec_1)
			{
				std::cout << "........To Binary This:   " << l_fn_source_1.to_string(3, 2, true) << std::endl;
				std::cout << "........From Binary This: " << l_fn_dist_1.to_string(3, 2, true) << std::endl;;
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			if (l_vec_2r != l_vec_2)
			{
				std::cout << "........To Binary This:   " << l_fn_source_2.to_string(3, 2, true) << std::endl;
				std::cout << "........From Binary This: " << l_fn_dist_2.to_string(3, 2, true) << std::endl;;
				std::cout << "........Failed Serialization to/from binary" << std::endl;
				return false;
			}
			std::cout << "....To/From Binary Successfully" << std::endl;

			return true;
		}
		template <typename  U = T >
		bool test_encode_decode(bool p_verbose, resolvedType<!p_state, U>* = nullptr)
		{
			std::cout << "....Encode/Decode  Test" << std::endl;
			std::vector<T> l_vec_1(10);
			for (auto& l_item : l_vec_1)
			{
				l_item = l_item.random();
			}
			std::vector<T> l_vec_2(5);
			for (auto& l_item : l_vec_2)
			{
				l_item = l_item.random();
			}

			vufArrayFn<T> l_fn_source(l_vec_1);
			std::string l_str = l_fn_source.to_string(64, 2, true);

			vufArrayFn<T> l_fn_source_1(l_vec_1);
			vufArrayFn<T> l_fn_source_2(l_vec_2);

			std::vector< char> l_buff;

			auto l_offset = l_fn_source_1.encode_to_buff(l_buff);
			l_fn_source_2.encode_to_buff(l_buff, l_offset);

			std::vector<T> l_vec_1r;
			std::vector<T> l_vec_2r;
			vufArrayFn<T> l_fn_dist_1(l_vec_1r);
			vufArrayFn<T> l_fn_dist_2(l_vec_2r);
			l_offset = l_fn_dist_1.decode_from_buff(l_buff);
			l_fn_dist_2.decode_from_buff(l_buff, l_offset);

			if (l_vec_1 != l_vec_1r)
			{
				std::cout << "........To Encode This:   " << l_fn_source_1.to_string(3, 2, true) << std::endl;
				std::cout << "........From Decode This: " << l_fn_dist_1.to_string(3, 2, true) << std::endl;
				std::cout << "........Failed Serialization Encode/Decode string" << std::endl;
				return false;
			}
			if (l_vec_2 != l_vec_2r)
			{
				std::cout << "........To Encode This:   " << l_fn_source_2.to_string(3, 2, true) << std::endl;
				std::cout << "........From Decode This: " << l_fn_dist_2.to_string(3, 2, true) << std::endl;
				std::cout << "........Failed Serialization Encode/Decode string" << std::endl;
				return false;
			}
			std::cout << "....Encode/Decode Test Successfully" << std::endl;
			return true;
		}
		
	};
}