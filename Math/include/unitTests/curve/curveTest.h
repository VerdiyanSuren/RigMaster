#pragma once

#include <curves/vufCurveOpenBSpline.h>
#include <curves/vufCurveContatiner.h>
#include <curves/quaternionFn/vufCurveQuaternionCloseFn.h>
#include <curves/scaleFn/vufCurveScaleCloseFn.h>
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
			uint32_t l_version;
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
			// to_binary from_binary
			std::vector<char> l_buff(128);
			uint64_t l_size			= l_crv_1->get_binary_size();
			uint64_t l_write_offset = l_crv_1->to_binary(l_buff);
			uint64_t l_read_offset	= l_crv_2->from_binary(l_buff, l_version);
			//uint64_t l_write_offset = l_crv_1->to_binary(p_buff,50);
			if (l_size  != l_write_offset || l_write_offset != l_read_offset)
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
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
			}
			// encode decode
			l_write_offset	= l_crv_1->encode_to_buff(l_buff);
			l_read_offset	= l_crv_2->decode_from_buff(l_buff);
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
				std::cout << std::endl;
				std::cout << "verbose for encode decode" << std::endl;
				std::cout << "buff size: " << l_buff.size() << std::endl;
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
				std::cout << "get_zip size from buff: \n" << vuf::txtSerializer::get_zip_size(l_buff) << std::endl;
				std::cout << "get_zip size from string: \n" << vuf::txtSerializer::get_zip_size(l_str) << std::endl;
				std::string l_zip_str;
				std::string l_zip_str2;
				vuf::txtSerializer::zip_to_string(l_buff, l_zip_str);
				vuf::txtSerializer::zip_to_string(l_str, l_zip_str2);
				std::cout << "zip to string from buff: " << l_zip_str << std::endl;
				std::cout << "zip to string from string: " << l_zip_str2 << std::endl;
				std::cout << "zip form buff size: " << l_zip_str.length() << std::endl;
				std::cout << "zip form stirng size: " << l_zip_str2.length() << std::endl;
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
			uint32_t l_version;
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
			uint64_t l_read_offset = l_crv_2->from_binary(l_buff, l_version);
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
#pragma region REBUILD_UNIFORM
	template<typename T>
	class vufCurveRebuildUniformTest
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_vufCurveRebuildUniformFn<") + typeid(T).name() + ">")));

			if (test_serialization(p_verbose) == false)	l_status = false;
			//if (test_math(p_verbose) == false)			l_status = false;;
			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_vufCurveRebuildUniformFn<") + typeid(T).name() + ">")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_vufCurveRebuildUniformFn<") + typeid(T).name() + ">")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			uint32_t l_version;
			auto l_cntnr	= vufCurveContainer<T, vufVector4>::create();
			auto l_crv_c	= vufCurveCloseBSpline<T, vufVector4, 4>::create();
			auto l_crv_o	= vufCurveOpenBSpline<T, vufVector4, 4>::create();
			auto l_rbld_1	= vufCurveRebuildUniformFn<T, vufVector4>::create();
			auto l_rbld_2	= vufCurveRebuildUniformFn<T, vufVector4>::create();
			// init curves
			l_crv_c->set_nodes_count(10);
			l_crv_o->set_nodes_count(10);
			for (int i = 0; i < 10; ++i)
			{
				l_crv_c->set_node_at(i, vufVector4<T>::random_vector());
				l_crv_o->set_node_at(i, vufVector4<T>::random_vector());
			}
			l_cntnr->set_curve_ptr(l_crv_o);
			l_cntnr->set_rebuild_fn_ptr(l_rbld_1);
			l_rbld_1->rebuild(*l_crv_o);
			// to_binary form_binary
			std::vector<char> l_buff;
			uint64_t l_size		= l_rbld_1->get_binary_size();
			uint64_t l_write	= l_rbld_1->to_binary(l_buff);
			uint64_t l_read		= l_rbld_2->from_binary(l_buff, l_version);
			if (l_size != l_write || l_write != l_read)
			{
				VF_LOG_ERR("Failed.to_binary form_binary rebuild uniform sizes are different");
				return false;

			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				l_cntnr->set_rebuild_fn_ptr(l_rbld_1);
				auto l_pos_1 = l_cntnr->get_pos_at(i);
				l_cntnr->set_rebuild_fn_ptr(l_rbld_2);
				auto l_pos_2 = l_cntnr->get_pos_at(i);
				if (l_pos_1 != l_pos_2)
				{
					VF_LOG_ERR("Failed.  to_binary form_binary rebuild uniform  are different");
					return false;
				}
				l_cntnr->set_rebuild_fn_ptr(l_rbld_1);
				auto l_tngnt_1 = l_cntnr->get_tangent_at(i);
				l_cntnr->set_rebuild_fn_ptr(l_rbld_2);
				auto l_tngnt_2 = l_cntnr->get_tangent_at(i);
				if (l_tngnt_1 != l_tngnt_2)
				{
					VF_LOG_ERR("Failed.  to_binary form_binary rebuild uniform  are different");
					return false;
				}
			}
			if (p_verbose == true)
			{
				std::cout << "l_size " << l_size << " l_write " << l_write << " l_read " << l_read << std::endl;
				//std::string l_str(l_buff.begin(), l_buff.end());
				//std::cout << l_str << std::endl;
			}
			// encode decode 
			l_write		= l_rbld_1->encode_to_buff(l_buff);
			l_read		= l_rbld_2->decode_from_buff(l_buff);
			if ( l_write != l_read)
			{
				VF_LOG_ERR("Failed.encode decode rebuild uniform sizes are different");
				return false;

			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				l_cntnr->set_rebuild_fn_ptr(l_rbld_1);
				auto l_pos_1 = l_cntnr->get_pos_at(i);
				l_cntnr->set_rebuild_fn_ptr(l_rbld_2);
				auto l_pos_2 = l_cntnr->get_pos_at(i);
				if (l_pos_1 != l_pos_2)
				{
					VF_LOG_ERR("Failed. encode_to_buff and decode_from_buff rebuild uniform are different");
					return false;
				}
				l_cntnr->set_rebuild_fn_ptr(l_rbld_1);
				auto l_tngnt_1 = l_cntnr->get_tangent_at(i);
				l_cntnr->set_rebuild_fn_ptr(l_rbld_2);
				auto l_tngnt_2 = l_cntnr->get_tangent_at(i);
				if (l_tngnt_1 != l_tngnt_2)
				{
					VF_LOG_ERR("Failed. encode_to_buff and decode_from_buff rebuild uniform  are different");
					return false;
				}
			}
			if (p_verbose == true)
			{
				std::cout << std::endl;
				std::cout << "verbose for encode decode" << std::endl;
				std::cout << " l_write " << l_write << " l_read " << l_read << std::endl;				

				std::cout << "buff size: " << l_buff.size() << std::endl;
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
				// zip
				// string
				std::cout << "get_zip size from buff: \n" << vuf::txtSerializer::get_zip_size(l_buff) << std::endl;
				std::cout << "get_zip size from string: \n" << vuf::txtSerializer::get_zip_size(l_str) << std::endl;
				std::string l_zip_str;
				std::string l_zip_str2;
				vuf::txtSerializer::zip_to_string(l_buff, l_zip_str);
				vuf::txtSerializer::zip_to_string(l_str, l_zip_str2);
				std::cout << "zip to string from buff: " << l_zip_str << std::endl;
				std::cout << "zip to string from string: " << l_zip_str2 << std::endl;
				std::cout << "zip from buff size: " << l_zip_str.length() << std::endl;
				std::cout << "zip from string size: " << l_zip_str2.length() << std::endl;

				// buff
				std::vector<char> l_zip_buff_from_str;
				std::vector<char> l_zip_buff_from_buff;
				vuf::txtSerializer::zip_to_buff(l_str, l_zip_buff_from_str);
				vuf::txtSerializer::zip_to_buff(l_buff, l_zip_buff_from_buff);
				
				std::string l_zip_str_s( l_zip_buff_from_str.begin(),	l_zip_buff_from_str.end());
				std::string l_zip_str_b( l_zip_buff_from_buff.begin(),	l_zip_buff_from_buff.end());
				
				std::cout << "zip to buff from string: \n"		<< l_zip_str_s << std::endl;
				std::cout << "zip to buff from string size: "	<< l_zip_buff_from_str.size() << std::endl;
				std::cout << "zip to buff from buff: \n"		<< l_zip_str_b << std::endl;
				std::cout << "zip to buff from buff size: "		<< l_zip_buff_from_buff.size() << std::endl;
				//vuf::txtSerializer::zip_to_string(l_str, l_zip_str2);

				//unzip
				std::cout << std::endl;
				std::cout << "get_unzip size from buff: \n" <<		vuf::txtSerializer::get_unzip_size(l_zip_str) << std::endl;
				std::cout << "get_unzip size from string: \n" <<	vuf::txtSerializer::get_unzip_size(l_zip_buff_from_str) << std::endl;
				// string
				std::string l_unzip_str_s;
				std::string l_unzip_str_b;
				vuf::txtSerializer::unzip_to_string(l_zip_str,				l_unzip_str_s);
				vuf::txtSerializer::unzip_to_string(l_zip_buff_from_str,	l_unzip_str_b);
				std::cout << "unzip to string from string: " << l_unzip_str_s << std::endl;
				std::cout << "unzip to string from string size: " << l_unzip_str_s.length() << std::endl;
				std::cout << "unzip matched: " << (l_unzip_str_s == l_str) << std::endl;
				std::cout << "unzip to string from buff: " << l_unzip_str_b << std::endl;
				std::cout << "unzip from string size: " << l_unzip_str_b.length() << std::endl;
				std::cout << "unzip matched: " << (l_unzip_str_b == l_str) << std::endl;

				//buff
				std::vector<char> l_unzip_buff_from_str;
				std::vector<char> l_unzip_buff_from_buff;
				vuf::txtSerializer::unzip_to_buff(l_zip_str,			l_unzip_buff_from_str);
				vuf::txtSerializer::unzip_to_buff(l_zip_buff_from_str,	l_unzip_buff_from_buff);
				l_unzip_str_s = std::string(l_unzip_buff_from_str.begin(), l_unzip_buff_from_str.end());
				l_unzip_str_b = std::string(l_unzip_buff_from_buff.begin(), l_unzip_buff_from_buff.end());
				std::cout << "unzip to buff from string: \n" << l_unzip_str_s << std::endl;
				std::cout << "unzip to buff from string size: " << l_unzip_str_s.length() << std::endl;
				std::cout << "unzip matched: " << (l_unzip_str_s == l_str) << std::endl;
				std::cout << "unzip to buff from buff: \n" << l_unzip_str_b << std::endl;
				std::cout << "unzip to buff from buff size: " << l_unzip_str_b.length() << std::endl;
				std::cout << "unzip matched: " << (l_unzip_str_b == l_str) << std::endl;
			}

			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
	};
