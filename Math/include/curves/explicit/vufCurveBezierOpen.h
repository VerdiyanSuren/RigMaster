#ifndef VF_MATH_OPEN_BEZER_OPN_CRV_H
#define VF_MATH_OPEN_BEZER_OPN_CRV_H

#include <typeinfo>
#include <iostream>
#include <curves/explicit/vufCurveExplicit.h>
#include <curves/vufCurvesInclude.h>
#include <vufNumericArrayObject.h>
#include <math/vufPolinom.h>
#include <coreUtils\vufTimer.h>
namespace vufMath
{
	template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>
	class vufCurveBezierOpen : public vufCurveExplicit<T, V>
	{
	private:
		vufCurveBezierOpen() :vufCurveExplicit<T, V>()
		{
			//std::cout << "OpenBSpline constructor" << std::endl;
			vufCurve<T, V>::m_has_degree = true;
			vufCurve<T, V>::m_degree = CURVE_DEGREE;
			vufCurve<T, V>::m_close = false;
		}
	public:
		virtual ~vufCurveBezierOpen() {}
		VF_MATH_CURVE_DEFINE_CREATOR(vufCurveBezierOpen);
		// inherited virtual methods from vufCurve class
		VF_MATH_CURVE_DEFINE_TYPE_CATEGORY(k_open_bezier_piecewise, k_bezier_category);
		VF_MATH_CRV_REBUILD_CLAMPED;

		virtual V<T>			get_closest_point(	const V<T>& p_point,
													T			p_start = 0,
													T			p_end = 1,
													uint32_t	p_divisions = 10,
													T p_percition = vufCurve_kTol) const override
		{
			return get_pos_at_i(get_closest_point_param_i(p_point, p_start, p_end, p_divisions, p_percition));
		}
		virtual T				get_closest_point_param(const V<T>& p_point,
														T			p_start = 0,
														T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
														uint32_t	p_divisions = 10,
														T	p_percition = vufCurve_kTol) const override
		{
			return get_closest_point_param_i(p_point, p_start, p_end, p_divisions, p_percition);
		}

		VF_MATH_GET_PARAM_COMPONENT;

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
		
		/// Get copy of this curve.	Original curve is unchenged
		virtual std::shared_ptr<vufCurve<T, V>>		get_copy() const override
		{
			std::shared_ptr< vufCurveBezierOpen > l_ptr = vufCurveBezierOpen::create();

			l_ptr->m_degree = vufCurve<T, V>::m_degree;
			l_ptr->m_close = vufCurve<T, V>::m_close;
			l_ptr->m_valid = vufCurve<T, V>::m_valid;

			l_ptr->m_nodes_pos_v = vufCurveExplicit<T, V>::m_nodes_pos_v;
			l_ptr->m_pos_offset = vufCurveExplicit<T, V>::m_pos_offset;
			l_ptr->m_knot_v = m_knot_v;
			return l_ptr;
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)	const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			l_ss << l_str_offset << "[ General Open Bezier <" << typeid(T).name() << ", " << typeid(V).name() << ", " << CURVE_DEGREE << "> ]" << std::endl;
			l_ss << l_str_offset << "____Is Valid: " << m_valid << std::endl;
			l_ss << l_str_offset << "____Controls Count: " << m_nodes_count << std::endl;
			l_ss << l_str_offset << "____Intervals Count: " << m_interval_count << std::endl;
			l_ss << l_str_offset << "____Intervals Length: " << m_interval_length << std::endl;
			l_ss << l_str_offset << "____Knots: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_knot_v);
			l_ss << std::endl;
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
			if (init_knot_vector_i() == true)
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

		virtual std::shared_ptr<vufCurveBezierOpen <T, V, 1>>		as_open_bezier_mono()	const override { return nullptr; }
		virtual std::shared_ptr<vufCurveBezierOpen <T, V, 2>>		as_open_bezier_di()		const override { return nullptr; }
		virtual std::shared_ptr<vufCurveBezierOpen <T, V, 3>>		as_open_bezier_tri()	const override { return nullptr; }

