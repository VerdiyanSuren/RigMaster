#include <iostream>

#include <unitTests/vufTestAll.h>
#include <unitTests/curve/curveTest.h>
//#include <curves/vufCurveContatiner.h>
#include <mathUtils/vufMathUtil.h>
#include <curves/explicit/vufCurveBSplineOpen.h>
#include <curves/explicit/vufCurveBSplineClose.h>
#include <curves/vufCurveContatiner.h>
#include <vufNumericArrayObject.h>
#include <math/vufPolinom.h>
#include <vufLog.h>
#include <noise/vufPelinNoise.h>
#include <unitTests/polinoms/vufPolinomTest.h>

#include <chrono>
#include <coreUtils/vufTimer.h>
VF_TXT_WRITER_DEFINE_STATIC_VARS(); //Define txt serializer variables
VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
using namespace vufMath;
using namespace std::literals::chrono_literals;
int main()
{
	/*
	vufPolinomCoeff<double, 8> l_k8;
	vufPolinomCoeff<double, 7> l_k7;
	vufPolinomCoeff<double, 6> l_k6;
	vufPolinomCoeff<double, 5> l_k5;
	vufPolinomCoeff<double, 4> l_k4;
	vufPolinomCoeff<double, 3> l_k3;
	vufPolinomCoeff<double, 2> l_k2;
	vufPolinomCoeff<double, 1> l_k1;
	vufPolinomCoeff<double, 0> l_k0;
	vufPolinomCoeff<double, 9> l_p1;
	l_p1.a[0] = 0;
	l_p1.a[1] = -5;
	l_p1.a[2] = 1;
	l_p1.a[3] = 30;
	l_p1.a[4] = 1;
	l_p1.a[5] = 1;
	l_p1.a[6] = 1;
	l_p1.a[7] = 1;
	l_p1.a[8] = 1;
	l_p1.a[9] = -1;
	int l_a = sturm_chain_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0);
	std::cout << l_p1.to_string() << std::endl;
	std::cout << l_k8.to_string() << std::endl;
	std::cout << l_k7.to_string() << std::endl;
	std::cout << l_k6.to_string() << std::endl;
	std::cout << l_k5.to_string() << std::endl;
	std::cout << l_k4.to_string() << std::endl;
	std::cout << l_k3.to_string() << std::endl;
	std::cout << l_k2.to_string() << std::endl;
	std::cout << l_k1.to_string() << std::endl;
	std::cout << l_k0.to_string() << std::endl;
	//std::cout << l_a << " " << l_k4.eval(-2) << " " << l_k3.eval(-2) << std::endl;
	
	double l_start = -2;
	double l_end = 3;
	std::cout << "root_count: " << sturm_root_count_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end) <<std::endl;
	sturm_isolate_root_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end);
	std::cout << "isolate: " << l_start << " " << l_end << std::endl;
	std::cout << "root_count: " << sturm_root_count_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end) << std::endl;
	l_start = l_end;
	l_end = 3;
	sturm_isolate_root_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end);
	std::cout << "isolate: " << l_start << " " << l_end << std::endl;
	std::cout << "root_count: " << sturm_root_count_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end) << std::endl;
	l_start = l_end;
	l_end = 3;
	sturm_isolate_root_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end);
	std::cout << "isolate: " << l_start << " " << l_end << std::endl;
	std::cout << "root_count: " << sturm_root_count_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end) << std::endl;
	
	l_start = 5;
	l_end = 6;
	sturm_isolate_root_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end);
	std::cout << "isolate: " << l_start << " " << l_end << std::endl;
	std::cout << "root_count: " << sturm_root_count_9<double>(l_p1, l_k8, l_k7, l_k6, l_k5, l_k4, l_k3, l_k2, l_k1, l_k0, l_start, l_end) << std::endl;
	

	std::vector<double> l_arr(8);
	int l_root_count;
//auto l_st = std::chrono::high_resolution_clock::now();
	{
		vuf::vufTimer l_timer("Root finding ");
		l_root_count = l_p1.find_root_on_interval(-2, 3, l_arr.data());
		//auto l_ed = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<float> l_d = l_ed - l_st;
		//std::cout << std::fixed <<	(l_d.count()*1000.0f) << "ms" << std::endl;
	}
	std::cout << "Solved: " << l_root_count << std::endl;
	
	vufNumericArrayFn<double> l_fn(l_arr);
	std::cout << l_fn.to_string() << std::endl;
	system("pause");
	//return 0;	
	
	//system("pause");
	//return 0;
	*/
	
	vuf::txtSerializer::init();
	auto l_test = vufMath::vufTestAll();
	l_test.run(false);

	system("pause");
	return 0;


	vufPelinNoise<double> l_noise;
	auto a = l_noise.simplex_4d(0, 1, 2, 3);
	auto b = l_noise.simplex_2d(0, 1);
	//system("pause");
	vufCurveOpenBSplineTest<double> l_crv_test;
	l_crv_test.test_rebuild(true);
	system("pause");
	return 0;
	auto l_cntnr = vufCurveContainer<double, vufVector4>::create();
	auto l_crv_1 = vufCurveBSplineOpen<double, vufVector4, 3>::create();
	auto l_crv_2 = vufCurveBSplineOpen<double, vufVector4, 3>::create();
	auto l_crv_cls_1 = vufCurveBSplineClose<double, vufVector4, 3>::create();
	auto l_crv_cls_2 = vufCurveBSplineClose<double, vufVector4, 3>::create();

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
	bool l_rebuild_res = l_crv_1->rebuild(l_uniform_to_curve_val_v, l_curve_to_uniform_val_v, l_curve_val_to_length_v, 10);
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