// Delete this later
// ThisIsSuren44 github password


#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
#include <coreUtils/vufFileUtils.h>
#include <serializer/vufTxtSerializer.h>
#include <serializer/vufStdVectorNumberSerializerFn.h>
#include <unitTestCore/serializer/vufTxtSeriaslizerUT.h>
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
	vuf::TxtSeriaslizerUT().run(true);
	system("pause");
	return 0;

	vuf::txtSerializer::init();
	double l_dbl = 0.122132;
	auto l_txt_vector = txtSerializer::encode_to_chars(&l_dbl, sizeof(l_dbl));
	auto l_string_txt = txtSerializer::encode_to_chars_string(&l_dbl, sizeof(l_dbl));

	auto l_str_decode = txtSerializer::encode_to_chars_string("Протестим вывод информации");
	auto l_str_encode = txtSerializer::encode_to_chars_string("Протестим вывод информации");

	//vufLog::g_log->info(txtSerializer::decode_to_chars_string(l_str_decode), __FILE__, __LINE__);
	std::vector<double> l_test_arr(10);
	for (int i = 0; i < (int)l_test_arr.size(); ++i)
	{
		l_test_arr[i]= (double)(rand()) / (double)(RAND_MAX);;
	}
	stdVectorNumberSerializerFn<double> l_fn(l_test_arr);
	std::string l_arr_str = l_fn.to_string(4, 1, '.');
	std::cout << l_arr_str << std::endl;
	std::vector<unsigned char> l_buff;
	uint64_t l_offset = l_fn.to_binary(l_buff);
	auto l_str_arr_encode = txtSerializer::encode_to_chars_string(l_buff);
	std::cout << l_str_arr_encode << std::endl;
	std::vector<double> l_test_arr2;
	stdVectorNumberSerializerFn<double> l_fn2(l_test_arr2);
	l_fn2.from_binary(l_buff);
	std::string l_arr_str2 = l_fn2.to_string(4, 1, '.');
	std::cout << l_arr_str2 << std::endl;

	setlocale(LC_ALL, "");
	vufLog::g_log->info("Протестим вывод информации", __FILE__, __LINE__);
	vufLog::g_log->warning("Протестим вывод предупреждения", __FILE__, __LINE__);
	vufLog::g_log->error("Протестим вывод ошибки", __FILE__, __LINE__);
	std::cout << "MAIN" << std::endl;

	std::string l_str = "pCube1.translate";
	std::vector<std::string> l_vec = vufStringUtils::string_split_by(l_str, ".");
	for (auto& i : l_vec)
	{
		VF_LOG_INFO(i);
	}
	/*
	auto l_padded = vufStringUtils::wstring_padding(L"Падинг",40);
	vufLog::g_log->info(l_padded, __FILE__, __LINE__);
	VF_LOG_INFO(vufFileUtils::get_home_dir() );
	*/
	system("pause");
	return 1;
}