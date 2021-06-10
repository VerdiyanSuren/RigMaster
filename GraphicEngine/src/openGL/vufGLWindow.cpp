#include <vufLog.h>
#include <openGL/vufGLWindow.h>

using namespace vufEngine;

GLWindow::GLWindow(const WindowProps& p_props)
{
	init(p_props);
}
GLWindow::~GLWindow()
{
	shutdown();
}
GLWindow* GLWindow::create(const WindowProps& props = WindowProps())
{
	return new GLWindow(props);
}
void GLWindow::init(const WindowProps& p_props)
{
	m_data.m_title	= p_props.m_title;
	m_data.m_width	= p_props.m_width;
	m_data.m_height = p_props.m_height;

	int l_success = glfwInit();
	if (!l_success)
	{
		VF_LOG_ERR("Failed to initialize glfw");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow((int)p_props.m_width, (int)p_props.m_height, m_data.m_title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_data);
	set_vsync(true);

}
void GLWindow::on_update()
{
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
void GLWindow::shutdown()
{
	glfwDestroyWindow(m_window);
}
void GLWindow::set_vsync(bool enabled)
{
	if (enabled == true)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}
	m_data.m_vsync = enabled;
}
bool GLWindow::is_vsync() const
{
	return m_data.m_vsync;
}