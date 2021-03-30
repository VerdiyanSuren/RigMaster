#ifndef VF_LUA_WRAPPER_H
#define VF_LUA_WRAPPER_H

#include <vufLog.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <map>
#include <vector>
#include <string>
#include <vufStringUtils.h>


namespace vuf
{
	class VF_API vufTxt
	{
	public:
		void set_txt(const VF_STRING& p_str)
		{
			if (p_str != m_script)
			{
				m_script == p_str;
				m_hash = vufStringUtils<VF_STRING>::get_hash_of(m_script);
			}
		}
		uint64_t get_hash() const { return m_hash; }
		const VF_STRING& get_script() const
		{
			return m_script;
		}
	private:
		VF_STRING	m_script	= "";
		uint64_t	m_hash		= 0;
	};

	class VF_API luaWrapper
	{
	public:
		luaWrapper() {}
		virtual ~luaWrapper() 
		{
			close_machine();
		}
		// override to add your libs
		virtual bool open_machine()
		{
			close_machine();
			m_L = luaL_newstate();
			if (m_L != nullptr)
			{
				luaL_openlibs(m_L);
				return true;
			}
			return false;
		}
		// override this if you need some extra actions before close machine
		virtual void close_machine()
		{
			if (m_L != nullptr)
			{
				lua_close(m_L);
				m_L = nullptr;
			}
		}

		void add_script(const VF_STRING& p_str)
		{
			m_script.set_txt(p_str);
		}


		void dump_stack(lua_State* L)
		{
			int l_top = lua_gettop(L);
			if (l_top == 0)
			{
				vufLog::g_log->log_info("The Lua Stack is empty.");
			}
			for (int i = 1; i <= l_top; ++i)
			{
				int l_type = lua_type(L, i);
				switch (l_type)
				{
					case LUA_TSTRING:
					{
						vufLog::g_log->log_info(lua_tostring(L, i));
						break;
					}
					case LUA_TBOOLEAN:
					{
						vufLog::g_log->log_info(lua_toboolean(L, i) ? "true": "false" );
						break;
					}
					case LUA_TNUMBER:
					{
						double l_val = lua_tonumber(L, i);
						vufLog::g_log->log_info(std::to_string(l_val));
						break;
					}
					default:
					{
						vufLog::g_log->log_info(lua_typename(L, l_type));
						break;
					}
				}
			}
		}
		void clear_stack(lua_State* L)
		{
			lua_settop(L, 0);
		}
		bool do_string(lua_State* L, const VF_STRING& p_cmd)
		{
			return check_lua(L, luaL_dostring(L, p_cmd.c_str()));
		}
	private:
		bool check_lua(lua_State* L, int p_r) const
		{
			if (p_r != LUA_OK)
			{
				// there is a error. get that error
				VF_STRING l_error_msg = lua_tostring(L, -1);// gete message and remove it from stack
				lua_pop(L, -1);
				vufLog::g_log->log_error(l_error_msg);
				return false;
			}
			return true;
		}

	private:
		vufTxt			m_script;
		lua_State*		m_L = nullptr;
		//std::map<std::string,std::map<string,std::vector<std::string>> m_class_map;
	};
}

#endif // !VF_LUA_WRAPPER_H
