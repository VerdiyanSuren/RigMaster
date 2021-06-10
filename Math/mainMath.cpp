#include <iostream>

#include <unitTests/vufTestAll.h>
//#include <curves/vufCurveContatiner.h>
#include <mathUtils/vufMathUtil.h>
#include <curves/vufCurveOpenBSpline.h>
#include <curves/vufCurveCloseBSpline.h>
#include <curves/vufCurveContatiner.h>
#include <vufLog.h>

VF_TXT_WRITER_DEFINE_STATIC_VARS(); //Define txt serializer variables
VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
using namespace vufMath;

int main()
{
	vuf::txtSerializer::init();
	vufTestAll().run(true);
	system("pause");

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
	l_offset = l_crv_1->to_binary(l_buff);
	std::cout << "offset_1: " << l_offset << std::endl;
	l_offset = l_crv_2->from_binary(l_buff);
	std::cout << "offset_2: " << l_offset << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "close bspline size: " << l_crv_cls_1->get_binary_size() << std::endl;
	l_offset = l_crv_cls_1->to_binary(l_buff);
	std::cout << "offset_1: " << l_offset << std::endl;
	l_offset = l_crv_cls_1->from_binary(l_buff);
	std::cout << "offset_2: " << l_offset << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	//vufCurveContainer<double, vufVector4> l_container;
	l_offset = l_cntnr->to_binary(l_buff);
	std::cout << "offset_container_to_bionary: " << l_offset << std::endl;
	system("pause");
}