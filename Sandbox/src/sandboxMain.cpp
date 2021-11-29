// Delete this later
// ThisIsSuren44 github password

#include <type_traits>
#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
#include <coreUtils/vufFileUtils.h>
#include <serializer/vufTxtSerializer.h>
#include <serializer/vufTxtStdVectorSerializerFn.h>
//#include <unitTestCore/serializer/vufTxtSeriaslizerUT.h>
//#include <unitTestCore/serializer/vufTxtStdVectorSerializerFnUt.h>
#include <vufObjectArrayFn.h>
#include <math/vufMatrix.h>
#include <math/vufVector.h>
#include <iostream>

//#include <vufApplication.h>

extern "C"
{
#include "lua.h"

}
VF_TXT_WRITER_DEFINE_STATIC_VARS(); //Define txt serializer variables
VF_LOG_DEFINE_STD_LOGGER();
/*
class vufSandbox : public vuf::Application
{
public:
	vufSandbox() :Application() {}
	~vufSandbox() {}

};
extern vuf::Application* vuf::createApplication();
vuf::Application* vuf::createApplication()
{
	//return new vufSandbox();
	return new vuf::Application();
}
*/
using namespace vuf;
using namespace vufMath;

int main()
{
	//vuf::txtSeriaslizerUT().run(true);
	//vuf::txtStdVectorSerializerFnUt().run(true);
	/*
	auto g_app = vuf::createApplication();
	g_app->run();
	delete g_app;

	*/
	//system("pause");
	//return 0;

	vuf::txtSerializer::init();

	std::vector <int>					l_int_read_arr;
	std::vector <int>					l_int_arr(5);
	for (int i = 0; i < (int)l_int_arr.size(); ++i) l_int_arr[i] = i;

	std::vector <vufMatrix4<double>>	l_matr_read_arr;
	std::vector <vufMatrix4<double>>	l_matr_arr(10);
	for (int i = 0; i < (int)l_matr_arr.size(); ++i) l_matr_arr[i] = l_matr_arr[i].random_matrix();

	std::vector<vufVector4<double>>		l_vector_read_arr;
	std::vector<vufVector4<double>>		l_vector_arr(10);
	for (int i = 0; i < (int)l_vector_arr.size(); ++i) l_vector_arr[i] = l_vector_arr[i].random_vector();

	vufArrayFn<int> l_int_array_fn(l_int_arr);
	vufArrayFn<int> l_int_read_array_fn(l_int_read_arr);

	vufArrayFn<vufMatrix4<double>> l_matr_arr_fn(l_matr_arr);
	vufArrayFn<vufMatrix4<double>> l_matr_read_arr_fn(l_matr_read_arr);

	vufArrayFn<vufVector4<double>>	l_vector_arr_fn(l_vector_arr);
	vufArrayFn<vufVector4<double>>	l_vector_read_arr_fn(l_vector_read_arr);

	auto l_sd = l_int_array_fn.to_string(64, 4);
	auto l_sv = l_vector_arr_fn.to_string(64, 4);
	auto l_sm = l_matr_arr_fn.to_string(64,4);


	std::cout << l_sd << std::endl << std::endl;
	std::cout << l_sm << std::endl << std::endl;
	std::cout << l_sv << std::endl << std::endl;

	l_int_read_array_fn.from_string(l_sd);
	if (l_int_read_arr == l_int_arr)
	{
		std::cout << "int array matched" << std::endl;
	}
	else
	{
		std::cout << "int array FAILED" << std::endl;
	}

	l_matr_read_arr_fn.from_string(l_sm);
	if (l_matr_read_arr == l_matr_arr)
	{
		std::cout << "matrix array matched" << std::endl;
	}
	else
	{
		std::cout << "matrix array FAILED" << std::endl;
		auto l_str = l_matr_read_arr_fn.to_string(3);
		std::cout << l_str << std::endl;
	}

	l_vector_read_arr_fn.from_string(l_sv);
	if (l_vector_read_arr == l_vector_arr)
	{
		std::cout << "vector array matched" << std::endl;
	}
	else
	{
		std::cout << "vector array FAILED" << std::endl;
		auto l_str = l_vector_read_arr_fn.to_string(3);
		std::cout << l_str << std::endl;
	}

	system("pause");
	return true;



	/*
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
	std::vector<char> l_encoded;
	std::vector<char> l_zipped;
	uint64_t  l_offset = 0;
	std::string l_string = "This is test string";

	std::cout << "Source:   " << l_string << std::endl;
	txtSerializer::encode_std_string_to_buff(l_string, l_encoded);
	std::cout << "Encoded:  " << std::string(l_encoded.data(), l_encoded.size()) << std::endl;
	txtSerializer::zip_to_buff(l_encoded, l_zipped);
	std::cout << "Zipped:   " << std::string(l_zipped.data(), l_zipped.size()) << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::vector<char> l_decoded;
	std::vector<char> l_unzipped;
	txtSerializer::unzip_to_buff(l_zipped, l_unzipped);
	std::cout << "UnZipped: " << std::string(l_unzipped.data(), l_unzipped.size()) << std::endl;
	txtSerializer::decode_std_vector_from_buff(l_decoded,l_unzipped);
	std::cout << "Decoded: "  << std::string(l_decoded.data(),	l_decoded.size()) << std::endl;
	
	
	system("pause");
	*/
	return 1;
}