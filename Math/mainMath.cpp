#include <iostream>

#include <unitTests/vufTestAll.h>
#include <unitTests/curve/curveTest.h>
//#include <curves/vufCurveContatiner.h>
#include <mathUtils/vufMathUtil.h>
#include <curves/explicit/vufCurveOpenBSpline.h>
#include <curves/explicit/vufCurveCloseBSpline.h>
#include <curves/vufCurveContatiner.h>
#include <vufNumericArrayObject.h>
#include <math/vufPolinom.h>
#include <vufLog.h>
#include <noise/vufPelinNoise.h>

VF_TXT_WRITER_DEFINE_STATIC_VARS(); //Define txt serializer variables
VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
using namespace vufMath;

int main()
{
	vufPolinomCoeff<double, 2> l_k2;
	vufPolinomCoeff<double, 1> l_k1;
	vufPolinomCoeff<double, 0> l_k0;
	vufPolinomCoeff<double, 3> l_p1;
	l_p1.a[0] = -1;
	l_p1.a[1] = 1;
	l_p1.a[2] = 5;
	l_p1.a[3] = -3;
	int l_a = sturm_chain_3<double>(l_p1, l_k2, l_k1, l_k0);
	std::cout << l_p1.to_string() << std::endl;
	std::cout << l_k2.to_string() << std::endl;
	std::cout << l_k1.to_string() << std::endl;
	std::cout << l_k0.to_string() << std::endl;
	double l_start = -1;
	double l_end = 2;
	std::cout << "root_count: " << sturm_root_count_3<double>(l_p1, l_k2, l_k1, l_k0, -1, 2) <<std::endl;
	sturm_isolate_root_3<double>(l_p1, l_k2, l_k1, l_k0, l_start, l_end);
	std::cout << "isolate: " << l_start << " " << l_end << std::endl;
	std::cout << "root_count: " << sturm_root_count_3<double>(l_p1, l_k2, l_k1, l_k0, l_start, l_end) << std::endl;
	l_start = l_end;
	l_end = 2;
	sturm_isolate_root_3<double>(l_p1, l_k2, l_k1, l_k0, l_start, l_end);
	std::cout << "isolate: " << l_start << " " << l_end << std::endl;
	std::cout << "root_count: " << sturm_root_count_3<double>(l_p1, l_k2, l_k1, l_k0, l_start, l_end) << std::endl;
	l_start = l_end;
	l_end = 2;
	sturm_isolate_root_3<double>(l_p1, l_k2, l_k1, l_k0, l_start, l_end);
	std::cout << "isolate: " << l_start << " " << l_end << std::endl;
	std::cout << "root_count: " << sturm_root_count_3<double>(l_p1, l_k2, l_k1, l_k0, l_start, l_end) << std::endl;
	
	l_start = 5;
	l_end = 6;
	sturm_isolate_root_3<double>(l_p1, l_k2, l_k1, l_k0, l_start, l_end);
	std::cout << "isolate: " << l_start << " " << l_end << std::endl;
	std::cout << "root_count: " << sturm_root_count_3<double>(l_p1, l_k2, l_k1, l_k0, l_start, l_end) << std::endl;
	
	std::vector<double> l_arr;
	l_p1.find_root_on_interval(-1, 2, l_arr);
	vufNumericArrayFn<double> l_fn(l_arr);
	std::cout << l_fn.to_string() << std::endl;
	
	system("pause");
	return 0;
	vufPolinomCoeff<double, 4> l_p2;
	l_p2.a[0] = -11;
	l_p2.a[1] = 11;
	l_p2.a[2] = 11;
	l_p2.a[3] = 12;
	l_p2.a[4] = 13;
	//l_p2.a[5] = 33;
	vufPolinomCoeff<double, 1> l_res;
	vufPolinomCoeff<double, 2> l_remainder;
	std::cout << l_p2.to_string() << std::endl;

	//auto l_p3 = l_p1.mult(l_p2);
	//std::cout << l_p3.to_string() << std::endl;
	
	l_p2.div(l_p1, l_res, l_remainder);
	std::cout << " P1:    " << l_p2.to_string() << std::endl;
	std::cout << " P2:    " << l_p1.to_string() << std::endl;
	std::cout << " RES:   " << l_res.to_string() << std::endl;
	std::cout << " REM:   " << l_remainder.to_string() << std::endl;
	std::cout << " CHECK: " <<(l_res.mult(l_p1).add(l_remainder).to_string()) << std::endl;
	system("pause");
	return 0;
	
	
	
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