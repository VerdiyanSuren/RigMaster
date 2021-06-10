#ifndef VF_ENGINE_WINDOW_H
#define VF_ENGINE_WINDOW_H
#include <string>
#include <vufEngineInclude.h>

namespace vufEngine
{
	struct WindowProps
	{
		std::string		m_title;
		uint32_t		m_width;
		uint32_t		m_height;

		WindowProps(const std::string& p_title = "Hazel Engine",
					uint32_t p_width = 1600,
					uint32_t p_height = 900)
					: m_title(p_title), m_width(p_width), m_height(p_height)
		{}
	};
	class VF_ENGINE_API Window
	{
	public:
		virtual ~Window() = default;

		virtual void		on_update()				= 0;
		virtual uint32_t	get_width()	const	= 0;
		virtual uint32_t	get_height()	const	= 0;
		virtual void		set_vsync(bool enabled) = 0;
		virtual bool		is_vsync()		const	= 0;
	};
}
#endif // !VF_ENGINE_WINDOW_H
