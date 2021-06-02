#pragma once

#include <curves/vufCurveOpenBSpline.h>

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>

namespace vufMath
{
#pragma region OPEN_BSPLINE
	template<typename T>
	class vufCurveOpenBSplineTest
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_vufCurveBSplineTest<") + typeid(T).name() + ">")));

			if (test_serialization(p_verbose) == false)	l_status = false;
			//if (test_math(p_verbose) == false)			l_status = false;;
			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_vufCurveBSplineTest<") + typeid(T).name() + ">")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_vufCurveBSplineTest<") + typeid(T).name() + ">")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			auto l_crv_1 = vufCurveOpenBSpline<double, vufVector4, 4>::create();
			auto l_crv_2 = vufCurveOpenBSpline<double, vufVector4, 4>::create();
			l_crv_1->set_nodes_count(4);
			if (l_crv_1->is_valid() != false)
			{
				VF_LOG_ERR("is_valid has to be false for empty curve.");
				return false;
			}
			l_crv_1->set_nodes_count(5);
			for (int i = 0; i < 5; ++i)
			{
				l_crv_1->set_node_at(i,		vufVector4<double>::random_vector());
			}
			if (l_crv_1->is_valid() == false)
			{
				VF_LOG_ERR("is_valid has to be true for curve of degree 4 with 5 controls .");
				return false;
			}
			std::vector<char> l_buff(128);
			uint64_t l_size			= l_crv_1->get_binary_size();
			uint64_t l_write_offset = l_crv_1->to_binary(l_buff,50);
			uint64_t l_read_offset = l_crv_2->from_binary(l_buff,50);
			//uint64_t l_write_offset = l_crv_1->to_binary(p_buff,50);
			if (l_size + 50 != l_write_offset || l_write_offset != l_read_offset)
			{
				VF_LOG_ERR("get_binary_size()  to_binary() from_binary() have different sizes.");
				std::cout << "get_binary_size " << l_size << std::endl;
				std::cout << "to_binary "		<< l_write_offset << std::endl;
				std::cout << "from_binary "		<< l_read_offset << std::endl;
				return false;
			}
			for(T i = 0.0; i < 1.0; i += 0.1)
			{
				auto l_pos_1 = l_crv_1->get_pos_at(i);
				auto l_pos_2 = l_crv_2->get_pos_at(i);
				if (l_pos_1 != l_pos_2)
				{
					VF_LOG_ERR("Failed. read and writed curves are different");
					return false;
				}
				auto l_tngnt_1 = l_crv_1->get_tangent_at(i);
				auto l_tngnt_2 = l_crv_2->get_tangent_at(i);
				if (l_tngnt_1 != l_tngnt_2)
				{
					VF_LOG_ERR("Failed. read and writed curves are different");
					return false;
				}
			}
			
			if (p_verbose == true)
			{
				std::cout << "get_binary_size " << l_size			<< std::endl;
				std::cout << "to_binary "		<< l_write_offset	<< std::endl;
				std::cout << "from_binary "		<< l_read_offset	<< std::endl;
			}
			// encode decode
			l_write_offset	= l_crv_1->encode_to_buff(l_buff, 50);
			l_read_offset	= l_crv_2->decode_from_buff(l_buff, 50);
			if ( l_write_offset != l_read_offset)
			{
				VF_LOG_ERR("encode_to_buff()  decode_from_buff() have different sizes.");
				std::cout << "encode_to_buff "		<< l_write_offset	<< std::endl;
				std::cout << "decode_from_buff "	<< l_read_offset	<< std::endl;
				return false;
			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				auto l_pos_1 = l_crv_1->get_pos_at(i);
				auto l_pos_2 = l_crv_2->get_pos_at(i);
				if (l_pos_1 != l_pos_2)
				{
					VF_LOG_ERR("Failed. encode_to_buff and decode_from_buff curves are different");
					return false;
				}
				auto l_tngnt_1 = l_crv_1->get_tangent_at(i);
				auto l_tngnt_2 = l_crv_2->get_tangent_at(i);
				if (l_tngnt_1 != l_tngnt_2)
				{
					VF_LOG_ERR("Failed. encode_to_buff and decode_from_buff curves are different");
					return false;
				}
			}

			if (p_verbose == true)
			{
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
			}
			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
	};
