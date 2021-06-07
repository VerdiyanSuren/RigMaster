#ifndef VF_ENGINE_APPLICATION_H
#define VF_ENGINE_APPLICATION_H

#include <vufEngineInclude.h>

namespace vuf
{
	class VF_ENGINE_API Application
	{
	public:
		Application() {};
		virtual  ~Application() {};

		virtual void run();
	};
	// To be defined by client
	Application* createApplication();
}

#endif // !VF_ENGINE_APPLICATION_H