#pragma endregion REBUILD_UNIFORM
#pragma region QUAT_CLOSE
	template<typename T>
	class vufCurveQuaternionCloseTest
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_vufCurveQuaternionCloseFn<") + typeid(T).name() + ">")));

			if (test_serialization(p_verbose) == false)	l_status = false;
			//if (test_math(p_verbose) == false)			l_status = false;;
			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_vufCurveQuaternionCloseFn<") + typeid(T).name() + ">")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_vufCurveQuaternionCloseFn") + typeid(T).name() + ">")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			uint32_t l_version;
			auto l_cntnr = vufCurveContainer<T, vufVector4>::create();
			auto l_crv_c = vufCurveCloseBSpline<T, vufVector4, 4>::create();
			auto l_crv_o = vufCurveOpenBSpline<T, vufVector4, 4>::create();
			auto l_rbld_1 = vufCurveRebuildUniformFn<T, vufVector4>::create();
			auto l_rbld_2 = vufCurveRebuildUniformFn<T, vufVector4>::create();
			auto l_quat_1 = vufCurveQuaternionCloseFn<T, vufVector4>::create();
			auto l_quat_2 = vufCurveQuaternionCloseFn<T, vufVector4>::create();
			// init curves
			l_crv_c->set_nodes_count(10);
			l_crv_o->set_nodes_count(10);
			for (int i = 0; i < 10; ++i)
			{
				l_crv_c->set_node_at(i, vufVector4<T>::random_vector());
				l_crv_o->set_node_at(i, vufVector4<T>::random_vector());
			}
			l_cntnr->set_curve_ptr(l_crv_o);
			l_cntnr->set_rebuild_fn_ptr(l_rbld_1);
			l_rbld_1->rebuild(*l_crv_o);
			// init quaternions
			l_quat_1->set_item_count(5);
			for (int i = 0; i < 5; ++i)
			{
				vufMatrix4<double> l_matr = vufMatrix4<double>::random_matrix();
				l_quat_1->set_item_at(i, l_matr);
			}
			l_quat_1->compute_bind_params(*l_cntnr, 10);
			l_quat_1->match_quaternions(*l_cntnr);
			// to_binary form_binary
			std::vector<char> l_buff;
			uint64_t l_size		= l_quat_1->get_binary_size();
			uint64_t l_write	= l_quat_1->to_binary(l_buff);
			uint64_t l_read		= l_quat_2->from_binary(l_buff, l_version);
			if (l_size != l_write || l_write != l_read)
			{
				VF_LOG_ERR("Failed.to_binary form_binary quaternion close sizes are different");
				return false;
			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				l_cntnr->set_quaternion_fn_ptr(l_quat_1);
				auto l_q_1 = l_cntnr->get_pos_at(i);
				l_cntnr->set_quaternion_fn_ptr(l_quat_2);
				auto l_q_2 = l_cntnr->get_pos_at(i);
				if (l_q_1 != l_q_2)
				{
					VF_LOG_ERR("Failed.  to_binary form_binary quaternion close are different");
					return false;
				}
			}
			if (p_verbose == true)
			{
				std::cout << "l_size " << l_size << " l_write " << l_write << " l_read " << l_read << std::endl;
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
			}
			
			// encode decode 
			l_write = l_quat_1->encode_to_buff(l_buff);
			l_read	= l_quat_2->decode_from_buff(l_buff);
			if (l_write != l_read)
			{
				std::cout << l_write << " " << l_read << std::endl;
				VF_LOG_ERR("Failed.encode decode quaternion close sizes are different");
				return false;

			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				l_cntnr->set_quaternion_fn_ptr(l_quat_1);
				auto l_q_1 = l_cntnr->get_pos_at(i);
				l_cntnr->set_quaternion_fn_ptr(l_quat_2);
				auto l_q_2 = l_cntnr->get_pos_at(i);
				if (l_q_1 != l_q_2)
				{
					VF_LOG_ERR("Failed.  encode decode  rebuild quaternion close  has different content.");
					return false;
				}
			}
			if (p_verbose == true)
			{
				std::cout << " l_write " << l_write << " l_read " << l_read << std::endl;
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
			}
			
			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
	};
