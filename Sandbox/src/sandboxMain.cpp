#include <vufLog.h>
#include <vufStringUtils.h>
#include <vufFileUtils.h>
#include <iostream>

extern "C"
{
#include "lua.h"

}

VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;

int main()
{
	setlocale(LC_ALL, "");
	vufLog::g_log->info("��������� ����� ����������", __FILE__, __LINE__);
	vufLog::g_log->warning("��������� ����� ��������������", __FILE__, __LINE__);
	vufLog::g_log->error("��������� ����� ������", __FILE__, __LINE__);
	std::cout << "MAIN" << std::endl;

	auto l_padded = vufStringUtils::wstring_padding(L"������",40);
	vufLog::g_log->info(l_padded, __FILE__, __LINE__);
	VF_LOG_INFO(vufFileUtils::get_home_dir() );
	system("pause");
	return 1;
}