		// inherited methods from vufCurveExplicit
		virtual bool		set_nodes_count(uint32_t p_count) override
		{
			// reserve elements from start and end
			if (vufCurveExplicit<T, V>::m_nodes_pos_v.size() != p_count)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v.resize(p_count);
				return init_knot_vector_i();
			}
			return true;
		}
		virtual uint32_t	get_nodes_count() const override
		{
			return m_nodes_count;
		}
		virtual void		set_node_at(uint32_t p_index, const V<T>& p_vector) override
		{
			if (p_index == 0)
			{
				vufCurveExplicit<T, V>::m_pos_offset = p_vector;
			}
			vufCurveExplicit<T, V>::m_nodes_pos_v[p_index] = p_vector - vufCurveExplicit<T, V>::m_pos_offset;
		}
		virtual V<T>		get_node_at(uint32_t p_index) const override
		{
			return vufCurveExplicit<T, V>::m_nodes_pos_v[p_index];
		}
		
		virtual T			get_interval_t_min(int p_interval_index) const override
		{
			return get_interval_t_min_i(p_interval_index);
		}
		virtual T			get_interval_t_max(int p_interval_index) const override
		{
			return get_interval_t_max_i(p_interval_index);
		}
		virtual int			get_interval_index(T p_t) const override 
		{ 
			return get_interval_index_i(p_t);
		}
		virtual uint32_t	get_interval_count() const override 
		{ 
			return m_interval_count;
		}

	private:
		VF_MATH_CRV_GET_CLOSEST_PARAM_ON_INTERVAL_I_BODY;
		VF_MATH_CRV_GATHER_INFO_I_BODY;
		VF_MATH_CRV_GET_CLOSEST_PARAM_I;
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
			//vufNumericArrayFn<T> l_fn(m_knot_v);
			//std::cout << "Knot Vector " <<  l_fn.to_string() << std::endl;
			//std::cout << m_interval_length << std::endl;
			//std::cout << m_interval_count << std::endl;
			//std::cout << m_nodes_count << std::endl;

			vufCurve<T, V>::m_valid = true;
			return true;
		}
		inline T			get_interval_t_min_i(int p_interval_index) const
		{
			//return (T)p_interval_index * m_interval_length;
			return p_interval_index == 0 ? 0.0 : m_knot_v[p_interval_index - 1];
		}
		inline T			get_interval_t_max_i(int p_interval_index) const
		{
			return m_knot_v[p_interval_index];
		}
		inline int			get_interval_index_i(T p_t) const
		{
			if (p_t <= 0.0) return 0;
			if (p_t >= 1.0) return m_interval_count - 1;
			return int(p_t / m_interval_length);
		}
		inline uint32_t		get_interval_count_i() const
		{
			return m_interval_count;
		}
		
		inline V<T>			get_pos_at_i(T p_t)					const
		{
			return V<T>();
		}
		// for closest point to minimize numerical errors
		inline V<T>			get_pos_no_offset_i(T p_t)		const
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
		//Real contols count N = n*degree +1. In General m_nodes count != m_nodes_pos_v.size() 
		uint32_t			m_nodes_count		= 0;

		static T g_arr[CURVE_DEGREE + 1][CURVE_DEGREE + 1];
		static T g_darr[CURVE_DEGREE +1][CURVE_DEGREE];
		static vufPolinomCoeff<T, CURVE_DEGREE> g_basis[];
		static vufPolinomCoeff<T, CURVE_DEGREE -1> g_dbasis[];
	};
	//specializations
