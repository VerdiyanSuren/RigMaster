#ifndef VF_ENGINE_OGL_APP_H
#define VF_ENGINE_OGL_APP_H

#include <vufApplication.h>
#include <openGL/vufGLWindow.h>

namespace vufEngine
{
	class VF_ENGINE_API GLApplication :public Application
	{
	public:
		GLApplication();
		virtual  ~GLApplication() {};

		virtual void run() override;

		GLWindow* m_window;
	};	
}

#endif //!VF_ENGINE_OGL_APP_H