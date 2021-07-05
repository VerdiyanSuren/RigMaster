#ifndef VF_MATH_CURVE_QUAT_INCLUDE_H
#define VF_MATH_CURVE_QUAT_INCLUDE_H


#define VF_MATH_QUAT_TRANPORT_CHAIN_FORWARD(CRV_PTR,TANGENT_ARRAY, QUAT_PARAM_ARRAY,Y_AXIS_ARRAY)	\
{																								\
	TANGENT_ARRAY[0] = CRV_PTR->get_tangent_normalized_at(QUAT_PARAM_ARRAY[0]);					\
	Y_AXIS_ARRAY[0].make_ortho_to_in_place(TANGENT_ARRAY[0]);									\
	Y_AXIS_ARRAY[0].normalize_in_place();														\
	for (uint32_t i = 1; i < QUAT_PARAM_ARRAY.size(); ++i)										\
	{																							\
		TANGENT_ARRAY[i] = CRV_PTR->get_tangent_normalized_at(QUAT_PARAM_ARRAY[i]);				\
		V<T> l_cross = TANGENT_ARRAY[i - 1].get_cross(TANGENT_ARRAY[i]);						\
																								\
		if (l_cross.length2() < 0.000001)														\
		{																						\
			Y_AXIS_ARRAY[i] = Y_AXIS_ARRAY[i - 1];												\
			continue;																			\
		}																						\
		l_cross.normalize_in_place();															\
		T l_dot = TANGENT_ARRAY[i - 1].dot(TANGENT_ARRAY[i]);									\
		T l_angle = -acos(VF_CLAMP(-1., 1., l_dot));											\
		vufQuaternion<T> l_rot(l_angle, l_cross);												\
		Y_AXIS_ARRAY[i] = l_rot.rotate_vector_by_quaternion(Y_AXIS_ARRAY[i - 1]);				\
		Y_AXIS_ARRAY[i].normalize_in_place();													\
		if (Y_AXIS_ARRAY[i].dot(Y_AXIS_ARRAY[i - 1]) < 0.0)										\
		{																						\
			Y_AXIS_ARRAY[i] = -Y_AXIS_ARRAY[i];													\
		}																						\
	}																							\
}

#define VF_MATH_QUAT_Y_AXIS_TO_QUATERNION(TANGENT_ARRAY, QUAT_ARRAY, Y_AXIS_ARRAY )	\
for (uint32_t i = 0; i < TANGENT_ARRAY.size(); ++i)									\
{																					\
	V<T> l_z = TANGENT_ARRAY[i].get_cross(Y_AXIS_ARRAY[i]);							\
	l_z.normalize_in_place();														\
	vufMatrix4<T> l_matr;															\
	l_matr.set_axis_x(m_tangent_v[i]);												\
	l_matr.set_axis_y(m_y_axis_v[i]);												\
	l_matr.set_axis_z(l_z);															\
	QUAT_ARRAY[i] = l_matr.get_quaternion();										\
}

#endif // !VF_MATH_CURVE_QUAT_INCL_H
