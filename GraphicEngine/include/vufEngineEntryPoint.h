#ifndef VF_ENGINE_ENTRY_POINT_H
#define VF_ENGINE_ENTRY_POINT_H

#include <vufApplication.h>
#include <vufLog.h>

extern vufEngine::Application* vufEngine::createApplication();

VF_LOG_DEFINE_STD_LOGGER();
int main(int argc, char** argv)
{
	VF_LOG_INFO("Initialize Engine");

	auto g_app = vufEngine::createApplication();
	g_app->run();
	delete g_app;
}

#endif // !VF_ENGINE_ENTRY_POINT_H
