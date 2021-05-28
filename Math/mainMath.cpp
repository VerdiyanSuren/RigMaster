#include <iostream>

#include <unitTests/vufTestAll.h>
#include <curves/vufCurveContatiner.h>

#include <vufLog.h>

VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
using namespace vufMath;

int main()
{
	vufTestAll().run();

	vufCurveContainer<double, vufVector4> l_container;
	system("pause");
}