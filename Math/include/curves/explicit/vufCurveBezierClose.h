#ifndef VF_MATH_OPEN_BEZER_CLZ_CRV_H
#define VF_MATH_OPEN_BEZER_CLZ_CRV_H

#include <typeinfo>
#include <iostream>
#include <curves/explicit/vufCurveExplicit.h>
#include <curves/vufCurvesInclude.h>
#include <vufMathInclude.h>

namespace vufMath
{
	template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>
	class vufCurveBezierClose : public vufCurveExplicit<T, V>
	{
	private:
		vufCurveBezierClose() :vufCurveExplicit<T, V>()
		{
			//std::cout << "OpenBSpline constructor" << std::endl;
			vufCurve<T, V>::m_has_degree = true;
			vufCurve<T, V>::m_degree = CURVE_DEGREE;
			vufCurve<T, V>::m_close = true;
		}
	public:
		virtual ~vufCurveBezierClose() {}
		VF_MATH_CURVE_DEFINE_CREATOR(vufCurveBezierClose);
		// inherited virtual methods from vufCurve class
		VF_MATH_CURVE_DEFINE_TYPE_CATEGORY(k_close_bezier_piecewise, k_bezier_category);
		VF_MATH_CRV_REBUILD_SEGMENTED;
		VF_MATH_CRV_DEFINE_CLOSEST_POINTS;
		VF_MATH_CRV_DEFINE_PARAM_COMPONENT;

