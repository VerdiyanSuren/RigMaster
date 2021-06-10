#ifndef VF_ENGINE_APPLICATION_H
#define VF_ENGINE_APPLICATION_H

#include <vufEngineInclude.h>
#include <vufWindow.h>

namespace vufEngine
{
	class VF_ENGINE_API Application
	{
	public:
		Application() {};
		virtual  ~Application() {};

		virtual void	run();
		virtual Window* get_window() const { return nullptr; }
		void			on_event(Event& e);
		
		
		bool m_running = true;
	};
	// To be defined by client
	Application* createApplication();
}

#endif // !VF_ENGINE_APPLICATION_H
