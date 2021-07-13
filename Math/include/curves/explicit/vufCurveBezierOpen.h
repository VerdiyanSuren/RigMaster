#ifndef VF_MATH_OPEN_BEZER_OPN_CRV_H
#define VF_MATH_OPEN_BEZER_OPN_CRV_H

#include <typeinfo>
#include <iostream>
#include <curves/explicit/vufCurveExplicit.h>
#include <curves/vufCurvesInclude.h>
#include <vufNumericArrayObject.h>

namespace vufMath
{
	template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>
	class vufCurveOpenBezier : public vufCurveExplicit<T, V>
	{
	private:
		vufCurveOpenBezier() :vufCurveExplicit<T, V>()
		{
			//std::cout << "OpenBSpline constructor" << std::endl;
			vufCurve<T, V>::m_has_degree = true;
			vufCurve<T, V>::m_degree = CURVE_DEGREE;
			vufCurve<T, V>::m_close = false;
		}
	public:
		virtual ~vufCurveOpenBezier() {}
		VF_MATH_CURVE_DEFINE_CREATOR(vufCurveOpenBezier);
		VF_MATH_CURVE_DEFINE_TYPE_CATEGORY(k_open_bezier_piecewise, k_bezier_category);
		virtual bool			rebuild(std::vector<T>& p_uniform_to_curve_val_v,
			std::vector<T>& p_curve_to_uniform_val_v,
			std::vector<T>& p_curve_val_to_length_v,
			uint32_t		p_divisions = 10,
			T				p_start = 0 /*interval on which we need rebuild*/,
			T				p_end = 1) const override 
		{
			// TO DO
			return false;
		}

		virtual bool			rebuild_along_axis(	const V<T>&		p_axis/*project curve on this axis*/,
													std::vector<T>& p_uniform_to_curve_val_v,
													std::vector<T>& p_curve_to_uniform_val_v,
													std::vector<T>& p_curve_val_to_length_v,
													uint32_t		p_division_count = 10,
													T				p_start = 0 /*interval on which we need rebuild*/,
													T				p_end = 1) const override
		{
			// TO DO
			return false;
		}


		virtual V<T>			get_closest_point(	const V<T>& p_point,
													T			p_start = 0,
													T			p_end = 1,
													uint32_t	p_divisions = 10,
													T p_percition = vufCurve_kTol) const override
		{
			// TO DO
			return V<T>();
		}
		virtual T				get_closest_point_param(const V<T>& p_point,
														T			p_start = 0,
														T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
														uint32_t	p_divisions = 10,
														T	p_percition = vufCurve_kTol) const override
		{
			// TO DO
			return 0;
		}

		virtual T				get_param_by_vector_component(	T			p_value,
																uint32_t	p_component_index = 0/*x by default*/,
																T			p_start = 0,
																T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
																uint32_t	p_divisions = 10,
																T			p_percition = vufCurve_kTol)	const override
		{
			// TO DO
			return 0;
		}
		virtual V<T>			get_pos_at(T p_t)					const override
		{
			return get_pos_at_i(p_t);
		}
		virtual V<T>			get_tangent_at(T p_t)				const override
		{
			return get_tangent_at_i(p_t);
		}
		virtual V<T>			get_tangent_normalized_at(T p_t)	const override
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}