		virtual V<T>		get_pos_at(T p_t)					const override
		{
			return get_pos_at_i(p_t);
		}
		virtual V<T>		get_tangent_at(T p_t)				const override
		{
			return get_tangent_at_i(p_t);
		}
		virtual V<T>		get_tangent_normalized_at(T p_t)	const override
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}
		/// Get copy of this curve.	Original curve is unchenged
		virtual std::shared_ptr<vufCurve<T, V>>		get_copy() const override
		{
			std::shared_ptr< vufCurveBezierClose<T,V,CURVE_DEGREE> > l_ptr = vufCurveBezierClose::create();
			l_ptr->copy_members_from_i(std::static_pointer_cast<vufCurveBezierClose>( m_this.lock()));
			return l_ptr;
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)	const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			l_ss << l_str_offset << "[ General Close Bezier <" << typeid(T).name() << ", " << typeid(V).name() << ", " << CURVE_DEGREE << "> ]" << std::endl;
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
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0, uint32_t* p_version = nullptr) override
		{
			vufCurve<T, V>::m_valid = false;
			p_offset = vufCurveExplicit<T, V>::from_binary(p_buff, p_offset, p_version);
			if (p_offset == 0)
			{
				vufCurve<T, V>::m_valid = false;
				return 0;
			};
			if (init_knot_vector_i((uint32_t)vufCurveExplicit<T, V>::m_nodes_pos_v.size()) == true)
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
		
		// virtual casts
		virtual std::shared_ptr<vufCurveBezierClose <T, V, 1>>		as_close_bezier_mono()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveBezierClose <T, V, 2>>		as_close_bezier_di()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveBezierClose <T, V, 3>>		as_close_bezier_tri()	const { return nullptr; }

		// inherited methods from vufCurveExplicit
		virtual bool		set_nodes_count(uint32_t p_count) override
		{
			// reserve elements from start and end
			if (vufCurveExplicit<T, V>::m_nodes_pos_v.size() != p_count + 1)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v.resize(p_count + 1);
				return init_knot_vector_i(p_count);
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
				vufCurveExplicit<T, V>::m_nodes_pos_v[m_nodes_count - 1] = V<T>(0,0,0);
				vufCurveExplicit<T, V>::m_nodes_pos_v[0] = V<T>(0,0,0);
				return;
			}
			if (p_index == m_nodes_count - 1)
			{
				return;
			}
			vufCurveExplicit<T, V>::m_nodes_pos_v[p_index] = p_vector - m_pos_offset;
		}
		virtual V<T>		get_node_at(uint32_t p_index) const override
		{
			return vufCurveExplicit<T, V>::m_nodes_pos_v[p_index] + m_pos_offset;
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
		bool				init_knot_vector_i(uint32_t p_sz)
		{
			if (p_sz < 2 * CURVE_DEGREE)
			{
				vufCurve<T, V>::m_valid = false;
				return false;
			}
			m_nodes_count = p_sz - p_sz% CURVE_DEGREE + 1;
			m_interval_count = p_sz / CURVE_DEGREE;
			//m_nodes_count = m_interval_count * CURVE_DEGREE + 1;
			//m_interval_count = p_sz - p_sz%CURVE_DEGREE + 1;
			m_knot_v.resize(m_interval_count);
			T l_t_min = 0.0;
			T l_t_max = 1.0;
			m_interval_length = 1. / (T)(m_interval_count);
			for (uint32_t i = 0; i < m_interval_count; ++i)
			{
				m_knot_v[i] = (T(i + 1)) * m_interval_length;
			}
			//vufNumericArrayFn<T> l_fn(m_knot_v);
			//std::cout << l_fn.to_string() << std::endl;
			
			vufCurve<T, V>::m_valid = true;
			return true;
		}
		inline T			get_interval_t_min_i(int p_interval_index) const
		{
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
		inline uint32_t		get_interval_count_i()  const
		{
			return m_interval_count;
		}
		inline V<T>			get_pos_at_i(T p_t)		const
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

		void		copy_members_from_i(std::shared_ptr<vufCurveBezierClose> p_crv)
		{
			vufCurveExplicit<T, V>::copy_members_from_i(p_crv);
			m_knot_v			= p_crv->m_knot_v;
			m_interval_length	= p_crv->m_interval_length;
			m_interval_count	= p_crv->m_interval_count;
			m_nodes_count		= p_crv->m_nodes_count;
		}
	private:
		std::vector<T>		m_knot_v;
		T					m_interval_length = 0;
		uint32_t			m_interval_count = 0;
		uint32_t			m_nodes_count = 0;

		static T g_arr[CURVE_DEGREE + 1][CURVE_DEGREE + 1];
		static T g_darr[CURVE_DEGREE + 1][CURVE_DEGREE];
		static vufPolinomCoeff<T, CURVE_DEGREE> g_basis[];
		static vufPolinomCoeff<T, CURVE_DEGREE - 1> g_dbasis[];
	};
	//specializations
#pragma region STATIC VARIABLES SPECIALIZATION
		//  curve degree 1
	template<>
	double vufCurveBezierClose <double, vufVector4, 1>::g_arr[2][2] = { {1, -1}, {0, 1} };
	template<>
	double vufCurveBezierClose <double, vufVector4, 1>::g_darr[2][1] = { {-1}, {1} };
	template <>
	vufPolinomCoeff<double, 1> vufCurveBezierClose <double, vufVector4, 1>::g_basis[] = { vufPolinomCoeff<double,1>(g_arr[0]),
																						 vufPolinomCoeff<double,1>(g_arr[1]) };
	template <>
	vufPolinomCoeff<double, 0> vufCurveBezierClose <double, vufVector4, 1>::g_dbasis[] = { vufPolinomCoeff<double,0>(g_darr[0]),
																						  vufPolinomCoeff<double,0>(g_darr[1]) };

	// curve degree 2
	template<>
	double vufCurveBezierClose <double, vufVector4, 2>::g_arr[3][3] = { {1, -2, 1}, {0, 2, -2}, {0,0,1} };
	template<>
	double vufCurveBezierClose <double, vufVector4, 2>::g_darr[3][2] = { {-2, 2}, {2,-4}, {0,2} };
	template <>
	vufPolinomCoeff<double, 2> vufCurveBezierClose <double, vufVector4, 2>::g_basis[] = { vufPolinomCoeff<double,2>(vufCurveBezierClose <double, vufVector4, 2>::g_arr[0]),
																						 vufPolinomCoeff<double,2>(vufCurveBezierClose <double, vufVector4, 2>::g_arr[1]),
																						 vufPolinomCoeff<double,2>(vufCurveBezierClose <double, vufVector4, 2>::g_arr[2]) };
	template <>
	vufPolinomCoeff<double, 1> vufCurveBezierClose <double, vufVector4, 2>::g_dbasis[] = { vufPolinomCoeff<double,1>(g_darr[0]),
																						  vufPolinomCoeff<double,1>(g_darr[1]),
																						  vufPolinomCoeff<double,1>(g_darr[2]) };


	// curve degree 3
	template<>
	double vufCurveBezierClose <double, vufVector4, 3>::g_arr[4][4] = { {1, -3, 3, -1}, {0, 3, -6, 3}, {0,0,3,-3},{0,0,0,1} };
	template<>
	double vufCurveBezierClose <double, vufVector4, 3>::g_darr[4][3] = { {-3, 6,-3}, {3,-12,9}, {0,6,-9},{0,0,3} };
	template <>
	vufPolinomCoeff<double, 3> vufCurveBezierClose <double, vufVector4, 3>::g_basis[] = { vufPolinomCoeff<double,3>(vufCurveBezierClose <double, vufVector4, 3>::g_arr[0]),
																						 vufPolinomCoeff<double,3>(vufCurveBezierClose <double, vufVector4, 3>::g_arr[1]),
																						 vufPolinomCoeff<double,3>(vufCurveBezierClose <double, vufVector4, 3>::g_arr[2]),
																						 vufPolinomCoeff<double,3>(vufCurveBezierClose <double, vufVector4, 3>::g_arr[3]) };
	template <>
	vufPolinomCoeff<double, 2> vufCurveBezierClose <double, vufVector4, 3>::g_dbasis[] = { vufPolinomCoeff<double,2>(g_darr[0]),
																						  vufPolinomCoeff<double,2>(g_darr[1]),
																						  vufPolinomCoeff<double,2>(g_darr[2]),
																						  vufPolinomCoeff<double,2>(g_darr[3]) };

#pragma endregion
#pragma region CAST SPECIALIZATION
	template<>
	std::shared_ptr<vufCurveBezierClose <double, vufVector4, 1>>		vufCurveBezierClose <double, vufVector4, 1>::as_close_bezier_mono()	const
	{
		return std::static_pointer_cast<vufCurveBezierClose<double, vufVector4, 1>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveBezierClose <double, vufVector4, 2>>		vufCurveBezierClose <double, vufVector4, 2>::as_close_bezier_di()		const
	{
		return std::static_pointer_cast<vufCurveBezierClose<double, vufVector4, 2>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveBezierClose <double, vufVector4, 3>>		vufCurveBezierClose <double, vufVector4, 3>::as_close_bezier_tri()	const
	{
		return std::static_pointer_cast<vufCurveBezierClose<double, vufVector4, 3>>(vufCurve<double, vufVector4>::m_this.lock());
	}

#pragma endregion
#pragma region POSITION AND TANGENTS SPECIALIZATIONS
	template<>
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 1>::get_tangent_at_i(double p_t)	const
	{
		p_t -= std::floor(p_t);
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
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 1>::get_pos_at_i(double p_t)	const
	{
		p_t -= std::floor(p_t);
		if (p_t <= 0)
		{
			return m_pos_offset + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_pos_offset + m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = uint32_t(p_t);
		p_t -= l_ndx;
		return	m_pos_offset +	g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
								g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1];
		return	m_pos_offset + m_nodes_pos_v[l_ndx] * (1 - p_t) + m_nodes_pos_v[l_ndx + 1] * p_t;
	}
	// for closest point to minimize numerical errors
	template<>
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 1>::get_pos_no_offset_i(double p_t)		const
	{
		p_t -= std::floor(p_t);
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
		return	g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
				g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1];
		return	m_nodes_pos_v[l_ndx] * (1 - p_t) + m_nodes_pos_v[l_ndx + 1] * p_t;
	}


	template<>
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 2>::get_tangent_at_i(double p_t) const
	{
		p_t -= std::floor(p_t);
		if (p_t <= 0)
		{
			return m_nodes_pos_v[0] * (-2.0) + m_nodes_pos_v[1] * 2.0;
		}
		if (p_t >= 1.0)
		{
			return  m_nodes_pos_v[m_nodes_count - 2] * (-2.0) + m_nodes_pos_v[m_nodes_count - 1] * 2.0;

		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 2 * uint32_t(p_t);
		p_t -= uint32_t(p_t);
		return m_nodes_pos_v[l_ndx] * 2.0 * (p_t - 1) + m_nodes_pos_v[l_ndx + 1] * (2.0 - 4.0 * p_t) + m_nodes_pos_v[l_ndx + 2] * 2.0 * p_t;
	}
	template<>
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 2>::get_pos_at_i(double p_t)	const
	{
		p_t -= std::floor(p_t);
		if (p_t <= 0)
		{
			return m_pos_offset + m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_pos_offset + m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}

		p_t /= m_interval_length;
		uint32_t l_ndx = 2 * uint32_t(p_t);
		p_t -= uint32_t(p_t);
		return	m_pos_offset +	g_basis[0].eval(p_t) * m_nodes_pos_v[l_ndx] +
								g_basis[1].eval(p_t) * m_nodes_pos_v[l_ndx + 1] +
								g_basis[2].eval(p_t) * m_nodes_pos_v[l_ndx + 2];
		return	m_nodes_pos_v[l_ndx] * (1 - p_t) * (1 - p_t) + 2.0 * m_nodes_pos_v[l_ndx + 1] * p_t * (1 - p_t) + m_nodes_pos_v[l_ndx + 2] * p_t * p_t;
	}
	template<>
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 2>::get_pos_no_offset_i(double p_t)	const
	{
		p_t -= std::floor(p_t);
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
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 3>::get_tangent_at_i(double p_t)	const
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

		return 3.0 * (m_nodes_pos_v[l_ndx] * (p_t - 1) * (1 - p_t) + m_nodes_pos_v[l_ndx + 1] * (1.0 - 4.0 * p_t + 3.0 * p_t * p_t) +
			m_nodes_pos_v[l_ndx + 2] * p_t * (2.0 - 3.0 * p_t) + m_nodes_pos_v[l_ndx + 3] * p_t * p_t);
	}
	template<>
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 3>::get_pos_at_i(double p_t)	const
	{
		if (p_t <= 0)
		{
			return m_pos_offset + m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
		}
		if (p_t >= 1.0)
		{
			return m_pos_offset + m_nodes_pos_v[m_nodes_count - 1] + get_tangent_at_i(1) * (p_t - 1);
		}
		p_t /= m_interval_length;
		uint32_t l_ndx = 3 * uint32_t(p_t);
		p_t -= uint32_t(p_t);

		return m_pos_offset + m_nodes_pos_v[l_ndx] * (1 - p_t) * (1 - p_t) * (1 - p_t) + 3.0 * m_nodes_pos_v[l_ndx + 1] * p_t * (1 - p_t) * (1 - p_t) +
			3.0 * m_nodes_pos_v[l_ndx + 2] * p_t * p_t * (1 - p_t) + m_nodes_pos_v[l_ndx + 3] * p_t * p_t * p_t;
	}
	template<>
	vufVector4<double> vufCurveBezierClose<double, vufVector4, 3>::get_pos_no_offset_i(double p_t)	const
	{
		p_t -= std::floor(p_t);
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
	double		vufCurveBezierClose <double, vufVector4, 1>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
	{
		p_start = VF_CLAMP(0, 1, p_start);
		p_end	= VF_CLAMP(0, 1, p_end);
		auto l_point = p_point - m_pos_offset;
		double l_dist_min = (get_pos_no_offset_i(p_start) - l_point).length2();//std::numeric_limits<double>::max();
		double l_param = p_start;
		double l_temp_dist = (get_pos_no_offset_i(p_end) - l_point).length2();
		if (l_temp_dist < l_dist_min)
		{
			l_dist_min = l_temp_dist;
			l_param = p_end;
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
			double l_t = l_min + p_arr[0] * m_interval_length;
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
		//std::cout << "l_param: " <<  l_param << std::endl;
		return l_param;
	}
	template<>
	double		vufCurveBezierClose <double, vufVector4, 2>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
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
			l_px1 = g_basis[0] * m_nodes_pos_v[l_node_id].x + g_basis[1] * m_nodes_pos_v[l_node_id + 1].x + g_basis[2] * m_nodes_pos_v[l_node_id + 2].x;
			l_py1 = g_basis[0] * m_nodes_pos_v[l_node_id].y + g_basis[1] * m_nodes_pos_v[l_node_id + 1].y + g_basis[2] * m_nodes_pos_v[l_node_id + 2].y;
			l_pz1 = g_basis[0] * m_nodes_pos_v[l_node_id].z + g_basis[1] * m_nodes_pos_v[l_node_id + 1].z + g_basis[2] * m_nodes_pos_v[l_node_id + 2].z;
			l_tx1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].x + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].x + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].x;
			l_ty1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].y + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].y + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].y;
			l_tz1 = g_dbasis[0] * m_nodes_pos_v[l_node_id].z + g_dbasis[1] * m_nodes_pos_v[l_node_id + 1].z + g_dbasis[2] * m_nodes_pos_v[l_node_id + 2].z;

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
	double		vufCurveBezierClose <double, vufVector4, 3>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
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
			l_px1 = g_basis[0] * m_nodes_pos_v[l_node_id].x + g_basis[1] * m_nodes_pos_v[l_node_id + 1].x + g_basis[2] * m_nodes_pos_v[l_node_id + 2].x + g_basis[3] * m_nodes_pos_v[l_node_id + 3].x;
			l_py1 = g_basis[0] * m_nodes_pos_v[l_node_id].y + g_basis[1] * m_nodes_pos_v[l_node_id + 1].y + g_basis[2] * m_nodes_pos_v[l_node_id + 2].y + g_basis[3] * m_nodes_pos_v[l_node_id + 3].y;
			l_pz1 = g_basis[0] * m_nodes_pos_v[l_node_id].z + g_basis[1] * m_nodes_pos_v[l_node_id + 1].z + g_basis[2] * m_nodes_pos_v[l_node_id + 2].z + g_basis[3] * m_nodes_pos_v[l_node_id + 3].z;
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
#endif // !VF_MATH_OPEN_BEZER_CLZ_CRV_H
