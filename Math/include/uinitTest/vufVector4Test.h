#ifndef VF_MATH_V4_UNT_TST_H
#define VF_MATH_V4_UNT_TST_H
#include <vufVector.h>
#include <vufMatrix.h>
#include <vufLog.h>
#include <coreUtils/vufStringUtils.h>
namespace vuf
{
	template <typename T> 
	class vector4UT
	{
	public:
		bool run()
		{
			std::cout << std::endl;
			VF_LOG_INFO(vufStringUtils::string_padding(""));
			VF_LOG_INFO(( vufStringUtils::string_padding(std::string("START_TESTS_FOR_TYPE_") + typeid(vufVector4<T>).name()) ));
			
			if (test_basics() == false )	return false;
			//if (test_serialization() == false)	return false;

			VF_LOG_INFO(( vufStringUtils::string_padding(std::string("END_TESTS_FOR_TYPE_")  + typeid(vufVector4<T>).name()) ));
			VF_LOG_INFO(vufStringUtils::string_padding(""));
			std::cout << std::endl;

			return true;
		}
	private:
		bool test_basics()
		{
			vufVector4<T> l_dflt,l_res;
			vufVector4<T> l_cnstr(1., 2., 3., 4.);
			vufVector4<T> l_other(l_cnstr);
			vufMatrix4<T> l_matr;
			
			// constructors
			if (l_dflt.x != .0 || l_dflt.y != .0 || l_dflt.z != .0 || l_dflt.w != 1.)
			{
				VF_LOG_ERR( vufStringUtils::string_padding( "Failed default constructor") );
				return false;
			}			
			if (l_cnstr.x != 1. || l_cnstr.y != 2. || l_cnstr.z != 3. || l_cnstr.w != 4.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed constructor parameters"));
				return false;
			}
			if (l_other.x != 1. || l_other.y != 2. || l_other.z != 3. || l_other.w != 4.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed copy constructor"));
				return false;
			}
			
			// operators
			if (l_cnstr(0) != 1. || l_cnstr(1) != 2. || l_cnstr(2) != 3. || l_cnstr(3) != 4.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator()"));
				return false;
			}
			if (l_cnstr[0] != 1. || l_cnstr[1] != 2. || l_cnstr[2] != 3. || l_cnstr[3] != 4.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator [] const"));
				return false;
			}
			
			l_other[0] = 11.; l_other[1] = 12.; l_other[2] = 13.; l_other[3] = 14.;
			if (l_other[0] != 11. || l_other[1] != 12. || l_other[2] != 13. || l_other[3] != 14.)
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator [] "));
				return false;
			}


			// operator *
			l_matr = vufMatrix4<T>::numerate_matrix(1);
			l_dflt.set(1., 2., 3., 4.);
			l_res = l_dflt * l_matr;
			if (l_res != vufVector4<T>(90, 100, 110, 120))
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator *(matrix)"));
				return false;
			}
			l_dflt *= l_matr;
			if (l_dflt != vufVector4<T>(90, 100, 110, 120))
			{
				VF_LOG_ERR(vufStringUtils::string_padding("Failed operator *=(matrix)"));
				return false;
			}

			
			return true;
		}
	};
}
#endif // !VF_MATH_V4_UNT_TST_H
