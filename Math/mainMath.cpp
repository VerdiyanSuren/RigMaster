#include <iostream>

#include <unitTests/vufTestAll.h>
#include <unitTests/curve/curveTest.h>
//#include <curves/vufCurveContatiner.h>
#include <mathUtils/vufMathUtil.h>
#include <curves/explicit/vufCurveOpenBSpline.h>
#include <curves/explicit/vufCurveCloseBSpline.h>
#include <curves/vufCurveContatiner.h>
#include <vufNumericArrayObject.h>
#include <vufLog.h>
#include <noise/vufPelinNoise.h>

VF_TXT_WRITER_DEFINE_STATIC_VARS(); //Define txt serializer variables
VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
using namespace vufMath;

int main()
{
	auto l_cc = vufCurveCloseBezier<double, vufVector4, 3>::create();
	auto l_co = vufCurveOpenBezier<double, vufVector4,3>::create();
	l_co->set_nodes_count(11);
	l_cc->set_nodes_count(9);
	std::cout << l_cc->get_interval_index_i(0) << std::endl;
	std::cout << l_cc->get_interval_index_i(.5) << std::endl;
	std::cout << l_cc->get_interval_index_i(.8) << std::endl;
	std::cout << l_cc->get_interval_index_i(2) << std::endl;
	for (int i = 0; i < 9; ++i)
	{
		l_cc->set_node_at(i, vufVector4<double>::random_vector());
	}
	//std::cout << l_cc->get_interval_t_min_i(3) << std::endl;

	//l_c->get_pos_at(0.9);
	//std::cout << l_c->to_string() << std::endl;
	system("pause");
	return 0;


	vufPelinNoise<double> l_noise;
	auto a = l_noise.simplex_4d(0, 1, 2, 3);
	auto b = l_noise.simplex_2d(0, 1);
	//vuf::txtSerializer::init();
	//vufTestAll().run(true);
	//system("pause");
	vufCurveOpenBSplineTest<double> l_crv_test;
	l_crv_test.test_rebuild(true);
	l_crv_test.test_rebuild_axis(true);
	system("pause");
	return 0;
	auto l_cntnr = vufCurveContainer<double, vufVector4>::create();
	auto l_crv_1 = vufCurveOpenBSpline<double, vufVector4, 4>::create();
	auto l_crv_2 = vufCurveOpenBSpline<double, vufVector4, 4>::create();
	auto l_crv_cls_1 = vufCurveCloseBSpline<double, vufVector4, 4>::create();
	auto l_crv_cls_2 = vufCurveCloseBSpline<double, vufVector4, 4>::create();

	l_crv_1->set_nodes_count(10);
	l_crv_cls_1->set_nodes_count(10);
	for (int i = 0; i < 10; ++i)
	{
		l_crv_1->set_node_at(i, vufVector4<double>::random_vector());
		l_crv_cls_1->set_node_at(i, vufVector4<double>::random_vector());
	}
	std::vector<double> l_uniform_to_curve_val_v;
	std::vector<double> l_curve_to_uniform_val_v;
	std::vector<double> l_curve_val_to_length_v;
	bool l_rebuild_res = l_crv_1->rebuild(l_uniform_to_curve_val_v, l_curve_to_uniform_val_v, l_curve_val_to_length_v, 10, 0, 1);
	if ( l_rebuild_res == true )
	{
		vufNumericArrayFn<double> l_arr_1(l_uniform_to_curve_val_v);
		vufNumericArrayFn<double> l_arr_2(l_curve_to_uniform_val_v);
		vufNumericArrayFn<double> l_arr_3(l_curve_val_to_length_v);
		std::cout << "l_uniform_to_curve_val_v:.." << l_arr_1.to_string() << std::endl;
		std::cout << "l_curve_to_uniform_val_v:.." << l_arr_2.to_string() << std::endl;
		std::cout << "l_curve_val_to_length_v:..." << l_arr_3.to_string() << std::endl;
	}
	else
	{
		std::cout << "Failed" << std::endl;
	}
	system("pause");
	/*
	l_cntnr->set_curve_ptr(l_crv_1);
	l_cntnr->switch_rebuild_fn(vufCurveRebuildFnType::k_constant_step);
	l_cntnr->rebuild();
	l_cntnr->switch_quaternion_fn(vufCurveQuatFnType::k_closest);
	l_cntnr->compute_bind_params( 10);
	l_cntnr->match_quaternions();
	
	std::cout << l_cntnr->to_string() << std::endl;

	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "open bspline size: " << l_crv_1->get_binary_size() << std::endl;
	std::vector<char> l_buff(2048);
	uint64_t l_offset = 0;
	uint32_t l_version;
	l_offset = l_crv_1->to_binary(l_buff);
	std::cout << "offset_1: " << l_offset << std::endl;
	l_offset = l_crv_2->from_binary(l_buff, l_version);
	std::cout << "offset_2: " << l_offset << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "close bspline size: " << l_crv_cls_1->get_binary_size() << std::endl;
	l_offset = l_crv_cls_1->to_binary(l_buff);
	std::cout << "offset_1: " << l_offset << std::endl;
	l_offset = l_crv_cls_1->from_binary(l_buff, l_version);
	std::cout << "offset_2: " << l_offset << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	
	//vufCurveContainer<double, vufVector4> l_container;
	l_offset = l_cntnr->to_binary(l_buff);
	std::cout << "offset_container_to_bionary: " << l_offset << std::endl;
	*/
	system("pause");
}