#pragma region STATIC VARIABLES SPECIALIZATION
	//  curve degree 1
	template<>
	double vufCurveBezierOpen <double, vufVector4, 1>::g_arr[2][2] = { {1, -1}, {0, 1} };
	template<>
	double vufCurveBezierOpen <double, vufVector4, 1>::g_darr[2][1] = { {-1}, {1} };
	template <>
	vufPolinomCoeff<double, 1> vufCurveBezierOpen <double, vufVector4, 1>::g_basis[] = { vufPolinomCoeff<double,1>(g_arr[0]),
																						 vufPolinomCoeff<double,1>(g_arr[1]) };
	template <>
	vufPolinomCoeff<double, 0> vufCurveBezierOpen <double, vufVector4, 1>::g_dbasis[] = { vufPolinomCoeff<double,0>(g_darr[0]),
																						  vufPolinomCoeff<double,0>(g_darr[1]) };
	
	// curve degree 2
	template<>
	double vufCurveBezierOpen <double, vufVector4, 2>::g_arr[3][3] = { {1, -2, 1}, {0, 2, -2}, {0,0,1} };
	template<>
	double vufCurveBezierOpen <double, vufVector4, 2>::g_darr[3][2] = { {-2, 2}, {2,-4}, {0,2} };
	template <>
	vufPolinomCoeff<double, 2> vufCurveBezierOpen <double, vufVector4, 2>::g_basis[] = { vufPolinomCoeff<double,2>(vufCurveBezierOpen <double, vufVector4, 2>::g_arr[0]),
																						 vufPolinomCoeff<double,2>(vufCurveBezierOpen <double, vufVector4, 2>::g_arr[1]),
																						 vufPolinomCoeff<double,2>(vufCurveBezierOpen <double, vufVector4, 2>::g_arr[2]) };
	template <>
	vufPolinomCoeff<double, 1> vufCurveBezierOpen <double, vufVector4, 2>::g_dbasis[] = { vufPolinomCoeff<double,1>(g_darr[0]),
																						  vufPolinomCoeff<double,1>(g_darr[1]),
																						  vufPolinomCoeff<double,1>(g_darr[2]) };


	// curve degree 3
	template<>
	double vufCurveBezierOpen <double, vufVector4, 3>::g_arr[4][4] = { {1, -3, 3, -1}, {0, 3, -6, 3}, {0,0,3,-3},{0,0,0,1} };
	template<>
	double vufCurveBezierOpen <double, vufVector4, 3>::g_darr[4][3] = { {-3, 6,-3}, {3,-12,9}, {0,6,-9},{0,0,3} };
	template <>
	vufPolinomCoeff<double, 3> vufCurveBezierOpen <double, vufVector4, 3>::g_basis[] = { vufPolinomCoeff<double,3>(vufCurveBezierOpen <double, vufVector4, 3>::g_arr[0]),
																						 vufPolinomCoeff<double,3>(vufCurveBezierOpen <double, vufVector4, 3>::g_arr[1]),
																						 vufPolinomCoeff<double,3>(vufCurveBezierOpen <double, vufVector4, 3>::g_arr[2]),
																						 vufPolinomCoeff<double,3>(vufCurveBezierOpen <double, vufVector4, 3>::g_arr[3]) };
	template <>
	vufPolinomCoeff<double, 2> vufCurveBezierOpen <double, vufVector4, 3>::g_dbasis[] = { vufPolinomCoeff<double,2>(g_darr[0]),
																						  vufPolinomCoeff<double,2>(g_darr[1]),
																						  vufPolinomCoeff<double,2>(g_darr[2]),
																						  vufPolinomCoeff<double,2>(g_darr[3]) };