#pragma endregion QUAT_CLOSE
#pragma region SCALE_CLOSE
	template<typename T>
	class vufCurveScaleCloseTest
	{
	public:
		bool run(bool p_verbose = false)
		{			
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_vufCurveScaleCloseFn<") + typeid(T).name() + ">")));

			if (test_serialization(p_verbose) == false)	l_status = false;
			//if (test_math(p_verbose) == false)			l_status = false;;
			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_vufCurveScaleCloseFn<") + typeid(T).name() + ">")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_vufCurveScaleCloseFn") + typeid(T).name() + ">")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			uint32_t l_version;
			auto l_cntnr = vufCurveContainer<T, vufVector4>::create();
			auto l_crv_c = vufCurveCloseBSpline<T, vufVector4, 4>::create();
			auto l_crv_o = vufCurveOpenBSpline<T, vufVector4, 4>::create();
			auto l_rbld_1 = vufCurveRebuildUniformFn<T, vufVector4>::create();
			auto l_rbld_2 = vufCurveRebuildUniformFn<T, vufVector4>::create();
			auto l_quat_1 = vufCurveQuaternionCloseFn<T, vufVector4>::create();
			auto l_quat_2 = vufCurveQuaternionCloseFn<T, vufVector4>::create();
			auto l_scl_1 = vufScaleCloseCurveFn<T, vufVector4>::create();
			auto l_scl_2 = vufScaleCloseCurveFn<T, vufVector4>::create();
			
			l_cntnr->set_curve_ptr(l_crv_o);
			l_cntnr->set_scale_fn_ptr(l_scl_1);
			// init curves
			l_crv_c->set_nodes_count(10);
			l_crv_o->set_nodes_count(10);
			for (int i = 0; i < 10; ++i)
			{
				l_crv_c->set_node_at(i, vufVector4<T>::random_vector());
				l_crv_o->set_node_at(i, vufVector4<T>::random_vector());
			}
			l_scl_1->set_item_count(5);
			for (uint32_t i = 0; i < 5; ++i)
			{
				vufMatrix4<double> l_matr = vufMatrix4<double>::random_matrix();
				vufVector_4d l_scl_vec(	l_matr.get_scale_x(),
										l_matr.get_scale_y(),
										l_matr.get_scale_z());
				l_scl_1->set_item_at(i, l_matr.get_translation_4(), l_scl_vec);
			}
			l_scl_1->compute_bind_param(*l_cntnr, 10);
			l_scl_1->match_scales(*l_cntnr);
			// to_binary from_binary
			std::vector<char> l_buff;
			uint64_t l_size		= l_scl_1->get_binary_size();
			uint64_t l_write	= l_scl_1->to_binary(l_buff);
			uint64_t l_read		= l_scl_2->from_binary(l_buff, l_version);
			if (l_size != l_write || l_write != l_read)
			{
				std::cout << "l_size " << l_size << " l_write " << l_write << " l_read " << l_read << std::endl;
				VF_LOG_ERR("Failed.to_binary form_binary scale close sizes are different");
				return false;
			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				l_cntnr->set_scale_fn_ptr(l_scl_1);
				auto l_s_1 = l_cntnr->get_scale_at(i);
				l_cntnr->set_scale_fn_ptr(l_scl_2);
				auto l_s_2 = l_cntnr->get_scale_at(i);
				if (l_s_1 != l_s_2)
				{
					VF_LOG_ERR("Failed.  to_binary form_binary scale close are different");
					return false;
				}
			}
			if (p_verbose == true)
			{
				std::cout << "l_size " << l_size << " l_write " << l_write << " l_read " << l_read << std::endl;
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
			}
			// encode decode
			l_write = l_scl_1->encode_to_buff(l_buff);
			l_read	= l_scl_2->decode_from_buff(l_buff);
			if (l_write != l_read)
			{
				std::cout << l_write << " " << l_read << std::endl;
				VF_LOG_ERR("Failed.encode decode scale close sizes are different");
				return false;
			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				l_cntnr->set_scale_fn_ptr(l_scl_1);
				auto l_s_1 = l_cntnr->get_scale_at(i);
				l_cntnr->set_scale_fn_ptr(l_scl_2);
				auto l_s_2 = l_cntnr->get_scale_at(i);
				if (l_s_1 != l_s_2)
				{
					VF_LOG_ERR("Failed.  encode decode scale close are different");
					return false;
				}
			}
			if (p_verbose == true)
			{
				std::cout << "l_size " << l_size << " l_write " << l_write << " l_read " << l_read << std::endl;
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
			}
			std::cout << "....Serialization Test Pass Successfully" << std::endl;
			return true;
		}
	};
