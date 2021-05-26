#ifndef VF_MATH_CRV_INCLUDE_H
#define VF_MATH_CRV_INCLUDE_H

#define VF_MATH_CRV_GATHER_INFO_I_BODY																	\
/** get information beatween point in space and point on curve. Return true if point is on curve */		\
inline bool			gather_info_i( const V<T>& p_point, T p_curve_param, T& p_dist, T& p_dot ) const	\
{																										\
	V<T> l_dir = p_point - get_pos_at_i(p_curve_param);													\
	p_dist = l_dir.length();																			\
	if (p_dist < VF_MATH_EPSILON)																		\
	{																									\
		p_dot = 0;																						\
		return true;																					\
	}																									\
	p_dot = (l_dir * (1. / p_dist)).dot(get_tangent_at_i(p_curve_param).get_normalized());				\
	return false;																						\
}

#define VF_MATH_CRV_GET_CLOSEST_PARAM_ON_INTERVAL_I_BODY												\
		bool				get_closest_point_param_on_interval_i(	const V<T>& p_point,				\
																	T p_t_1, T p_t_2,					\
																	T p_dist_1, T p_dist_2,				\
																	T& p_res_t, T& p_res_dist, T p_percition = 0.00001) const	\
		{																								\
			T l_closest_dist = p_dist_1;																\
			T l_closest_t = p_t_1;																		\
			if (p_dist_2 < p_dist_1)																	\
			{																							\
				l_closest_dist = p_dist_2;																\
				l_closest_t = p_t_2;																	\
			}																							\
			if (p_t_2 - p_t_1 < p_percition)															\
			{																							\
				p_res_dist = l_closest_dist;															\
				p_res_t = l_closest_t;																	\
				return false;																			\
			}																							\
																										\
			T l_middle_t, l_middle_dist, l_middle_dot;													\
			l_middle_t = (p_t_1 + p_t_2) * 0.5;															\
			if (gather_info_i(p_point, l_middle_t, l_middle_dist, l_middle_dot) == true)				\
			{																							\
				/* this point is on the curve	*/														\
				p_res_t = l_middle_t;																	\
				p_res_dist = l_middle_dist;																\
				return true;																			\
			}																							\
			if (l_middle_dot < 0)																		\
			{																							\
				if (get_closest_point_param_on_interval_i(p_point, p_t_1, l_middle_t,					\
					p_dist_1, l_middle_dist,															\
					p_res_t, p_res_dist, p_percition) == true)											\
				{																						\
					/* the point is on the curve*/														\
					return true;																		\
				}																						\
				if (l_closest_dist < p_res_dist)														\
				{																						\
					p_res_dist = l_closest_dist;														\
					p_res_t = l_closest_t;																\
				}																						\
				return false;																			\
			}																							\
			if (get_closest_point_param_on_interval_i(p_point, l_middle_t, p_t_2,						\
				l_middle_dist, p_dist_2,																\
				p_res_t, p_res_dist, p_percition) == true)												\
			{																							\
				/* the point is on the curve*/															\
				return true;																			\
			}																							\
			if (l_closest_dist < p_res_dist)															\
			{																							\
				p_res_dist = l_closest_dist;															\
				p_res_t = l_closest_t;																	\
			}																							\
			return false;																				\
		}

#define VF_MATH_CRV_GET_CLOSEST_PARAM_I_BODY															\


#endif // !VF_MATH_CRV_INCLUDE_H
