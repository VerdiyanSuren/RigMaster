#include <vufEngineInclude.h>
#include <vufMathInclude.h>
#include <string>
#include <functional>

#define VF_EVENT_CLASS_TYPE(type)												\
static  EventType get_static_type() { return EventType::##type; }				\
virtual EventType get_event_type()	const override { return GetStaticType(); }	\
virtual const char* get_name()		const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)									\
virtual int get_category_flags() const override { return category; }

namespace vufEngine
{
	enum class EventType
	{
		k_none = 0,
		k_window_close, 
		k_window_resize, 
		k_window_focus, 
		k_window_lost_focus, 
		k_window_moved,
		k_app_tick, 
		k_app_update, 
		k_app_render,
		k_key_pressed, 
		k_key_released, 
		k_key_typed,
		k_mouse_button_pressed, 
		k_mouse_button_released, 
		k_mouse_moved, 
		k_mouse_scrolled
	};

	enum EventCategory
	{
		k_none = 0,
		k_event_category_application	= VF_BIT(0),
		k_event_category_input			= VF_BIT(1),
		k_event_category_keyboard		= VF_BIT(2),
		k_event_category_mouse			= VF_BIT(3),
		k_event_category_mouse_button	= VF_BIT(4)
	};

	class Event
	{
	public:
		friend class EventDispatcher;
		virtual ~Event() = default;


		virtual EventType	get_event_type()		const = 0;
		virtual const char* get_name()				const = 0;
		virtual int			get_category_flags()	const = 0;
		virtual std::string to_string()				const { return get_name(); }

		bool is_in_category(EventCategory category)
		{
			return get_category_flags() & category;
		}

		bool m_handled = false;
	};
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& p_event)
			: m_event(p_event)
		{}
		template<typename T>
		using EventFn = std::function<bool(T&)>;

		template<typename T>
		bool dispatch(const EventFn<T>& func)
		{
			if (m_event.get_event_type() == T::get_static_type())
			{
				m_event.m_handled |= func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.to_string();
	}
}