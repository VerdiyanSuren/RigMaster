#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
#include <iostream>

using namespace vuf;

void vufLogStd::info(		const std::string& p_str, const char* p_file, int p_line)
{
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_LIGHT_GREEN, VF_CONSOLE_COLOR_GRAY);
#endif
	std::cout << "INFO:> " << p_str;
	if (p_line != -1) std::cout << "	in line " << p_line;
	if (p_file != nullptr) std::cout << "	in file " << p_file;
	std::cout << std::endl;
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_RESET_COLOR();
#endif
}
void vufLogStd::warning(	const std::string& p_str, const char* p_file, int p_line)
{
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_LIGHT_YELLOW, VF_CONSOLE_COLOR_GRAY);
#endif
	std::cout << "WARN:> " << p_str;
	if (p_line != -1) std::cout << "	in line " << p_line;
	if (p_file != nullptr) std::cout << "	in file " << p_file;
	std::cout << std::endl;
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_RESET_COLOR();
#endif
}
void vufLogStd::error(		const std::string& p_str, const char* p_file, int p_line)
{
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_LIGHT_RED, VF_CONSOLE_COLOR_GRAY);
#endif
	std::cout << "ERRR:> " << p_str;
	if (p_line != -1) std::cout << "	in line " << p_line;
	if (p_file != nullptr) std::cout << "	in file " << p_file;
	std::cout << std::endl;
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_RESET_COLOR();
#endif
}

void vufLogStd::info(		const std::wstring& p_str, const char* p_file, int  p_line)
{
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_LIGHT_GREEN, VF_CONSOLE_COLOR_GRAY);
#endif
	std::wcout << L"INFO:> " << p_str;
	if (p_line != -1) std::wcout << L"	in line " << p_line;
	if (p_file != nullptr) std::wcout << L"	in file " << vufStringUtils::string_to_wstring(p_file);
	std::wcout << std::endl;
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_RESET_COLOR();
#endif
}
void vufLogStd::warning(	const std::wstring& p_str, const char* p_file, int p_line)
{
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_LIGHT_YELLOW, VF_CONSOLE_COLOR_GRAY);
#endif
	std::wcout << L"WARN:> " << p_str;
	if (p_line != -1) std::wcout << L"	in line " << p_line;
	if (p_file != nullptr) std::wcout << L"	in file " << vufStringUtils::string_to_wstring(p_file);
	std::wcout << std::endl;
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_RESET_COLOR();
#endif
}
void vufLogStd::error(	const std::wstring& p_str, const char* p_file, int  p_line)
{
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_LIGHT_RED, VF_CONSOLE_COLOR_GRAY);
#endif
	std::wcout << L"ERRR:> " << p_str;
	if (p_line != -1) std::wcout << L"	in line " << p_line;
	if (p_file != nullptr) std::wcout << L"	in file " << vufStringUtils::string_to_wstring(p_file);
	std::wcout << std::endl;
#ifdef VF_CONSOLE_USE_COLORS
	VF_CONSOLE_RESET_COLOR();
#endif

}

