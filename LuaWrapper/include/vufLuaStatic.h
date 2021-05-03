#ifndef VF_LUA_STATIC_H
#define VF_LUA_STATIC_H

#include <string>

#define VF_LUA_STATIC_INITIALIZE()							\
std::string vuf::vufLuaStatic::g_vec4_tbl_name	= "vec4";	\
std::string vuf::vufLuaStatic::g_vec4_meta_name	= "vec4M";	\
std::string vuf::vufLuaStatic::g_mat4_tbl_name	= "mat4";	\
std::string vuf::vufLuaStatic::g_mat4_meta_name	= "mat4M";	\
std::string vuf::vufLuaStatic::g_quat_tbl_name	= "quat";	\
std::string vuf::vufLuaStatic::g_quat_meta_name = "quatM";	

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
