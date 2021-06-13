#include "coreUtils/vufStringUtils.h"
#include "vufLog.h"
#include "vufCoreInclude.h"
#include <stdexcept>

using namespace vuf;

uint64_t		vufStringUtils::wstring_to_uint64(const std::wstring& p_str)
{
	std::wstring l_str = p_str;
	uint64_t l_res;
	try
	{
		size_t l_sz;
		l_res = std::stoull(l_str, &l_sz);
		if (l_sz != l_str.size())
		{
			VF_LOG_ERR((std::wstring(L"Failed to convert string to id. ") + p_str));
			return 0;
		}
		return l_res;
	}
	catch (const std::invalid_argument&)
	{
		// if no conversion could be performed.
		 VF_LOG_ERR((std::wstring(L"Failed. No conversion string to id could be performed. ") + p_str));
		return 0;
	}
	catch (const std::out_of_range&)
	{
		VF_LOG_ERR((std::wstring(L"Failed. Converted value would fall out of the range of the result type. ") + p_str));
		//  if the converted value would fall out of the range of the result type.
		return 0;
	}
	return 0;
}
std::wstring	vufStringUtils::id_to_wstring(uint64_t p_id)
{
	return std::to_wstring(p_id);
}
uint64_t		vufStringUtils::get_hash_of(const std::wstring& p_str)
{
	std::hash <std::wstring> l_hasher;
	return l_hasher(p_str);
}
uint64_t		vufStringUtils::get_hash_of(const std::string& p_str)
{
	std::hash <std::string> l_hasher;
	return l_hasher(p_str);
}

std::wstring	vufStringUtils::string_to_wstring(const std::string& p_str)
{
	return std::wstring(p_str.begin(), p_str.end());
}
std::string		vufStringUtils::wstring_to_string(const std::wstring& p_wstr)
{
	return std::string(p_wstr.begin(), p_wstr.end());
}
std::wstring	vufStringUtils::get_cur_user_name()
{
	/*
#if VF_OS == VF_OS_WND
	TCHAR  l_info_buf[512];
	DWORD  l_buf_char_count = 512;
	if (!GetUserName(l_info_buf, &l_buf_char_count))
	{
		return std::wstring();
	}
	return std::wstring(l_info_buf);
#elif VF_OS == VF_OS_LINUX;
	char* l_ch = std::getenv("USER");
	if (l_ch == nullptr)
	{
		l_ch = std::getenv("USERNAME");
	}
	if (l_ch == nullptr)
	{
		return std::wstring();
	}
	return str_to_wstr(l_ch);
#endif 
	*/
	return std::wstring();
}
std::wstring	vufStringUtils::get_env_variable(const std::wstring& p_env)
{
	std::string l_str = wstring_to_string(p_env);
	char* l_ch = std::getenv(l_str.data());
	if (l_ch == nullptr)
	{
		return std::wstring();
	}
	return string_to_wstring(l_ch);
}
std::wstring	vufStringUtils::wstring_padding(const std::wstring& p_original, int p_char_count, bool p_cut)
{
	//p_original.resize(p_char_count, ' ');
	auto l_sz = p_original.size();
	if (l_sz > VF_PADDING_MAX && p_cut == true)
	{
		return p_original;
	}
	auto l_res = p_original;
	return l_res.append(p_char_count - l_sz, L'.');
}
std::string		vufStringUtils::string_padding(const std::string& p_original, int p_char_count, bool p_cut )
{
	//p_original.resize(p_char_count, ' ');
	auto l_sz = p_original.size();
	if (l_sz > VF_PADDING_MAX && p_cut == true)
	{
		return p_original;
	}
	auto l_res = p_original;
	return l_res.append(p_char_count - l_sz, '.');

}

std::vector<std::string>			vufStringUtils::string_split_by(const std::string& p_string, const std::string& p_delimiter)
{
	std::vector<std::string> l_res;
	std::string		l_s = p_string;
	size_t			l_pos = 0;
	std::string		l_token;

	while ((l_pos = l_s.find(p_delimiter)) != std::string::npos) 
	{
		l_token = l_s.substr(0, l_pos);
		l_res.push_back(l_token);
		l_s.erase(0, l_pos + p_delimiter.length());
	}
	l_res.push_back(l_s);
	return l_res;
}