		virtual bool		set_nodes_count(uint32_t p_count) override
		{
			// reserve elements from start and end
			if (vufCurveExplicit<T, V>::m_nodes_pos_v.size() != p_count)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v.resize(p_count);
				return init_knot_vector_i();
			}
			return false;
		}
		virtual uint32_t	get_nodes_count() const override
		{
			return m_nodes_count;
			return (uint32_t)vufCurveExplicit<T, V>::m_nodes_pos_v.size();
		}
		virtual void		set_node_at(uint32_t p_index, const V<T>& p_vector) override
		{
			vufCurveExplicit<T, V>::m_nodes_pos_v[p_index] = p_vector;
		}
		virtual V<T>		get_node_at(uint32_t p_index) const override
		{
			return vufCurveExplicit<T, V>::m_nodes_pos_v[p_index];
		}
		virtual T			get_interval_t_min(int p_interval_index) const override
		{
			return 0;
		}
		virtual T			get_interval_t_max(int p_interval_index) const override
		{
			return 0;
		}
		virtual int			get_interval_index(T p_t) const override { return 0; }
		virtual uint32_t	get_interval_count() const override { return 0; }


		virtual std::shared_ptr<vufCurve<T, V>>		get_copy() const override
		{
			std::shared_ptr< vufCurveOpenBezier > l_ptr = vufCurveOpenBezier::create();

			l_ptr->m_degree = vufCurve<T, V>::m_degree;
			l_ptr->m_close = vufCurve<T, V>::m_close;
			l_ptr->m_valid = vufCurve<T, V>::m_valid;

			l_ptr->m_nodes_pos_v = vufCurveExplicit<T, V>::m_nodes_pos_v;
			l_ptr->m_knot_v = m_knot_v;
			return l_ptr;
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)	const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			
			return l_ss.str();
		}
		//virtual uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0) override;
		virtual uint64_t		get_binary_size()													const override
		{
			uint64_t l_size = 0;
			l_size += vufCurveExplicit<T, V>::get_binary_size();
			return  l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)			const override
		{
			//std::cout << "-------------------------------------OPEN_BSPLINE::to_binary()--------------------- " << std::endl;
			uint64_t l_curve_size = get_binary_size();
			//------------------------------------------------------
			// resize if needed
			if (p_buff.size() < p_offset + l_curve_size)
			{
				p_buff.resize(p_offset + l_curve_size);
			}
			//------------------------------------------------------
			p_offset = vufCurveExplicit<T, V>::to_binary(p_buff, p_offset);
			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0) override
		{
			vufCurve<T, V>::m_valid = false;
			p_offset = vufCurveExplicit<T, V>::from_binary(p_buff, p_version, p_offset);
			if (p_offset == 0)
			{
				vufCurve<T, V>::m_valid = false;
				return 0;
			};
			if (init_knot_vector_i() == true )
			{
				vufCurve<T, V>::m_valid = true;
			}
			else
			{
				vufCurve<T, V>::m_valid = false;
			}
			return p_offset;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)	const override
		{
			return vufCurve<T, V>::encode_to_buff(p_buff, p_offset);
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0) override
		{
			return vufCurve<T, V>::decode_from_buff(p_buff, p_offset);
		}




		bool				init_knot_vector_i()
		{
			uint32_t l_sz = (uint32_t)vufCurveExplicit<T, V>::m_nodes_pos_v.size();
			if ((l_sz < CURVE_DEGREE + 1) )
			{
				vufCurve<T, V>::m_valid = false;
				return false;
			}
			m_interval_count = (l_sz -1) / CURVE_DEGREE;
			m_nodes_count = m_interval_count * CURVE_DEGREE + 1;
			m_knot_v.resize(m_interval_count);
			T l_t_min = 0.0;
			T l_t_max = 1.0;
			m_interval_length = 1. / (T)(m_interval_count);
			for (uint32_t i = 0; i < m_interval_count; ++i)
			{
				m_knot_v[i] = (T(i + 1)) * m_interval_length;
			}
			vufNumericArrayFn<T> l_fn(m_knot_v);
			std::cout << l_fn.to_string() << std::endl;
			std::cout << m_interval_length << std::endl;
			std::cout << m_interval_count << std::endl;
			std::cout << m_nodes_count << std::endl;

			vufCurve<T, V>::m_valid = true;
			return true;
		}
		inline T			get_interval_t_min_i(int p_interval_index) const
		{
			return p_interval_index == 0 ? 0.0 :m_knot_v[l_knot_index];
		}
		inline T			get_interval_t_max_i(int p_interval_index) const
		{
			return m_knot_v[l_knot_index + 1];
		}
		inline int			get_interval_index_i(T p_t) const
		{
			if (p_t < 0.0) return 0.;
			if (p_t > 1.0) return 1.;
			return int(p_t / m_interval_length);
		}
		inline uint32_t		get_interval_count_i() const
		{
			return ((int)m_knot_v.size() - 2 * (int)CURVE_DEGREE - 1);
		}
		inline V<T>			get_pos_at_i(T p_t)					const
		{
			return V<T>();
		}
		inline V<T>			get_tangent_at_i(T p_t) const
		{			
			return V<T>();
		}
		inline V<T>			get_tangent_normalized_at_i(T p_t)	const
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}

	private:
		std::vector<T>		m_knot_v;
		T					m_interval_length	= 0;
		uint32_t			m_interval_count	= 0;
		uint32_t			m_nodes_count		= 0;
	};
	//specializations
	template<>
	vufVector4<double> vufCurveOpenBezier<double, vufVector4, 1>::get_tangent_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return m_nodes_pos_v[1] - m_nodes_pos_v[0];
		}
		if (p_t >= 1.0)
		{
			return m_nodes_pos_v[m_nodes_count - 1] - m_nodes_pos_v[m_nodes_count - 2];
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = uint32_t(p_t);
		p_t -= l_ndx;

		return	m_nodes_pos_v[l_ndx + 1] - m_nodes_pos_v[l_ndx];
	}
	template<>
	vufVector4<double> vufCurveOpenBezier<double, vufVector4, 1>::get_pos_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = uint32_t(p_t);
		p_t -= l_ndx;
		return	m_nodes_pos_v[l_ndx] * (1 - p_t) + m_nodes_pos_v[l_ndx + 1] * p_t;
	}

	template<>
	vufVector4<double> vufCurveOpenBezier<double, vufVector4, 2>::get_tangent_at_i(double p_t) const
	{
		if (p_t <= 0)
		{
			return m_nodes_pos_v[0] * (-2.0)  + m_nodes_pos_v[1] * 2.0;
		}
		if (p_t >= 1.0)
		{
			return  m_nodes_pos_v[m_nodes_count-2] * (-2.0) + m_nodes_pos_v[m_nodes_count -1] * 2.0;

		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 2 * uint32_t(p_t);
		p_t -= uint32_t(p_t);
		return m_nodes_pos_v[l_ndx] * 2.0 * (p_t - 1) + m_nodes_pos_v[l_ndx + 1] * (2.0 - 4.0 * p_t) + m_nodes_pos_v[l_ndx + 2] * 2.0 * p_t;
	}
	template<>
	vufVector4<double> vufCurveOpenBezier<double, vufVector4, 2>::get_pos_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[m_nodes_count-1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 2 * uint32_t(p_t);
		p_t -= uint32_t(p_t);

		return	m_nodes_pos_v[l_ndx] * (1 - p_t) * (1 - p_t) + 2.0 * m_nodes_pos_v[l_ndx + 1] * p_t * (1 - p_t) + m_nodes_pos_v[l_ndx + 2] * p_t * p_t;
	}

	template<>
	vufVector4<double> vufCurveOpenBezier<double, vufVector4, 3>::get_tangent_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return 3.0 * (m_nodes_pos_v[1] - m_nodes_pos_v[0]);
		}
		if (p_t >= 1.0)
		{
			return 3.0 * (m_nodes_pos_v[m_nodes_count - 1] - m_nodes_pos_v[m_nodes_count - 2]);

		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 3 * uint32_t(p_t);
		p_t -= uint32_t(p_t);

		return 3.0 *(m_nodes_pos_v[l_ndx] * (p_t - 1) * (1 - p_t) +  m_nodes_pos_v[l_ndx + 1] * (1.0 - 4.0 * p_t + 3.0 * p_t * p_t) + 
				m_nodes_pos_v[l_ndx + 2] * p_t * (2.0 - 3.0 * p_t) + m_nodes_pos_v[l_ndx + 3] * p_t * p_t );
	}
	template<>
	vufVector4<double> vufCurveOpenBezier<double, vufVector4, 3>::get_pos_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 3 * uint32_t(p_t);
		p_t -= uint32_t(p_t);

		return m_nodes_pos_v[l_ndx] * (1 - p_t) * (1 - p_t) * (1 - p_t) + 3.0 * m_nodes_pos_v[l_ndx + 1] * p_t * (1 - p_t) * (1 - p_t) +
			3.0 * m_nodes_pos_v[l_ndx + 2] * p_t * p_t * (1 - p_t) + m_nodes_pos_v[l_ndx + 3] * p_t * p_t * p_t;
	}
}
#endif // !VF_MATH_OPEN_BEZER_OPN_CRV_H
