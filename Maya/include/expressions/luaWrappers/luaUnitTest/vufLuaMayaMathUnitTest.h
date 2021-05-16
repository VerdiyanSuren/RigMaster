#ifndef VF_LUA_MAYA_MATH_UNIT_TEST_H
#define VF_LUA_MAYA_MATH_UNIT_TEST_H

#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMEulerRotation.h>
#include <expressions/luaWrappers/vufLuaMVectorArray.h>
#include <coreUtils/vufStringUtils.h>

#include <sstream>

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
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_VECTOR")));
				return false;
			}
			if (MEulerRotation_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_EULER_ROTATION")));
				return false;
			}
			if (MQuaternion_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_QUATERNION")));
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
			vufLuaMVector::registrator(L);
			vufLuaMPoint::registrator(L);
			vufLuaMMatrix::registrator(L);
			vufLuaMQuaternion::registrator(L);
			vufLuaMVectorArray::registrator(L);
			vufLuaMEulerRotation::registrator(L);

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
				MVector* l_v1, * l_v2, * l_v3;
				l_bool = m_w.do_string("vec_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool & m_w.do_string("vec_2 = MVector.new(0.1,0.8,-0.6)");
				l_bool = l_bool & m_w.do_string("vec_3 = vec_1:cross(vec_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed MVector cross product script.")));
					return false;
				}
				vufLuaMVector::get_global(L, "vec_1", &l_v1);
				vufLuaMVector::get_global(L, "vec_2", &l_v2);
				vufLuaMVector::get_global(L, "vec_3", &l_v3);
				if (((*l_v1) ^ (*l_v2)) != *l_v3)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed MVector cross product.")));
					return false;
				}
			}
			// MVector dot
			{
				MVector* l_v1, * l_v2;
				l_bool = m_w.do_string("vec_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool & m_w.do_string("vec_2 = MVector.new(0.1,0.8,-0.6)");
				l_bool = l_bool & m_w.do_string("d = vec_1:dot(vec_2)");
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
				l_bool = m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool & m_w.do_string("q_1 = MQuaternion.new()");
				//l_bool = l_bool & m_w.do_string("vec_2 = vec_1:rotateBy(0.1,0.1,-0.1,0.9)");
				//l_bool = l_bool & m_w.do_string("vec_3 = vec_1:rotateBy(vec_2)");
			}

			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_VECTOR PASSED.")));
			return true;
		}
		bool MQuaternion_ut()
		{
			bool l_bool		= false;
			lua_State* L	= m_w.get_lua_state();
			// NQuaternion constructor
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new()");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("v_1 = MVector.new(0.2,0.3,0.4)");
				l_bool = l_bool &&	m_w.do_string("v_2 = MVector.new(0.4,0.1,-0.4)");
				l_bool = l_bool &&	m_w.do_string("q_3 = MQuaternion.new(v_1,v_2)");
				l_bool = l_bool &&	m_w.do_string("q_4 = MQuaternion.new(v_1,v_2,60)");
				l_bool = l_bool &&	m_w.do_string("q_5 = MQuaternion.new(60,v_1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:new) script")));
					return false;
				}
				MVector *l_v1, *l_v2;
				MQuaternion *l_q1, *l_q2, *l_q3, *l_q4, *l_q5;
				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_4", &l_q4);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_5", &l_q5);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:new) get globals.")));
					return false;
				}
				if (*l_q1 != MQuaternion() || *l_q2 != MQuaternion(0.2, 0.1, 0.6, 0.9) ||
					*l_q3 != MQuaternion(*l_v1, *l_v2) || *l_q4 != MQuaternion(*l_v1, *l_v2, 60) ||
					*l_q5 != MQuaternion(60, *l_v1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:new) failed")));
					return false;
				}
			}
			// slerp
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.6,0.3,0.7,0.1)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.1,0.3,0.2,0.8)");
				l_bool = l_bool &&  m_w.do_string("q_3 = MQuaternion.slerp(q_1, q_2, 0.5)");
				l_bool = l_bool &&	m_w.do_string("q_4 = MQuaternion.slerp(q_1, q_2, 0.5,1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:slerp script")));
					return false;
				}
				MQuaternion *l_q1, *l_q2, *l_q3, *l_q4;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool &&  vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				l_bool = l_bool &&  vufLuaMQuaternion::get_global(L, "q_4", &l_q4);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:slerp get globals.")));
					return false;
				}
				if (*l_q3 != slerp(*l_q1, *l_q2, 0.5) || *l_q4 != slerp(*l_q1, *l_q2, 0.5, 1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:slerp failed")));
					return false;
				}
			}
			// dslerp
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.6,0.3,0.7,0.1)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.1,0.3,0.2,0.8)");
				l_bool = l_bool &&	m_w.do_string("q_3 = MQuaternion.dslerp(q_1, q_2, 0.5,1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:dslerp script")));
					return false;
				}
				MQuaternion *l_q1, *l_q2, *l_q3;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:dslerp get globals.")));
					return false;
				}
				if ( *l_q3 != dslerp(*l_q1, *l_q2, 0.5, 1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:dslerp failed")));
					return false;
				}
			}
			// squad
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.6,0.3,0.7,0.1)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.1,0.3,0.2,0.8)");
				l_bool = l_bool &&	m_w.do_string("q_3 = MQuaternion.new(0.8,0.3,0.2,-0.8)");
				l_bool = l_bool &&	m_w.do_string("q_4 = MQuaternion.new(0.9,0.3,0.0,0.1)");
				l_bool = l_bool &&	m_w.do_string("q_5 = MQuaternion.squad(q_1, q_2, q_3, q_4, 0.5)");
				l_bool = l_bool &&	m_w.do_string("q_6 = MQuaternion.squad(q_1, q_2, q_3, q_4, 0.5, 1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:squad script")));
					return false;
				}
				MQuaternion *l_q1, *l_q2, *l_q3, *l_q4, *l_q5, *l_q6;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_4", &l_q4);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_5", &l_q5);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_6", &l_q6);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:squad get globals.")));
					return false;
				}
				if (*l_q5 != squad(*l_q1, *l_q2, *l_q3, *l_q4, 0.5) || *l_q6 != squad(*l_q1, *l_q2, *l_q3, *l_q4, 0.5, 1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:squad failed")));
					return false;
				}
			}
			// squad_pt
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.6,0.3,0.7,0.1)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.1,0.3,0.2,0.8)");
				l_bool = l_bool &&	m_w.do_string("q_3 = MQuaternion.new(0.1,0.3,0.2,0.8)");
				l_bool = l_bool &&	m_w.do_string("q_4 = MQuaternion.squadPt(q_1, q_2, q_3)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:squadPt script")));
					return false;
				}
				MQuaternion *l_q1, *l_q2, *l_q3, *l_q4;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_4", &l_q4);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:squadPt get globals.")));
					return false;
				}
				if (*l_q4 != squadPt(*l_q1, *l_q2, *l_q3))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:squadPt failed")));
					return false;
				}
			}
			// MQuaternion copy
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.6,0.3,0.7,0.1)");
				l_bool = l_bool &&	m_w.do_string("q_2 = q_1:copy()");
				l_bool = l_bool &&	m_w.do_string("q_1.x = 0.4");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:copy script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:copy get globals.")));
					return false;
				}
				if (*l_q1 == *l_q2 || *l_q2 != MQuaternion(0.6, 0.3, 0.7, 0.1) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:copy failed")));
					return false;
				}
			}
			// asMatrix
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.3,0.4,0.5,0.1)");
				l_bool = l_bool &&	m_w.do_string("m_1 = q_1:asMatrix()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:asMatrix script")));
					return false;
				}
				MQuaternion	*l_q1;
				MMatrix		*l_m1;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_1", &l_m1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:asMatrix get globals.")));
					return false;
				}
				if (*l_m1 != l_q1->asMatrix())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:asMatrix failed")));
					return false;
				}
			}
			// asEulerRotation
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.3,0.4,0.5,0.1)");
				l_bool = l_bool &&	m_w.do_string("r_1 = q_1:asEulerRotation()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:asEulerRotation script")));
					return false;
				}
				MQuaternion		*l_q1;
				MEulerRotation	*l_r1;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:asEulerRotation get globals.")));
					return false;
				}
				if (*l_r1 != l_q1->asEulerRotation())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:asEulerRotation failed")));
					return false;
				}
			}
			// setAxisAngle
			{
				l_bool =			m_w.do_string("q_1 =MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("v_1 = MVector.new(0.2,0.3,0.4)");
				l_bool = l_bool && m_w.do_string(" q_2 = q_1:setAxisAngle(v_1,60)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setAxisAngle) script")));
					return false;
				}
				MVector* l_v1;
				MQuaternion *l_q1, *l_q2;
				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&  vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&  vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setAxisAngle) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).setAxisAngle(*l_v1, 60))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setAxisAngle) failed")));
					return false;
				}
			}
			// getAxisAngle
			{
				l_bool =			m_w.do_string("q_1 =MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("v_1,a,b = q_1:getAxisAngle()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:getAxisAngle) script")));
					return false;
				}
				MVector* l_v1, l_v2;;
				MQuaternion *l_q1;
				double l_angle, l_angle2;
				int l_res;

				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&  vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				lua_getglobal(L, "a");
				l_angle = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "b");
				l_res = lua_toboolean(L, -1);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:getAxisAngle) get globals.")));
					return false;
				}
				if (l_res != (int)l_q1->getAxisAngle(l_v2, l_angle2) || l_v2 != *l_v1 || l_angle != l_angle2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:getAxisAngle) failed")));
					return false;
				}
			}
			// setToXAxis
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = q_1:setToXAxis(60)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToXAxis) script")));
					return false;
				}
				MQuaternion* l_q1,*l_q2;

				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToXAxis) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).setToXAxis(60) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToXAxis) failed")));
					return false;
				}
			}
			// setToXAxis
			{
				l_bool = m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool && m_w.do_string("q_2 = q_1:setToYAxis(60)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToYAxis) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;

				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToYAxis) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).setToYAxis(60))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToYAxis) failed")));
					return false;
				}
			}
			// setToZAxis
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = q_1:setToZAxis(60)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToZAxis) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;

				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToZAxis) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).setToZAxis(60))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:setToZAxis) failed")));
					return false;
				}
			}
			// negateIt
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = q_1:negateIt()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:negateIt) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:negateIt) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).negateIt())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:negateIt) failed")));
					return false;
				}
			}
			// isEquivalent
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.1,0.3,0.7,0.8)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.2,0.6,0.7,0.1)");
				l_bool = l_bool &&	m_w.do_string("b_1 = q_1:isEquivalent(q_2)");
				l_bool = l_bool &&	m_w.do_string("b_2 = q_1:isEquivalent(q_2,5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:isEquivalent script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				int l_b1, l_b2;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				lua_getglobal(L, "b_1");
				l_b1 = lua_toboolean(L, -1);
				lua_getglobal(L, "b_2");
				l_b2 = lua_toboolean(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:isEquivalent get globals.")));
					return false;
				}
				if (l_b1 != (int)l_q1->isEquivalent(*l_q2) ||
					l_b2 != (int)l_q1->isEquivalent(*l_q2, 5))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:isEquivalent failed")));
					return false;
				}
			}
			// scaleIt
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = q_1:scaleIt(60)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:scaleIt) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:scaleIt) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).scaleIt(60))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:scaleIt) failed")));
					return false;
				}
			}
			// normal
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = q_1:normal()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:normal) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:normal) get globals.")));
					return false;
				}
				if (*l_q1 == *l_q2 || *l_q2 != MQuaternion(0.2, 0.1, 0.6, 0.9).normal())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:normal) failed")));
					return false;
				}
			}
			// normalizeIt
			{
				l_bool = m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool && m_w.do_string("q_2 = q_1:normalizeIt()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:normalizeIt) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:normalizeIt) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).normalizeIt())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:normalizeIt) failed")));
					return false;
				}
			}
			// conjugate
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = q_1:conjugate()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:conjugate) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:conjugate) get globals.")));
					return false;
				}
				if (*l_q1 == *l_q2 || *l_q2 != MQuaternion(0.2, 0.1, 0.6, 0.9).conjugate())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:conjugate) failed")));
					return false;
				}
			}
			// conjugateIt
			{
				l_bool = m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool && m_w.do_string("q_2 = q_1:conjugateIt()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:conjugateIt) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:conjugateIt) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).conjugateIt())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:conjugateIt) failed")));
					return false;
				}
			}
			// inverse
			{
				l_bool = m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool && m_w.do_string("q_2 = q_1:inverse()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:inverse) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:inverse) get globals.")));
					return false;
				}
				if (*l_q1 == *l_q2 || *l_q2 != MQuaternion(0.2, 0.1, 0.6, 0.9).inverse())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:inverse) failed")));
					return false;
				}
			}
			// invertIt
			{
				l_bool = m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool && m_w.do_string("q_2 = q_1:invertIt()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:invertIt) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:invertIt) get globals.")));
					return false;
				}
				if (*l_q1 != *l_q2 || *l_q1 != MQuaternion(0.2, 0.1, 0.6, 0.9).invertIt())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:invertIt) failed")));
					return false;
				}
			}
			// log
			{
				l_bool = m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool && m_w.do_string("q_2 = q_1:log()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:log) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:log) get globals.")));
					return false;
				}
				if (*l_q1 == *l_q2 || *l_q2 != MQuaternion(0.2, 0.1, 0.6, 0.9).log())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:log) failed")));
					return false;
				}
			}
			// exp
			{
				l_bool = m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool && m_w.do_string("q_2 = q_1:exp()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:exp) script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:exp) get globals.")));
					return false;
				}
				if (*l_q1 == *l_q2 || *l_q2 != MQuaternion(0.2, 0.1, 0.6, 0.9).exp())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:exp) failed")));
					return false;
				}
			}
			// add
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.1,0.2,0.7,-0.5)");
				l_bool = l_bool &&	m_w.do_string("q_3 = q_1 + q_2");

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:add script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2, * l_q3;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:add get globals.")));
					return false;
				}
				if (*l_q3 != *l_q1 + *l_q2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:add failed")));
					return false;
				}
			}
			// sub
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.2,0.1,0.6,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.1,0.2,0.7,-0.5)");
				l_bool = l_bool &&	m_w.do_string("q_3 = q_1 - q_2");

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:sub script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2, * l_q3;
				l_bool = vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:sub get globals.")));
					return false;
				}
				if (*l_q3 != *l_q1 - *l_q2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:sub failed")));
					return false;
				}
			}
			// mul
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.3,0.4,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.1,0.6,0.7,0.3)");
				l_bool = l_bool &&	m_w.do_string("q_3 = q_1 *2");
				l_bool = l_bool &&	m_w.do_string("q_4 = 2 * q_1");
				l_bool = l_bool &&	m_w.do_string("q_5 = q_1 * q_2");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:mul script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2, * l_q3, * l_q4, *l_q5;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_4", &l_q4);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_5", &l_q5);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:mul get globals.")));
					return false;
				}
				if (*l_q3 != 2 * (*l_q1)  || *l_q4 != 2 * (*l_q1) || *l_q5 != (*l_q1) * (*l_q2) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:mul failed")));
					return false;
				}
			}
			// indecies
			{
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.1,0.2,0.3,0.9)");
				l_bool = l_bool &&	m_w.do_string("q_2 = MQuaternion.new(0.6,0.1,-0.3, 0.9)");
				l_bool = l_bool &&	m_w.do_string("x = q_1.x");
				l_bool = l_bool &&	m_w.do_string("y = q_1.y");
				l_bool = l_bool &&	m_w.do_string("z = q_1.z");
				l_bool = l_bool &&	m_w.do_string("w = q_1.w");
				l_bool = l_bool &&	m_w.do_string("q_2.x = 0.9");
				l_bool = l_bool &&	m_w.do_string("q_2.y = 0.8");
				l_bool = l_bool &&	m_w.do_string("q_2.z = 0.1");
				l_bool = l_bool &&	m_w.do_string("q_2.w = 0.2");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:indecies script")));
					return false;
				}
				MQuaternion* l_q1, * l_q2;
				vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				double l_x, l_y, l_z,l_w;
				lua_getglobal(L, "x"); l_x = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "y"); l_y = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "z"); l_z = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "w"); l_w = (double)lua_tonumber(L, -1);
				const char* l_char = lua_tostring(L, -1);
				std::string l_str(l_char);

				if (l_x != l_q1->x || l_y != l_q1->y || l_z != l_q1->z || l_w != l_q1->w ||
					*l_q2 != MQuaternion(0.9, 0.8, 0.1,0.2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MQuaternion:indecies failed")));
					return false;
				}
			}

			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_QUATERNION PASSED.")));
			return true;
		}
		bool MEulerRotation_ut()
		{
			bool l_bool = false;
			lua_State* L = m_w.get_lua_state();
			// MEulerRotation constructor
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new()");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(30,60,90)");
				l_bool = l_bool && m_w.do_string("r_3 = MEulerRotation.new(10,11,12,\"xzy\")");
				l_bool = l_bool && m_w.do_string("v_1 = MVector.new(90,30,60)");
				l_bool = l_bool && m_w.do_string("r_4 = MEulerRotation.new(v_1)");
				l_bool = l_bool && m_w.do_string("r_5 = MEulerRotation.new(v_1,\"zyx\")");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation constructor script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2, * l_r3, * l_r4, * l_r5;
				MVector* l_v1;

				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_4", &l_r4);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_5", &l_r5);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation constructor get globals.")));
					return false;
				}
				if (*l_r1 != MEulerRotation() ||
					*l_r2 != MEulerRotation(30, 60, 90) ||
					*l_r3 != MEulerRotation(10, 11, 12, MEulerRotation::kXZY) ||
					*l_r4 != MEulerRotation(*l_v1) ||
					*l_r5 != MEulerRotation(*l_v1, MEulerRotation::kZYX))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation constructor failed")));
					return false;
				}
			}
			// MEulerRotation copy
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,30,90)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:copy()");
				l_bool = l_bool && m_w.do_string("r_1.x = 100");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:copy script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:copy get globals.")));
					return false;
				}
				if (*l_r1 == *l_r2 || *l_r2 != MEulerRotation(60, 30, 90))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:copy failed")));
					return false;
				}
			}
			// decompose
			{
				l_bool = m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool && m_w.do_string("r_1 = MEulerRotation.decompose(m_1)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.decompose(m_1,\"zyx\")");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:decompose script")));
					return false;
				}
				MMatrix* l_m;
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMMatrix::get_global(L, "m_1", &l_m);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:decompose get globals.")));
					return false;
				}
				if (*l_r1 != MEulerRotation::decompose(*l_m, MEulerRotation::kXYZ) || *l_r2 != MEulerRotation::decompose(*l_m, MEulerRotation::kZYX))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:decompose failed")));
					return false;
				}
			}
			// MEulerROtation setValue
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new()");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:copy()");
				l_bool = l_bool && m_w.do_string("r_3 = r_1:copy()");
				l_bool = l_bool && m_w.do_string("r_4 = r_1:copy()");
				l_bool = l_bool && m_w.do_string("v_1 = MVector.new(10,20,30)");
				l_bool = l_bool && m_w.do_string("r_11 = r_1:setValue(v_1)");
				l_bool = l_bool && m_w.do_string("r_22 = r_2:setValue(v_1,\"zyx\")");
				l_bool = l_bool && m_w.do_string("r_33 = r_3:setValue(70,80,90)");
				l_bool = l_bool && m_w.do_string("r_44 = r_4:setValue(70,80,90,\"zyx\")");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setValue script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2, * l_r3, * l_r4;
				MEulerRotation* l_r11, * l_r22, * l_r33, * l_r44;
				MVector* l_v1;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_4", &l_r4);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_11", &l_r11);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_22", &l_r22);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_33", &l_r33);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_44", &l_r44);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setValue get globals.")));
					return false;
				}
				if (*l_r11 != MEulerRotation().setValue(*l_v1) ||
					*l_r22 != MEulerRotation().setValue(*l_v1, MEulerRotation::kZYX) ||
					*l_r33 != MEulerRotation().setValue(70, 80, 90) ||
					*l_r44 != MEulerRotation().setValue(70, 80, 90, MEulerRotation::kZYX) ||
					*l_r11 != *l_r1 ||
					*l_r22 != *l_r2 ||
					*l_r33 != *l_r3 ||
					*l_r44 != *l_r4)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setValue failed")));
					return false;
				}
			}
			// asQuaternion
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(30,40,50)");
				l_bool = l_bool && m_w.do_string("q_1 = r_1:asQuaternion()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asQuaternion script")));
					return false;
				}
				MEulerRotation* l_r;
				MQuaternion* l_q;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_1", &l_q);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asQuaternion get globals.")));
					return false;
				}
				if (*l_q != l_r->asQuaternion())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asQuaternion failed")));
					return false;
				}
			}
			// asMatrix
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(30,40,50)");
				l_bool = l_bool && m_w.do_string("m_1 = r_1:asMatrix()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asMatrix script")));
					return false;
				}
				MEulerRotation* l_r;
				MMatrix* l_m;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r);
				l_bool = l_bool && vufLuaMMatrix::get_global(L, "m_1", &l_m);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asMatrix get globals.")));
					return false;
				}
				if (*l_m != l_r->asMatrix())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asMatrix failed")));
					return false;
				}
			}
			// asVector
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(30,40,50)");
				l_bool = l_bool && m_w.do_string("v_1 = r_1:asVector()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asVector script")));
					return false;
				}
				MEulerRotation* l_r;
				MVector* l_v;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asVector get globals.")));
					return false;
				}
				if (*l_v != l_r->asVector())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:asVector failed")));
					return false;
				}
			}
			// isEquivalent
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(30,40,50)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(30,40,51)");
				l_bool = l_bool && m_w.do_string("b_1 = r_1:isEquivalent(r_2)");
				l_bool = l_bool && m_w.do_string("b_2 = r_1:isEquivalent(r_2,5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:isEquivalent script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				int l_b1, l_b2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				lua_getglobal(L, "b_1");
				l_b1 = lua_toboolean(L, -1);
				lua_getglobal(L, "b_2");
				l_b2 = lua_toboolean(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:isEquivalent get globals.")));
					return false;
				}
				if (l_b1 != (int)l_r1->isEquivalent(*l_r2) ||
					l_b2 != (int)l_r1->isEquivalent(*l_r2, 5))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:isEquivalent failed")));
					return false;
				}
			}
			// isZero
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(0.1,0.2,0.3)");
				l_bool = l_bool && m_w.do_string("b_1 = r_1:isZero()");
				l_bool = l_bool && m_w.do_string("b_2 = r_1:isZero(5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:isZero script")));
					return false;
				}
				MEulerRotation* l_r1;
				int l_b1, l_b2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				lua_getglobal(L, "b_1");
				l_b1 = lua_toboolean(L, -1);
				lua_getglobal(L, "b_2");
				l_b2 = lua_toboolean(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:isZero get globals.")));
					return false;
				}
				if (l_b1 != (int)l_r1->isZero() ||
					l_b2 != (int)l_r1->isZero(5))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:isZero failed")));
					return false;
				}
			}
			// incrementalRotateBy
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("v_1 = MVector.new(1,0,0)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:incrementalRotateBy(v_1,60)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:incrementalRotateBy script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				MVector* l_v1;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:incrementalRotateBy get globals.")));
					return false;
				}
				if (*l_r2 != *l_r2 || *l_r1 != MEulerRotation(60, 20, 30).incrementalRotateBy(*l_v1, 60))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:incrementalRotateBy failed")));
					return false;
				}
			}
			// inverse
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:inverse()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:inverse script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:inverse get globals.")));
					return false;
				}
				if (*l_r2 != l_r1->inverse() || *l_r1 == *l_r2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:inverse failed")));
					return false;
				}
			}
			// invertIt
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:invertIt()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:invertIt script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:invertIt get globals.")));
					return false;
				}
				if (*l_r2 != *l_r1 || *l_r1 != MEulerRotation(60, 20, 30).invertIt())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:invertIt failed")));
					return false;
				}
			}
			// reorder
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:reorder(\"zyx\")");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:reorder script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:reorder get globals.")));
					return false;
				}
				if (*l_r2 != l_r1->reorder(MEulerRotation::kZYX) || *l_r1 == *l_r2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:reorder failed")));
					return false;
				}
			}
			// reorderIt
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:reorderIt(\"zyx\")");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:reorderIt script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:reorderIt get globals.")));
					return false;
				}
				if (*l_r2 != *l_r1 || *l_r1 != MEulerRotation(60, 20, 30).reorderIt(MEulerRotation::kZYX))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:reorderIt failed")));
					return false;
				}
			}
			// bound
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:bound()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:bound script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:bound get globals.")));
					return false;
				}
				if (*l_r2 != l_r1->bound() || *l_r1 == *l_r2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:bound failed")));
					return false;
				}
			}
			// boundIt
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:boundIt()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:boundIt script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:boundIt get globals.")));
					return false;
				}
				if (*l_r2 != *l_r1 || *l_r1 != MEulerRotation(60, 20, 30).boundIt())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:boundIt failed")));
					return false;
				}
			}
			// alternateSolution
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = r_1:alternateSolution()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:alternateSolution script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:alternateSolution get globals.")));
					return false;
				}
				if (*l_r2 != l_r1->alternateSolution() || *l_r1 == *l_r2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:alternateSolution failed")));
					return false;
				}
			}
			// setToAlternateSolution
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(30,20,10)");
				l_bool = l_bool && m_w.do_string("r_3 = r_2:setToAlternateSolution(r_1)");
				l_bool = l_bool && m_w.do_string("r_4 = MEulerRotation.new(1,2,3)");
				l_bool = l_bool && m_w.do_string("r_5 = r_4:setToAlternateSolution()");

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToAlternateSolution script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2, * l_r3, * l_r4, * l_r5;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_4", &l_r4);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_5", &l_r5);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToAlternateSolution get globals.")));
					return false;
				}
				if (*l_r2 != *l_r3 || *l_r2 != MEulerRotation(30, 20, 10).setToAlternateSolution(*l_r1) ||
					*l_r4 != *l_r5 || *l_r4 != MEulerRotation(1, 2, 3).setToAlternateSolution())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToAlternateSolution failed")));
					return false;
				}
			}
			// closestSolution
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(360,220,-130)");
				l_bool = l_bool && m_w.do_string("r_3 = r_1:closestSolution(r_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:closestSolution script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2, * l_r3;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:closestSolution get globals.")));
					return false;
				}
				if (*l_r3 == *l_r1 || *l_r3 != l_r1->closestSolution(*l_r2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:closestSolution failed")));
					return false;
				}
			}
			// setToClosestSolution
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(360,220,-130)");
				l_bool = l_bool && m_w.do_string("r_3 = MEulerRotation.new()");
				l_bool = l_bool && m_w.do_string("r_4 = r_3:setToClosestSolution(r_1,r_2)");
				l_bool = l_bool && m_w.do_string("r_5 = r_1:setToClosestSolution(r_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToClosestSolution script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2, * l_r3, * l_r4, * l_r5;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_4", &l_r4);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_5", &l_r5);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToClosestSolution get globals.")));
					return false;
				}
				if (*l_r4 != MEulerRotation().setToClosestSolution(MEulerRotation(60, 20, 30), MEulerRotation(360, 220, -130)) || *l_r3 != *l_r4 ||
					*l_r5 != *l_r1 || *l_r5 != MEulerRotation(60, 20, 30).setToClosestSolution(*l_r2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToClosestSolution failed")));
					return false;
				}
			}
			// closestCut
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(360,220,-130)");
				l_bool = l_bool && m_w.do_string("r_3 = r_1:closestCut(r_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:closestCut script")));
					return false;
				}

				MEulerRotation* l_r1, * l_r2, * l_r3;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:closestCut get globals.")));
					return false;
				}
				if (*l_r3 == *l_r1 || *l_r3 != MEulerRotation(60, 20, 30).closestCut(*l_r2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:closestCut failed")));
					return false;
				}
			}
			// setToClosestCut
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(360,220,-130)");
				l_bool = l_bool && m_w.do_string("r_3 = MEulerRotation.new()");
				l_bool = l_bool && m_w.do_string("r_4 = r_3:setToClosestCut(r_1,r_2)");
				l_bool = l_bool && m_w.do_string("r_5 = r_1:setToClosestCut(r_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToClosestCut script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2, * l_r3, * l_r4, * l_r5;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_4", &l_r4);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_5", &l_r5);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToClosestCut get globals.")));
					return false;
				}
				if (*l_r4 != MEulerRotation().setToClosestCut(MEulerRotation(60, 20, 30), MEulerRotation(360, 220, -130)) || *l_r3 != *l_r4 ||
					*l_r5 != *l_r1 || *l_r5 != MEulerRotation(60, 20, 30).setToClosestCut(*l_r2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setToClosestCut failed")));
					return false;
				}
			}
			// to_string
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("s_1 = r_1:to_string()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:to_string script")));
					return false;
				}
				lua_getglobal(L, "s_1");
				const char* l_char = lua_tostring(L, -1);
				MEulerRotation* l_r1;
				vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				std::stringstream l_ss;
				l_ss << *l_r1;
				std::string l_s1 = l_ss.str();
				std::string l_s2(l_char);
				if (l_s1 != l_s2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:to_string failed")));
					return false;
				}
			}
			// to_type
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new()");
				l_bool = l_bool && m_w.do_string("s_1 = r_1:to_type()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:to_type script")));
					return false;
				}
				lua_getglobal(L, "s_1");
				const char* l_char = lua_tostring(L, -1);
				std::string l_s1(vufLuaMayaStatic::g_meuler_tbl_name);
				std::string l_s2(l_char);
				if (l_s1 != l_s2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:to_type failed")));
					return false;
				}
			}
			// add
			{
				l_bool =			m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool &&	m_w.do_string("r_2 = MEulerRotation.new(40,10,90)");
				l_bool = l_bool &&	m_w.do_string("r_3 = r_1 + r_2");

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:add script")));
					return false;
				}
				MEulerRotation *l_r1, *l_r2, *l_r3;
				l_bool =			vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:add get globals.")));
					return false;
				}
				if (*l_r3 != *l_r1 + *l_r2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:add failed")));
					return false;
				}
			}
			// sub
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(40,10,90)");
				l_bool = l_bool && m_w.do_string("r_3 = r_1 - r_2");

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:sub script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2, * l_r3;
				l_bool = vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:sub get globals.")));
					return false;
				}
				if (*l_r3 != *l_r1 - *l_r2)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:sub failed")));
					return false;
				}
			}
			// unm
			{
				l_bool =			 m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool &&	m_w.do_string("r_2 = -r_1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:unnm script")));
					return false;
				}
				MEulerRotation *l_r1, *l_r2;
				l_bool =			vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:inm get globals.")));
					return false;
				}
				if (*l_r2 == *l_r1 || *l_r2 != -*l_r1 )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:unm failed")));
					return false;
				}
			}
			// mul
			{
				l_bool =			m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool =			m_w.do_string("r_2 = MEulerRotation.new(10,40,60)");
				l_bool =			m_w.do_string("q_1 = MQuaternion.new(0.1,0.2,0.7,0.3)");
				l_bool = l_bool &&	m_w.do_string("r_3 = r_1 * 2");
				l_bool = l_bool &&	m_w.do_string("r_4 =  2 * r_1");
				l_bool = l_bool &&	m_w.do_string("r_5 = r_1 * r_2");
				l_bool = l_bool &&	m_w.do_string("r_6 = r_1 * q_1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:mul script")));
					return false;
				}
				MQuaternion* l_q1;
				MEulerRotation *l_r1, *l_r2, *l_r3, *l_r4,*l_r5,*l_r6;
				l_bool =			vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_4", &l_r4);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_5", &l_r5);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_6", &l_r6);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:mul get globals.")));
					return false;
				}
				if (*l_r3 != (*l_r1) *2 || *l_r4 != 2 *(*l_r1) || *l_r5 != (*l_r1) * (*l_r2) || *l_r6 != (*l_r1) * (*l_q1) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:mul failed")));
					return false;
				}
			}

			// indecies
			{
				l_bool = m_w.do_string("r_1 = MEulerRotation.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("r_2 = MEulerRotation.new(360,220,-130)");
				l_bool = l_bool && m_w.do_string("x = r_1.x");
				l_bool = l_bool && m_w.do_string("y = r_1.y");
				l_bool = l_bool && m_w.do_string("z = r_1.z");
				l_bool = l_bool && m_w.do_string("r = r_1.r");
				l_bool = l_bool && m_w.do_string("r_2.x = 10");
				l_bool = l_bool && m_w.do_string("r_2.y = 20");
				l_bool = l_bool && m_w.do_string("r_2.z = 30");
				l_bool = l_bool && m_w.do_string("r_2.r = \"zyx\"");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:indecies script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2;
				vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				double l_x, l_y, l_z;
				lua_getglobal(L, "x"); l_x = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "y"); l_y = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "z"); l_z = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "r");
				const char* l_char = lua_tostring(L, -1);
				std::string l_str(l_char);

				if (l_x != l_r1->x || l_y != l_r1->y || l_z != l_r1->z || l_str != "xyz"  ||
					*l_r2 != MEulerRotation(10, 20, 30, MEulerRotation::kZYX))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:indecies failed")));
					return false;
				}
			}

			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_EULER_ROTATION PASSED.")));
			return true;

		}
	private:
		vuf::vufLuaWrapper	m_w;
	};
}
#endif // !VF_LUA_MAYA_MATH_UNIT_TEST_H
