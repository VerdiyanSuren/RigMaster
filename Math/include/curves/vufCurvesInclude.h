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

//implementation from blend curve
#define VF_MATH_CRV_DEFINE_CLOSEST_POINTS																\
virtual V<T>			get_closest_point(	const V<T>&	p_point,										\
											T			p_start = 0,									\
											T			p_end = 1,										\
											uint32_t	p_divisions = 10,								\
											T p_percition = vufCurve_kTol) const override				\
{																										\
	return get_pos_at_i(get_closest_point_param_i(p_point, p_start, p_end, p_divisions, p_percition));	\
}																										\
virtual T				get_closest_point_param(const V<T>& p_point,									\
												T				p_start = 0,							\
												T				p_end = 1,								\
												uint32_t		p_divisions = 10,						\
												T				p_percition = vufCurve_kTol) const override	\
{																										\
	return get_closest_point_param_i(p_point, p_start, p_end, p_divisions, p_percition);				\
}																										\
inline T	get_closest_point_param_i(	const V<T>& p_point,											\
										T			p_start = 0.0,										\
										T			p_end = 1.0,										\
										uint32_t	p_divisions = 10,									\
										T			p_percition = vufCurve_kTol) const					\
{																										\
	p_divisions++;																						\
	T l_interval_step = (p_end - p_start) / (T)p_divisions;												\
	T		l_start = p_start;																			\
	V<T>	l_start_pos = get_pos_at_i(l_start) - p_point;												\
	T		l_dot_start = l_start_pos.dot(get_tangent_at_i(l_start));									\
	T		l_sign_start = (VF_ABS(l_dot_start)) > VF_MATH_EPSILON ? l_dot_start : 0.0;					\
	T l_dist_min = l_start_pos.length2();																\
	T l_param_min = p_start;																			\
	for (uint32_t i = 1; i <= p_divisions; ++i)															\
	{																									\
		T		l_end = p_start + (T)i * l_interval_step;												\
		V<T>	l_end_pos = get_pos_at_i(l_end) - p_point;												\
		T		l_dot_end = l_end_pos.dot(get_tangent_at_i(l_end));										\
		T		l_sign_end = (VF_ABS(l_dot_end)) > VF_MATH_EPSILON ? l_dot_end : 0;						\
		T		l_dist_end = l_end_pos.length2();														\
																										\
		if (l_dist_end < l_dist_min)																	\
		{																								\
			l_param_min = l_end;																		\
			l_dist_min = l_dist_end;																	\
		}																								\
		T l_t_1 = l_start;																				\
		T l_t_2 = l_end;																				\
		T l_s_1 = l_sign_start;																			\
		T l_s_2 = l_sign_end;																			\
		while (l_s_1 * l_s_2 < 0.0 && (l_t_2 - l_t_1) > p_percition)									\
		{																								\
			T		l_t = (l_t_1 + l_t_2) * 0.5;														\
			V<T>	l_t_pos = get_pos_at_i(l_t) - p_point;												\
			T		l_t_dot = l_t_pos.dot(get_tangent_at_i(l_t));										\
			T		l_t_sign = (VF_ABS(l_t_dot)) > VF_MATH_EPSILON ? l_t_dot : 0.0;						\
			if (l_t_sign == 0)																			\
			{																							\
				T l_t_dist = l_t_pos.length2();															\
				if (l_t_dist < l_dist_min)																\
				{																						\
					l_dist_min = l_t_dist;																\
					l_param_min = l_t;																	\
				}																						\
				break;																					\
			}																							\
			if (l_t_sign * l_s_1 < 0.0)																	\
			{																							\
				l_t_2 = l_t;																			\
				l_s_2 = l_t_sign;																		\
				continue;																				\
			}																							\
			l_t_1 = l_t;																				\
			l_s_1 = l_t_sign;																			\
		}																								\
		l_end_pos = get_pos_at_i(l_t_1) - p_point;														\
		l_dist_end = l_end_pos.length2();																\
		if (l_dist_end < l_dist_min)																	\
		{																								\
			l_param_min = l_t_1;																		\
			l_dist_min = l_dist_end;																	\
		}																								\
																										\
		l_start = l_end;																				\
		l_sign_start = l_sign_end;																		\
	}																									\
	return l_param_min;																					\
}



