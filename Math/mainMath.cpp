#include <iostream>
#include <vufVector.h>

#include <unitTests/vufTestAll.h>
#include <vufLog.h>

VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
using namespace vufMath;

int main()
{
	vufTestAll().run();
	system("pause");
}