#pragma endregion OPEN_BSPLINE
#pragma region CLOSE_BSPLINE
	template<typename T>
	class vufCurveCloseBSplineTest
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_vufCurveCloseBSpline<") + typeid(T).name() + ">")));

			if (test_serialization(p_verbose) == false)	l_status = false;
			//if (test_math(p_verbose) == false)			l_status = false;;
			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_vufCurveCloseBSpline<") + typeid(T).name() + ">")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_vufCurveCloseBSpline<") + typeid(T).name() + ">")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			auto l_crv_1 = vufCurveCloseBSpline<double, vufVector4, 4>::create();
			auto l_crv_2 = vufCurveCloseBSpline<double, vufVector4, 4>::create();
			l_crv_1->set_nodes_count(4);
			if (l_crv_1->is_valid() != false)
			{
				VF_LOG_ERR("is_valid has to be false for empty curve.");
				return false;
			}
			l_crv_1->set_nodes_count(5);
			for (int i = 0; i < 5; ++i)
			{
				l_crv_1->set_node_at(i, vufVector4<double>::random_vector());
			}
			if (l_crv_1->is_valid() == false)
			{
				VF_LOG_ERR("is_valid has to be true for curve of degree 4 with 5 controls .");
				return false;
			}
			std::vector<char> l_buff;
			uint64_t l_size = l_crv_1->get_binary_size();
			uint64_t l_write_offset = l_crv_1->to_binary(l_buff);
			uint64_t l_read_offset = l_crv_2->from_binary(l_buff);
			if (l_size != l_write_offset || l_write_offset != l_read_offset)
			{
				VF_LOG_ERR("get_binary_size()  to_binary() from_binary() have different sizes.");
				std::cout << "get_binary_size " << l_size << std::endl;
				std::cout << "to_binary " << l_write_offset << std::endl;
				std::cout << "from_binary " << l_read_offset << std::endl;
				return false;
			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				auto l_pos_1 = l_crv_1->get_pos_at(i);
				auto l_pos_2 = l_crv_2->get_pos_at(i);
				if (l_pos_1 != l_pos_2)
				{
					VF_LOG_ERR("Failed. read and writed curves are different");
					return false;
				}
				auto l_tngnt_1 = l_crv_1->get_tangent_at(i);
				auto l_tngnt_2 = l_crv_2->get_tangent_at(i);
				if (l_tngnt_1 != l_tngnt_2)
				{
					VF_LOG_ERR("Failed. read and writed curves are different");
					return false;
				}
			}
			if (p_verbose == true)
			{
				std::cout << "get_binary_size " << l_size			<< std::endl;
				std::cout << "to_binary "		<< l_write_offset	<< std::endl;
				std::cout << "from_binary "		<< l_read_offset	<< std::endl;
			}
			// encode decode
			l_buff.resize(0);
			l_write_offset = l_crv_1->encode_to_buff(l_buff);
			l_read_offset = l_crv_2->decode_from_buff(l_buff);
			if (l_write_offset != l_read_offset)
			{
				VF_LOG_ERR("encode_to_buff()  decode_from_buff() have different sizes.");
				std::cout << "encode_to_buff " << l_write_offset << std::endl;
				std::cout << "decode_from_buff " << l_read_offset << std::endl;
				return false;
			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				auto l_pos_1 = l_crv_1->get_pos_at(i);
				auto l_pos_2 = l_crv_2->get_pos_at(i);
				if (l_pos_1 != l_pos_2)
				{
					VF_LOG_ERR("Failed. encode_to_buff and decode_from_buff curves are different");
					return false;
				}
				auto l_tngnt_1 = l_crv_1->get_tangent_at(i);
				auto l_tngnt_2 = l_crv_2->get_tangent_at(i);
				if (l_tngnt_1 != l_tngnt_2)
				{
					VF_LOG_ERR("Failed. encode_to_buff and decode_from_buff curves are different");
					return false;
				}
			}
			
			if (p_verbose == true)
			{
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
			}
			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
	};
#pragma endregion CLOSE_BSPLINE


}