#pragma once

#include <unitTests/vufMatrixTest.h>
#include <unitTests/vufVectorTest.h>
#include <unitTests/vufQuaternionTest.h>

#include <unitTests/vufVectorObjectArrayTest.h>
#include <unitTests/vufMatrixObjectArrayTest.h>
#include <unitTests/vufQuaternionObjectArrayTest.h>
#include <unitTests/vufNumericArrayObjectTest.h>

namespace vufMath
{
	class vufTestAll
	{
	public:
		vufTestAll() {}

		bool run(bool p_verbose = false)
		{
			//vufVectorTest<double, vufVector2> test_vector_2d;
			//vufVectorTest<double, vufVector3> test_vector_3d;
			vufVector4Test<double>		l_test_vector_4d;
			vufMatrix4Test<double>		l_test_matrix_4d;
			vufQuaternionTest<double>	l_test_quaternion;

			//vufVectorObjectArrayTest<double, vufVector2> test_vector_object_array_2d;
			//vufVectorObjectArrayTest<double, vufVector3> test_vector_object_array_3d;
			//vufVectorObjectArrayTest<double, vufVector4> l_test_vector_object_array_4d;

			//vufQuaternionObjectArrayTest<double> test_quaternion_object_array_d;

			//vufMatrixTest<double, vufMatrix2> test_matrix_2d;
			//vufMatrixTest<double, vufMatrix3> test_matrix_3d;

			//vufMatrixObjectArrayTest<double, vufMatrix2> test_matrix_object_array_2d;
			//vufMatrixObjectArrayTest<double, vufMatrix3> test_matrix_object_array_3d;
			//vufMatrixObjectArrayTest<double, vufMatrix4> l_test_matrix_object_array_4d;

			//vufNumericArrayObjectTest<double>	test_double_array;
			//vufNumericArrayObjectTest<float>	test_float_array;
			//vufNumericArrayObjectTest<int>		test_int_array;
			//vufNumericArrayObjectTest<bool>		test_bool_array;

			//if (!test_vector_2d.run()) { std::cin.get();		return false; }
			//if (!test_vector_3d.run()) { std::cin.get();		return false; }
			if (l_test_quaternion.run(p_verbose)== false) { std::cin.get();		return false; }
			if (l_test_vector_4d.run(p_verbose) == false) { std::cin.get();		return false; }
			if (l_test_matrix_4d.run(p_verbose) == false) { std::cin.get();		return false; }

			//if (!test_vector_object_array_2d.run()) { std::cin.get();		return false; }
			//if (!test_vector_object_array_3d.run()) { std::cin.get();		return false; }
			//if (l_test_vector_object_array_4d.run() == false) { std::cin.get();		return false; }

			//if (!test_matrix_2d.run()) { std::cin.get();		return false; }
			//if (!test_matrix_3d.run()) { std::cin.get();		return false; }

			//if (!test_matrix_object_array_2d.run())		return -1;
			//if (!test_matrix_object_array_2d.run()) { std::cin.get();		return false; }
			//if (!test_matrix_object_array_3d.run()) { std::cin.get();		return false; }
			//if ( l_test_matrix_object_array_4d.run() == false) { std::cin.get();	return false; }

			//if (!test_quaternion_object_array_d.run()) { std::cin.get();		return false; }

			//if (!test_double_array.run()) { std::cin.get();		return false; }
			//if (!test_float_array.run()) { std::cin.get();		return false; }
			//if (!test_int_array.run()) { std::cin.get();		return false; }
			//if (!test_bool_array.run()) { std::cin.get();		return false; }

			return true;
		}
	};
}