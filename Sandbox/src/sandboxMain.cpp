// Delete this later
// ThisIsSuren44 github password


#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
#include <coreUtils/vufFileUtils.h>
#include <serializer/vufTxtSerializer.h>
#include <serializer/vufTxtStdVectorSerializerFn.h>
#include <unitTestCore/serializer/vufTxtSeriaslizerUT.h>
#include <unitTestCore/serializer/vufTxtStdVectorSerializerFnUt.h>
#include <iostream>

#include <vufApplication.h>

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
*/
extern vuf::Application* vuf::createApplication();
vuf::Application* vuf::createApplication()
{
	//return new vufSandbox();
	return new vuf::Application();
}
using namespace vuf;



int main()
{
	vuf::txtSeriaslizerUT().run(true);
	vuf::txtStdVectorSerializerFnUt().run(true);

	auto g_app = vuf::createApplication();
	g_app->run();
	delete g_app;

	system("pause");
	return 0;

	vuf::txtSerializer::init();

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