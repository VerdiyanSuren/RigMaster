#ifndef VF_MATH_CRV_BLEND_H
#define VF_MATH_CRV_BLEND_H

#include <curves/vufCurve.h>
//#include <curves/vufCurveContatiner.h>
#include <math/vufVector.h>
#include <memory>

namespace vufMath
{
	template<class T, template<typename> class V>		class vufCurveContainer;
	template <	class T, template<typename> class V>	class vufCurve;
	
	template <class T, template<typename> class V >
	class vufCurveBlend : public vufCurve<T, V>
	{
	private:
		vufCurveBlend() :vufCurve<T, V>()
		{
			vufCurve<T, V>::m_degree = 0;
			vufCurve<T, V>::m_close = false;
		}
	public:
		virtual ~vufCurveBlend() {}
		VF_MATH_CURVE_DEFINE_CREATOR(vufCurveBlend);
		VF_MATH_CURVE_DEFINE_TYPE_CATEGORY(k_blend_curve, k_compound_category);
		// inherited virtual methods from vufCurve class. Curve interface
		VF_MATH_CRV_REBUILD;
		virtual V<T>			get_closest_point(const V<T>&	p_point,
													T			p_start = 0,
													T			p_end = 1,
													uint32_t	p_divisions = 10,
													T			p_percition = vufCurve_kTol) const override
		{
			return get_pos_at_i(get_closest_point_param_i(p_point, p_start, p_end, p_divisions, p_percition));
		}
		virtual T				get_closest_point_param(const V<T>& p_point,
														T			p_start = 0,
														T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
														uint32_t	p_divisions = 10,
														T			p_percition = vufCurve_kTol) const override
		{
			return get_closest_point_param_i(p_point, p_start, p_end, p_divisions, p_percition);
		}

		virtual T				get_param_by_vector_component(	T			p_value,
																uint32_t	p_component_index = 0/*x by default*/,
																T			p_start = 0,
																T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
																uint32_t	p_divisions = 10,
																T			p_percition = vufCurve_kTol)	const override
		{
			return get_param_by_vector_component_i(p_value, p_component_index, p_start, p_end, p_divisions, p_percition);
		}

