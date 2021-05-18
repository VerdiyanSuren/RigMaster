#ifndef VF_LUA_MAYA_MATH_UNIT_TEST_H
#define VF_LUA_MAYA_MATH_UNIT_TEST_H

#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMEulerRotation.h>
#include <expressions/luaWrappers/vufLuaMVectorArray.h>
#include <expressions/luaWrappers/vufLuaMPointArray.h>
#include <expressions/luaWrappers/vufLuaMIntArray.h>
#include <expressions/luaWrappers/vufLuaMDoubleArray.h>
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
			if (MPoint_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_POINT")));
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
			if (MMatrix_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_MATRIX")));
				return false;
			}
			if (MVectorArray_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_VECTOR_ARRAY")));
				return false;
			}
			if (MPointArray_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_POINT_ARRAY")));
				return false;
			}
			if (MIntArray_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_INT_ARRAY")));
				return false;
			}
			if (MDoubleArray_ut() == false)
			{
				VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA M_DOUBLE_ARRAY")));
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
			vufLuaMEulerRotation::registrator(L);
			vufLuaMVectorArray::registrator(L);
			vufLuaMPointArray::registrator(L);
			vufLuaMIntArray::registrator(L);
			vufLuaMDoubleArray::registrator(L);

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
				l_bool =			m_w.do_string("vec_1 = MVector.new()");
				l_bool = l_bool &&	m_w.do_string("pnt_1 = MPoint.new(0.5,0.6,0.7,1)");
				l_bool = l_bool &&	m_w.do_string("vec_2 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("vec_3 = MVector.new(pnt_1)");

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector constructor script")));
					return false;
				}
				MVector* l_v1, * l_v2, *l_v3;
				MPoint* l_p;
				l_bool = l_bool && vufLuaMVector::get_global(L, "vec_1", &l_v1);
				l_bool = l_bool && vufLuaMVector::get_global(L, "vec_2", &l_v2);
				l_bool = l_bool && vufLuaMVector::get_global(L, "vec_3", &l_v3);
				l_bool = l_bool && vufLuaMPoint::get_global( L, "pnt_1", &l_p);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed vufLuaMVector::get_global")));
					return false;
				}
				if (*l_v1 != MVector() || *l_v2 != MVector(0.5, 0.6, 0.7) || *l_v3 != MVector(*l_p) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector constructor(MPoint).")));
					return false;
				}
			}
			// MVector copy
			{
				l_bool = m_w.do_string("vec_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool && m_w.do_string("vec_2 = vec_1:copy()");
				l_bool = l_bool && m_w.do_string("vec_1.x = 2");
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
				l_bool = l_bool && m_w.do_string("vec_2 = MVector.new(0.1,0.8,-0.6)");
				l_bool = l_bool && m_w.do_string("vec_3 = vec_1:cross(vec_2)");
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
				l_bool = l_bool && m_w.do_string("vec_2 = MVector.new(0.1,0.8,-0.6)");
				l_bool = l_bool && m_w.do_string("d = vec_1:dot(vec_2)");
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
			// MVector rotateBy
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("q_1 = MQuaternion.new()");
				l_bool = l_bool &&	m_w.do_string("r_1 = MEulerRotation.new(20,30,40)");
				l_bool = l_bool &&	m_w.do_string("v_2 = vec_1:rotateBy(0.1,0.1,-0.1,0.9)");
				l_bool = l_bool &&	m_w.do_string("v_3 = vec_1:rotateBy(\"x\",60)");
				l_bool = l_bool &&	m_w.do_string("v_4 = vec_1:rotateBy(q_1)");
				l_bool = l_bool &&	m_w.do_string("v_5 = vec_1:rotateBy(r_1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:rotateBy script")));
					return false;
				}
				MQuaternion		*l_q1;
				MEulerRotation	*l_r1;
				MVector			*l_v1, *l_v2,*l_v3,*l_v4,*l_v5;
				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_3", &l_v3);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_4", &l_v4);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_5", &l_v5);
				l_bool = l_bool && vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:rotateBy get globals.")));
					return false;
				}
				if (*l_v2 != l_v1->rotateBy(0.1, 0.1, -0.1, 0.9) ||
					*l_v3 != l_v1->rotateBy(MVector::kXaxis, 60) ||
					*l_v4 != l_v1->rotateBy(*l_q1) ||
					*l_v5 != l_v1->rotateBy(*l_r1) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation:setValue failed")));
					return false;
				}
			}
			// MVector rotateTo
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("v_2 = MVector.new(-1,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("q_1 = v_1:rotateTo(v_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:rotateTo script")));
					return false;
				}
				MQuaternion* l_q1;
				MVector* l_v1, * l_v2;
				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool &&  vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:rotateTo get globals.")));
					return false;
				}
				if (*l_q1 != l_v1->rotateTo(*l_v2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:rotateTo failed")));
					return false;
				}
			}
			// length
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("d = v_1:length()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:length script")));
					return false;
				}
				MVector* l_v1;
				l_bool = vufLuaMVector::get_global(L, "v_1", &l_v1);
				lua_getglobal(L, "d"); 
				double l_lngth = lua_tonumber(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:length get globals.")));
					return false;
				}
				if (l_lngth != l_v1->length())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:length failed")));
					return false;
				}
			}
			// noraml
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("v_2 = v_1:normal()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:normal script")));
					return false;
				}
				MVector* l_v1, *l_v2;
				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:normal get globals.")));
					return false;
				}
				if (*l_v2 != l_v1->normal())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:normal failed")));
					return false;
				}
			}
			// normalize
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("b_1 = v_1:normalize()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:normalize script")));
					return false;
				}
				MVector* l_v1;
				l_bool = vufLuaMVector::get_global(L, "v_1", &l_v1);
				lua_getglobal(L, "b_1");
				int l_b = (int)lua_toboolean(L, -1);
				if (l_b != (int)(MVector(0.5, 0.6, 0.7).normalize() == MS::kSuccess) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:normalize failed")));
					return false;
				}
			}
			// isEquivalent
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("v_2 = MVector.new(0.2,0.4,0.8)");
				l_bool = l_bool &&	m_w.do_string("b_1 = v_1:isEquivalent(v_2)");
				l_bool = l_bool &&	m_w.do_string("b_2 = v_1:isEquivalent(v_2,5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:isEquivalent script")));
					return false;
				}
				MVector* l_v1, * l_v2;
				int l_b1, l_b2;
				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				lua_getglobal(L, "b_1");
				l_b1 = lua_toboolean(L, -1);
				lua_getglobal(L, "b_2");
				l_b2 = lua_toboolean(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:isEquivalent get globals.")));
					return false;
				}
				if (l_b1 != (int)l_v1->isEquivalent(*l_v2) ||
					l_b2 != (int)l_v1->isEquivalent(*l_v2, 5))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:isEquivalent failed")));
					return false;
				}
			}
			// isParallel
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("v_2 = MVector.new(0.2,0.4,0.8)");
				l_bool = l_bool &&	m_w.do_string("b_1 = v_1:isParallel(v_2)");
				l_bool = l_bool &&	m_w.do_string("b_2 = v_1:isParallel(v_2,5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:isParallel script")));
					return false;
				}
				MVector* l_v1, * l_v2;
				int l_b1, l_b2;
				l_bool = vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_2", &l_v2);
				lua_getglobal(L, "b_1");
				l_b1 = lua_toboolean(L, -1);
				lua_getglobal(L, "b_2");
				l_b2 = lua_toboolean(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:isParallel get globals.")));
					return false;
				}
				if (l_b1 != (int)l_v1->isParallel(*l_v2) ||
					l_b2 != (int)l_v1->isParallel(*l_v2, 5))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:isParallel failed")));
					return false;
				}
			}
			// transformAsNormal
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&	m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("v_2 = v_1:transformAsNormal(m_1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed (MVector:transformAsNormal) script")));
					return false;
				}
				MMatrix* l_m;
				MVector* l_v1,*l_v2;
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool && vufLuaMMatrix::get_global(L, "m_1", &l_m);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed (MVector:transformAsNormal) get_global")));
					return false;
				}
				if (*l_v2 != l_v1->transformAsNormal(*l_m))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string(" Failed (MVector:transformAsNormal).")));
					return false;
				}
			}
			// add
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(60,20,30)");
				l_bool = l_bool &&	m_w.do_string("v_2 = MVector.new(10,40,60)");
				l_bool = l_bool &&	m_w.do_string("p_1 = MPoint.new(0.1,0.2,0.3,1)");
				l_bool = l_bool &&	m_w.do_string("v_3 = v_1 + v_2");
				l_bool = l_bool &&	m_w.do_string("p_2 = v_1 + p_1");
				l_bool = l_bool &&	m_w.do_string("p_3 = p_1 + v_1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:add script")));
					return false;
				}
				MVector* l_v1,*l_v2,*l_v3;
				MPoint*  l_p1, *l_p2, *l_p3;
				l_bool =			vufLuaMPoint::get_global(L, "p_1", &l_p1);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_3", &l_p3);

				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_3", &l_v3);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:add get globals.")));
					return false;
				}
				if (*l_v3 != (*l_v1) + (*l_v2) || *l_p2 != (*l_v1) + (*l_p1) || *l_p3 != (*l_p1) + (*l_v1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:add failed")));
					return false;
				}
			}
			// sub
			{
				l_bool = m_w.do_string("v_1 = MVector.new(60,20,30)");
				l_bool = l_bool && m_w.do_string("v_2 = MVector.new(10,40,60)");
				l_bool = l_bool && m_w.do_string("p_1 = MPoint.new(0.1,0.2,0.3,1)");
				l_bool = l_bool && m_w.do_string("v_3 = v_1 - v_2");
				l_bool = l_bool && m_w.do_string("p_2 = v_1 - p_1");
				l_bool = l_bool && m_w.do_string("p_3 = p_1 - v_1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:sub script")));
					return false;
				}
				MVector* l_v1, * l_v2, * l_v3;
				MPoint* l_p1, * l_p2, * l_p3;
				l_bool = vufLuaMPoint::get_global(L, "p_1", &l_p1);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_3", &l_p3);

				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_3", &l_v3);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:sub get globals.")));
					return false;
				}
				if (*l_v3 != (*l_v1) - (*l_v2) || *l_p2 != (*l_v1) - (*l_p1) || *l_p3 != (*l_p1) - (*l_v1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:sub failed")));
					return false;
				}
			}
			// mul
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(60,20,30)");
				l_bool = l_bool &&	m_w.do_string("v_2 = MVector.new(10,40,60)");
				l_bool = l_bool &&	m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("v_3 = v_1 * 2");
				l_bool = l_bool &&	m_w.do_string("v_4 =  2 * v_1");
				l_bool = l_bool &&	m_w.do_string("v_5 = v_1 * m_1");
				l_bool = l_bool &&	m_w.do_string("v_6 = m_1 * v_1");
				l_bool = l_bool &&	m_w.do_string("d = v_1 * v_2");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:mul script")));
					return false;
				}
				MMatrix* l_m1;
				MVector *l_v1, *l_v2, *l_v3, *l_v4, *l_v5, *l_v6;
				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_3", &l_v3);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_4", &l_v4);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_5", &l_v5);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_6", &l_v6);
				l_bool = l_bool && vufLuaMMatrix::get_global(L, "m_1", &l_m1);
				lua_getglobal(L, "d");
				double l_d = (double)luaL_checknumber(L, -1);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:mul get globals.")));
					return false;
				}
				if (*l_v3 != (*l_v1) * 2 || *l_v4 != 2 * (*l_v1) || *l_v5 != (*l_v1) * (*l_m1) || *l_v6 != (*l_m1) * (*l_v1) ||l_d != (*l_v1)*(*l_v2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:mul failed")));
					return false;
				}
			}
			// indecies
			{
				l_bool =			m_w.do_string("V_1 = MVector.new(0.1,0.2,0.3)");
				l_bool = l_bool &&	m_w.do_string("V_2 = MVector.new(0.6,0.1,-0.3)");
				l_bool = l_bool &&	m_w.do_string("x = v_1.x");
				l_bool = l_bool &&	m_w.do_string("y = v_1.y");
				l_bool = l_bool &&	m_w.do_string("z = v_1.z");
				l_bool = l_bool &&	m_w.do_string("v_2.x = 0.9");
				l_bool = l_bool &&	m_w.do_string("v_2.y = 0.8");
				l_bool = l_bool &&	m_w.do_string("v_2.z = 0.1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:indecies script")));
					return false;
				}
				MVector* l_v1, * l_v2;
				vufLuaMVector::get_global(L, "v_1", &l_v1);
				vufLuaMVector::get_global(L, "v_2", &l_v2);
				double l_x, l_y, l_z;
				lua_getglobal(L, "x"); l_x = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "y"); l_y = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "z"); l_z = (double)lua_tonumber(L, -1);
				const char* l_char = lua_tostring(L, -1);
				std::string l_str(l_char);

				if (l_x != l_v1->x || l_y != l_v1->y || l_z != l_v1->z ||
					*l_v2 != MVector(0.9, 0.8, 0.1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVector:indecies failed")));
					return false;
				}
			}

			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_VECTOR PASSED.")));
			return true;
		}
		bool MPoint_ut()
		{
			bool l_bool = false;
			lua_State* L = m_w.get_lua_state();
			// constructor
			{
				l_bool =			m_w.do_string("v_1 = MVector.new(0.5,0.6,0.7)");
				l_bool = l_bool &&  m_w.do_string("p_1 = MPoint.new(0.5,0.2,-0.7,1)");
				l_bool = l_bool &&  m_w.do_string("p_2 = MPoint.new(v_1)");
				l_bool = l_bool &&  m_w.do_string("p_3 = MPoint.new()");

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint constructor script")));
					return false;
				}
				MVector* l_v1;;
				MPoint* l_p1,*l_p2,*l_p3;
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_1", &l_p1);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_3", &l_p3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint constructor get_global")));
					return false;
				}
				if (*l_p3 != MPoint() || *l_p1 != MPoint(0.5, 0.2, -0.7, 1) || *l_p2 != MPoint(*l_v1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint constructor(MPoint).")));
					return false;
				}
			}
			// isEquivalent
			{
				l_bool =			m_w.do_string("p_1 = MPoint.new(0.5,0.6,0.7,1)");
				l_bool = l_bool &&	m_w.do_string("v_2 = MPoint.new(0.2,0.4,0.8,1)");
				l_bool = l_bool &&	m_w.do_string("b_1 = p_1:isEquivalent(p_2)");
				l_bool = l_bool &&	m_w.do_string("b_2 = p_1:isEquivalent(p_2,5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:isEquivalent script")));
					return false;
				}
				MPoint* l_p1, * l_p2;
				int l_b1, l_b2;
				l_bool =			vufLuaMPoint::get_global(L, "p_1", &l_p1);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_2", &l_p2);
				lua_getglobal(L, "b_1");
				l_b1 = lua_toboolean(L, -1);
				lua_getglobal(L, "b_2");
				l_b2 = lua_toboolean(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:isEquivalent get globals.")));
					return false;
				}
				if (l_b1 != (int)l_p1->isEquivalent(*l_p2) ||
					l_b2 != (int)l_p1->isEquivalent(*l_p2, 5))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:isEquivalent failed")));
					return false;
				}
			}
			// add
			{
				l_bool =			m_w.do_string("p_1 = MPoint.new(60,20,30,1)");
				l_bool = l_bool &&	m_w.do_string("p_2 = MPoint.new(10,40,60,1)");
				l_bool = l_bool &&	m_w.do_string("v_1 = MVector.new()");
				l_bool = l_bool &&	m_w.do_string("p_3 = p_1 + p_2");
				l_bool = l_bool &&	m_w.do_string("p_4 = v_1 + p_1");
				l_bool = l_bool &&	m_w.do_string("p_5 = p_1 + v_1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:add script")));
					return false;
				}
				MVector* l_v1;
				MPoint* l_p1, * l_p2, * l_p3, * l_p4, * l_p5;
				l_bool = vufLuaMPoint::get_global(L, "p_1", &l_p1);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_3", &l_p3);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_4", &l_p4);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_5", &l_p5);
				l_bool = l_bool && vufLuaMVector::get_global(L,"v_1", &l_v1);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:add get globals.")));
					return false;
				}
				if ( *l_p3 != (*l_p1) +(*l_p2) || *l_p4 != (*l_v1) + (*l_p1) || *l_p5 != (*l_p1) + (*l_v1) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:add failed")));
					return false;
				}
			}
			// sub
			{
				l_bool =			m_w.do_string("p_1 = MPoint.new(60,20,30,1)");
				l_bool = l_bool &&	m_w.do_string("p_2 = MPoint.new(10,40,60,1)");
				l_bool = l_bool &&	m_w.do_string("v_1 = MVector.new()");
				l_bool = l_bool &&	m_w.do_string("p_3 = p_1 - p_2");
				l_bool = l_bool &&	m_w.do_string("p_4 = v_1 - p_1");
				l_bool = l_bool &&	m_w.do_string("p_5 = p_1 - v_1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:sub script")));
					return false;
				}
				MVector* l_v1;
				MPoint* l_p1, * l_p2, * l_p3, * l_p4, * l_p5;
				l_bool = vufLuaMPoint::get_global(L, "p_1", &l_p1);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_3", &l_p3);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_4", &l_p4);
				l_bool = l_bool && vufLuaMPoint::get_global(L, "p_5", &l_p5);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v1);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:sub get globals.")));
					return false;
				}
				if (*l_p3 != (*l_p1) - (*l_p2) || *l_p4 != (*l_v1) - (*l_p1) || *l_p5 != (*l_p1) - (*l_v1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:sub failed")));
					return false;
				}
			}
			// mul
			{
				l_bool =			m_w.do_string("p_1 = MPoint.new(60,20,30,1)");
				l_bool = l_bool &&	m_w.do_string("p_2 = MPoint.new(10,40,60,1)");
				l_bool = l_bool &&	m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("p_3 = p_1 * 2");
				l_bool = l_bool &&	m_w.do_string("p_4 =  2 * p_1");
				l_bool = l_bool &&	m_w.do_string("p_5 = p_1 * m_1");
				l_bool = l_bool &&	m_w.do_string("p_6 = m_1 * p_1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:mul script")));
					return false;
				}
				MMatrix* l_m1;
				MPoint* l_p1, * l_p2, * l_p3, * l_p4, * l_p5, * l_p6;
				l_bool =			vufLuaMPoint::get_global(L, "p_1", &l_p1);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_2", &l_p2);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_3", &l_p3);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_4", &l_p4);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_5", &l_p5);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "p_6", &l_p6);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_1", &l_m1);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:mul get globals.")));
					return false;
				}
				if (*l_p3 != (*l_p1) * 2 || *l_p4 != 2 * (*l_p1) || *l_p5 != (*l_p1) * (*l_m1) || *l_p6 != (*l_m1) * (*l_p1) )
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:mul failed")));
					return false;
				}
			}
			// indecies
			{
				l_bool =			m_w.do_string("p_1 = MPoint.new(0.1,0.2,0.3,0.9)");
				l_bool = l_bool &&	m_w.do_string("p_2 = MPoint.new(0.6,0.1,-0.3, 0.9)");
				l_bool = l_bool &&	m_w.do_string("x = p_1.x");
				l_bool = l_bool &&	m_w.do_string("y = p_1.y");
				l_bool = l_bool &&	m_w.do_string("z = p_1.z");
				l_bool = l_bool &&	m_w.do_string("w = p_1.w");
				l_bool = l_bool &&	m_w.do_string("p_2.x = 0.9");
				l_bool = l_bool &&	m_w.do_string("p_2.y = 0.8");
				l_bool = l_bool &&	m_w.do_string("p_2.z = 0.1");
				l_bool = l_bool &&	m_w.do_string("p_2.w = 0.2");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:indecies script")));
					return false;
				}
				MPoint* l_p1, * l_p2;
				vufLuaMPoint::get_global(L, "p_1", &l_p1);
				vufLuaMPoint::get_global(L, "p_2", &l_p2);
				double l_x, l_y, l_z, l_w;
				lua_getglobal(L, "x"); l_x = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "y"); l_y = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "z"); l_z = (double)lua_tonumber(L, -1);
				lua_getglobal(L, "w"); l_w = (double)lua_tonumber(L, -1);
				const char* l_char = lua_tostring(L, -1);
				std::string l_str(l_char);

				if (l_x != l_p1->x || l_y != l_p1->y || l_z != l_p1->z || l_w != l_p1->w ||
					*l_p2 != MPoint(0.9, 0.8, 0.1, 0.2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPoint:indecies failed")));
					return false;
				}
			}


			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_POINT PASSED.")));
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
				l_bool = l_bool &&	m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&  m_w.do_string("r_1 = MEulerRotation.new(10,20,30)");
				l_bool = l_bool &&	m_w.do_string("v_2 = MVector.new(0.4,0.1,-0.4)");
				l_bool = l_bool &&	m_w.do_string("q_3 = MQuaternion.new(v_1,v_2)");
				l_bool = l_bool &&	m_w.do_string("q_4 = MQuaternion.new(v_1,v_2,60)");
				l_bool = l_bool &&	m_w.do_string("q_5 = MQuaternion.new(60,v_1)");
				l_bool = l_bool &&	m_w.do_string("q_6 = MQuaternion.new(m_1)");
				l_bool = l_bool &&	m_w.do_string("q_7 = MQuaternion.new(r_1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:new) script")));
					return false;
				}
				MVector *l_v1, *l_v2;
				MQuaternion *l_q1, *l_q2, *l_q3, *l_q4, *l_q5, *l_q6, *l_q7;
				MMatrix* l_m;
				MEulerRotation *l_r;
				l_bool =			vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_1", &l_q1);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_2", &l_q2);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_3", &l_q3);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_4", &l_q4);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_5", &l_q5);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_6", &l_q6);
				l_bool = l_bool &&	vufLuaMQuaternion::get_global(L, "q_7", &l_q7);
				l_bool = l_bool &&  vufLuaMMatrix::get_global(L, "m_1", &l_m);
				l_bool = l_bool &&  vufLuaMEulerRotation::get_global(L, "r_1", &l_r);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed (MQuaternion:new) get globals.")));
					return false;
				}
				MQuaternion l_qm, l_qr;
				l_qm = *l_m;
				l_qr = *l_r;

				if (*l_q1 != MQuaternion() || *l_q2 != MQuaternion(0.2, 0.1, 0.6, 0.9) ||
					*l_q3 != MQuaternion(*l_v1, *l_v2) || *l_q4 != MQuaternion(*l_v1, *l_v2, 60) ||
					*l_q5 != MQuaternion(60, *l_v1) ||
					l_qm != *l_q6 || l_qr != *l_q7 )
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
				l_bool =			m_w.do_string("r_1 = MEulerRotation.new()");
				l_bool = l_bool &&	m_w.do_string("r_2 = MEulerRotation.new(30,60,90)");
				l_bool = l_bool &&	m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("q_1 = MQuaternion.new(0.1,0.2,0.3,0.9)");
				l_bool = l_bool &&	m_w.do_string("r_3 = MEulerRotation.new(10,11,12,\"xzy\")");
				l_bool = l_bool &&	m_w.do_string("v_1 = MVector.new(90,30,60)");
				l_bool = l_bool &&	m_w.do_string("r_4 = MEulerRotation.new(v_1)");
				l_bool = l_bool &&	m_w.do_string("r_5 = MEulerRotation.new(v_1,\"zyx\")");
				l_bool = l_bool &&	m_w.do_string("r_6 = MEulerRotation.new(m_1)");
				l_bool = l_bool &&	m_w.do_string("r_7 = MEulerRotation.new(q_1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation constructor script")));
					return false;
				}
				MEulerRotation* l_r1, * l_r2, * l_r3, * l_r4, * l_r5, *l_r6, *l_r7;
				MVector* l_v1;
				MMatrix* l_m;
				MQuaternion* l_q;
				l_bool =			vufLuaMEulerRotation::get_global(L, "r_1", &l_r1);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_2", &l_r2);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_3", &l_r3);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_4", &l_r4);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_5", &l_r5);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_6", &l_r6);
				l_bool = l_bool &&	vufLuaMEulerRotation::get_global(L, "r_7", &l_r7);

				l_bool = l_bool && vufLuaMMatrix::get_global(L, "m_1", &l_m);
				l_bool = l_bool && vufLuaMQuaternion::get_global(L, "q_1", &l_q);

				MEulerRotation l_rm, l_rq;
				l_rm = *l_m;
				l_rq = *l_q;

				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_1", &l_v1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MEulerRotation constructor get globals.")));
					return false;
				}
				if (*l_r1 != MEulerRotation() ||
					*l_r2 != MEulerRotation(30, 60, 90) ||
					*l_r3 != MEulerRotation(10, 11, 12, MEulerRotation::kXZY) ||
					*l_r4 != MEulerRotation(*l_v1) ||
					*l_r5 != MEulerRotation(*l_v1, MEulerRotation::kZYX) ||
					*l_r6 != l_rm || *l_r7 != l_rq )
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
		bool MMatrix_ut()
		{
			bool l_bool = false;
			lua_State* L = m_w.get_lua_state();
			// MMatrix constructor
			{
				l_bool =			m_w.do_string("m_1 = MMatrix.new()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix constructor script")));
					return false;
				}
				MMatrix* l_m;
				l_bool = l_bool && vufLuaMMatrix::get_global(L, "m_1", &l_m);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix constructor get_global")));
					return false;
				}
				if (*l_m != MMatrix())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix constructor.")));
					return false;
				}
			}
			// copy
			{
				l_bool =			m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_2 = m_1:copy()");
				l_bool = l_bool &&	m_w.do_string("m_1:set(0,0,0)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:copy script")));
					return false;
				}
				MMatrix* l_m1, * l_m2;
				l_bool =			vufLuaMMatrix::get_global(L, "m_1", &l_m1);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_2", &l_m2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:copy get globals.")));
					return false;
				}
				if (*l_m1 == *l_m2 || *l_m2 != MMatrix())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:copy failed")));
					return false;
				}
			}
			// get set
			{
				l_bool =			m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_1:set(1,2,10)");
				l_bool = l_bool &&	m_w.do_string("d = m_1:get(1,2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:(get,set) script")));
					return false;
				}
				MMatrix* l_m1;
				l_bool =	vufLuaMMatrix::get_global(L, "m_1", &l_m1);
				lua_getglobal(L, "d");
				double l_val = (double)lua_tonumber(L, -1);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:(get,set) get globals.")));
					return false;
				}
				if (l_val != 10.)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:(get,set) failed")));
					return false;
				}
			}
			// setToProduct 
			{
				l_bool =			m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_2 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_1:set(1,1,10)");
				l_bool = l_bool &&	m_w.do_string("m_2:set(1,1,20)");
				l_bool = l_bool &&	m_w.do_string("m_3 = m_1:copy()");
				l_bool = l_bool &&	m_w.do_string("m_4 = m_3:setToProduct(m_1,m_2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:setToProduct script")));
					return false;
				}
				MMatrix *l_m1,*l_m2,*l_m3,*l_m4;
				l_bool =			vufLuaMMatrix::get_global(L, "m_1", &l_m1);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_2", &l_m2);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_3", &l_m3);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_4", &l_m4);

				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:setToProduct get globals.")));
					return false;
				}
				if (  *l_m3 != *l_m4 ||  *l_m4 != l_m1->setToProduct(*l_m1,*l_m2))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:setToProduct failed")));
					return false;
				}
			}
			// isEquivalent
			{
				l_bool =			m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_2 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_1:set(0,0,10)");
				l_bool = l_bool &&	m_w.do_string("m_2:set(0,0,11)");
				l_bool = l_bool &&	m_w.do_string("b_1 = m_1:isEquivalent(m_2)");
				l_bool = l_bool &&	m_w.do_string("b_2 = m_1:isEquivalent(m_2,5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:isEquivalent script")));
					return false;
				}
				MMatrix* l_m1, * l_m2;
				int l_b1, l_b2;
				l_bool =			vufLuaMMatrix::get_global(L, "m_1", &l_m1);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_2", &l_m2);
				lua_getglobal(L, "b_1");
				l_b1 = lua_toboolean(L, -1);
				lua_getglobal(L, "b_2");
				l_b2 = lua_toboolean(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:isEquivalent get globals.")));
					return false;
				}
				if (l_b1 != (int)l_m1->isEquivalent(*l_m2) ||
					l_b2 != (int)l_m1->isEquivalent(*l_m2, 5))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:isEquivalent failed")));
					return false;
				}
			}
			// isSingular
			{
				l_bool =			m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_2 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_1:set(0,0,0)");
				l_bool = l_bool &&	m_w.do_string("b_1 = m_1:isSingular()");
				l_bool = l_bool &&	m_w.do_string("b_2 = m_2:isSingular()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:isSingular script")));
					return false;
				}
				MMatrix* l_m1, * l_m2;
				int l_b1, l_b2;
				l_bool = vufLuaMMatrix::get_global(L, "m_1", &l_m1);
				l_bool = l_bool && vufLuaMMatrix::get_global(L, "m_2", &l_m2);
				lua_getglobal(L, "b_1");
				l_b1 = lua_toboolean(L, -1);
				lua_getglobal(L, "b_2");
				l_b2 = lua_toboolean(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:isSingular get globals.")));
					return false;
				}
				if (l_b1 != (int)l_m1->isSingular() ||
					l_b2 != (int)l_m2->isSingular())
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:isSingular failed")));
					return false;
				}
			}
			// mul
			{
				l_bool =			m_w.do_string("m_1 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_2 = MMatrix.new()");
				l_bool = l_bool &&	m_w.do_string("m_1:set(0,0,2)");
				l_bool = l_bool &&	m_w.do_string("m_2:set(0,0,3)");
				l_bool = l_bool &&	m_w.do_string("v_1 = MVector.new(0.2,0.1,0.3)");
				l_bool = l_bool &&	m_w.do_string("p_1 = MPoint.new(0.1,0.2,0.3,1)");
				l_bool = l_bool &&	m_w.do_string("m_3 = m_1 * 2");
				l_bool = l_bool &&	m_w.do_string("m_4 = 2 * m_1");
				l_bool = l_bool &&	m_w.do_string("m_5 = m_1 * m_2");
				l_bool = l_bool &&	m_w.do_string("p_2 = m_1 * p_1");
				l_bool = l_bool &&	m_w.do_string("p_3 = p_1 * m_1");
				l_bool = l_bool &&	m_w.do_string("v_2 = v_1 * m_1");
				l_bool = l_bool &&	m_w.do_string("v_3 = m_1 * v_1");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:mul script")));
					return false;
				}
				MMatrix *l_m1, *l_m2, *l_m3, *l_m4, *l_m5;
				MPoint *l_p1,*l_p2,*l_p3;
				MVector *l_v1,*l_v2,*l_v3;
				l_bool =			vufLuaMMatrix::get_global(L, "m_1", &l_m1);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_2", &l_m2);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_3", &l_m3);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_4", &l_m4);
				l_bool = l_bool &&	vufLuaMMatrix::get_global(L, "m_5", &l_m5);
				l_bool = l_bool &&	vufLuaMPoint::get_global( L, "p_1", &l_p1);
				l_bool = l_bool &&	vufLuaMPoint::get_global( L, "p_2", &l_p2);
				l_bool = l_bool &&	vufLuaMPoint::get_global( L, "p_3", &l_p3);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_2", &l_v2);
				l_bool = l_bool &&	vufLuaMVector::get_global(L, "v_3", &l_v3);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:mul get global")));
					return false;
				}
				if (*l_m3 != (*l_m1) * 2 || *l_m4 != 2 * (*l_m1) || *l_m5 != (*l_m1) * (*l_m2) ||
					*l_p2 != (*l_m1) * (*l_p1) || *l_p3 != (*l_p1) * (*l_m1) ||
					*l_v2 != (*l_v1) * (*l_m1) || *l_v3 != (*l_m1) * (*l_v1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MMatrix:mul")));
					return false;
				}
			}
			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_MATRIX PASSED.")));
			return true;
		}
		bool MVectorArray_ut()
		{
			bool l_bool = false;
			lua_State* L = m_w.get_lua_state();
			// constructor
			{
				MVectorArray l_arr;
				vufLuaMVectorArray::set_global_ref(L, "a_ref", &l_arr);
				l_bool = m_w.do_string("a_1 = MVectorArray.new()");
				l_bool = l_bool && m_w.do_string("a_1:setLength(10)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray constructor script")));
					return false;
				}
				MVectorArray* l_a1, * l_a2;
				l_bool = vufLuaMVectorArray::get_global(L, "a_ref", &l_a1);
				l_bool = l_bool && vufLuaMVectorArray::get_global(L, "a_1", &l_a2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:new get globals.")));
					return false;
				}
				if (&l_arr != l_a1 || l_a2->length() != 10)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:new failed")));
					return false;
				}
			}
			// set get
			{
				MVectorArray l_arr;
				vufLuaMVectorArray::set_global_ref(L, "a_ref", &l_arr);
				l_bool = m_w.do_string("a_1 = MVectorArray.new()");
				l_bool = l_bool && m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool && m_w.do_string("a_ref:setLength(10)");
				l_bool = l_bool && m_w.do_string("v_1 = MVector.new(10,20,30)");
				l_bool = l_bool && m_w.do_string("a_1:set(0,v_1)");
				l_bool = l_bool && m_w.do_string("v_2 = a_1:get(0)");
				l_bool = l_bool && m_w.do_string("a_ref:set(0,v_1 + v_1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:get/set script")));
					return false;
				}
				MVectorArray* l_a1, * l_ar;
				MVector* l_v1, * l_v2;
				l_bool = vufLuaMVectorArray::get_global(L, "a_ref", &l_ar);
				l_bool = l_bool && vufLuaMVectorArray::get_global(L, "a_1", &l_a1);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_1", &l_v1);
				l_bool = l_bool && vufLuaMVector::get_global(L, "v_2", &l_v2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:get/set get globals.")));
					return false;
				}
				if ((*l_a1)[0] != *l_v1 || *l_v1 != *l_v2 || l_arr[0] != (*l_v1) + (*l_v1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:get/set failed")));
					return false;
				}
			}
			// remove
			{
				l_bool =			m_w.do_string("a_1 = MVectorArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:set(3,MVector.new(10,20,30))");
				l_bool = l_bool &&	m_w.do_string("a_1:remove(3)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:remove script")));
					return false;
				}
				MVectorArray *l_a1;
				l_bool = vufLuaMVectorArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:remove get global")));
					return false;
				}
				if (l_a1->length() != 9 || (*l_a1)[3] == MVector(10, 20, 30))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:remove")));
					return false;
				}
			}
			// insert
			{
				l_bool =			m_w.do_string("a_1 = MVectorArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:insert(3,MVector.new(10,20,30))");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:insert script")));
					return false;
				}
				MVectorArray* l_a1;
				l_bool = vufLuaMVectorArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:insert get global")));
					return false;
				}
				if (l_a1->length() != 11 || (*l_a1)[3] != MVector(10, 20, 30))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray insert")));
					return false;
				}
			}
			// append
			{
				l_bool =			m_w.do_string("a_1 = MVectorArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:append(MVector.new(10,20,30))");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:append script")));
					return false;
				}
				MVectorArray* l_a1;
				l_bool = vufLuaMVectorArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray:append get global")));
					return false;
				}
				if (l_a1->length() != 11 || (*l_a1)[10] != MVector(10, 20, 30))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MVectorArray append")));
					return false;
				}
			}
			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_MVECTOR_ARRAY PASSED.")));
			return true;
		}
		bool MPointArray_ut()
		{
			bool l_bool = false;
			lua_State* L = m_w.get_lua_state();
			// constructor
			{
				MPointArray l_arr;
				vufLuaMPointArray::set_global_ref(L, "a_ref", &l_arr);
				l_bool =			m_w.do_string("a_1 = MPointArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray constructor script")));
					return false;
				}
				MPointArray* l_a1, * l_a2;
				l_bool =			vufLuaMPointArray::get_global(L, "a_ref", &l_a1);
				l_bool = l_bool &&  vufLuaMPointArray::get_global(L, "a_1", &l_a2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:new get globals.")));
					return false;
				}
				if (&l_arr != l_a1 || l_a2->length() != 10)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:new failed")));
					return false;
				}
			}
			// set get
			{
				MPointArray l_arr;
				vufLuaMPointArray::set_global_ref(L, "a_ref", &l_arr);
				l_bool =			m_w.do_string("a_1 = MPointArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_ref:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("v_1 = MPoint.new(10,20,30,1)");
				l_bool = l_bool &&	m_w.do_string("a_1:set(0,v_1)");
				l_bool = l_bool &&	m_w.do_string("v_2 = a_1:get(0)");
				l_bool = l_bool &&	m_w.do_string("a_ref:set(0,v_1 + v_1)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:get/set script")));
					return false;
				}
				MPointArray* l_a1, * l_ar;
				MPoint* l_v1, * l_v2;
				l_bool =			vufLuaMPointArray::get_global(L, "a_ref", &l_ar);
				l_bool = l_bool &&	vufLuaMPointArray::get_global(L, "a_1", &l_a1);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "v_1", &l_v1);
				l_bool = l_bool &&	vufLuaMPoint::get_global(L, "v_2", &l_v2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:get/set get globals.")));
					return false;
				}
				if ((*l_a1)[0] != *l_v1 || *l_v1 != *l_v2 || l_arr[0] != (*l_v1) + (*l_v1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:get/set failed")));
					return false;
				}
			}
			// remove
			{
				l_bool = m_w.do_string("a_1 = MPointArray.new()");
				l_bool = l_bool && m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool && m_w.do_string("a_1:set(3,MPoint.new(10,20,30,1))");
				l_bool = l_bool && m_w.do_string("a_1:remove(3)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:remove script")));
					return false;
				}
				MPointArray* l_a1;
				l_bool = vufLuaMPointArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:remove get global")));
					return false;
				}
				if (l_a1->length() != 9 || (*l_a1)[3] == MPoint(10, 20, 30,1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:remove")));
					return false;
				}
			}
			// insert
			{
				l_bool =	 m_w.do_string("a_1 = MPointArray.new()");
				l_bool = l_bool && m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool && m_w.do_string("a_1:insert(3,MPoint.new(10,20,30,1))");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:insert script")));
					return false;
				}
				MPointArray* l_a1;
				l_bool = vufLuaMPointArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:insert get global")));
					return false;
				}
				if (l_a1->length() != 11 || (*l_a1)[3] != MPoint(10, 20, 30,1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray insert")));
					return false;
				}
			}
			// append
			{
				l_bool = m_w.do_string("a_1 = MPointArray.new()");
				l_bool = l_bool && m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool && m_w.do_string("a_1:append(MPoint.new(10,20,30,1))");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:append script")));
					return false;
				}
				MPointArray* l_a1;
				l_bool = vufLuaMPointArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray:append get global")));
					return false;
				}
				if (l_a1->length() != 11 || (*l_a1)[10] != MPoint(10, 20, 30,1))
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MPointArray append")));
					return false;
				}
			}
			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_MPOINT_ARRAY PASSED.")));
			return true;
		}
		bool MIntArray_ut()
		{
			bool l_bool = false;
			lua_State* L = m_w.get_lua_state();
			// constructor
			{
				MIntArray l_arr;
				vufLuaMIntArray::set_global_ref(L, "a_ref", &l_arr);
				l_bool =			m_w.do_string("a_1 = MIntArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray constructor script")));
					return false;
				}
				MIntArray* l_a1, * l_a2;
				l_bool =			vufLuaMIntArray::get_global(L, "a_ref", &l_a1);
				l_bool = l_bool &&	vufLuaMIntArray::get_global(L, "a_1", &l_a2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:new get globals.")));
					return false;
				}
				if (&l_arr != l_a1 || l_a2->length() != 10)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:new failed")));
					return false;
				}
			}
			// set get
			{
				MIntArray l_arr;
				vufLuaMIntArray::set_global_ref(L, "a_ref", &l_arr);
				l_bool =			m_w.do_string("a_1 = MIntArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_ref:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:set(0,10)");
				l_bool = l_bool &&	m_w.do_string("d = a_1:get(0)");
				l_bool = l_bool &&	m_w.do_string("a_ref:set(0,20)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:get/set script")));
					return false;
				}
				MIntArray* l_a1, * l_ar;
				int l_d;
				l_bool =			vufLuaMIntArray::get_global(L, "a_ref", &l_ar);
				l_bool = l_bool &&	vufLuaMIntArray::get_global(L, "a_1", &l_a1);
				lua_getglobal(L, "d");
				l_d = (int)luaL_checkinteger(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:get/set get globals.")));
					return false;
				}
				if ((*l_a1)[0] != 10 || l_d != 10 || l_arr[0] != 20)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:get/set failed")));
					return false;
				}
			}
			// remove
			{
				l_bool =			m_w.do_string("a_1 = MIntArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:set(3,15)");
				l_bool = l_bool &&	m_w.do_string("a_1:remove(3)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:remove script")));
					return false;
				}
				MIntArray* l_a1;
				l_bool = vufLuaMIntArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:remove get global")));
					return false;
				}
				if (l_a1->length() != 9 || (*l_a1)[3] == 15)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:remove")));
					return false;
				}
			}
			// insert
			{
				l_bool =			m_w.do_string("a_1 = MIntArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:insert(3,15)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:insert script")));
					return false;
				}
				MIntArray* l_a1;
				l_bool = vufLuaMIntArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:insert get global")));
					return false;
				}
				if (l_a1->length() != 11 || (*l_a1)[3] != 15)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray insert")));
					return false;
				}
			}
			// append
			{
				l_bool =			m_w.do_string("a_1 = MIntArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:append(30)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:append script")));
					return false;
				}
				MIntArray* l_a1;
				l_bool = vufLuaMIntArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray:append get global")));
					return false;
				}
				if (l_a1->length() != 11 || (*l_a1)[10] != 30)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MIntArray append")));
					return false;
				}
			}
			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_MINT_ARRAY PASSED.")));
			return true;
		}
		bool MDoubleArray_ut()
		{
			bool l_bool = false;
			lua_State* L = m_w.get_lua_state();
			// constructor
			{
				MDoubleArray l_arr;
				vufLuaMDoubleArray::set_global_ref(L, "a_ref", &l_arr);
				l_bool =			m_w.do_string("a_1 = MDoubleArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray constructor script")));
					return false;
				}
				MDoubleArray* l_a1, * l_a2;
				l_bool =			vufLuaMDoubleArray::get_global(L, "a_ref", &l_a1);
				l_bool = l_bool &&	vufLuaMDoubleArray::get_global(L, "a_1", &l_a2);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:new get globals.")));
					return false;
				}
				if (&l_arr != l_a1 || l_a2->length() != 10)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:new failed")));
					return false;
				}
			}
			// set get
			{
				MDoubleArray l_arr;
				vufLuaMDoubleArray::set_global_ref(L, "a_ref", &l_arr);
				l_bool =			m_w.do_string("a_1 = MDoubleArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_ref:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:set(0,10.5)");
				l_bool = l_bool &&	m_w.do_string("d = a_1:get(0)");
				l_bool = l_bool &&	m_w.do_string("a_ref:set(0,20.5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:get/set script")));
					return false;
				}
				MDoubleArray* l_a1, * l_ar;
				double l_d;
				l_bool =			vufLuaMDoubleArray::get_global(L, "a_ref", &l_ar);
				l_bool = l_bool &&	vufLuaMDoubleArray::get_global(L, "a_1", &l_a1);
				lua_getglobal(L, "d");
				l_d = (double)luaL_checknumber(L, -1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:get/set get globals.")));
					return false;
				}
				if ((*l_a1)[0] != 10.5 || l_d != 10.5 || l_arr[0] != 20.5)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:get/set failed")));
					return false;
				}
			}
			// remove
			{
				l_bool =			m_w.do_string("a_1 = MDoubleArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:set(3,15.5)");
				l_bool = l_bool &&	m_w.do_string("a_1:remove(3)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:remove script")));
					return false;
				}
				MDoubleArray* l_a1;
				l_bool = vufLuaMDoubleArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:remove get global")));
					return false;
				}
				if (l_a1->length() != 9 || (*l_a1)[3] == 15.5)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:remove")));
					return false;
				}
			}
			// insert
			{
				l_bool =			m_w.do_string("a_1 = MDoubleArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:insert(3,15.5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:insert script")));
					return false;
				}
				MDoubleArray* l_a1;
				l_bool = vufLuaMDoubleArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:insert get global")));
					return false;
				}
				if (l_a1->length() != 11 || (*l_a1)[3] != 15.5)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray insert")));
					return false;
				}
			}
			// append
			{
				l_bool =			m_w.do_string("a_1 = MDoubleArray.new()");
				l_bool = l_bool &&	m_w.do_string("a_1:setLength(10)");
				l_bool = l_bool &&	m_w.do_string("a_1:append(30)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:append script")));
					return false;
				}
				MDoubleArray* l_a1;
				l_bool = vufLuaMDoubleArray::get_global(L, "a_1", &l_a1);
				if (l_bool == false)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray:append get global")));
					return false;
				}
				if (l_a1->length() != 11 || (*l_a1)[10] != 30)
				{
					VF_LOG_ERR(vuf::vufStringUtils::string_padding(std::string("Failed MDoubleArray append")));
					return false;
				}
			}
			VF_LOG_INFO(vuf::vufStringUtils::string_padding(std::string("....UNIT TEST FOR M_MDOUBLE_ARRAY PASSED.")));
			return true;
		}
	private:
		vuf::vufLuaWrapper	m_w;
	};
}
#endif // !VF_LUA_MAYA_MATH_UNIT_TEST_H
