#ifndef VF_LUA_MATH_UT_H
#define VF_LUA_MATH_UT_H

#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
#include <vufLuaVector4.h>
#include <vufLuaMatrix4.h>

namespace vuf
{	
	template<typename T>
	class vufLuaMathUT
	{
	public:
		bool run()
		{
			std::cout << std::endl;
			VF_LOG_INFO( vufStringUtils::string_padding(""));
			VF_LOG_INFO( vufStringUtils::string_padding(std::string("START_TESTS_FOR_LUA_MATH") ));

			if (prepare()	== false)	return false;
			//if (vector_ut() == false)	return false;
			if (matrix_ut() == false)	return false;
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
				VF_LOG_ERR( vufStringUtils::string_padding(std::string("Failed to create lua machune.") ));
				return false;
			}
			lua_State* L = m_w.get_lua_state();
			vufLuaVector4<double>::registrator(L, "vector4", "vector4Meta");
			vufLuaMatrix4<double>::registrator(L, "matrix4", "matrix4Meta", "quat", "quatMeta", "vector4", "vector4Meta");
			return true;
		}
		bool vector_ut()
		{
			lua_State* L = m_w.get_lua_state();			
			vufVector4<T> l_res,l_res_b,l_ort;
			bool l_bool = false;

			// create
			if (m_w.do_string("vec = vector4.new()") == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to create vector")));
				return false;
			}
			l_bool = m_w.do_string("vec[0] = 2");

			// assign by index
			l_bool = l_bool && m_w.do_string("vec[1] = 3");
			l_bool = l_bool && m_w.do_string("vec[2] = 4");
			l_bool = l_bool && m_w.do_string("vec[3] = 5");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign vector by index script")));
				return false;
			}
			if (vufLuaVector4<T>::get_global(L, "vec", l_res) == false )
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to get global vector")));
				return false;

			}
			if (l_res != vufVector4<T>(2, 3, 4, 5))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed assign by indeses")));
				return false;
			}

			// assign by key
			l_bool = m_w.do_string("vec.x = 12");
			l_bool = l_bool && m_w.do_string("vec.y = 13");
			l_bool = l_bool && m_w.do_string("vec.z = 14");
			l_bool = l_bool && m_w.do_string("vec.w = 15");
			if ( l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign vector by key script")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			if (l_res != vufVector4<T>(12, 13, 14, 15))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed assign by key")));
				return false;
			}
			
			// assign copy
			l_bool = m_w.do_string("pnt = vec");
			vufLuaVector4<T>::get_global(L, "pnt", l_res_b);
			if (l_res_b != l_res)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign vector copy")));
				return false;
			}
			l_res_b.x = 0;
			if (l_res_b == l_res)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign vector copy.It's instance.")));
				return false;
			}
			
			// set
			if (m_w.do_string("vec:set()") == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set() script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			if (l_res != vufVector4<T>())
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set().")));
				return false;
			}
			if (m_w.do_string("vec:set(5)") == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set(5) script")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			if (l_res != vufVector4<T>(5))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5).")));
				return false;
			}
			if (m_w.do_string("vec:set(5,6)") == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set(5,6) script")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			if (l_res != vufVector4<T>(5,6))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6).")));
				return false;
			}
			if (m_w.do_string("vec:set(5,6,7)") == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set(5,6,7) script")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			if (l_res != vufVector4<T>(5, 6, 7))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6,7).")));
				return false;
			}
			if (m_w.do_string("vec:set(5,6,7,8)") == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to  vector set(5,6,7,8) script")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			if (l_res != vufVector4<T>(5, 6, 7, 8))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to set(5,6,7,8).")));
				return false;
			}

			// normalize
			l_bool = m_w.do_string("vec:set(3,4,12)");
			l_bool = l_bool && m_w.do_string("pnt = vec:normalize()");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector normalize script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			vufLuaVector4<T>::get_global(L, "pnt", l_res_b);
			if (l_res.length() != 1 || l_res_b.length() != 1)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector normalize.")));
				return false;
			}
			
			// ortho to
			l_bool = m_w.do_string("vec:set(2,2,3)");
			l_bool = l_bool && m_w.do_string("pnt:set(-14,4,2)");
			l_bool = l_bool && m_w.do_string("ort = vec:ortho_to(pnt)");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector ortho script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			vufLuaVector4<T>::get_global(L, "pnt", l_res_b);
			vufLuaVector4<T>::get_global(L, "ort", l_ort);
			if (l_ort.dot(l_res_b) != 0)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector ortho.")));
				return false;
			}
			
			// parallel to
			l_bool = m_w.do_string("vec:set(2,2,3)");
			l_bool = l_bool && m_w.do_string("pnt:set(-14,4,2)");
			l_bool = l_bool && m_w.do_string("ort = vec:parallel_to(pnt)");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector parallel_to script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			vufLuaVector4<T>::get_global(L, "pnt", l_res_b);
			vufLuaVector4<T>::get_global(L, "ort", l_ort);
			if (l_ort.get_cross(l_res_b).length() != 0)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector parallel_to.")));
				return false;
			}
			
			// get_cross
			l_bool = m_w.do_string("vec:set(2,2,0)");
			l_bool = l_bool && m_w.do_string("pnt:set(-14,4,0)");
			l_bool = l_bool && m_w.do_string("ort = vec:cross(pnt)");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector cross script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			vufLuaVector4<T>::get_global(L, "pnt", l_res_b);
			vufLuaVector4<T>::get_global(L, "ort", l_ort);
			if (l_ort.dot(l_res_b) != 0 || l_ort.dot(l_res))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector cross.")));
				return false;
			}

			// length
			l_bool = m_w.do_string("vec:set(3,4,12)");
			l_bool = l_bool && m_w.do_string("c = vec:length()");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector length script.")));
				return false;
			}
			lua_getglobal(L, "c");
			T l_val = (T)lua_tonumber(L, -1);
			if (l_val != 13)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector length.")));
				return false;
			}
			
			// distance to
			l_bool = m_w.do_string("vec:set(4,6,15)");
			l_bool = l_bool &&m_w.do_string("pnt:set(1,2,3)");
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
			
			// dot
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

			// to string
			l_bool = m_w.do_string("vec:set(1,2,3)");
			l_bool = l_bool & m_w.do_string("s = vec:to_string()");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector to_string script.")));
				return false;
			}
			lua_getglobal(L, "s");
			std::string l_str(lua_tostring(L, -1));
			vufLuaVector4<T>::get_global(L, "vec", l_res);
			if (l_str != l_res.to_string())
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector to_string.")));
				return false;
			}

			// to type 
			l_bool = m_w.do_string("s = vec:to_type()");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector to_type script.")));
				return false;
			}
			lua_getglobal(L, "s");
			l_str = lua_tostring(L, -1);
			if (l_str != "vector4")
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector to_type.")));
				return false;
			}
			
			// add
			l_bool = m_w.do_string("vec:set(1,2,3)");
			l_bool = l_bool && m_w.do_string("pnt:set(2,3,4)");
			l_bool = l_bool && m_w.do_string("ort = vec + pnt");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector + script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "ort", l_res);
			if (l_res != vufVector4<T>(3, 5, 7))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector + .")));
				return false;
			}

			// sub
			l_bool = m_w.do_string("vec:set(1,2,3)");
			l_bool = l_bool && m_w.do_string("pnt:set(2,3,4)");
			l_bool = l_bool && m_w.do_string("ort = vec - pnt");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector - script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "ort", l_res);
			if (l_res != vufVector4<T>(-1, -1, -1))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector - .")));
				return false;
			}

			// unm
			l_bool = m_w.do_string("vec:set(1,2,3)");
			l_bool = l_bool && m_w.do_string("ort = -vec");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector unar - script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "ort", l_res);
			if (l_res != vufVector4<T>(-1, -2, -3))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector unar- ")));
				return false;
			}
			
			
			// mul
			// To Do
			// Test Matrix mult
			l_bool = m_w.do_string("vec:set(1,2,3)");
			l_bool = l_bool && m_w.do_string("ort = vec * 4");
			l_bool = l_bool && m_w.do_string("pnt = 5 * vec");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector  mul script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "ort", l_res);
			vufLuaVector4<T>::get_global(L, "pnt", l_res_b);
			if (l_res != vufVector4<T>(4, 8, 12) || l_res_b != vufVector4<T>(5, 10, 15))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector  mul .")));
				return false;
			}

			// div
			l_bool = m_w.do_string("vec:set(2,4,6)");
			l_bool = l_bool && m_w.do_string("ort = vec / 2");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector  div script.")));
				return false;
			}
			vufLuaVector4<T>::get_global(L, "ort", l_res);
			if (l_res != vufVector4<T>(1, 2, 3))
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to vector  div .")));
				return false;
			}


			//std::cout << " result   " << l_res << std::endl;
			//std::cout << " result_b " << l_res_b << std::endl;
			//std::cout << " result ort   " << l_ort << std::endl;
			return true;
		}
		bool matrix_ut()
		{
			lua_State* L = m_w.get_lua_state();
			vufVector4<T> l_res, l_res_b, l_ort;
			vufMatrix4<T> l_m, l_m_b;
			bool l_bool = true;
			// create
			if (m_w.do_string("mat = matrix4.new()") == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to create matrix")));
				return false;
			}
			// assign by index
			// assign by index
			l_bool = l_bool && m_w.do_string("mat[0][0] = 3");
			l_bool = l_bool && m_w.do_string("mat[1][2] = 4");
			l_bool = l_bool && m_w.do_string("mat[2][3] = 5");
			l_bool = l_bool && m_w.do_string("mat[3][1] = 5");
			if (l_bool == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to asssign matrix by index script")));
				return false;
			}
			if (vufLuaMatrix4<T>::get_global(L, "mat", l_m) == false)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed to get global matrix")));
				return false;
			}			
			if (l_m[0][0] !=3 || l_m[1][2] != 4 || l_m[2][3] != 5 || l_m[3][1] != 5)
			{
				VF_LOG_ERR(vufStringUtils::string_padding(std::string("Failed assign by indeses")));
				return false;
			}
			
			std::cout << l_m << std::endl;
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