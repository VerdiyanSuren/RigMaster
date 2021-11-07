#include <coreUtils/vufTimer.h>
#include <vufLog.h>
#include <iostream>
using namespace vuf;

vufTimer::vufTimer(const std::string& p_name) :m_name(p_name)
{
	m_start = std::chrono::high_resolution_clock::now();
}
vufTimer::~vufTimer()
{
	show_time();
}
void vufTimer::set_name(const std::string& p_name)
{
	m_name = p_name;
}
const std::string& vufTimer::get_name() const
{
	return m_name;
}
void vufTimer::show_time()
{
	m_end = std::chrono::high_resolution_clock::now();
	auto l_start	= std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch().count();
	auto l_end		= std::chrono::time_point_cast<std::chrono::microseconds>(m_end).time_since_epoch().count();
	auto l_us = l_end - l_start;
	//std::cout << "duration " << l_us << "us " <<( (double)l_us *0.001) << "ms" << std::endl;
	//return;
	uint64_t l_ms		=	l_us /1000;
	uint64_t l_sec		= l_ms >1000. ? uint64_t(l_ms * 0.001):0;
	uint64_t l_min		= l_sec / 60;
	uint64_t l_hours	= l_min / 60;
	uint64_t l_days		= l_hours / 24;
	l_us -= l_ms * 1000;
	l_ms -= l_sec * 1000;
	l_sec = l_sec % 60;
	l_min = l_hours % 60;
	l_hours = l_days % 24;
	std::string l_str = m_name + " ";
	if (l_days != 0) l_str += std::to_string(l_days)	+ " d ";
	if (l_hours != 0) l_str += std::to_string(l_hours)	+ " h ";
	if (l_min != 0) l_str += std::to_string(l_min)		+ " m ";
	if (l_sec != 0) l_str += std::to_string(l_sec)		+ " s ";
	if (l_ms != 0)	l_str += std::to_string(l_ms)		+ " ms ";
	if (l_us != 0)	l_str += std::to_string(l_us)		+ " us ";
	VF_LOG_INFO(l_str);

}
void vufTimer::restart()
{
	m_start = std::chrono::high_resolution_clock::now();
}