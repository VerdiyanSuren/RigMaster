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
		// inherited virtual methods from vufCurve class
		virtual bool			rebuild(std::vector<T>& p_uniform_to_curve_val_v,
										std::vector<T>& p_curve_to_uniform_val_v,
										std::vector<T>& p_curve_val_to_length_v,
										uint32_t		p_divisions = 10,
										T				p_start = 0 /*interval on which we need rebuild*/,
										T				p_end = 1) const override
		{
			// To Do 
			// Implement
			if (m_valid == false || abs(p_end - p_start) < vufCurve_kTol)
			{
				return false;
			}
			return false;
		}

		// non-virtual methods
		inline  T	get_closest_point_param_i(const V<T>& p_point, T p_start_param, T p_percition = 0.00001) const
		{
			if (m_first_container_ptr != nullptr)
			{
				if (m_second_container_ptr != nullptr)
				{
					T l_param_1 = m_first_container_ptr->get_curve_ptr()->get_closest_point_param(  p_point );
					T l_param_2 = m_second_container_ptr->get_curve_ptr()->get_closest_point_param( p_point );
					return  l_param_1 * (1. - m_weight) +   l_param_2  * m_weight;
				}
				return m_first_container_ptr->get_curve_ptr()->get_closest_point_param(p_point);
			}
			if (m_second_container_ptr != nullptr)
			{
				return m_second_container_ptr->get_curve_ptr()->get_closest_point_param(p_point);
			}
			return 0;
		}	
		inline T	get_closest_point_param_on_interval_i(	const V<T>& p_point,
															T p_t_1, T p_t_2, T p_percition = 0.00001) const
		{
			if (m_first_container_ptr != nullptr)
			{
				if (m_second_container_ptr != nullptr)
				{
					T l_param_1, l_param_2;
					l_param_1 = m_first_container_ptr->get_curve_ptr()->get_closest_point_param_on_interval(p_point, p_t_1, p_t_2, p_percition);
					l_param_2 = m_second_container_ptr->get_curve_ptr()->get_closest_point_param_on_interval(p_point, p_t_1, p_t_2, p_percition);
					return  l_param_1 * (1. - m_weight) + l_param_2 * m_weight;
				}
				return m_first_container_ptr->get_curve_ptr()->get_closest_point_param_on_interval(p_point, p_t_1, p_t_2, p_percition);
			}
			if (m_second_container_ptr != nullptr)
			{
				return m_second_container_ptr->get_curve_ptr()->get_closest_point_param_on_interval(p_point, p_t_1, p_t_2, p_percition);
			}
			return false;
		}
		void				set_container_ptr_1(std::shared_ptr < vufCurveContainer<T, V>> p_ptr)
		{
			m_first_container_ptr = p_ptr;
			vufCurve<T, V>::m_valid = m_first_container_ptr != nullptr && m_second_container_ptr != nullptr;
		}
		void				set_container_ptr_2(std::shared_ptr < vufCurveContainer<T, V>> p_ptr)
		{
			m_second_container_ptr = p_ptr;
			vufCurve<T, V>::m_valid = m_first_container_ptr != nullptr && m_second_container_ptr != nullptr;
		}
		
		std::shared_ptr < vufCurveContainer<T, V>> get_container_ptr_1() const
		{
			return m_first_container_ptr;
		}
		std::shared_ptr < vufCurveContainer<T, V>> get_container_ptr_2() const
		{
			return m_second_container_ptr;
		}

		void				set_weight(T p_val) { m_weight = p_val; }
		T					get_weight() const { return m_weight; }

		virtual vufCurveType	get_curve_type()			const override
		{
			return vufCurveType::k_blend_curve;
		}
		virtual int				get_curve_category()		const override
		{
			return k_compound_category;
		}
		virtual V<T>			get_pos_at(T p_t)			const override
		{
			if (m_first_container_ptr != nullptr)
			{
				if (m_second_container_ptr != nullptr)
				{
					return m_first_container_ptr->get_pos_at(p_t) * (1. - m_weight) + m_second_container_ptr->get_pos_at(p_t) * m_weight;
				}
				return m_first_container_ptr->get_pos_at(p_t);
			}
			if (m_second_container_ptr != nullptr)
			{
				return m_second_container_ptr->get_pos_at(p_t);
			}
			return V<T>();
		}
		virtual V<T>			get_tangent_at(T p_t)		const override
		{
			if (m_first_container_ptr != nullptr)
			{
				if (m_second_container_ptr != nullptr)
				{
					return m_first_container_ptr->get_tangent_at(p_t) * (1. - m_weight) + m_second_container_ptr->get_tangent_at(p_t) * m_weight;
				}
				return m_first_container_ptr->get_tangent_at(p_t);
			}
			if (m_second_container_ptr != nullptr)
			{
				return m_second_container_ptr->get_tangent_at(p_t);
			}
			return V<T>();
		}
		/** set and get control point position*/
		virtual V<T>			get_closest_point(	const V<T>& p_point,
													T			p_start		= 0,
													T			p_end		= 1,
													uint32_t	p_divisions = 10,
													T			p_percition = vufCurve_kTol) const override
		{
			// To Do 
			// implement this
			return V<T>();
		}
		virtual T	get_closest_point_param(	const V<T>& p_point, 
												T p_start = 0,
												T p_end = 1 /*if p_start == p_end then interval is infinite*/, 
												uint32_t p_divisions = 10, 
												T p_percition = 0.00001) const override
											{
												return get_closest_point_param_i(p_point, p_divisions, p_percition);
											}

		virtual T				get_param_by_vector_component(	T			p_value,
																uint32_t	p_component_index = 0/*x by default*/,
																T			p_start = 0,
																T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
																uint32_t	p_divisions = 10,
																T			p_percition = vufCurve_kTol)	const override
		{
			// To Do 
			// Implement this
			return 0;
		}
		/*
		virtual T	get_closest_point_param_on_interval(const V<T>& p_point,T p_t_1, T p_t_2, T p_percition = 0.00001) const  override
		{
			return get_closest_point_param_on_interval_i(	p_point, 
															p_t_1, p_t_1, p_percition);
		}
		*/
		//virtual int			get_interval_count() const = 0;
		/// Get copy of this curve.	Original curve is unchenged
		virtual std::shared_ptr<vufCurve<T, V>> get_copy() const override
		{
			std::shared_ptr< vufCurveBlend > l_ptr = vufCurveBlend::create();

			l_ptr->m_degree = vufCurve<T, V>::m_degree;
			l_ptr->m_close = vufCurve<T, V>::m_close;
			l_ptr->m_valid = vufCurve<T, V>::m_valid;

			l_ptr->m_first_container_ptr  = m_first_container_ptr == nullptr ? nullptr : m_first_container_ptr->get_copy();
			l_ptr->m_second_container_ptr = m_second_container_ptr == nullptr ? nullptr : m_second_container_ptr->get_copy();

			l_ptr->m_weight = m_weight;

			return l_ptr;
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			// To Do 
			// Implement This
			return std::string();
		}
		virtual uint64_t		get_binary_size() const override
		{
			// To Do 
			// Implement This
			return  0;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			// To Do 
			// Implement This
			return 0;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& l_version, uint64_t p_offset = 0)		override
		{
			// To Do 
			// Implement This
			return 0;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			// To Do 
			// Implement This
			return 0;
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		override
		{
			// To Do 
			// Implement This
			return 0;
		}

		virtual std::shared_ptr<vufCurveBlend <T,V>>		as_curve_blend()	const override 
		{ 
			return std::static_pointer_cast<vufCurveBlend<T, V>>(vufCurve<T, V>::m_this.lock());
		}
	private:
		std::shared_ptr< vufCurveContainer<T, V> >	m_first_container_ptr	= nullptr;
		std::shared_ptr< vufCurveContainer<T, V> >	m_second_container_ptr	= nullptr;
		std::shared_ptr< vufCurveContainer<T, V> >	m_blend_func_ptr		= nullptr;
		T											m_weight = 0;
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