#define VF_MATH_CRV_REBUILD_CLAMPED																		\
virtual bool		rebuild(																			\
								std::vector<T>& p_uniform_to_curve_val_v,								\
								std::vector<T>& p_curve_to_uniform_val_v,								\
								std::vector<T>& p_curve_val_to_length_v,								\
								uint32_t		p_division_count = 10,									\
								T				p_start = 0 /*interval on which we need rebuild*/,		\
								T				p_end = 1) const override								\
	{																									\
		p_start = VF_CLAMP(0.0, 1.0, p_start);															\
		p_end = VF_CLAMP(0.0, 1.0, p_end);																\
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

#define VF_MATH_DEFINE_PARAM_COMPONENT																		\
virtual T	get_param_by_vector_component(	T			p_value,										\
											uint32_t	p_component_index = 0,							\
											T			p_start = 0,									\
											T			p_end = 1 ,										\
											uint32_t	p_divisions = 10,								\
											T			p_percition = vufCurve_kTol)	const override	\
{																										\
	return get_param_by_vector_component_i(p_value, p_component_index, p_start, p_end, p_divisions, p_percition);	\
}																										\
inline T	get_param_by_vector_component_i(T			p_value,										\
											uint32_t	p_component_index = 0,							\
											T			p_start = 0,									\
											T			p_end = 1 ,										\
											uint32_t	p_divisions = 10,								\
											T			p_percition = vufCurve_kTol)	const			\
{																										\
	p_divisions++;																						\
	T	l_interval_step = (p_end - p_start) / (T)p_divisions;											\
																										\
	T	l_start = p_start;																				\
	T	l_start_val = get_pos_at_i(l_start)[p_component_index] - p_value;								\
	T	l_start_err = VF_ABS(l_start_val);																\
																										\
	T l_param_res = l_start;																			\
	T l_param_err = l_start_err;																		\
	for (uint32_t i = 1; i <= p_divisions; ++i)															\
	{																									\
		T		l_end = p_start + (T)i * l_interval_step;												\
		T		l_end_val = get_pos_at_i(l_end)[p_component_index] - p_value;							\
		T		l_end_err = VF_ABS(l_end_val);															\
																										\
		if (l_end_err < l_param_err)																	\
		{																								\
			l_param_res = l_end;																		\
			l_param_err = l_end_err;																	\
		}																								\
		T l_t_1 = l_start;																				\
		T l_t_2 = l_end;																				\
		T l_v_1 = l_start_val;																			\
		T l_v_2 = l_end_val;																			\
		while (l_v_1 * l_v_2 < 0.0 && (l_t_2 - l_t_1) > p_percition)									\
		{																								\
			T		l_t = (l_t_1 + l_t_2) * 0.5;														\
			T		l_t_val = get_pos_at_i(l_t)[p_component_index] - p_value;							\
			T		l_t_err = VF_ABS(l_t_val);															\
																										\
			if (l_t_err == 0)																			\
			{																							\
				return l_t;																				\
			}																							\
			if (l_t_val * l_v_1 < 0.0)																	\
			{																							\
				l_t_2 = l_t;																			\
				l_v_2 = l_t_val;																		\
				continue;																				\
			}																							\
			l_t_1 = l_t;																				\
			l_v_1 = l_t_val;																			\
		}																								\
		l_start_val = get_pos_at_i(l_t_1)[p_component_index] - p_value;									\
		l_start_err = VF_ABS(l_start_val);																\
		if (l_start_err < l_param_err)																	\
		{																								\
			l_param_res = l_t_1;																		\
			l_param_err = l_start_err;																	\
		}																								\
		l_start = l_end;																				\
		l_start_err = l_end_err;																		\
		l_start_val = l_end_val;																		\
	}																									\
	return l_param_res;																					\
}

#endif // !VF_MATH_CRV_INCLUDE_H
