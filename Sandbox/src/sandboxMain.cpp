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
#include <math/vufPolinom.h>
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

	vufPolinomCoeff<double, 3> l_p;
	l_p = l_p.random();
	std::cout << l_p.to_string(3) << std::endl;
	std::string l_input;
	while (l_input != "q")
	{

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