#pragma endregion SCALE_CLOSE
#pragma region SCALE_2_ENDS

#pragma endregion SCALE_2_ENDS

#pragma region CURVE_CONTAINER
	template<typename T>
	class vufCurveContainerTest
	{
	public:
		bool run(bool p_verbose = false)
		{
			bool l_status = true;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			VF_LOG_INFO((vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_vufCurveContainer<") + typeid(T).name() + ">")));

			if (test_serialization(p_verbose) == false)	l_status = false;
			//if (test_math(p_verbose) == false)			l_status = false;;
			if (l_status == true)
			{
				VF_LOG_INFO((vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_vufCurveContainer<") + typeid(T).name() + ">")));
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				return true;
			}
			VF_LOG_ERR((vufStringUtils::string_padding(std::string("FAILED_TESTS_FOR_TYPE_vufCurveContainer<") + typeid(T).name() + ">")));
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}
		bool test_serialization(bool p_verbose = false)
		{
			std::cout << "....Serialization Test" << std::endl;
			uint32_t l_version;
			auto l_cntnr_1	= vufCurveContainer<T, vufVector4>::create();
			auto l_cntnr_2	= vufCurveContainer<T, vufVector4>::create();
			auto l_crv		= vufCurveCloseBSpline<T, vufVector4, 4>::create();
			auto l_rbld		= vufCurveRebuildUniformFn<T, vufVector4>::create();
			auto l_quat		= vufCurveQuaternionCloseFn<T, vufVector4>::create();
			auto l_scl		= vufScaleCloseCurveFn<T, vufVector4>::create();

			// init curves
			l_crv->set_nodes_count(10);
			for (int i = 0; i < 10; ++i)
			{
				l_crv->set_node_at(i, vufVector4<T>::random_vector());
			}
			l_cntnr_1->set_curve_ptr(l_crv);

			// init rebuild
			l_cntnr_1->set_rebuild_fn_ptr(l_rbld);
			l_cntnr_1->rebuild();

			// init quaternions
			l_quat->set_item_count(5);
			for (int i = 0; i < 5; ++i)
			{
				vufMatrix4<double> l_matr = vufMatrix4<double>::random_matrix();
				l_quat->set_item_at(i, l_matr);
			}
			l_quat->compute_bind_params(*l_cntnr_1, 10);
			l_quat->match_quaternions(*l_cntnr_1);

			l_cntnr_1->set_quaternion_fn_ptr(l_quat);

			// init scales
			l_scl->set_item_count(5);
			for (uint32_t i = 0; i < 5; ++i)
			{
				vufMatrix4<double> l_matr = vufMatrix4<double>::random_matrix();
				vufVector_4d l_scl_vec(l_matr.get_scale_x(),
					l_matr.get_scale_y(),
					l_matr.get_scale_z());
				l_scl->set_item_at(i, l_matr.get_translation_4(), l_scl_vec);
			}
			l_scl->compute_bind_param(*l_cntnr_1, 10);
			l_scl->match_scales(*l_cntnr_1);
			l_scl->compute_bind_param(*l_cntnr_1, 10);
			l_scl->match_scales(*l_cntnr_1);

			l_cntnr_1->set_scale_fn_ptr(l_scl);

			// to_binary from_binary
			std::vector<char> l_buff;
			uint64_t l_size		= l_cntnr_1->get_binary_size();
			uint64_t l_write	= l_cntnr_1->to_binary(l_buff);
			uint64_t l_read		= l_cntnr_2->from_binary(l_buff, l_version);
			if (l_size != l_write || l_write != l_read)
			{
				std::cout << "l_size " << l_size << " l_write " << l_write << " l_read " << l_read << std::endl;
				VF_LOG_ERR("Failed.to_binary form_binary container sizes are different");
				return false;
			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				auto l_p_1 = l_cntnr_1->get_pos_at(i);
				auto l_q_1 = l_cntnr_1->get_quaternion_at(i);
				auto l_s_1 = l_cntnr_1->get_scale_at(i);
				
				auto l_p_2 = l_cntnr_2->get_pos_at(i);
				auto l_q_2 = l_cntnr_2->get_quaternion_at(i);
				auto l_s_2 = l_cntnr_2->get_scale_at(i);
				
				if (l_s_1 != l_s_2)
				{
					VF_LOG_ERR("Failed.  to_binary form_binary scale container are different");
					return false;
				}
				if (l_q_1 != l_q_2)
				{
					VF_LOG_ERR("Failed.  to_binary form_binary quaternion container are different");
					return false;
				}
				if (l_p_1 != l_p_2)
				{
					VF_LOG_ERR("Failed.  to_binary form_binary position container are different");
					return false;
				}
			}
			/*
			if (p_verbose == true)
			{
				std::cout << "l_size " << l_size << " l_write " << l_write << " l_read " << l_read << std::endl;
				std::string l_str(l_buff.begin(), l_buff.end());
				std::cout << l_str << std::endl;
			}
			*/
			// encode decode
			std::vector<char> l_buff_2;
			uint64_t l_write_2	= l_cntnr_1->encode_to_buff(l_buff_2);
			uint64_t l_read_2	= l_cntnr_2->decode_from_buff(l_buff_2);
			if ( l_write != l_read)
			{
				std::cout  << " l_write " << l_write << " l_read " << l_read << std::endl;
				VF_LOG_ERR("Failed.encode decode container sizes are different");
				return false;
			}
			for (T i = 0.0; i < 1.0; i += 0.1)
			{
				auto l_p_1 = l_cntnr_1->get_pos_at(i);
				auto l_q_1 = l_cntnr_1->get_quaternion_at(i);
				auto l_s_1 = l_cntnr_1->get_scale_at(i);

				auto l_p_2 = l_cntnr_2->get_pos_at(i);
				auto l_q_2 = l_cntnr_2->get_quaternion_at(i);
				auto l_s_2 = l_cntnr_2->get_scale_at(i);

				if (l_s_1 != l_s_2)
				{
					VF_LOG_ERR("Failed.  encode decode scale container are different");
					return false;
				}
				if (l_q_1 != l_q_2)
				{
					VF_LOG_ERR("Failed.  encode decode quaternion container are different");
					return false;
				}
				if (l_p_1 != l_p_2)
				{
					VF_LOG_ERR("Failed.  encode decode position container are different");
					return false;
				}
			}
			if (p_verbose == true)
			{
				std::cout << " l_write " << l_write << " l_read " << l_read << std::endl;
				std::string l_str(l_buff_2.begin(), l_buff_2.end());
				std::cout << l_str << std::endl;
			}

			std::cout << "....Serialization Test Pass Successfully" << std::endl;

			return true;
		}
	};
#pragma endregion CURVE_CONTAINER
}