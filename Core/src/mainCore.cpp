#include <iostream>
#include <vufLog.h>
extern "C"
{
#include "lua.h"

}
VF_DECLARE_STD_LOGGER();
VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;
int main()
{
	vufLog::g_log->log_info("Test Log");
	std::cout << "MAIN" << std::endl;
	system("pause");
	return 1;
}