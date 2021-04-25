#ifndef VF_LUA_STATIC_H
#define VF_LUA_STATIC_H

#include <string>

namespace vuf
{
	class vufLuaStatic
	{
	public:
		static std::string g_vec4_tbl_name;
		static std::string g_vec4_meta_name;

		static std::string g_mat4_tbl_name;
		static std::string g_mat4_meta_name;

		static std::string g_quat_tbl_name;
		static std::string g_quat_meta_name;

	};
}
#endif // !VF_LUA_STATIC_H
