#ifndef VF_LOG_H
#define VF_LOG_H

#include <vufCoreInclude.h>
#include <memory>

// if you implement your own loger
// then use this in cpp file to link you logger
#define VF_DEFINE_CUSTOM_LOGGER(CUSTOM_LOG_CLASS) \
	std::shared_ptr<vuf::vufLog> vuf::vufLog::g_log =  std::shared_ptr<vuf::CUSTOM_LOG_CLASS>(new vuf::CUSTOM_LOG_CLASS);

#define VF_LOG_DEFINE_STD_LOGGER(); \
	VF_DEFINE_CUSTOM_LOGGER(vufLogStd)

#define VF_LOG_INFO(STRING) \
	vuf::vufLog::g_log->info(STRING,__FILE__,__LINE__);
#define VF_LOG_WARN(STRING) \
	vuf::vufLog::g_log->warning(STRING,__FILE__,__LINE__);
#define VF_LOG_ERR(STRING) \
	vuf::vufLog::g_log->error(STRING,__FILE__,__LINE__);

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




namespace vuf
{
	class VF_API vufLog
	{
	public:
		virtual void info(		const std::string&, const char* p_file = nullptr, int p_line = -1)	= 0;
		virtual void warning(	const std::string&, const char* p_file = nullptr, int p_line = -1)	= 0;
		virtual void error(		const std::string&, const char* p_file = nullptr, int p_line = -1)	= 0;
		
		virtual void info(		const std::wstring&, const char* p_file = nullptr, int p_line = -1) = 0;
		virtual void warning(	const std::wstring&, const char* p_file = nullptr, int p_line = -1) = 0;
		virtual void error(		const std::wstring&, const char* p_file = nullptr, int p_line = -1) = 0;

		static std::shared_ptr<vufLog> g_log;
	};

	// some predefined loggers
	// standart std::cout/ wcout logger
	class VF_API vufLogStd :public vufLog
	{
	public:
		virtual void info(		const std::string&, const char* p_file = nullptr, int p_line = -1) override;
		virtual void warning(	const std::string&, const char* p_file = nullptr, int p_line = -1) override;
		virtual void error(		const std::string&, const char* p_file = nullptr, int p_line = -1) override;

		virtual void info(		const std::wstring&, const char* p_file = nullptr, int p_line = -1) override;
		virtual void warning(	const std::wstring&, const char* p_file = nullptr, int p_line = -1) override;
		virtual void error(		const std::wstring&, const char* p_file = nullptr, int p_line = -1) override;
	};
}
#endif // !VF_LOG_H
