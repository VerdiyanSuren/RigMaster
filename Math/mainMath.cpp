#include <iostream>
#include <vufVector.h>

#include <uinitTest/vufVector4Test.h>
#include <uinitTest/vufMatrix4Test.h>

VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;

int main()
{
	//auto l_vec = vufVector4<double>();
	vector4UT<double> l_vctr4d_test;
	matrix4UT<double> l_mtrx4d_test;

	l_vctr4d_test.run();
	l_mtrx4d_test.run();
	
	vufQuaternion<double> l_q;
	vufVector4<double> l_v;
	l_q.set_by_angle_and_axis(60., l_v);
	std::cout << l_q << std::endl;
	system("pause");
}