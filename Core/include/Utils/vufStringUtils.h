#ifndef VF_CORE_STRING_UTILS_H
#define VF_CORE_STRING_UTILS_H

#include <vufCoreInclude.h>

namespace vuf
{
	class VF_API vufStringUtils
	{
	public:
		static uint64_t				wstring_to_uint64(const std::wstring&);
		static std::wstring			id_to_wstring(uint64_t);
		static uint64_t				get_hash_of(const std::wstring&);
		static uint64_t				get_hash_of(const std::string&);

		// string 
		static std::wstring			string_to_wstring(const std::string&);
		static std::string			wstring_to_string(const std::wstring&);
		static std::wstring			get_cur_user_name();
		static std::wstring			get_env_variable(const std::wstring& p_env);

		static std::wstring			wstring_padding(const std::wstring& p_original, int char_pos, bool p_cut = false);
	};
}
#endif // !VF_CORE_STRING_UTILS_H
