#ifndef VF_LUA_MAYA_MATH_UNIT_TEST_H
#define VF_LUA_MAYA_MATH_UNIT_TEST_H

#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMVectorArray.h>
#include <coreUtils/vufStringUtils.h>

namespace vufRM
{
	class vufLuaMayaMathUT
	{
	public:
		bool run()
		{
			VF_LOG_INFO(vuf::vufStringUtils::string_padding("..................................................."));
			VF_LOG_INFO(vuf::vufStringUtils::string_padding("Start Unit tests for lua wrapp around maya objects."));
			if (prepare() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding("....Failed to prepare lua machine"));
				return false;
			}
			if (MVector_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA MVECTOR")));
				return false;
			}

			VF_LOG_INFO(vuf::vufStringUtils::string_padding("..................................................."));
			release();
			return true;
		}
	private:
		bool prepare()
		{
			if (m_w.open_machine() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed to create lua machune.")));
				return false;
			}
			lua_State* L = m_w.get_lua_state();
			vufLuaMVector::		registrator(L);
			vufLuaMPoint::		registrator(L);
			vufLuaMMatrix::		registrator(L);
			vufLuaMQuaternion::	registrator(L);
			vufLuaMVectorArray::registrator(L);

			return true;
		}
		void release()
		{
			m_w.close_machine();
		}
		bool MVector_ut()
		{
			bool l_bool = false;
			lua_State* L = m_w.get_lua_state();
			// MVector constructor unit tests
			{
				l_bool = m_w.do_string("vec_1 = MVector.new()");
				l_bool = l_bool && m_w.do_string("vec_2 = MVector.new(0.5,0.6,0.7)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector constructor script")));
					return false;
				}
				MVector* l_v1, * l_v2;
				if (vufLuaMVector::get_global(L, "vec_1", &l_v1) == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed vufLuaMVector::get_global")));
					return false;
				}
				if (vufLuaMVector::get_global(L, "vec_2", &l_v2) == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed vufLuaMVector::get_global")));
					return false;
				}
				if (*l_v1 != MVector() || *l_v2 != MVector(0.5, 0.6, 0.7))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed constructor match MVector")));
					return false;
				}
				l_bool = m_w.do_string("pnt_1 = MPoint.new(1,2,3,4)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector constructor MPoint(double[4]) script")));
					return false;
				}
				l_bool = m_w.do_string("vec_3 = MVector.new(pnt_1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector constructor MVector(MPoint) script")));
					return false;
				}
				if (vufLuaMVector::get_global(L, "vec_3", &l_v1) == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector constructor(MPoint) to get global.")));
					return false;
				}
				if (*l_v1 != MVector(MPoint(1, 2, 3, 4)))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector constructor(MPoint).")));
					return false;
				}
			}
			// MVector copy
			{				
				l_bool = m_w.do_string("vec_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool & m_w.do_string("vec_2 = vec_1:copy()");
				l_bool = l_bool & m_w.do_string("vec_1.x = 2");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector copy constructor script")));
					return false;
				}
				MVector* l_v1, * l_v2;
				vufLuaMVector::get_global(L, "vec_1", &l_v1);
				vufLuaMVector::get_global(L, "vec_2", &l_v2);
				if (*l_v2 != MVector(0.5, 0.6, 0.7) || *l_v1 == *l_v2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector copy.")));
					return false;
				}
			}
			// MVector croos
			{
				MVector* l_v1, * l_v2, *l_v3;
				l_bool =			m_w.do_string("vec_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &	m_w.do_string("vec_2 = MVector.new(0.1,0.8,-0.6)");
				l_bool = l_bool &	m_w.do_string("vec_3 = vec_1:cross(vec_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed MVector cross product script.")));
					return false;
				}
				vufLuaMVector::get_global(L, "vec_1", &l_v1);
				vufLuaMVector::get_global(L, "vec_2", &l_v2);
				vufLuaMVector::get_global(L, "vec_3", &l_v3);
				if ( ((*l_v1) ^ (*l_v2)) != *l_v3)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed MVector cross product.")));
					return false;
				}
			}
			// MVector dot
			{
				MVector* l_v1, * l_v2;
				l_bool =			m_w.do_string("vec_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &	m_w.do_string("vec_2 = MVector.new(0.1,0.8,-0.6)");
				l_bool = l_bool &	m_w.do_string("d = vec_1:dot(vec_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed MVector dot product script.")));
					return false;
				}
				vufLuaMVector::get_global(L, "vec_1", &l_v1);
				vufLuaMVector::get_global(L, "vec_2", &l_v2);
				lua_getglobal(L, "d");
				double l_val = (double)lua_tonumber(L, -1);
				if (l_val != (*l_v1) * (*l_v2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed MVector dot product.")));
					return false;
				}
			}
			// MVector rotate by
			{
				l_bool = m_w.do_string(			"v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool & m_w.do_string("q_1 = MQuaternion.new()");
				//l_bool = l_bool & m_w.do_string("vec_2 = vec_1:rotateBy(0.1,0.1,-0.1,0.9)");
				//l_bool = l_bool & m_w.do_string("vec_3 = vec_1:rotateBy(vec_2)");
			}

			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR MVECTOR PASSED.")));
			return true;
		}
	private:
		vuf::vufLuaWrapper	m_w;
	};
}
#endif // !VF_LUA_MAYA_MATH_UNIT_TEST_H
