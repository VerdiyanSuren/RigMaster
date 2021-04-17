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
#include <coreUtils/vufStringUtils.h>

#include <iostream>

#define VF_GET_AND_LOG_LUA_ERROR(L)					\
{													\
	std::string l_error_msg = lua_tostring(L, -1);	\
	lua_pop(m_L, -1);								\
	VF_LOG_ERR(l_error_msg);						\
}
#define VF_GET_AND_LOG_LUA_ERROR_AND_RETURN_FALSE(L)\
{													\
	std::string l_error_msg = lua_tostring(L, -1);	\
	lua_pop(m_L, -1);								\
	VF_LOG_ERR(l_error_msg);						\
	return false;									\
}

namespace vuf
{
	class VF_API vufTxt
	{
	public:
		void set_script(const std::string& p_str)
		{
			if (p_str != m_script)
			{
				m_script = p_str;
				m_hash = vufStringUtils::get_hash_of(m_script);
			}
		}
		const std::string& get_script() const
		{
			return m_script;
		}
		uint64_t get_hash() const { return m_hash; }
	private:
		std::string	m_script	= "";
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

		void add_script(const std::string& p_str)
		{
			m_script.set_script(p_str);
		}


		void		dump_stack()
		{
			int l_top = lua_gettop(m_L);
			if (l_top == 0)
			{
				VF_LOG_INFO("The Lua Stack is empty.");
			}
			for (int i = 1; i <= l_top; ++i)
			{
				int l_type = lua_type(m_L, i);
				switch (l_type)
				{
					case LUA_TSTRING:
					{
						VF_LOG_INFO(lua_tostring(m_L, i));
						break;
					}
					case LUA_TBOOLEAN:
					{
						VF_LOG_INFO(lua_toboolean(m_L, i) ? "true": "false" );
						break;
					}
					case LUA_TNUMBER:
					{
						double l_val = lua_tonumber(m_L, i);
						VF_LOG_INFO(std::to_string(l_val));
						break;
					}
					default:
					{
						VF_LOG_INFO(lua_typename(m_L, l_type));
						break;
					}
				}
			}
		}
		void		clear_stack()
		{
			lua_settop(m_L, 0);
		}
		// compile lua script into the lua binary function
		bool		load_and_compile( const std::string& p_lua_script )
		{
			if (luaL_loadstring(m_L, p_lua_script.c_str()) == LUA_OK)
			{ 
				m_bytes_v.clear();
				lua_dump(m_L, lua_writer, this, false);
				lua_pop(m_L, -1);
				return true;
			}
			VF_GET_AND_LOG_LUA_ERROR_AND_RETURN_FALSE(m_L);
		}
		// evaluate internal lua script
		bool		do_string()
		{
			if (luaL_dostring(m_L, m_script.get_script().c_str()) == LUA_OK)
			{
				return true;
			}
			VF_GET_AND_LOG_LUA_ERROR_AND_RETURN_FALSE(m_L);
		}
		// evaluate lua script
		bool		do_string( const std::string& p_cmd)
		{
			if (luaL_dostring(m_L, p_cmd.c_str()) == LUA_OK)
			{
				return true;
			}
			VF_GET_AND_LOG_LUA_ERROR_AND_RETURN_FALSE(m_L);
		}
		bool		do_compiled()
		{
			if (lua_load(m_L, lua_reader, this, "myChunk",nullptr) != LUA_OK)
			//if (luaL_loadbuffer(m_L, (const char*)m_bytes_v.data(), m_bytes_v.size(),) != LUA_OK)
			{
				VF_GET_AND_LOG_LUA_ERROR_AND_RETURN_FALSE(m_L);
			}
			if (lua_pcall(m_L, 0, 0, 0) != LUA_OK)
			{
				VF_GET_AND_LOG_LUA_ERROR_AND_RETURN_FALSE(m_L);
			}
			return true;
		}
		size_t		get_compiled_size() const { return m_bytes_v.size(); }
		lua_State*	get_lua_state() { return m_L; }
	private:
		bool				check_lua( int p_r) const
		{
			if (p_r != LUA_OK)
			{
				// there is a error. get that error
				VF_STRING l_error_msg = lua_tostring(m_L, -1);// gete message and remove it from stack
				lua_pop(m_L, -1);
				VF_LOG_ERR(l_error_msg);
				return false;
			}
			return true;
		}		
		static int			lua_writer(lua_State*, const void* p_data, size_t p_size, void* p_this)
		{
			luaWrapper* l_this = (luaWrapper*)p_this;
			size_t l_old_size = l_this->m_bytes_v.size();
			l_this->m_bytes_v.resize(l_old_size + p_size);
			//std::memcpy(&(l_this->m_bytes_v[0]), p_data, p_size);
			char* l_data = (char*)p_data;
			for (int i = 0; i < (int)p_size; ++i)
			{
				l_this->m_bytes_v[l_old_size +i] = l_data[i];
			}
			return LUA_OK;
		}
		static const char*	lua_reader(lua_State* L, void* p_this, size_t* p_size)
		{
			luaWrapper* l_this	= (luaWrapper*)p_this;
			*p_size				= l_this->m_bytes_v.size();
			return	l_this->m_bytes_v.data();
		}
	public:
		static void 		dump_stack(lua_State* L)
		{
			int l_top = lua_gettop(L);
			if (l_top == 0)
			{
				VF_LOG_INFO("The Lua Stack is empty.");
			}
			for (int i = 1; i <= l_top; ++i)
			{
				int l_type = lua_type(L, i);
				switch (l_type)
				{
					case LUA_TSTRING:
					{
						VF_LOG_INFO(lua_tostring(L, i));
						break;
					}
					case LUA_TBOOLEAN:
					{
						VF_LOG_INFO(lua_toboolean(L, i) ? "true" : "false");
						break;
					}
					case LUA_TNUMBER:
					{
						double l_val = lua_tonumber(L, i);
						VF_LOG_INFO(std::to_string(l_val));
						break;
					}
					default:
					{
						VF_LOG_INFO(lua_typename(L, l_type));
						break;
					}
				}
			}
		}
	private:
		vufTxt				m_script;
		lua_State*			m_L = nullptr;
		std::vector<char>	m_bytes_v;
		//std::map<std::string,std::map<string,std::vector<std::string>> m_class_map;
	};
}

#endif // !VF_LUA_WRAPPER_H
