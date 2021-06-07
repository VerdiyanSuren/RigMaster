#include <vufEngineInclude.h>
#include <vufWindow.h>
#include <string>
#include <GLFW/glfw3.h>


namespace vuf
{
	struct windowProps
	{
		std::string m_title;
		uint32_t	m_width;
		uint32_t	m_height;

		windowProps(const std::string& p_title = "Engine",
					uint32_t p_width = 1600,
					uint32_t p_height = 900)
			: m_title(p_title), m_width(p_width), m_height(p_height)
		{}
	};
	class VF_ENGINE_API OGLWindow:public Window
	{
	public:
		OGLWindow(const windowProps& props);
		virtual ~OGLWindow();

		virtual void			on_update()		override;
		virtual unsigned int	get_width()		const	override { return m_data.m_width; }
		virtual unsigned int	get_height()	const	override { return m_data.m_height; }
		virtual void			set_vsync(bool enabled) override;
		virtual bool			is_vsync()		const	override;

		//static Window* create(const WindowProps& props = WindowProps());
	private:
		void init(const windowProps& props);
		void shutdown();
	private:
		struct windowData
		{
			std::string		m_title;
			unsigned int	m_width;
			unsigned int	m_height;
			bool			m_vsync;

			//EventCallbackFn EventCallback;
		};
		windowData	m_data;
		GLFWwindow* m_window;
	};
}