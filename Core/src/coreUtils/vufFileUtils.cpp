#include <coreUtils/vufStringUtils.h>
#include <coreUtils/vufFileUtils.h>
#include <vufLog.h>
#include <vufCoreInclude.h>

#include <codecvt>
#include <filesystem>
#include <sstream>
#include <boost/filesystem.hpp>

using namespace vuf;
namespace fs = boost::filesystem;

bool					vufFileUtils::file_exists(const std::wstring& p_file_path)
{
	auto l_path = fs::path(p_file_path);
	return fs::is_regular_file(l_path) && fs::exists(l_path);
}
bool					vufFileUtils::dir_exists(const std::wstring& p_dir_path)
{
	auto l_path = fs::path(p_dir_path);
	return fs::is_directory(l_path) && fs::exists(l_path);
}
bool					vufFileUtils::make_dir(const std::wstring& p_dir_path)
{
	try
	{
		fs::create_directory(p_dir_path);
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("make_dir failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
std::wstring			vufFileUtils::path_join(const std::wstring& p_path, const std::wstring& p_join)
{
	auto l_path = fs::path(p_path);
	auto l_join = fs::path(p_join);
	fs::path l_joined = (l_path / l_join);
	return l_joined.make_preferred().wstring();
}
bool					vufFileUtils::delete_dir(const std::wstring& p_dir_path)
{
	if (dir_exists(p_dir_path) == false)
	{
		VF_LOG_ERR(std::wstring(L"dir doesnt exist ") + p_dir_path);
		return false;
	}
	try
	{
		fs::remove_all(p_dir_path);
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("delete_dir is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool					vufFileUtils::delete_file(const std::wstring& p_file_path)
{
	if (file_exists(p_file_path) == false)
	{
		VF_LOG_ERR(std::wstring(L"file doesnt exist ") + p_file_path);
		return false;
	}
	try
	{
		fs::remove(p_file_path);
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("delete_file is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
std::wstring			vufFileUtils::get_home_dir()
{
#ifdef VF_PLATFORM_WINDOWS
	return vufStringUtils::get_env_variable(L"HOMEPATH");
	/*
	TCHAR l_path_tchar[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_PERSONAL | CSIDL_FLAG_CREATE,
		NULL,
		0,
		l_path_tchar)))
	{
		return std::wstring(l_path_tchar);
	}
	return std::wstring();
	*/
#elif VF_OS == VF_OS_LINUX
	const char* home_dir;
	if ((home_dir = getenv("HOME")) == nullptr)
	{
		home_dir = getpwuid(getuid())->pw_dir;
	}
	return str_to_wstr(std::string(home_dir))
#else
	comile error.course unknown system
#endif // 
}
std::wstring			vufFileUtils::format_path_slashes(const std::wstring& p_path)
{
	return fs::path(p_path).make_preferred().wstring();
}
std::vector<std::wstring>	vufFileUtils::get_dirs_from_env(const std::wstring& p_env_var)
{
#ifdef VF_PLATFORM_WINDOWS
	/*
	std::wstring l_buff, l_temp;
	DWORD l_buff_size = 65535;
	l_buff.resize(l_buff_size);
	l_buff_size = GetEnvironmentVariableW(p_env_var.data(), &l_buff[0], l_buff_size);
	if (l_buff_size == 0)
	{
		return std::vector<std::wstring>();
	}
	l_buff.resize(l_buff_size);
	std::vector<std::wstring> l_vec;
	std::wstringstream l_wss(l_buff);
	while (std::getline(l_wss, l_temp, L';'))
	{
		//std::wcout << "!!!!!!!!!!!!: " << l_temp << std::endl;
		//std::wcout << l_temp.size() << std::endl;
		l_vec.push_back(l_temp);
	}
	*/
	return std::vector<std::wstring>();
	//return l_vec;
#else
	comile error.course unknown system
#endif // 

	throw std::runtime_error("get_dirs_from_env not implemented yet.");
	return std::vector<std::wstring>();
}
std::vector<std::wstring>	vufFileUtils::get_subdirs(const std::wstring& p_dir, bool p_full_path)
{
	//std::wcout << std::endl;
	if (fs::exists(p_dir) == false || fs::is_directory(p_dir) == false)
	{
		return std::vector<std::wstring>();
	}
	std::vector<std::wstring> l_vec;
	fs::directory_iterator l_end_itr; // default construction yields past-the-end
	for (fs::directory_iterator l_itr(p_dir); l_itr != l_end_itr; ++l_itr)
	{
		if (fs::is_directory(*l_itr))
		{
			fs::path l_path = l_itr->path();
			if (p_full_path == true)
			{
				l_vec.push_back(l_path.make_preferred().wstring());
				//std::wcout << l_path.make_preferred().wstring() << std::endl;
				continue;
			}
			//std::wcout << l_path.leaf() << std::endl;
			l_vec.push_back(l_path.leaf().wstring());
		}
	}
	return l_vec;
	//throw std::runtime_error("get_subdirs not implemented yet.");
}
bool						vufFileUtils::read_lines_from_file(std::vector<std::wstring>& p_vec, const std::wstring& p_file)
{
	std::wifstream l_fstream(p_file);
	if (l_fstream.is_open() == false)
	{
		VF_LOG_ERR(std::wstring(L"read_lines_from_file.Failed to open file ") + p_file);
		return false;
	}
	l_fstream.imbue(std::locale(""));
	p_vec.clear();
	std::wstring l_wstr;
	while (std::getline(l_fstream, l_wstr))
	{
		p_vec.push_back(l_wstr);
	}
	l_fstream.close();
	return true;
}
bool						vufFileUtils::save_lines_to_file(const std::vector<std::wstring>& p_lines, const std::wstring& p_file)
{
	//std::wofstream l_fstream(p_file, std::ios_base::app);
	std::wofstream l_fstream(p_file);
	if (l_fstream.is_open() == false)
	{
		VF_LOG_ERR(std::wstring(L"save_lines_to_file.Failed to open file ") + p_file);
		return false;
	}
	//l_fstream.imbue(std::locale("rus_rus.1251"));
	l_fstream.imbue(std::locale(""));
	for (auto& l_line : p_lines)
	{
		l_fstream << l_line << std::endl;
	}
	l_fstream.close();
	return true;
}

