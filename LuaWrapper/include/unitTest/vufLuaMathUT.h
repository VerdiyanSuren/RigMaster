#ifndef VF_LUA_MATH_UT_H
#define VF_LUA_MATH_UT_H

#include <iomanip>
#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
#include <vufLuaVector4.h>
#include <vufLuaMatrix4.h>
#include <vufLuaQuaternion.h>

#define VF_CHECK_STATUS_AND_RETURN_IT(STATUS)											\
if (STATUS == false)																	\
{																						\
	VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to get global.")));	\
	return false;																		\
}

namespace vuf
{	
	template<typename T>
	class vufLuaMathUT
	{
	public:
		bool run()
		{
			std::cout << std::setprecision(10) << std::fixed <<  std::endl;
			VF_LOG_INFO( vufStringUtils::string_padding(""));
			VF_LOG_INFO( vufStringUtils::string_padding(std::string("START_TESTS_FOR_LUA_MATH") ));

			if (prepare()	== false)	return false;
			if (vector4_ut() == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA VECTOR 4")));
				return false;
			}
			else
			{
				VF_LOG_INFO(vufStringUtils::string_padding(std::string("....LUA VECTOR 4 PASSED.")));
			}
			if (matrix_ut() == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA MATRIX 4")));
				return false;
			}
			else
			{
				VF_LOG_INFO(vufStringUtils::string_padding(std::string("....LUA MATRIX 4 PASSED.")));
			}
			if (quaternion_ut() == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("....FAILED TESTS FOR LUA QUATERNION")));
				return false;
			}
			else
			{
				VF_LOG_INFO(vufStringUtils::string_padding(std::string("....LUA QUATERNION  PASSED.")));
			}
			
			//if (test_serialization() == false)	return false;
			release();

			VF_LOG_INFO( vufStringUtils::string_padding(std::string("END_TESTS_FOR_LUA_MATH") ));
			VF_LOG_INFO( vufStringUtils::string_padding(""));
			std::cout << std::endl;

			return true;
		}
		bool prepare()
		{
			if (m_w.open_machine() == false)
			{
				VF_LOG_ERR( vufStringUtils::string_padding(std::string("Failed to create lua machine.") ));
				return false;
			}
			lua_State* L = m_w.get_lua_state();
			vufLuaVector4<double>::registrator(L);
			//vufLuaVector3<double>::registrator(L);
			vufLuaQuaternion<double>::registrator(L);
			//vufLuaMatrix3<double>::registrator(L);
			vufLuaMatrix4<double>::registrator(L);
			return true;
		}
		bool vector4_ut()
		{
			lua_State* L = m_w.get_lua_state();			
			vufVector4<T> *l_res_ptr, *l_res_b_ptr,*l_ort_ptr;
			bool l_bool = false;
			T l_val;
			std::string l_str;

			// create
			{
				if (m_w.do_string("vec = vec4.new()") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to create vector")));
					return false;
				}
			}
			// assign by key
			{
				l_bool = m_w.do_string("vec.x = 12");
				l_bool = l_bool && m_w.do_string("vec.y = 13");
				l_bool = l_bool && m_w.do_string("vec.z = 14");
				l_bool = l_bool && m_w.do_string("vec.w = 15");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign vector by key script")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				if (*l_res_ptr != vufVector4<T>(12, 13, 14, 15))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed assign by key")));
					return false;
				}
			}
			// assign copy
			{

				l_bool = m_w.do_string("pnt = vec:copy()");
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				vufLuaVector4<T>::get_global(L, "pnt", &l_res_b_ptr);
				if (*l_res_b_ptr != *l_res_ptr)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign vector copy")));
					return false;
				}
				l_bool = m_w.do_string("pnt = vec:copy()");
				l_bool = m_w.do_string("vec.x = 100");
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				vufLuaVector4<T>::get_global(L, "pnt", &l_res_b_ptr);
				if (*l_res_b_ptr == *l_res_ptr)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign vector copy.It's instance.")));
					return false;
				}
			}
			// set
			{
				if (m_w.do_string("vec:set()") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set() script.")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				if (*l_res_ptr != vufVector4<T>())
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set().")));
					return false;
				}
				if (m_w.do_string("vec:set(5)") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set(5) script")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				if (*l_res_ptr != vufVector4<T>(5))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5).")));
					return false;
				}
				if (m_w.do_string("vec:set(5,6)") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set(5,6) script")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				if (*l_res_ptr != vufVector4<T>(5, 6))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6).")));
					return false;
				}
				if (m_w.do_string("vec:set(5,6,7)") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set(5,6,7) script")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				if (*l_res_ptr != vufVector4<T>(5, 6, 7))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6,7).")));
					return false;
				}
				if (m_w.do_string("vec:set(5,6,7,8)") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set(5,6,7,8) script")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				if (*l_res_ptr != vufVector4<T>(5, 6, 7, 8))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6,7,8).")));
					return false;
				}
			}
			// normalize
			{
				l_bool = m_w.do_string("vec:set(3,4,12)");
				l_bool = l_bool && m_w.do_string("pnt = vec:normalized()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector normalize script.")));
					return false;
				}
				//vufLuaVector4<T>::get_global(L, "vec", l_res);
				vufLuaVector4<T>::get_global(L, "pnt", &l_res_b_ptr);
				if ( (*l_res_b_ptr).length() != 1)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector normalize.")));
					return false;
				}
			}
			// ortho to
			{
				l_bool = m_w.do_string("vec:set(2,2,3)");
				l_bool = l_bool && m_w.do_string("pnt:set(-14,4,2)");
				l_bool = l_bool && m_w.do_string("ort = vec:ortho_to(pnt)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector ortho script.")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				vufLuaVector4<T>::get_global(L, "pnt", &l_res_b_ptr);
				vufLuaVector4<T>::get_global(L, "ort", &l_ort_ptr);
				if ((*l_ort_ptr).dot(*l_res_b_ptr) != 0)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector ortho.")));
					return false;
				}
			}
			// parallel to
			{
				l_bool = m_w.do_string("vec:set(2,2,3)");
				l_bool = l_bool && m_w.do_string("pnt:set(-14,4,2)");
				l_bool = l_bool && m_w.do_string("ort = vec:parallel_to(pnt)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector parallel_to script.")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				vufLuaVector4<T>::get_global(L, "pnt", &l_res_b_ptr);
				vufLuaVector4<T>::get_global(L, "ort", &l_ort_ptr);
				if ((*l_ort_ptr).get_cross(*l_res_b_ptr).length() != 0)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector parallel_to.")));
					return false;
				}
			}
			// get_cross
			{
				l_bool = m_w.do_string("vec:set(2,2,0)");
				l_bool = l_bool && m_w.do_string("pnt:set(-14,4,0)");
				l_bool = l_bool && m_w.do_string("ort = vec:cross(pnt)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector cross script.")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				vufLuaVector4<T>::get_global(L, "pnt", &l_res_b_ptr);
				vufLuaVector4<T>::get_global(L, "ort", &l_ort_ptr);
				if ((*l_ort_ptr).dot(*l_res_b_ptr) != 0 || (*l_ort_ptr).dot(*l_res_ptr))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector cross.")));
					return false;
				}
			}
			// length
			{
				l_bool = m_w.do_string("vec:set(3,4,12)");
				l_bool = l_bool && m_w.do_string("c = vec:length()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector length script.")));
					return false;
				}
				lua_getglobal(L, "c");
				l_val = (T)lua_tonumber(L, -1);
				if (l_val != 13)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector length.")));
					return false;
				}
			}
			// distance to
			{
				l_bool = m_w.do_string("vec:set(4,6,15)");
				l_bool = l_bool && m_w.do_string("pnt:set(1,2,3)");
				l_bool = l_bool && m_w.do_string("c = vec:distance_to(pnt)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector distance_to script.")));
					return false;
				}
				lua_getglobal(L, "c");
				l_val = (T)lua_tonumber(L, -1);
				if (l_val != 13)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector distance_to.")));
					return false;
				}
			}
			// dot
			{
				l_bool = m_w.do_string("vec:set(1,2,3)");
				l_bool = l_bool && m_w.do_string("pnt:set(-20,3,5)");
				l_bool = l_bool && m_w.do_string("c = vec:dot(pnt)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector dot script.")));
					return false;
				}
				lua_getglobal(L, "c");
				l_val = (T)lua_tonumber(L, -1);
				if (l_val != 1)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector dot.")));
					return false;
				}
			}
			// to string
			{
				l_bool = m_w.do_string("vec:set(1,2,3)");
				l_bool = l_bool & m_w.do_string("s = vec:to_string()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector to_string script.")));
					return false;
				}
				lua_getglobal(L, "s");
				l_str = lua_tostring(L, -1);
				vufLuaVector4<T>::get_global(L, "vec", &l_res_ptr);
				if (l_str != (*l_res_ptr).to_string())
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector to_string.")));
					return false;
				}
			}
			// to type 
			{
				l_bool = m_w.do_string("s = vec:to_type()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector to_type script.")));
					return false;
				}
				lua_getglobal(L, "s");
				l_str = lua_tostring(L, -1);
				if (l_str != "vec4")
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector to_type.")));
					return false;
				}
			}
			// add
			{
				l_bool = m_w.do_string("vec:set(1,2,3)");
				l_bool = l_bool && m_w.do_string("pnt:set(2,3,4)");
				l_bool = l_bool && m_w.do_string("ort = vec + pnt");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector + script.")));
					return false;
				}
				VF_CHECK_STATUS_AND_RETURN_IT(vufLuaVector4<T>::get_global(L, "ort", &l_res_ptr));
				if (*l_res_ptr != vufVector4<T>(3, 5, 7))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector + . Got ") + (*l_res_ptr).to_string() + " experct " + vufVector4<T>(3, 5, 7).to_string()));
					return false;
				}				
			}
			// sub
			{
				l_bool = m_w.do_string("vec:set(1,2,3)");
				l_bool = l_bool && m_w.do_string("pnt:set(2,3,4)");
				l_bool = l_bool && m_w.do_string("ort = vec - pnt");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector - script.")));
					return false;
				}
				VF_CHECK_STATUS_AND_RETURN_IT(vufLuaVector4<T>::get_global(L, "ort", &l_res_ptr));
				if (*l_res_ptr != vufVector4<T>(-1, -1, -1))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector - . Got ") + (*l_res_ptr).to_string() + " experct " + vufVector4<T>(-1, -1, -1).to_string()));
					return false;
				}
			}
			// unm
			{
				l_bool = m_w.do_string("vec:set(1,2,3)");
				l_bool = l_bool && m_w.do_string("ort = -vec");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector unar - script.")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "ort", &l_res_ptr);
				if (*l_res_ptr != vufVector4<T>(-1, -2, -3))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector unar- ")));
					return false;
				}
			}
			// mul			
			{
				l_bool = m_w.do_string("vec:set(1,2,3)");
				l_bool = l_bool && m_w.do_string("ort = vec * 4");
				l_bool = l_bool && m_w.do_string("pnt = 5 * vec");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector  mul script.")));
					return false;
				}
				VF_CHECK_STATUS_AND_RETURN_IT(vufLuaVector4<T>::get_global(L, "ort", &l_res_ptr));
				VF_CHECK_STATUS_AND_RETURN_IT(vufLuaVector4<T>::get_global(L, "pnt", &l_res_b_ptr));
				if (*l_res_ptr != vufVector4<T>(4, 8, 12) || *l_res_b_ptr != vufVector4<T>(5, 10, 15))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector  mul .")));
					return false;
				}				
				// Test Matrix mult
				
				l_bool = m_w.do_string("mat = mat4.new()");
				l_bool = l_bool && m_w.do_string("mat = mat4.numerate()");				
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix create for vecotor4 mul script.")));
					return false;
				}
				vufMatrix4<T> *l_matr_ptr;
				vufLuaMatrix4<T>::get_global( L, "mat", &l_matr_ptr);
				l_bool = m_w.do_string("vt = ort * mat");
				vufLuaVector4<T>::get_global(L, "vt", &l_res_b_ptr);
				if (*l_res_b_ptr != vufVector4<T>(140, 165, 190, 215))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector * matrix.")));
					return false;
				}
			}			
			// div
			{
				l_bool = m_w.do_string("vec:set(2,4,6)");
				l_bool = l_bool && m_w.do_string("ort = vec / 2");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector  div script.")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "ort", &l_res_ptr);
				if (*l_res_ptr != vufVector4<T>(1, 2, 3))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector  div .")));
					return false;
				}
			}
			//std::cout << " result   " << l_res << std::endl;
			//std::cout << " result_b " << l_res_b << std::endl;
			//std::cout << " result ort   " << l_ort << std::endl;
			return true;
		}
		bool quaternion_ut()
		{

			lua_State* L = m_w.get_lua_state();
			//vufQuaternion<T> l_q_1, l_q_2, l_q_3;
			vufQuaternion<T> *l_q_1_ptr, *l_q_2_ptr;
			vufVector4<T> *l_v_ptr;
			bool l_bool = false;
			//T l_val;
			std::string l_str;

			// create
			{
				if (m_w.do_string("q = quat.new()") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to create quaternion")));
					return false;
				}
			}
			// assign by key
			{
				l_bool = m_w.do_string("q.x = 12");
				l_bool = l_bool && m_w.do_string("q.y = 13");
				l_bool = l_bool && m_w.do_string("q.z = 14");
				l_bool = l_bool && m_w.do_string("q.w = 15");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign quaternion by key script")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				if (*l_q_1_ptr != vufQuaternion<T>(12, 13, 14, 15))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed assign by key")));
					return false;
				}
			}
			// assign copy
			{
				l_bool = m_w.do_string("q2 = q:copy()");
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				vufLuaQuaternion<T>::get_global(L, "q2", &l_q_2_ptr);
				if (*l_q_2_ptr != *l_q_1_ptr)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign quaternion copy")));
					return false;
				}
				l_bool = m_w.do_string("q2.x = -10");
				vufLuaQuaternion<T>::get_global(L, "q2", &l_q_2_ptr);
				if (*l_q_2_ptr == *l_q_1_ptr)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign quaternion copy.It's instance.")));
					return false;
				}
			}
			// set			
			{
				if (m_w.do_string("q:set()") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  quaternion set() script.")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				if (*l_q_1_ptr != vufQuaternion<T>())
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set().")));
					return false;
				}
				if (m_w.do_string("q:set(5)") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  quaternion set(5) script")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				if (*l_q_1_ptr != vufQuaternion<T>(5))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5).")));
					return false;
				}
				if (m_w.do_string("q:set(5,6)") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  quaternion set(5,6) script")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				if (*l_q_1_ptr != vufQuaternion<T>(5, 6))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6).")));
					return false;
				}
				if (m_w.do_string("q:set(5,6,7)") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  quaternion set(5,6,7) script")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				if (*l_q_1_ptr != vufQuaternion<T>(5, 6, 7))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6,7).")));
					return false;
				}
				if (m_w.do_string("q:set(5,6,7,8)") == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  quaternion set(5,6,7,8) script")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				if (*l_q_1_ptr != vufQuaternion<T>(5, 6, 7, 8))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6,7,8).")));
					return false;
				}
			}
			// set by axis and angle
			{
				l_bool = m_w.do_string("v = vec4.new()");
				l_bool = l_bool && m_w.do_string("v:set(1,1,1)");
				l_bool = l_bool && m_w.do_string("v = v:normalized()");
				l_bool = l_bool && m_w.do_string("a = 60");
				l_bool = l_bool && m_w.do_string("q = quat.by_axis_and_angle(v,a)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set quaternion by angle and axis script")));
					return false;
				}
				vufLuaVector4<T>::get_global(L, "v", &l_v_ptr);
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				if ( *l_q_1_ptr != vufQuaternion<T>().set_by_angle_and_axis(60., *l_v_ptr))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Test Failed.( by_axis_and_angle ).")));
					return false;
				}
			}
			// normalize
			{
				l_bool = m_w.do_string("q:set(3,4,12)");
				l_bool = l_bool && m_w.do_string("q2 = q:normalized()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion normalize script.")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q2", &l_q_2_ptr);
				if (std::abs((*l_q_2_ptr).length() - 1) > VF_MATH_EPSILON)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion normalize.")));
					return false;
				}
			}
			// conjugate
			{
				l_bool = m_w.do_string("q:set(3,4,12)");
				l_bool = l_bool && m_w.do_string("q = q:normalized()");
				l_bool = l_bool && m_w.do_string("q2 = q:conjugated()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion conjugate script.")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				vufLuaQuaternion<T>::get_global(L, "q2", &l_q_2_ptr);
				if (*l_q_2_ptr != (*l_q_1_ptr).get_conjugated())
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion conjugate.")));
					return false;
				}
			}
			// invert
			{
				l_bool = m_w.do_string("q:set(3,4,12)");
				l_bool = l_bool && m_w.do_string("q = q:normalized()");
				l_bool = l_bool && m_w.do_string("q2 = q:inverted()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion invert script.")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q",  &l_q_1_ptr);
				vufLuaQuaternion<T>::get_global(L, "q2", &l_q_2_ptr);
				if (*l_q_2_ptr != (*l_q_1_ptr).get_inverted())
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion invert.")));
					return false;
				}
			}
			//length
			{
				l_bool = m_w.do_string("q:set(3,4,12)");
				l_bool = l_bool && m_w.do_string("q = q:normalized()");
				l_bool = l_bool && m_w.do_string("l = q:length()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion length script.")));
					return false;
				}
				lua_getglobal(L, "l");
				T l_lngth = (T)lua_tonumber(L, -1);
				if (std::abs(l_lngth - 1) > VF_MATH_EPSILON)
				{
					std::cout << l_lngth << std::endl;
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion length.")));
					return false;
				}
			}
			// angle
			{
				l_bool = m_w.do_string("q:set(3,4,12,5)");
				l_bool = l_bool && m_w.do_string("q = q:normalized()");
				l_bool = l_bool && m_w.do_string("a = q:angle()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion angle script.")));
					return false;
				}
				lua_getglobal(L, "a");
				T l_angle = (T)lua_tonumber(L, -1);
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				if (l_angle != (*l_q_1_ptr).get_angle())
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion angle.")));
					return false;
				}
			}
			// axis
			{
				l_bool = m_w.do_string("q:set(3,4,12,5)");
				l_bool = l_bool && m_w.do_string("q = q:normalized()");
				l_bool = l_bool && m_w.do_string("v = q:axis()");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion axis script.")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q", &l_q_1_ptr);
				vufLuaVector4<T>::get_global(L, "v", &l_v_ptr);
				if (*l_v_ptr != (*l_q_1_ptr).get_axis_as_v4())
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion axis.")));
					return false;
				}
			}
			// dot 
			{
				l_bool = true;
				l_bool = l_bool && m_w.do_string("q2 = quat.new()");
				l_bool = l_bool && m_w.do_string("q:set(3,-4,3,1)");
				l_bool = l_bool && m_w.do_string("q2:set(3,4,12,5)");
				l_bool = l_bool && m_w.do_string("d = q:dot(q2)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion dot script.")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q",  &l_q_1_ptr);
				vufLuaQuaternion<T>::get_global(L, "q2", &l_q_2_ptr);
				lua_getglobal(L, "d");
				T l_d = (T)lua_tonumber(L, -1);
				if (l_d != (*l_q_1_ptr).dot(*l_q_2_ptr))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion dot.")));
					return false;
				}
			}
			// mul
			{
				l_bool = true;
				l_bool = l_bool && m_w.do_string("q1 = quat.new()");
				l_bool = l_bool && m_w.do_string("q2 = quat.new()");
				l_bool = l_bool && m_w.do_string("q1:set(3,-4,3,1)");
				l_bool = l_bool && m_w.do_string("q2:set(3,4,12,5)");
				l_bool = l_bool && m_w.do_string("qm1 = q1*2");
				l_bool = l_bool && m_w.do_string("qm2 = 3*q1");
				l_bool = l_bool && m_w.do_string("qm3 = q1*q2");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion mul script.")));
					return false;
				}
				vufLuaQuaternion<T>::get_global(L, "q1", &l_q_1_ptr);
				vufLuaQuaternion<T>::get_global(L, "q2", &l_q_2_ptr);
				vufQuaternion<T> *l_qm_1_ptr, *l_qm_2_ptr, *l_qm_3_ptr;
				vufLuaQuaternion<T>::get_global(L, "qm1", &l_qm_1_ptr);
				vufLuaQuaternion<T>::get_global(L, "qm2", &l_qm_2_ptr);
				vufLuaQuaternion<T>::get_global(L, "qm3", &l_qm_3_ptr);
				if (*l_qm_1_ptr != (*l_q_1_ptr)*2 || *l_qm_2_ptr != 3 * (*l_q_1_ptr) || *l_qm_3_ptr != (*l_q_1_ptr) * (*l_q_2_ptr))
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to quaternion mul .")));
					return false;
				}
			}
			return true;
		}
		bool matrix_ut()
		{
			lua_State* L = m_w.get_lua_state();
			//vufVector4<T>* l_res_b_ptr;// *l_ort_ptr;
			vufMatrix4<T>* l_m_a_ptr, *l_m_b_ptr;
			T l_val;
			bool l_bool = true;
			// create
			if (m_w.do_string("mat = mat4.new()") == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to create matrix")));
				return false;
			}

			// assign by set
			{
				l_bool = l_bool && m_w.do_string("mat:set(0,0,3)");
				l_bool = l_bool && m_w.do_string("mat:set(1,2,4)");
				l_bool = l_bool && m_w.do_string("mat:set(2,3,5)");
				l_bool = l_bool && m_w.do_string("mat:set(3,1,5)");
				if (l_bool == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign matrix by index script")));
					return false;
				}
				if (vufLuaMatrix4<T>::get_global(L, "mat", &l_m_a_ptr) == false)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to get global matrix")));
					return false;
				}
				if ((*l_m_a_ptr)[0][0] != 3 || (*l_m_a_ptr)[1][2] != 4 || (*l_m_a_ptr)[2][3] != 5 || (*l_m_a_ptr)[3][1] != 5)
				{
					VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed assign by indeses")));
					return false;
				}
		}
			// assign maatrix element to variable
			l_bool = m_w.do_string("a = mat:get(3,1)");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed assign maatrix element to variable script.")));
				return false;
			}
			lua_getglobal(L, "a");
			l_val = (T)lua_tonumber(L, -1);
			if (l_val != 5)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed assign maatrix element to variable.")));
				return false;
			}

			// numerate
			l_bool = m_w.do_string("mat = mat4.numerate()");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix numerate script")));
				return false;
			}
			vufLuaMatrix4<T>::get_global(L, "mat", &l_m_a_ptr);
			if (*l_m_a_ptr != vufMatrix4<T>::numerate_matrix())
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix numerate")));
				return false;
			}

			// Transpose 4
			l_bool = m_w.do_string("tr = mat:transpose()");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix transpose script")));
				return false;
			}
			vufLuaMatrix4<T>::get_global(L, "tr", &l_m_b_ptr);
			if ((*l_m_a_ptr).get_transposed_4() != *l_m_b_ptr)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix get_transpose.")));
				return false;
			}
			

			// Transpose 3
			l_bool = m_w.do_string("mat = mat4.numerate()");
			l_bool = l_bool && m_w.do_string("tr = mat:transpose3()");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix get_transpose script")));
				return false;
			}
			vufLuaMatrix4<T>::get_global(L, "tr", &l_m_b_ptr);
			if ((*l_m_a_ptr).get_transposed_3() != *l_m_b_ptr)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix get_transpose3.")));
				return false;
			}
			
			// determinant
			l_bool = m_w.do_string("mat = mat4.numerate()");
			l_bool = l_bool && m_w.do_string("mat:set(0,0,1)");
			l_bool = l_bool && m_w.do_string("mat:set(1,1,-10)");
			l_bool = l_bool && m_w.do_string("mat:set(2,2,-10)");
			l_bool = l_bool && m_w.do_string("a = mat:determinant()");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix determinant script")));
				return false;
			}
			lua_getglobal(L, "a");
			l_val = (T)lua_tonumber(L, -1);
			if (l_val != -5920)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to matrix determinant.")));
				return false;
			}

			return true;
		}
		void release()
		{
			m_w.close_machine();
		}
	private:
		vufLuaWrapper	m_w;
	};
}

#endif !VF_LUA_MATH_UT_H