		virtual V<T>				get_pos_at(T p_t)			const override
		{
			return get_pos_at_i(p_t);
		}
		virtual V<T>				get_tangent_at(T p_t)		const override
		{
			return get_tangent_at_i(p_t);
		}
		virtual V<T>				get_tangent_normalized_at(T p_t)						const override
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}
		virtual vufQuaternion<T>	get_quaternion_at(T p_t)			const
		{
			if (m_first_container_ptr != nullptr && m_second_container_ptr != nullptr)
			{
				T l_weight = get_computed_weight_i(p_t);
				V<T> l_tngnt				= get_tangent_normalized_at(p_t);
				vufQuaternion<T>	l_q_a	= m_first_container_ptr->get_quaternion_at(p_t);
				vufQuaternion<T>	l_q_b	= m_second_container_ptr->get_quaternion_at(p_t);
				V<T>				l_v_1(1.0);
				V<T> l_v_a	= l_q_a.rotate_vector_by_quaternion(l_v_1);
				l_q_a		= l_q_a.increment_quaternion_with_2vectors(l_v_a, l_tngnt);
				V<T> l_v_b	= l_q_b.rotate_vector_by_quaternion(l_v_1);
				l_q_b		= l_q_b.increment_quaternion_with_2vectors(l_v_b, l_tngnt);
				if (l_q_a.dot(l_q_b) < 0.0)
				{
					l_q_b = -l_q_b;
				}
				vufQuaternion<T> l_q_r = l_q_a * (1. - l_weight) + l_q_b * l_weight;
				V<T> l_v_r = l_q_r.rotate_vector_by_quaternion(l_v_1);
				l_q_r = l_q_r.increment_quaternion_with_2vectors(l_v_r, l_tngnt);
				return l_q_r;
			}
			V<T> l_vec(1.0);
			V<T> l_tng = get_tangent_normalized_at(p_t);
			vufQuaternion<T> l_res = vufQuaternion<T>();
			return l_res.increment_quaternion_with_2vectors(l_vec, l_tng);
		}
		virtual V<T>				get_scale_at(T p_t)					const
		{
			if (m_first_container_ptr != nullptr && m_second_container_ptr != nullptr)
			{
				T l_weight = get_computed_weight_i(p_t);
				V<T> l_s_a = m_first_container_ptr->get_scale_at(p_t);
				V<T> l_s_b = m_second_container_ptr->get_scale_at(p_t);
				return l_s_a * (1. - l_weight) + l_s_b * l_weight;
			}
			return V<T>(1.0, 1.0, 1.0);
		}
		/// Get copy of this curve.	Original curve is unchenged
		virtual std::shared_ptr<vufCurve<T, V>> get_copy() const override
		{
			std::shared_ptr< vufCurveBlend > l_ptr = vufCurveBlend::create();
			l_ptr->copy_members_from_i(std::static_pointer_cast<vufCurveBlend>(m_this.lock()));

			return l_ptr;
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{			
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			l_ss << l_str_offset << "[ General Blend Curve <" << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl;
			l_ss << l_str_offset << "____Valid: " << m_valid << std::endl;
			l_ss << l_str_offset << "____Input Curve First: " << (m_first_container_ptr == nullptr ? "Absent" : m_first_container_ptr->to_string(p_precision, p_tab_count + 1)) << std::endl;
			l_ss << l_str_offset << "____Input Curve Second: " << (m_second_container_ptr == nullptr ? "Absent" : m_second_container_ptr->to_string(p_precision, p_tab_count + 1)) << std::endl;
			l_ss << l_str_offset << "____Input Curve Function: " << (m_blend_func_ptr == nullptr? "Absent": m_blend_func_ptr->to_string(p_precision, p_tab_count + 1))<< std::endl;
			l_ss << l_str_offset << "____Weight: " << m_weight << std::endl;
			l_ss << l_str_offset << "____Use Function: " << m_use_fcurve << std::endl;
			l_ss << l_str_offset << "____Should Serialize: Input_First: " << m_should_serialize_first << std::endl;
			l_ss << l_str_offset << "____Should Serialize: Input_Second: " << m_should_serialize_second << std::endl;
			l_ss << l_str_offset << "____Should Serialize: Input_Function: " << m_should_serialize_fcurve << std::endl;

			return l_ss.str();
		}
		virtual uint64_t		get_binary_size() const override
		{
			// To Do 
			// Implement This
			uint64_t l_size = vufCurve<T, V>::get_binary_size();
			
			// input first
			l_size += sizeof(bool);
			if (m_first_container_ptr != nullptr && m_should_serialize_first == true)
			{
				l_size += m_first_container_ptr->get_binary_size();
			}

			// input second
			l_size += sizeof(bool);
			if (m_second_container_ptr != nullptr && m_should_serialize_second == true)
			{
				l_size += m_second_container_ptr->get_binary_size();
			}

			// input function
			l_size += sizeof(bool);
			if (m_blend_func_ptr != nullptr && m_should_serialize_fcurve == true)
			{
				l_size += m_blend_func_ptr->get_binary_size();
			}

			l_size += sizeof(m_weight);
			l_size += sizeof(m_use_fcurve);
			return  0;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			// To Do 
			// Implement This
			uint64_t l_curve_size = get_binary_size();
			if (p_buff.size() < p_offset + l_curve_size)
			{
				p_buff.resize(p_offset + l_curve_size);
			}
			p_offset = vufCurve<T, V>::to_binary(p_buff, p_offset);
			// input first
			if (m_first_container_ptr != nullptr && m_should_serialize_first == true)
			{
				bool l_save = true;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
				p_offset = m_first_container_ptr->to_binary(p_buff, p_offset);
			}
			else
			{
				bool l_save = false;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
			}
			// input second
			if (m_second_container_ptr != nullptr && m_should_serialize_second == true)
			{
				bool l_save = true;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
				p_offset = m_second_container_ptr->to_binary(p_buff, p_offset);
			}
			else
			{
				bool l_save = false;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
			}
			// input function
			if (m_blend_func_ptr != nullptr && m_should_serialize_fcurve == true)
			{
				bool l_save = true;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
				p_offset = m_blend_func_ptr->to_binary(p_buff, p_offset);
			}
			else
			{
				bool l_save = false;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
			}

			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_weight, sizeof(m_weight));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_use_fcurve, sizeof(m_use_fcurve));
			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)		override
		{
			p_offset = vufCurve<T, V>::from_binary(p_buff, p_offset);
			
			bool l_load;
			// input first
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_load, sizeof(l_load));
			if (l_load == true)
			{
				m_should_serialize_first = true;
				m_first_container_ptr =  vufCurveContainer<T, V>::create();
				p_offset = m_first_container_ptr->from_binary(p_buff, p_offset);
			}
			else
			{
				m_should_serialize_first	= false;
				m_first_container_ptr		= nullptr;
			}
			// input second
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_load, sizeof(l_load));
			if (l_load == true)
			{
				m_should_serialize_second = true;
				m_second_container_ptr = vufCurveContainer<T, V>::create();
				p_offset = m_second_container_ptr->from_binary(p_buff, p_offset);
			}
			else
			{
				m_should_serialize_second = false;
				m_second_container_ptr = nullptr;
			}
			// input function
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_load, sizeof(l_load));
			if (l_load == true)
			{
				m_should_serialize_fcurve = true;
				m_blend_func_ptr = vufCurveContainer<T, V>::create();
				p_offset = m_blend_func_ptr->from_binary(p_buff, p_offset);
			}
			else
			{
				m_should_serialize_fcurve	= false;
				m_blend_func_ptr			= nullptr;
			}

			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_weight, sizeof(m_weight));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_use_fcurve, sizeof(m_use_fcurve));

			return 0;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			return vufCurve<T, V>::encode_to_buff(p_buff, p_offset);
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		override
		{
			return vufCurve<T, V>::decode_from_buff(p_buff, p_offset);
		}
		// virtual casts
		virtual std::shared_ptr<vufCurveBlend <T, V>>		as_curve_blend()	const override
		{
			return std::static_pointer_cast<vufCurveBlend<T, V>>(vufCurve<T, V>::m_this.lock());
		}

		// non-virtual methods
		inline V<T>		get_pos_at_i(T p_t)		const
		{
			T l_weight = get_computed_weight_i(p_t);
			if (m_first_container_ptr != nullptr && m_second_container_ptr != nullptr)
			{
				return m_first_container_ptr->get_pos_at(p_t) * (1. - l_weight) + m_second_container_ptr->get_pos_at(p_t) * l_weight;
			}
			return V<T>();
		}
		inline V<T>		get_tangent_at_i(T p_t)	const
		{
			T l_weight = get_computed_weight_i(p_t);
			V<T> l_pos_1 = get_pos_at_i(p_t);
			V<T> l_pos_2 = get_pos_at_i(p_t + 0.00001);
			return (l_pos_2 - l_pos_1) * (1. / 0.00001);
		}
		inline V<T>		get_tangent_normalized_at_i(T p_t) const
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}

		inline T	get_closest_point_param_i(	const V<T>& p_point, 
												T			p_start = 0.0 , 
												T			p_end = 1.0,
												uint32_t	p_divisions = 10,
												T			p_percition = vufCurve_kTol) const
		{
			p_divisions++;// to avoid divide by zero
			T l_interval_step = (p_end - p_start) / (T)p_divisions;
			// start point and min distance initialization
			T		l_start			= p_start;													// start param
			V<T>	l_start_pos		= get_pos_at_i(l_start) - p_point;							// direction from point to start point
			T		l_dot_start		= l_start_pos.dot(get_tangent_at_i(l_start));
			T		l_sign_start	=  (VF_ABS(l_dot_start)) > VF_MATH_EPSILON ? l_dot_start : 0.0;

			T l_dist_min		= l_start_pos.length2();//std::numeric_limits<double>::max();
			T l_param_min		= p_start;

			for (uint32_t i = 1; i <= p_divisions; ++i)
			{
				T		l_end = p_start + (T)i * l_interval_step;
				V<T>	l_end_pos = get_pos_at_i(l_end) - p_point;
				T		l_dot_end = l_end_pos.dot(get_tangent_at_i(l_end));
				T		l_sign_end = (VF_ABS(l_dot_end)) > VF_MATH_EPSILON ? l_dot_end : 0;
				T		l_dist_end = l_end_pos.length2();

				if (l_dist_end < l_dist_min)
				{
					l_param_min = l_end;
					l_dist_min	= l_dist_end;
				}
				T l_t_1 = l_start;
				T l_t_2 = l_end;
				T l_s_1 = l_sign_start;
				T l_s_2 = l_sign_end;
				while (l_s_1 * l_s_2 < 0.0 && (l_t_2 - l_t_1) > p_percition)
				{
					T		l_t			= (l_t_1 + l_t_2) * 0.5;
					V<T>	l_t_pos		= get_pos_at_i(l_t) - p_point;
					T		l_t_dot		= l_t_pos.dot(get_tangent_at_i(l_t));
					T		l_t_sign	= (VF_ABS(l_t_dot)) > VF_MATH_EPSILON ? l_t_dot : 0.0;
					if (l_t_sign == 0)
					{
						T l_t_dist = l_t_pos.length2();
						if (l_t_dist < l_dist_min)
						{
							l_dist_min = l_t_dist;
							l_param_min = l_t;
						}
						break;
					}
					if (l_t_sign * l_s_1 < 0.0)
					{
						l_t_2 = l_t;
						l_s_2 = l_t_sign;
						continue;
					}
					l_t_1 = l_t;
					l_s_1 = l_t_sign;
				}
				l_end_pos = get_pos_at_i(l_t_1) - p_point;
				l_dist_end = l_end_pos.length2();
				if (l_dist_end < l_dist_min)
				{
					l_param_min = l_t_1;
					l_dist_min	= l_dist_end;
				}

				l_start			= l_end;
				l_sign_start	= l_sign_end;
			}
			return l_param_min;
		}	
		inline T	get_param_by_vector_component_i(T			p_value,
													uint32_t	p_component_index = 0/*x by default*/,
													T			p_start = 0,
													T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
													uint32_t	p_divisions = 10,
													T			p_percition = vufCurve_kTol)	const
		{
			// To avoid crush we increment p_division. Never divide by zero.
			p_divisions++;
			T	l_interval_step = (p_end - p_start) / (T)p_divisions;

			T	l_start			= p_start;												// start param
			T	l_start_val		= get_pos_at_i(l_start)[p_component_index] - p_value;	// direction from point to start point
			T	l_start_err		= VF_ABS(l_start_val);

			T l_param_res = l_start;
			T l_param_err = l_start_err;
			for (uint32_t i = 1; i <= p_divisions; ++i)
			{
				T		l_end		= p_start + (T)i * l_interval_step;
				T		l_end_val	= get_pos_at_i(l_end)[p_component_index] - p_value;
				T		l_end_err	= VF_ABS(l_end_val);

				if (l_end_err < l_param_err)
				{
					l_param_res = l_end;
					l_param_err = l_end_err;
				}
				T l_t_1 = l_start;
				T l_t_2 = l_end;
				T l_v_1 = l_start_val;
				T l_v_2 = l_end_val;
				while (l_v_1 * l_v_2 < 0.0 && (l_t_2 - l_t_1) > p_percition)
				{
					T		l_t = (l_t_1 + l_t_2) * 0.5;
					T		l_t_val = get_pos_at_i(l_t)[p_component_index] - p_value;
					T		l_t_err = VF_ABS(l_t_val);

					if (l_t_err == 0)
					{
						return l_t;
					}
					if (l_t_val * l_v_1 < 0.0)
					{
						l_t_2 = l_t;
						l_v_2 = l_t_val;
						continue;
					}
					l_t_1 = l_t;
					l_v_1 = l_t_val;
				}
				l_start_val = get_pos_at_i(l_t_1)[p_component_index] - p_value;
				l_start_err = VF_ABS(l_start_val);
				if (l_start_err < l_param_err)
				{
					l_param_res = l_t_1;
					l_param_err = l_start_err;
				}
				l_start		= l_end;
				l_start_err = l_end_err;
				l_start_val = l_end_val;
			}
			return l_param_res;
		}

		void		set_container_ptr_1(std::shared_ptr < vufCurveContainer<T, V>> p_ptr)
		{
			m_first_container_ptr = p_ptr;
			vufCurve<T, V>::m_valid = m_first_container_ptr != nullptr && m_second_container_ptr != nullptr;
		}
		void		set_container_ptr_2(std::shared_ptr < vufCurveContainer<T, V>> p_ptr)
		{
			m_second_container_ptr = p_ptr;
			vufCurve<T, V>::m_valid = m_first_container_ptr != nullptr && m_second_container_ptr != nullptr;
		}
		void		set_container_ptr_blend_func(std::shared_ptr<vufCurveContainer<T, V>> p_ptr)
		{
			m_blend_func_ptr = p_ptr;
		}
		std::shared_ptr < vufCurveContainer<T, V>> get_container_ptr_1() const
		{
			return m_first_container_ptr;
		}
		std::shared_ptr < vufCurveContainer<T, V>> get_container_ptr_2() const
		{
			return m_second_container_ptr;
		}
		std::shared_ptr < vufCurveContainer<T, V>> get_container_ptr_blend_func() const
		{
			return m_blend_func_ptr;
		}

		void				set_use_fcurve_i(bool p_val) { m_use_fcurve = p_val; }
		void				set_weight_i(T p_val) { m_weight = p_val; }
		bool				get_use_fcurve_i()	const { return  m_use_fcurve; }
		T					get_weight_i()		const { return m_weight; }
		
		
		void make_input_first_unique()
		{
			if (m_first_container_ptr != nullptr)
			{
				m_first_container_ptr = m_first_container_ptr->get_copy();
			}
		}
		void make_input_second_unique()
		{
			if (m_second_container_ptr != nullptr)
			{
				m_second_container_ptr = m_second_container_ptr->get_copy();
			}
		}
		void make_blend_func_unique()
		{
			if (m_blend_func_ptr != nullptr)
			{
				m_blend_func_ptr = m_blend_func_ptr->get_copy();
			}
		}
		// mark curves as seriaslizable
		void				make_selializable_first(bool p_val)		{ m_should_serialize_first = p_val;  }
		void				make_selializable_second(bool p_val)	{ m_should_serialize_second = p_val; }
		void				make_selializable_fcurve(bool p_val)	{ m_should_serialize_fcurve = p_val; }
		// check if curve serializable
		bool				is_serializable_first() const	{ return m_should_serialize_first; }
		bool				is_serializable_second() const	{ return m_should_serialize_second; }
		bool				is_serializable_fcurve() const	{ return m_should_serialize_fcurve; }
		/* get computed weight: weight + function curve(t) */
		inline T			get_computed_weight_i(T p_param) const
		{
			T l_weight = m_weight;
			if (m_blend_func_ptr != nullptr && m_use_fcurve == true)
			{
				l_weight *= m_blend_func_ptr->get_pos_at(m_blend_func_ptr->get_param_by_vector_component(p_param, 0, m_domain_min, m_domain_max,1))[1];
			}
			l_weight = VF_CLAMP(0.0, 1.0, l_weight);
			return l_weight;
		}
		/** set and get control point position*/
		/*
		virtual T	get_closest_point_param_on_interval(const V<T>& p_point,T p_t_1, T p_t_2, T p_percition = 0.00001) const  override
		{
			return get_closest_point_param_on_interval_i(	p_point, 
															p_t_1, p_t_1, p_percition);
		}
		*/
		//virtual int			get_interval_count() const = 0;
		/// Get copy of this curve.	Original curve is unchenged


		void		copy_members_from_i(std::shared_ptr<vufCurveBlend> p_crv)
		{
			vufCurve<T, V>::copy_members_from_i(p_crv);

			m_first_container_ptr		= p_crv->m_first_container_ptr == nullptr ? nullptr : p_crv->m_first_container_ptr->get_copy();
			m_second_container_ptr		= p_crv->m_second_container_ptr == nullptr ? nullptr : p_crv->m_second_container_ptr->get_copy();
			m_blend_func_ptr			= p_crv->m_blend_func_ptr == nullptr ? nullptr : p_crv->m_blend_func_ptr->get_copy();
			m_weight					= p_crv->m_weight;
			m_use_fcurve				= p_crv->m_use_fcurve;
			m_should_serialize_first	= p_crv->m_should_serialize_first;
			m_should_serialize_second	= p_crv->m_should_serialize_second;
			m_should_serialize_fcurve	= p_crv->m_should_serialize_fcurve;

		}

	private:
		std::shared_ptr< vufCurveContainer<T, V> >	m_first_container_ptr	= nullptr;
		std::shared_ptr< vufCurveContainer<T, V> >	m_second_container_ptr	= nullptr;
		std::shared_ptr< vufCurveContainer<T, V> >	m_blend_func_ptr		= nullptr;
		T											m_weight = 0;
		bool										m_use_fcurve = true;
		bool										m_should_serialize_first	= false;
		bool										m_should_serialize_second	= false;
		bool										m_should_serialize_fcurve	= false;
	};	

#pragma region USING
	using vufCurveBlend_2f = vufCurveBlend<float,  vufVector2>;
	using vufCurveBlend_2d = vufCurveBlend<double, vufVector2>;
	using vufCurveBlend_3f = vufCurveBlend<float,  vufVector3>;
	using vufCurveBlend_3d = vufCurveBlend<double, vufVector3>;
	using vufCurveBlend_4f = vufCurveBlend<float,  vufVector4>;
	using vufCurveBlend_4d = vufCurveBlend<double, vufVector4>;
#pragma endregion

}
#endif // !VF_MATH_CRV_BLEND_H