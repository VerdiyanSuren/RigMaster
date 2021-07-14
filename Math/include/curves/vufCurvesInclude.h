#ifndef VF_MATH_CRV_INCLUDE_H
#define VF_MATH_CRV_INCLUDE_H

#define VF_MATH_CURVE_CREATOR_BODY(SHARED_CRV) \
		SHARED_CRV->m_this = std::static_pointer_cast<vufCurve<T, V>>(SHARED_CRV);

#define VF_MATH_CURVE_DEFINE_CREATOR(CLASS_NAME)										\
static  std::shared_ptr< CLASS_NAME > create()											\
{																						\
std::shared_ptr< CLASS_NAME >  l_ptr = std::shared_ptr<CLASS_NAME>(new CLASS_NAME());	\
VF_MATH_CURVE_CREATOR_BODY(l_ptr);														\
return l_ptr;																			\
}

#define VF_MATH_CURVE_DEFINE_TYPE_CATEGORY(TYPE,CATEGORY)								\
virtual vufCurveType	get_curve_type()					const override				\
{																						\
	return vufCurveType::TYPE;															\
}																						\
virtual int				get_curve_category()				const override				\
{																						\
	return vufCurveCategory::CATEGORY;													\
}



#define VF_MATH_CRV_GATHER_INFO_I_BODY																	\
/** get information between point in space and point on curve. Return true if point is on curve */		\
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
bool				get_closest_point_param_on_interval_i(	const V<T>& p_point,						\
															T p_t_1, T p_t_2,							\
															T p_dist_1, T p_dist_2,						\
															T& p_res_t, T& p_res_dist, T p_percition = 0.00001) const	\
	{																									\
		T l_closest_dist = p_dist_1;																	\
		T l_closest_t = p_t_1;																			\
		if (p_dist_2 < p_dist_1)																		\
		{																								\
			l_closest_dist = p_dist_2;																	\
			l_closest_t = p_t_2;																		\
		}																								\
		if (p_t_2 - p_t_1 < p_percition)																\
		{																								\
			p_res_dist = l_closest_dist;																\
			p_res_t = l_closest_t;																		\
			return false;																				\
		}																								\
																										\
		T l_middle_t, l_middle_dist, l_middle_dot;														\
		l_middle_t = (p_t_1 + p_t_2) * 0.5;																\
		if (gather_info_i(p_point, l_middle_t, l_middle_dist, l_middle_dot) == true)					\
		{																								\
			/* this point is on the curve	*/															\
			p_res_t = l_middle_t;																		\
			p_res_dist = l_middle_dist;																	\
			return true;																				\
		}																								\
		if (l_middle_dot < 0)																			\
		{																								\
			if (get_closest_point_param_on_interval_i(p_point, p_t_1, l_middle_t,						\
				p_dist_1, l_middle_dist,																\
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
		}																								\
		if (get_closest_point_param_on_interval_i(p_point, l_middle_t, p_t_2,							\
			l_middle_dist, p_dist_2,																	\
			p_res_t, p_res_dist, p_percition) == true)													\
		{																								\
			/* the point is on the curve*/																\
			return true;																				\
		}																								\
		if (l_closest_dist < p_res_dist)																\
		{																								\
			p_res_dist = l_closest_dist;																\
			p_res_t = l_closest_t;																		\
		}																								\
		return false;																					\
	}

#define VF_MATH_CRV_GET_CLOSEST_PARAM_I																	\
	T					get_closest_point_param_i(const V<T>& p_point, T p_start, T p_end, uint32_t p_divisions = 10, T p_percition = vufCurve_kTol) const	\
	{																									\
		T l_dist_prev, l_dist_next, l_closest_dist, l_closest_t = p_start;								\
		T l_dot_prev, l_dot_next;																		\
		T l_prev_t = p_start, l_next_t;																	\
		if (gather_info_i(p_point, p_start, l_dist_prev, l_dot_prev) == true)							\
		{																								\
			/* this point is on the curve */															\
			return p_start;																				\
		}																								\
		l_closest_dist = l_dist_prev;																	\
																										\
		T l_interval_step = (p_end - p_start) / (T)p_divisions;											\
		for (uint32_t i = 1; i <= p_divisions; ++i)														\
		{																								\
			l_next_t = p_start + (T)i * l_interval_step;												\
			if (gather_info_i(p_point, l_next_t, l_dist_next, l_dot_next) == true)						\
			{																							\
				/* this point is on the curve */														\
				return l_next_t;																		\
			}																							\
																										\
			T l_res_t, l_res_dist;																		\
			if (get_closest_point_param_on_interval_i(p_point, l_prev_t, l_next_t,						\
				l_dist_prev, l_dist_next,																\
				l_res_t, l_res_dist, p_percition) == true)												\
			{																							\
				/* the point is on the curve */															\
				return l_res_t;																			\
			}																							\
			if (l_res_dist < l_closest_dist)															\
			{																							\
				l_closest_dist = l_res_dist;															\
				l_closest_t = l_res_t;																	\
			}																							\
			l_dist_prev = l_dist_next;																	\
			l_prev_t = l_next_t;																		\
		}																								\
		return l_closest_t;																				\
	}



#define VF_MATH_CRV_REBUILD																				\
virtual bool		rebuild(																			\
								std::vector<T>& p_uniform_to_curve_val_v,								\
								std::vector<T>& p_curve_to_uniform_val_v,								\
								std::vector<T>& p_curve_val_to_length_v,								\
								uint32_t		p_division_count = 10,									\
								T				p_start = 0 /*interval on which we need rebuild*/,		\
								T				p_end = 1) const override								\
	{																									\
		if (m_valid == false || abs(p_end - p_start) < vufCurve_kTol)									\
		{																								\
			return false;																				\
		}																								\
		uint32_t	l_total_interval_count = p_division_count + 1;										\
		T			l_interval_length = 0;																\
		T			l_step = (p_end - p_start) / (T)l_total_interval_count;								\
		if (p_uniform_to_curve_val_v.size() != l_total_interval_count + 1)								\
		{																								\
			p_uniform_to_curve_val_v.resize(l_total_interval_count + 1);								\
			p_curve_to_uniform_val_v.resize(l_total_interval_count + 1);								\
			p_curve_val_to_length_v.resize(l_total_interval_count + 1);									\
		}																								\
		V<T> l_vec_prev = get_pos_at_i(p_start);														\
		p_uniform_to_curve_val_v[0] = p_start;															\
		p_curve_to_uniform_val_v[0] = p_start;															\
		p_uniform_to_curve_val_v.back() = p_end;														\
		p_curve_to_uniform_val_v.back() = p_end;														\
		p_curve_val_to_length_v[0] = 0;																	\
		/* Compute curve length until sample point */													\
		for (uint64_t i = 1; i < l_total_interval_count + 1; ++i)										\
		{																								\
			T l_crv_param = p_start + ((T)i) * l_step;													\
			V<T> l_vec = get_pos_at_i(l_crv_param);														\
			p_curve_val_to_length_v[i] = p_curve_val_to_length_v[i - 1] + l_vec_prev.distance_to(l_vec);\
			l_vec_prev = l_vec;																			\
		}																								\
		l_interval_length = p_curve_val_to_length_v.back();												\
		if (l_interval_length < vufCurve_kTol)															\
		{																								\
			return false;																				\
		}																								\
		for (uint64_t i = 1; i < l_total_interval_count; ++i)											\
		{																								\
			p_curve_to_uniform_val_v[i] = p_start + (p_end - p_start) * p_curve_val_to_length_v[i] / l_interval_length;	\
		}																								\
		T l_t_1;																						\
		T l_t_2;																						\
		T l_w_1;																						\
		T l_w_2;																						\
		uint64_t l_ndx = 1;																				\
		/* forward curve	*/																			\
		if (l_step > 0)																					\
		{																								\
			for (uint64_t i = 1; i < l_total_interval_count; ++i)										\
			{																							\
				T l_uniform_param = p_start + ((T)i) * l_step;											\
				while (l_uniform_param > p_curve_to_uniform_val_v[l_ndx])								\
				{																						\
					l_ndx++;																			\
				}																						\
				l_t_1 = p_start + ((T)(l_ndx - 1)) * l_step;											\
				l_t_2 = p_start + ((T)(l_ndx)) * l_step;												\
				l_w_1 = (p_curve_to_uniform_val_v[l_ndx] - l_uniform_param) / (p_curve_to_uniform_val_v[l_ndx] - p_curve_to_uniform_val_v[l_ndx - 1]);	\
				l_w_2 = 1. - l_w_1;																		\
																										\
				p_uniform_to_curve_val_v[i] = l_t_1 * l_w_1 + l_t_2 * l_w_2;							\
			}																							\
			return true;																				\
		}																								\
		/* if curve reversed */																			\
		l_ndx = 1;																						\
		for (uint64_t i = 1; i < l_total_interval_count; ++i)											\
		{																								\
			T l_uniform_param = p_start + ((T)i) * l_step;												\
			while (l_uniform_param < p_curve_to_uniform_val_v[l_ndx])/*/ differense beatween forward and reeverse is here */	\
			{																							\
				l_ndx++;																				\
			}																							\
			l_t_1 = p_start + ((T)(l_ndx - 1)) * l_step;												\
			l_t_2 = p_start + ((T)(l_ndx)) * l_step;													\
			l_w_1 = (p_curve_to_uniform_val_v[l_ndx] - l_uniform_param) / (p_curve_to_uniform_val_v[l_ndx] - p_curve_to_uniform_val_v[l_ndx - 1]);	\
			l_w_2 = 1. - l_w_1;																			\
																										\
			p_uniform_to_curve_val_v[i] = l_t_1 * l_w_1 + l_t_2 * l_w_2;								\
		}																								\
		return true;																					\
	}

#define VF_MATH_CRV_REBUILD_ALONG_AXIS																	\
		virtual bool		rebuild_along_axis( const V<T>&		p_axis/*project curve on this axis*/,	\
												std::vector<T>& p_uniform_to_curve_val_v,				\
												std::vector<T>& p_curve_to_uniform_val_v,				\
												std::vector<T>& p_curve_val_to_length_v,				\
												uint32_t		p_division_count = 10,					\
												T				p_start = 0 /*interval on which we need rebuild*/,	\
												T				p_end = 1) const override							\
	{																									\
		if (m_valid == false || abs(p_end - p_start) < vufCurve_kTol)									\
		{																								\
			return false;																				\
		}																								\
		uint32_t	l_total_interval_count = p_division_count + 1;										\
		T			l_interval_length = 0;																\
		T			l_step = (p_end - p_start) / (T)l_total_interval_count;								\
		if (p_uniform_to_curve_val_v.size() != l_total_interval_count + 1)								\
		{																								\
			p_uniform_to_curve_val_v.resize(l_total_interval_count + 1);								\
			p_curve_to_uniform_val_v.resize(l_total_interval_count + 1);								\
			p_curve_val_to_length_v.resize(l_total_interval_count + 1);									\
		}																								\
		V<T> l_vec_prev = get_pos_at_i(p_start);														\
		l_vec_prev.make_parallel_to_in_place(p_axis);													\
		p_uniform_to_curve_val_v[0] = p_start;															\
		p_curve_to_uniform_val_v[0] = p_start;															\
		p_uniform_to_curve_val_v.back() = p_end;														\
		p_curve_to_uniform_val_v.back() = p_end;														\
		p_curve_val_to_length_v[0] = 0;																	\
		/* Compute curve length along axis until sample point */										\
		for (uint64_t i = 1; i < l_total_interval_count + 1; ++i)										\
		{																								\
			T l_crv_param = p_start + ((T)i) * l_step;													\
			V<T> l_vec = get_pos_at_i(l_crv_param);														\
			l_vec.make_parallel_to_in_place(p_axis);													\
			p_curve_val_to_length_v[i] = p_curve_val_to_length_v[i - 1] + l_vec_prev.distance_to(l_vec);\
			l_vec_prev = l_vec;																			\
		}																								\
		l_interval_length = p_curve_val_to_length_v.back();												\
		if (l_interval_length < vufCurve_kTol)															\
		{																								\
			return false;																				\
		}																								\
		for (uint64_t i = 1; i < l_total_interval_count; ++i)											\
		{																								\
			p_curve_to_uniform_val_v[i] = p_start + (p_end - p_start) * p_curve_val_to_length_v[i] / l_interval_length; \
		}																								\
		T l_t_1;																						\
		T l_t_2;																						\
		T l_w_1;																						\
		T l_w_2;																						\
		uint64_t l_ndx = 1;																				\
		/* forward curve */																				\
		if (l_step > 0)																					\
		{																								\
			for (uint64_t i = 1; i < l_total_interval_count; ++i)										\
			{																							\
				T l_uniform_param = p_start + ((T)i) * l_step;											\
				while (l_uniform_param > p_curve_to_uniform_val_v[l_ndx])								\
				{																						\
					l_ndx++;																			\
				}																						\
				l_t_1 = p_start + ((T)(l_ndx - 1)) * l_step;											\
				l_t_2 = p_start + ((T)(l_ndx)) * l_step;												\
				l_w_1 = (p_curve_to_uniform_val_v[l_ndx] - l_uniform_param) / (p_curve_to_uniform_val_v[l_ndx] - p_curve_to_uniform_val_v[l_ndx - 1]); \
				l_w_2 = 1. - l_w_1;																		\
				p_uniform_to_curve_val_v[i] = l_t_1 * l_w_1 + l_t_2 * l_w_2;							\
			}																							\
			return true;																				\
		}																								\
		/* if curve reversed */																			\
		l_ndx = 1;																						\
		for (uint64_t i = 1; i < l_total_interval_count; ++i)											\
		{																								\
			T l_uniform_param = p_start + ((T)i) * l_step;												\
			while (l_uniform_param < p_curve_to_uniform_val_v[l_ndx])/* differense beatween forwardand reeverse is here */	\
			{																							\
				l_ndx++;																				\
			}																							\
			l_t_1 = p_start + ((T)(l_ndx - 1)) * l_step;												\
			l_t_2 = p_start + ((T)(l_ndx)) * l_step;													\
			l_w_1 = (p_curve_to_uniform_val_v[l_ndx] - l_uniform_param) / (p_curve_to_uniform_val_v[l_ndx] - p_curve_to_uniform_val_v[l_ndx - 1]);	\
			l_w_2 = 1. - l_w_1;																			\
																										\
			p_uniform_to_curve_val_v[i] = l_t_1 * l_w_1 + l_t_2 * l_w_2;								\
		}																								\
		return true;																					\
	}

#define VF_MATH_GET_PARAM_COMPONENT																		\
		virtual T			get_param_by_vector_component(	T			p_value,														\
															uint32_t	p_component_index = 0/*x by default*/,							\
															T			p_start = 0,													\
															T			p_end = 1 /*if p_start == p_end then interval is infinite*/,	\
															uint32_t	p_divisions = 10,												\
															T			p_percition = vufCurve_kTol)	const override					\
	{																									\
		/* To avoid crush we increment p_division.Never divide by zero. */								\
		p_divisions++;																					\
		T l_interval_step = (p_end - p_start) / (T)p_divisions;											\
		T l_res_param = p_start;																		\
		T l_close_value = get_pos_at_i(l_res_param)[p_component_index];									\
		T l_err = abs(p_value - l_close_value);															\
		if (l_err < p_percition)																		\
		{																								\
			return  l_res_param;																		\
		}																								\
																										\
		for (uint32_t i = 0; i < p_divisions; ++i)														\
		{																								\
			T l_param_1 = p_start + ((T)i) * l_interval_step;											\
			T l_param_2 = p_start + ((T)(i + 1)) * l_interval_step;										\
			T l_param_21 = (l_param_1 + l_param_2) * 0.5;												\
																										\
			T l_x_1 = get_pos_at_i(l_param_1)[p_component_index];										\
			T l_x_2 = get_pos_at_i(l_param_2)[p_component_index];										\
			T l_x_21 = get_pos_at_i(l_param_21)[p_component_index];										\
																										\
			T l_err_1 = abs(l_x_1 - p_value);															\
			T l_err_2 = abs(l_x_2 - p_value);															\
			T l_err_21 = abs(l_x_21 - p_value);															\
																										\
			if (l_err_1 < l_err)																		\
			{																							\
				l_err = l_err_1;																		\
				l_res_param = l_param_1;																\
			}																							\
			if (l_err_2 < l_err)																		\
			{																							\
				l_err = l_err_2;																		\
				l_res_param = l_param_2;																\
			}																							\
			if (l_err_21 < l_err)																		\
			{																							\
				l_err = l_err_21;																		\
				l_res_param = l_param_21;																\
			}																							\
																										\
			if (l_err < p_percition)																	\
			{																							\
				return l_res_param;																		\
			}																							\
																										\
			if ((p_value < l_x_1 && p_value < l_x_2) || (l_x_2 < p_value && l_x_1 < p_value))			\
			{																							\
				continue;																				\
			}																							\
			/*/ check are these point satisfy	 */														\
			if ((l_x_1 < p_value && p_value < l_x_21) || (l_x_21 < p_value && p_value < l_x_1))			\
			{																							\
				return get_param_by_vector_component_i(p_value, p_component_index, l_x_1, l_param_1, l_x_21, l_param_21, p_percition);	\
			}																							\
			if ((l_x_21 < p_value && p_value < l_x_2) || (l_x_2 < p_value && p_value < l_x_21))			\
			{																							\
				return get_param_by_vector_component_i(p_value, p_component_index, l_x_21, l_param_21, l_x_2, l_param_2, p_percition);	\
			}																							\
		}																								\
		return l_res_param;																				\
	}																									\
T					get_param_by_vector_component_i(T			p_value,								\
													uint32_t	p_component_index = 0/*x by default*/,	\
													T			p_x_1 = 0,								\
													T			p_t_1 = 0,								\
													T			p_x_2 = 1,								\
													T			p_t_2 = 1,								\
													T			p_percition = vufCurve_kTol)	const	\
	{																									\
		T l_middle_param = (p_t_1 + p_t_2) * 0.5;														\
		T l_middle_value = get_pos_at_i(l_middle_param)[p_component_index];								\
		if (abs(l_middle_value - p_value) < p_percition)												\
		{																								\
			return l_middle_param;																		\
		}																								\
		if ((p_x_1 < p_value && p_value < l_middle_value) || (l_middle_value < p_value && p_value < p_x_1))									\
		{																																	\
			return  get_param_by_vector_component_i(p_value, p_component_index, p_x_1, p_t_1, l_middle_value, l_middle_param, p_percition);	\
		}																																	\
		if ((l_middle_value < p_value && p_value < p_x_2) || (p_x_2 < p_value && p_value < l_middle_value))									\
		{																																	\
			return get_param_by_vector_component_i(p_value, p_component_index, l_middle_value, l_middle_param, p_x_2, p_t_2, p_percition);	\
		}																																	\
		return 0;																															\
	}

//#define VF_MATH_CRV_GET_CLOSEST_PARAM_I_BODY															\


#endif // !VF_MATH_CRV_INCLUDE_H
