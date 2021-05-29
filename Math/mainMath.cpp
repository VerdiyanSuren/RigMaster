#include <iostream>

#include <unitTests/vufTestAll.h>
#include <curves/vufCurveContatiner.h>

#include <vufLog.h>

VF_TXT_WRITER_DEFINE_STATIC_VARS(); //Define txt serializer variables
VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
using namespace vufMath;

int main()
{
	vuf::txtSerializer::init();
	vufTestAll().run();

	vufCurveContainer<double, vufVector4> l_container;
	system("pause");
}