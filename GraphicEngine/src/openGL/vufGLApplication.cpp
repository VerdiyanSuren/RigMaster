#include <openGL/vufGLApplication.h>


using namespace vufEngine;

GLApplication::GLApplication()
	:Application()
{
	m_window = GLWindow::create();
	//m_window->set_event_callback(BIND_EVENT_FN(on_event));
}
GLApplication::~GLApplication() 
{
};

void GLApplication::run()
{
	while (m_running == true)
	{
		m_window->on_update();
	}
}