#ifndef VF_CORE_FILE_UTILS_H
#define VF_CORE_FILE_UTILS_H

#include <vufCoreInclude.h>
#include <vector>
#include <string>

namespace vuf
{
	class VF_API vufFileUtils
	{
	public:
		static bool							file_exists(const std::wstring& p_file_name);
		static bool							dir_exists(const std::wstring& p_dir_name);
		static bool							make_dir(const std::wstring& p_dir_name);
		static std::wstring					path_join(const std::wstring& p_path, const std::wstring& p_join);
		static bool							delete_dir(const std::wstring& p_dir_name);
		static bool							delete_file(const std::wstring& p_file_name);
		static std::wstring					get_home_dir();
		static std::wstring					format_path_slashes(const std::wstring& p_path);
		static std::vector<std::wstring>	get_dirs_from_env(const std::wstring& p_env_var);
		static std::vector<std::wstring>	get_subdirs(const std::wstring& p_root_dir, bool p_full_path = true);
		static bool 						read_lines_from_file(std::vector<std::wstring>&, const std::wstring& p_file_name);
		static bool							save_lines_to_file(const std::vector<std::wstring>& p_lines, const std::wstring& p_file);

	};
}

#endif // !VF_CORE_FILE_UTILS_H