#pragma endregion
#pragma region CAST SPECIALIZATION
	// cast																					 
	template<>
	std::shared_ptr<vufCurveBezierOpen <double, vufVector4, 1>>		vufCurveBezierOpen <double, vufVector4, 1>::as_open_bezier_mono()	const
	{
		return std::static_pointer_cast<vufCurveBezierOpen<double, vufVector4, 1>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveBezierOpen <double, vufVector4, 2>>		vufCurveBezierOpen <double, vufVector4, 2>::as_open_bezier_di()		const
	{
		return std::static_pointer_cast<vufCurveBezierOpen<double, vufVector4, 2>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveBezierOpen <double, vufVector4, 3>>		vufCurveBezierOpen <double, vufVector4, 3>::as_open_bezier_tri()	const
	{
		return std::static_pointer_cast<vufCurveBezierOpen<double, vufVector4, 3>>(vufCurve<double, vufVector4>::m_this.lock());
	}
#pragma endregion	
#pragma region POSITION AND TANGENTS SPECIALIZATIONS
	//tangents and positions
	template<>
	vufVector4<double> vufCurveBezierOpen<double, vufVector4, 1>::get_tangent_at_i(double p_t)	const
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
		return  g_dbasis[0].eval(p_t) * m_nodes_pos_v[l_ndx] + g_dbasis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1];
		return	m_nodes_pos_v[l_ndx + 1] - m_nodes_pos_v[l_ndx];
	}
	template<>
	vufVector4<double> vufCurveBezierOpen<double, vufVector4, 1>::get_pos_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return m_pos_offset + get_tangent_at_i(0) * p_t;
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_pos_offset + m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = uint32_t(p_t);
		p_t -= l_ndx;
		return  m_pos_offset + g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] + g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1];
		return	m_pos_offset + m_nodes_pos_v[l_ndx] * (1 - p_t) + m_nodes_pos_v[l_ndx + 1] * p_t;
	}
	// for closest point to minimize numerical errors
	vufVector4<double> vufCurveBezierOpen<double,vufVector4, 1>::get_pos_no_offset_i(double p_t)		const
	{
		if (p_t <= 0.0)
		{
			return get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = uint32_t(p_t);
		p_t -= l_ndx;
		return g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
			   g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1];
		return	m_nodes_pos_v[l_ndx] * (1 - p_t) + m_nodes_pos_v[l_ndx + 1] * p_t;
	}

	template<>
	vufVector4<double> vufCurveBezierOpen<double, vufVector4, 2>::get_tangent_at_i(double p_t) const
	{
		if (p_t <= 0)
		{
			return m_nodes_pos_v[1] * (2.0);
			return m_nodes_pos_v[0] * (-2.0)  + m_nodes_pos_v[1] * (2.0);
		}
		if (p_t >= 1.0)
		{
			return  m_nodes_pos_v[m_nodes_count-2] * (-2.0) + m_nodes_pos_v[m_nodes_count -1] * 2.0;
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 2 * uint32_t(p_t);
		p_t -= uint32_t(p_t);
		return	g_dbasis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
				g_dbasis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1] +
				g_dbasis[2].eval(p_t) * m_nodes_pos_v[l_ndx + 2];
		return m_nodes_pos_v[l_ndx] * 2.0 * (p_t - 1) + m_nodes_pos_v[l_ndx + 1] * (2.0 - 4.0 * p_t) + m_nodes_pos_v[l_ndx + 2] * 2.0 * p_t;
	}
	template<>
	vufVector4<double> vufCurveBezierOpen<double, vufVector4, 2>::get_pos_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return m_pos_offset + get_tangent_at_i(0) * p_t;
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_pos_offset + m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[m_nodes_count-1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 2 * uint32_t(p_t);
		p_t -= uint32_t(p_t);
		return m_pos_offset + g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
							  g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1] +
							  g_basis[2].eval(p_t) * m_nodes_pos_v[l_ndx + 2];

		return	m_pos_offset +  
			m_nodes_pos_v[l_ndx] * (1 - p_t) * (1 - p_t) + 2.0 * m_nodes_pos_v[l_ndx + 1] * p_t * (1 - p_t) + m_nodes_pos_v[l_ndx + 2] * p_t * p_t;
	}
	template<>
	vufVector4<double> vufCurveBezierOpen<double, vufVector4, 2>::get_pos_no_offset_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return get_tangent_at_i(0) * p_t;
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[m_nodes_count-1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 2 * uint32_t(p_t);
		p_t -= uint32_t(p_t);
		return	g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
				g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1] +
				g_basis[2].eval(p_t) * m_nodes_pos_v[l_ndx + 2];

		return	m_nodes_pos_v[l_ndx] * (1 - p_t) * (1 - p_t) + 2.0 * m_nodes_pos_v[l_ndx + 1] * p_t * (1 - p_t) + m_nodes_pos_v[l_ndx + 2] * p_t * p_t;
	}


	template<>
	vufVector4<double> vufCurveBezierOpen<double, vufVector4, 3>::get_tangent_at_i(double p_t)	const
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
		return	g_dbasis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
				g_dbasis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1] +
				g_dbasis[2].eval(p_t) * m_nodes_pos_v[l_ndx + 2] +
				g_dbasis[3].eval(p_t) * m_nodes_pos_v[l_ndx + 3];
		return 3.0 *(m_nodes_pos_v[l_ndx] * (p_t - 1) * (1 - p_t) +  m_nodes_pos_v[l_ndx + 1] * (1.0 - 4.0 * p_t + 3.0 * p_t * p_t) + 
				m_nodes_pos_v[l_ndx + 2] * p_t * (2.0 - 3.0 * p_t) + m_nodes_pos_v[l_ndx + 3] * p_t * p_t );
	}
	template<>
	vufVector4<double> vufCurveBezierOpen<double, vufVector4, 3>::get_pos_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return m_pos_offset + get_tangent_at_i(0) * p_t;
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_pos_offset + m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 3 * uint32_t(p_t);
		p_t -= uint32_t(p_t);
		return	m_pos_offset +	g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
								g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1] +
								g_basis[2].eval(p_t) * m_nodes_pos_v[l_ndx + 2] +
								g_basis[3].eval(p_t) * m_nodes_pos_v[l_ndx + 3];

		return  m_pos_offset + 
				m_nodes_pos_v[l_ndx] * (1 - p_t) * (1 - p_t) * (1 - p_t) + 
				3.0 * m_nodes_pos_v[l_ndx + 1] * p_t * (1 - p_t) * (1 - p_t) +
				3.0 * m_nodes_pos_v[l_ndx + 2] * p_t * p_t * (1 - p_t) +
				m_nodes_pos_v[l_ndx + 3] * p_t * p_t * p_t;
	}
	template<>
	vufVector4<double> vufCurveBezierOpen<double, vufVector4, 3>::get_pos_no_offset_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return get_tangent_at_i(0) * p_t;
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
			//return vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 3 * uint32_t(p_t);
		p_t -= uint32_t(p_t);
		return	g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
				g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1] +
				g_basis[2].eval(p_t) * m_nodes_pos_v[l_ndx + 2] +
				g_basis[3].eval(p_t) * m_nodes_pos_v[l_ndx + 3];

		return  m_nodes_pos_v[l_ndx] * (1 - p_t) * (1 - p_t) * (1 - p_t) +
				3.0 * m_nodes_pos_v[l_ndx + 1] * p_t * (1 - p_t) * (1 - p_t) +
				3.0 * m_nodes_pos_v[l_ndx + 2] * p_t * p_t * (1 - p_t) +
				m_nodes_pos_v[l_ndx + 3] * p_t * p_t * p_t;
	}
