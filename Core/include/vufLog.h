#ifndef VF_LOG_H
#define VF_LOG_H

#include <vufCoreInclude.h>
#include <memory>

#pragma region VF_CONSOLE_COLORS
#ifdef VF_CONSOLE_USE_COLORS

	//	colors are
	//0 = Black		8 = Gray
	//1 = Blue		9 = Light Blue
	//2 = Green		a = Light Green (10)
	//3 = Aqua		b = Light Aqua	(11)
	//4 = Red		c = Light Red	(12)
	//5 = Purple	d = Light Purple(13)
	//6 = Yellow	e = Light Yellow(14)
	//7 = White		f = Bright White(15)
	#define VF_CONSOLE_COLOR_BLACK			0
	#define VF_CONSOLE_COLOR_BLUE			1
	#define VF_CONSOLE_COLOR_GREEN			2
	#define VF_CONSOLE_COLOR_AQUAA			3
	#define VF_CONSOLE_COLOR_RED			4
	#define VF_CONSOLE_COLOR_PURPLE			5
	#define VF_CONSOLE_COLOR_YELLOW			6
	#define VF_CONSOLE_COLOR_WHITE			7
	#define VF_CONSOLE_COLOR_GRAY			8
	#define VF_CONSOLE_COLOR_LIGHT_BLUE		9
	#define VF_CONSOLE_COLOR_LIGHT_GREEN	10
	#define VF_CONSOLE_COLOR_LIGHT_AQUA		11
	#define VF_CONSOLE_COLOR_LIGHT_RED		12
	#define VF_CONSOLE_COLOR_LIGHT_PURPLE	13
	#define VF_CONSOLE_COLOR_LIGHT_YELLOW	14
	#define VF_CONSOLE_COLOR_BRIGH_WHITE	15

	#ifdef VF_PLATFORM_WINDOWS
		#include <Windows.h>
		#define VF_CONSOLE_SET_COLOR(INDEX_TXT, INDEX_BCGRND)	\
			if ( GetStdHandle(STD_OUTPUT_HANDLE) != NULL)		\
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),( INDEX_TXT + INDEX_BCGRND*16) ); 

		#define VF_CONSOLE_RESET_COLOR()	\
			VF_CONSOLE_SET_COLOR(7,0)
	#else
		// make libux stype console colors
		#define VF_CONSOLE_SET_COLOR(INDEX_TXT, INDEX_BCGRND)
		#define VF_CONSOLE_RESET_COLOR()
	#endif

#else
	#define VF_CONSOLE_SET_COLOR(INDEX_TXT, INDEX_BCGRND)
	#define VF_CONSOLE_RESET_COLOR()
#endif

#pragma endregion
// if you implement your own loger
// then use this in cpp file to link you logger
#define VF_DEFINE_CUSTOM_LOGGER(CUSTOM_LOG_CLASS) \
	std::shared_ptr<vuf::vufLog> vuf::vufLog::g_log =  std::shared_ptr<vuf::CUSTOM_LOG_CLASS>(new vuf::CUSTOM_LOG_CLASS);

// some predefined loggers
#define VF_DECLARE_STD_LOGGER()																					\
	namespace vuf																								\
	{																											\
		class VF_API vufLogStd :public vufLog																	\
		{																										\
		public:																									\
			virtual void log_info(		const VF_STRING& p_str)	override { std::cout << p_str << std::endl; }	\
			virtual void log_warning(	const VF_STRING& p_str)	override { std::cout << p_str << std::endl; }	\
			virtual void log_error(		const VF_STRING& p_str)	override { std::cout << p_str << std::endl; }	\
			static std::shared_ptr<vufLog> g_log;																\
		};																										\
	}

#define VF_LOG_DEFINE_STD_LOGGER()			\
	VF_DEFINE_CUSTOM_LOGGER(vufLogStd);

namespace vuf
{
	class VF_API vufLog
	{
	public:
		virtual void log_info(		const VF_STRING&)	= 0;

		virtual void log_warning(	const VF_STRING&)	= 0;
		virtual void log_error(		const VF_STRING&)	= 0;

		static std::shared_ptr<vufLog> g_log;
	private:
		uint8_t m_color_txt_info;
		uint8_t m_color_bgr_info;
		uint8_t m_color_txt_warning;
		uint8_t m_color_bgr_warning;
		uint8_t m_color_txt_error;
		uint8_t m_color_bgr_error;


	};

}
#endif // !VF_LOG_H
