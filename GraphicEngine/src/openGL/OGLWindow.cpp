#include <vufLog.h>
#include <openGL/vufGLWindow.h>

using namespace vuf;

OGLWindow::OGLWindow(const windowProps& p_props)
{
	init(p_props);
}
OGLWindow::~OGLWindow()
{
	shutdown();
}

void OGLWindow::init(const windowProps& p_props)
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