#pragma endregion
#pragma region CLOSEST_POINTS
	template<>
	double		vufCurveBezierOpen <double, vufVector4, 1>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
	{		
		auto l_point = p_point - m_pos_offset;
		double l_dist_min = (get_pos_no_offset_i(p_start) - l_point).length2();//std::numeric_limits<double>::max();
		double l_param = p_start;
		double l_temp_dist = (get_pos_no_offset_i(p_end) - l_point).length2();
		if (l_temp_dist < l_dist_min)
		{
			l_dist_min = l_temp_dist;
			l_param = p_end;
		}
		// check if p_t < 0/ then just line from point[0]
		if (p_start <= 0)
		{
			vufVector4<double> l_tang = get_tangent_at_i(0);
			double l_b = l_tang.dot(m_nodes_pos_v[0] - l_point);
			double l_a = l_tang.length2();
			if ((VF_ABS(l_a)) < VF_MATH_EPSILON)
			{
				double l_t = -l_b / l_a;
				if (l_t <=0 && l_t > p_start && l_t < p_end)
				{
					l_temp_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
					if (l_temp_dist < l_dist_min)
					{
						l_dist_min = l_temp_dist;
						l_param = l_t;
					}
				}
			}
		}
		// check > 1
		if (p_end >= 1)
		{
			vufVector4<double> l_tang = get_tangent_at_i(1);
			double l_b = l_tang.dot(m_nodes_pos_v[m_nodes_count -1] - l_point);
			double l_a = l_tang.length2();
			if ((VF_ABS(l_a)) < VF_MATH_EPSILON)
			{
				double l_t = 1 -l_b / l_a;
				if (l_t >= 1 && l_t > p_start && l_t < p_end)
				{
					l_temp_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
					if (l_temp_dist < l_dist_min)
					{
						l_dist_min = l_temp_dist;
						l_param = l_t;
					}
				}
			}
		}
		// To Do add check for p_start and p_end
		uint32_t l_cnt = get_interval_count_i();
		for (uint32_t l_interval_id = 0; l_interval_id < l_cnt; ++l_interval_id)
		{
			vufPolinomCoeff<double, 1> l_px1, l_py1, l_pz1;
			vufPolinomCoeff<double, 0> l_tx1, l_ty1, l_tz1;
			vufPolinomCoeff<double, 1> l_p;
			double l_min = get_interval_t_min_i(l_interval_id);
			double l_max = get_interval_t_max_i(l_interval_id);
			if (l_max >= p_start && l_max <= p_end)
			{
				l_temp_dist = (get_pos_no_offset_i(l_max) - l_point).length2();
				if (l_temp_dist < l_dist_min)
				{
					l_dist_min = l_temp_dist;
					l_param = l_max;
				}
			}
			double p_arr[1];
			
			uint32_t l_node_id = l_interval_id;
			l_px1 = g_basis[0]  * m_nodes_pos_v[l_node_id].x + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].x;
			l_py1 = g_basis[0]  * m_nodes_pos_v[l_node_id].y + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].y;
			l_pz1 = g_basis[0]  * m_nodes_pos_v[l_node_id].z + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].z;
			l_tx1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].x + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].x;
			l_ty1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].y + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].y;
			l_tz1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].z + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].z;
			
			l_px1.a[0] -= l_point.x;
			l_py1.a[0] -= l_point.y;
			l_pz1.a[0] -= l_point.z;
			l_p = l_px1.mult(l_tx1) + l_py1.mult(l_ty1) + l_pz1.mult(l_tz1);
			double l_min2 = 0;
			double l_max2 = 1;
			auto l_solve_count = l_p.find_root_on_interval(l_min2, l_max2, p_arr, p_percition);
			if (l_solve_count == 0) continue;
			double l_t = l_min + p_arr[0]*m_interval_length;
			if ( l_t >= p_start && l_t <= p_end )
			{
				double l_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
				if (l_dist < l_dist_min)
				{
					l_dist_min = l_dist;
					l_param = l_t;
				}
			}
		}
		return l_param;
	}
	template<>
	double		vufCurveBezierOpen <double, vufVector4, 2>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
	{
		auto	l_point = p_point - m_pos_offset;
		double	l_dist_min = (get_pos_no_offset_i(p_start) - l_point).length2();//std::numeric_limits<double>::max();
		double	l_param = p_start;
		double	l_temp_dist = (get_pos_no_offset_i(p_end) - l_point).length2();
		if (l_temp_dist < l_dist_min)
		{
			l_dist_min = l_temp_dist;
			l_param = p_end;
		}
		// check if p_t < 0/ then just line from point[0]
		if (p_start <= 0)
		{
			vufVector4<double> l_tang = get_tangent_at_i(0);
			double l_b = l_tang.dot(m_nodes_pos_v[0] - l_point);
			double l_a = l_tang.length2();
			if ((VF_ABS(l_a)) < VF_MATH_EPSILON)
			{
				double l_t = -l_b / l_a;
				if (l_t <= 0 && l_t > p_start && l_t < p_end)
				{
					l_temp_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
					if (l_temp_dist < l_dist_min)
					{
						l_dist_min = l_temp_dist;
						l_param = l_t;
					}
				}
			}
		}
		// check > 1
		if (p_end >= 1)
		{
			vufVector4<double> l_tang = get_tangent_at_i(1);
			double l_b = l_tang.dot(m_nodes_pos_v[m_nodes_count - 1] - l_point);
			double l_a = l_tang.length2();
			if ((VF_ABS(l_a)) < VF_MATH_EPSILON)
			{
				double l_t = 1 - l_b / l_a;
				if (l_t >= 1 && l_t > p_start && l_t < p_end)
				{
					l_temp_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
					if (l_temp_dist < l_dist_min)
					{
						l_dist_min = l_temp_dist;
						l_param = l_t;
					}
				}
			}
		}

		uint32_t l_cnt = get_interval_count_i();
		for (uint32_t l_interval_id = 0; l_interval_id < l_cnt; ++l_interval_id)
		{
			vufPolinomCoeff<double, 2> l_px1, l_py1, l_pz1;
			vufPolinomCoeff<double, 1> l_tx1, l_ty1, l_tz1;
			vufPolinomCoeff<double, 3> l_p;
			double l_min = get_interval_t_min_i(l_interval_id);
			double l_max = get_interval_t_max_i(l_interval_id);
			if (l_max >= p_start && l_max <= p_end)
			{
				l_temp_dist = (get_pos_no_offset_i(l_max) - l_point).length2();
				if (l_temp_dist < l_dist_min)
				{
					l_dist_min = l_temp_dist;
					l_param = l_max;
				}
			}

			double p_arr[3];
			
			uint32_t l_node_id = l_interval_id * 2;
			l_px1 = g_basis[0]  * m_nodes_pos_v[l_node_id].x  + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].x  + g_basis[2]  * m_nodes_pos_v[l_node_id + 2].x;
			l_py1 = g_basis[0]  * m_nodes_pos_v[l_node_id].y  + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].y  + g_basis[2]  * m_nodes_pos_v[l_node_id + 2].y;
			l_pz1 = g_basis[0]  * m_nodes_pos_v[l_node_id].z  + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].z  + g_basis[2]  * m_nodes_pos_v[l_node_id + 2].z;
			l_tx1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].x  + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].x  + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].x;
			l_ty1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].y  + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].y  + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].y;
			l_tz1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].z  + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].z  + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].z;
			
			l_px1.a[0] -= l_point.x;
			l_py1.a[0] -= l_point.y;
			l_pz1.a[0] -= l_point.z;
			l_p = l_px1.mult(l_tx1) + l_py1.mult(l_ty1) + l_pz1.mult(l_tz1);
			double l_min2 = 0;
			double l_max2 = 1;
			auto l_solve_count = l_p.find_root_on_interval(l_min2, l_max2, p_arr, p_percition);
			if (l_solve_count == 0) continue;
			for (int i = 0; i < l_solve_count; ++i)
			{
				double l_t = l_min + p_arr[i] * m_interval_length;
				if (l_t >= p_start && l_t <= p_end)
				{
					double l_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
					if (l_dist < l_dist_min)
					{
						l_dist_min = l_dist;
						l_param = l_t;
					}
				}
			}
		}
		return l_param;
	}
	template<>
	double		vufCurveBezierOpen <double, vufVector4, 3>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
	{
		auto l_point = p_point - m_pos_offset;
		double l_dist_min = (get_pos_no_offset_i(p_start) - l_point).length2();//std::numeric_limits<double>::max();
		double l_param = p_start;
		double l_temp_dist = (get_pos_no_offset_i(p_end) - l_point).length2();
		if (l_temp_dist < l_dist_min)
		{
			l_dist_min = l_temp_dist;
			l_param = p_end;
		}
		// check if p_t < 0/ then just line from point[0]
		if (p_start <= 0)
		{
			vufVector4<double> l_tang = get_tangent_at_i(0);
			double l_b = l_tang.dot(m_nodes_pos_v[0] - l_point);
			double l_a = l_tang.length2();
			if ((VF_ABS(l_a)) < VF_MATH_EPSILON)
			{
				double l_t = -l_b / l_a;
				if (l_t <= 0 && l_t > p_start && l_t < p_end)
				{
					l_temp_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
					if (l_temp_dist < l_dist_min)
					{
						l_dist_min = l_temp_dist;
						l_param = l_t;
					}
				}
			}
		}
		// check > 1
		if (p_end > 1)
		{
			vufVector4<double> l_tang = get_tangent_at_i(1);
			double l_b = l_tang.dot(m_nodes_pos_v[m_nodes_count - 1] - l_point);
			double l_a = l_tang.length2();
			if ((VF_ABS(l_a)) < VF_MATH_EPSILON)
			{
				double l_t = 1 - l_b / l_a;
				if (l_t >= 1 && l_t > p_start && l_t < p_end)
				{
					l_temp_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
					if (l_temp_dist < l_dist_min)
					{
						l_dist_min = l_temp_dist;
						l_param = l_t;
					}
				}
			}
		}

		uint32_t l_cnt = get_interval_count_i();
		for (uint32_t l_interval_id = 0; l_interval_id < l_cnt; ++l_interval_id)
		{
			vufPolinomCoeff<double, 3> l_px1, l_py1, l_pz1;
			vufPolinomCoeff<double, 2> l_tx1, l_ty1, l_tz1;
			vufPolinomCoeff<double, 5> l_p; // polinom for (p-p(t)).dot(tangent(t))
			double l_min = get_interval_t_min_i(l_interval_id);
			double l_max = get_interval_t_max_i(l_interval_id);
			if (l_max >= p_start && l_max <= p_end)
			{
				l_temp_dist = (get_pos_no_offset_i(l_max) - l_point).length2();
				if (l_temp_dist < l_dist_min)
				{
					l_dist_min = l_temp_dist;
					l_param = l_max;
				}
			}

			double p_arr[5];

			uint32_t l_node_id = l_interval_id * 3;
			l_px1 = g_basis[0] * m_nodes_pos_v[l_node_id].x  + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].x + g_basis[2]  * m_nodes_pos_v[l_node_id + 2].x + g_basis[3]  * m_nodes_pos_v[l_node_id + 3].x;
			l_py1 = g_basis[0] * m_nodes_pos_v[l_node_id].y  + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].y + g_basis[2]  * m_nodes_pos_v[l_node_id + 2].y + g_basis[3]  * m_nodes_pos_v[l_node_id + 3].y;
			l_pz1 = g_basis[0] * m_nodes_pos_v[l_node_id].z  + g_basis[1]  * m_nodes_pos_v[l_node_id + 1].z + g_basis[2]  * m_nodes_pos_v[l_node_id + 2].z + g_basis[3]  * m_nodes_pos_v[l_node_id + 3].z;
			l_tx1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].x + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].x + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].x + g_dbasis[3] * m_nodes_pos_v[l_node_id + 3].x;
			l_ty1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].y + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].y + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].y + g_dbasis[3] * m_nodes_pos_v[l_node_id + 3].y;
			l_tz1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].z + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].z + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].z + g_dbasis[3] * m_nodes_pos_v[l_node_id + 3].z;

			l_px1.a[0] -= l_point.x;
			l_py1.a[0] -= l_point.y;
			l_pz1.a[0] -= l_point.z;
			l_p = l_px1.mult(l_tx1) + l_py1.mult(l_ty1) + l_pz1.mult(l_tz1);
			//std::cout << l_p.to_string() << std::endl;
			double l_min2 = 0;
			double l_max2 = 1;
			auto l_solve_count = l_p.find_root_on_interval(l_min2, l_max2, p_arr, p_percition);
			if (l_solve_count == 0) continue;
			for (int i = 0; i < l_solve_count; ++i)
			{
				double l_t = l_min + p_arr[i] * m_interval_length;
				if (l_t >= p_start && l_t <= p_end)
				{
					double l_dist = (get_pos_no_offset_i(l_t) - l_point).length2();
					if (l_dist < l_dist_min)
					{
						l_dist_min = l_dist;
						l_param = l_t;
					}
				}
			}
		}
		return l_param;
	}

#pragma endregion
}
#endif // !VF_MATH_OPEN_BEZER_OPN_CRV_H
