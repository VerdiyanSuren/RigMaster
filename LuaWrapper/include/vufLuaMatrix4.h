#ifndef VF_MATH_MATRIX_4_LUA_H
#define VF_MATH_MATRIX_4_LUA_H


extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <new>
#include <cassert>
#include <vufLuaWrapUtil.h>
#include <vufVector.h>
namespace vuf
{
	template <typename T>
	class vufLuaMatrix4
	{
	public:
		static void registrator(lua_State* L)
		{
		}
	};
}
#endif

