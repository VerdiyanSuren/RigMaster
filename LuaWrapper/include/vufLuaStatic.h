#ifndef VF_LUA_STATIC_H
#define VF_LUA_STATIC_H

#include <string>

/*
std::string vuf::vufLuaStatic::g_vec4_tbl_name	= "vec4";	\
std::string vuf::vufLuaStatic::g_vec4_meta_name	= "vec4M";	\
#define VF_LUA_STATIC_INITIALIZE()							\
std::string vuf::vufLuaStatic::g_mat4_tbl_name	= "mat4";	\
std::string vuf::vufLuaStatic::g_mat4_meta_name	= "mat4M";	\
std::string vuf::vufLuaStatic::g_quat_tbl_name	= "quat";	\
std::string vuf::vufLuaStatic::g_quat_meta_name = "quatM";	
*/

namespace vuf
{
	class vufLuaStatic
	{
	public:
		static constexpr char* g_vec4_tbl_name	=	"vec4";
		static constexpr char* g_vec4_meta_name =	"vec4M" ;

		static constexpr char* g_mat4_tbl_name	=	"mat4";
		static constexpr char* g_mat4_meta_name =	"mat4M";

		static constexpr char* g_quat_tbl_name	=	"quat";
		static constexpr char* g_quat_meta_name =	"quatM";

		//static constexpr char* g_quat_tbl_name = "arr";
		//static constexpr char* g_quat_meta_name = "quatM";

	};
}
#endif // !VF_LUA_STATIC_H
