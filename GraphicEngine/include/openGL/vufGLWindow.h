#include <vufEngineInclude.h>
#include <vufWindow.h>
#include <Events/vufEvent.h>
#include <string>

#include <GLFW/glfw3.h>

#pragma comment(lib, "opengl32.lib")
namespace vufEngine
{	
	class VF_ENGINE_API GLWindow:public Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		GLWindow(const WindowProps& props);
		virtual ~GLWindow();

		virtual void			on_update()		override;
		virtual unsigned int	get_width()		const	override { return m_data.m_width; }
		virtual unsigned int	get_height()	const	override { return m_data.m_height; }
		virtual void			set_vsync(bool enabled) override;
		virtual bool			is_vsync()		const	override;
		virtual void			set_event_callback(const EventCallbackFn& callback) = 0;
		static GLWindow* create(const WindowProps& props = WindowProps());
	private:
		void init(const WindowProps& props);
		void shutdown();
	private:
		struct WindowData
		{
			std::string		m_title;
			unsigned int	m_width;
			unsigned int	m_height;
			bool			m_vsync;

			//EventCallbackFn EventCallback;
		};
		WindowData	m_data;
		GLFWwindow* m_window;
	};
}