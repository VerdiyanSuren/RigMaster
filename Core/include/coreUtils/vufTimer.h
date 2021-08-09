#ifndef VF_CORE_TIMER_H
#define VF_CORE_TIMER_H

#include <chrono>
#include <string>
//#include <vufCoreInclude.h>
namespace vuf
{
	class vufTimer
	{
	public:
		vufTimer(const std::string& p_name);
		~vufTimer();
		void set_name(const std::string& p_name);
		const std::string& get_name() const;
		void restart();
		void show_time();
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_start, m_end;
		std::string m_name;
	};
}
#endif // !VF_CORE_TIMER_H
