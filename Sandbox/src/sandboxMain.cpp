// Delete this later
// ThisIsSuren44 github password


#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
#include <coreUtils/vufFileUtils.h>
#include <vufTxtSerializer.h>
#include <iostream>

extern "C"
{
#include "lua.h"

}
VF_TXT_WRITER_DEFINE_STATIC_VARS(); //Define txt serializer variables
VF_LOG_DEFINE_STD_LOGGER();

using namespace vuf;

int main()
{
	vuf::txtSerializer::init();
	double l_dbl = 0.122132;
	auto l_txt_vector = txtSerializer::to_chars(&l_dbl, sizeof(l_dbl));
	auto l_string_txt = txtSerializer::to_chars_string(&l_dbl, sizeof(l_dbl));

	auto l_str_decode = txtSerializer::to_chars_string("��������� ����� ����������");
	auto l_str_encode = txtSerializer::to_chars_string("��������� ����� ����������");

	vufLog::g_log->info(txtSerializer::to_chars_string(l_str_decode), __FILE__, __LINE__);

	setlocale(LC_ALL, "");
	vufLog::g_log->info("��������� ����� ����������", __FILE__, __LINE__);
	vufLog::g_log->warning("��������� ����� ��������������", __FILE__, __LINE__);
	vufLog::g_log->error("��������� ����� ������", __FILE__, __LINE__);
	std::cout << "MAIN" << std::endl;

	std::string l_str = "pCube1.translate";
	std::vector<std::string> l_vec = vufStringUtils::string_split_by(l_str, ".");
	for (auto& i : l_vec)
	{
		VF_LOG_INFO(i);
	}
	/*
	auto l_padded = vufStringUtils::wstring_padding(L"������",40);
	vufLog::g_log->info(l_padded, __FILE__, __LINE__);
	VF_LOG_INFO(vufFileUtils::get_home_dir() );
	*/
	system("pause");
	return 1;
}