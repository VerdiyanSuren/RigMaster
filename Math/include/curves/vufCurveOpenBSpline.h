#ifndef VF_MATH_OPEN_BSPLINE_CRV_H
#define VF_MATH_OPEN_BSPLINE_CRV_H

#include <typeinfo>
#include <iostream>
#include <curves/vufCurveExplicit.h>
#include <vufLog.h>
#include <curves/vufMathCurvesInclude.h>

//#define VF_MATH_DEBUG_BSPLINE
namespace vufMath
{
	template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>
	class vufCurveOpenBSpline : public vufCurveExplicit<T, V>
	{
	private:
	public:
		vufCurveOpenBSpline() :vufCurveExplicit<T, V>()
		{
			//std::cout << "OpenBSpline constructor" << std::endl;
			vufCurve<T, V>::m_degree = CURVE_DEGREE;
			vufCurve<T, V>::m_close = false;
		}
		virtual ~vufCurveOpenBSpline() {}
		static  std::shared_ptr< vufCurveOpenBSpline >	create()
		{
			std::shared_ptr< vufCurveOpenBSpline >  l_ptr = std::shared_ptr<vufCurveOpenBSpline>(new vufCurveOpenBSpline());
			VF_MATH_CURVE_CREATOR_BODY(l_ptr);
			return l_ptr;
		}
		/// Get copy of this curve.	Original curve is unchenged
		virtual std::shared_ptr<vufCurve<T, V>>		get_copy() const override
		{
			std::shared_ptr< vufCurveOpenBSpline > l_ptr = vufCurveOpenBSpline::create();

			l_ptr->m_degree = vufCurve<T, V>::m_degree;
			l_ptr->m_close	= vufCurve<T, V>::m_close;
			l_ptr->m_valid	= vufCurve<T, V>::m_valid;

			l_ptr->m_nodes_pos_v	= vufCurveExplicit<T,V>::m_nodes_pos_v;
			//l_ptr->m_nodes_max_influence_t_v = m_nodes_max_influence_t_v;

			l_ptr->m_knot_v = m_knot_v;
			l_ptr->m_n_v	= m_n_v;
			l_ptr->m_dn_v	= m_dn_v;

			return l_ptr;
		}
	private:
		//non virtual methods
		/** If we get knot custom vector. From external application, for example*/
		// Knots
		bool				set_knots_vector_by_other(const T* p_arr, uint64_t p_count)
		{
			uint32_t l_sz = (uint32_t)vufCurve<T, V>::m_nodes_pos_v.size();
			if (p_count != l_sz + CURVE_DEGREE + 1)
			{
				return false;
			}
			// Check is knot vector valid
			// The sigle reqirement is x[i] <= x[i+1]
			for (int i = 0; i < p_count - 1; ++i)
			{
				// knot vector elements has to be arranged
				if (p_arr[i] > p_arr[i + 1])
				{
					return false;
				}
			}

			vufCurve<T, V>::m_t_min = p_arr[0];
			vufCurve<T, V>::m_t_max = p_arr[p_count - 1];

			// We are going to match knot array into [0,1]
			T l_normalize_factor = vufCurve<T, V>::m_t_max - vufCurve<T, V>::m_t_min;
			if (l_normalize_factor == 0)
			{
				return false;
			}

			l_normalize_factor = 1. / l_normalize_factor;
			m_knot_v.resize(p_count);
			for (int i = 0; i < p_count; ++i)
			{
				m_knot_v[i] = (p_arr[i] - vufCurve<T, V>::m_t_min) * l_normalize_factor;
			}
			vufCurve<T, V>::m_t_max = 1.0;

#ifdef  VF_MATH_DEBUG_BSPLINE
			std::cout << "Set Knot Vector by other: [";
			for (int i = 0; i < (int)m_knot_v.size(); ++i)
			{
				std::cout << m_knot_v[i] << " ";
			}
			std::cout << "]" << std::endl;
#endif

			return true;
		}
		bool				set_knots_vector_by_other(const std::vector<T> p_other)
		{
			return set_knots_vector_by_other(p_other.data(), p_other.size());
		}
		bool				generate_knot_vector(std::vector<T>& p_knots)
		{
			int l_sz = (int)vufCurveExplicit<T,V>::m_nodes_pos_v.size();
			if (l_sz < (int)CURVE_DEGREE + 1)
			{
				vufCurve<T, V>::m_valid = false;
				return false;
			}

			T l_t_min = 0.0;
			T l_t_max = (T)(l_sz - CURVE_DEGREE);
			T l_normalize_factor = 1. / (l_t_max - l_t_min);
			l_t_max *= l_normalize_factor;
			p_knots.resize(l_sz + CURVE_DEGREE + 1);
			//fill starts of knot vector
			for (int i = 0; i < CURVE_DEGREE + 1; ++i)
			{
				p_knots[i] = l_t_min;
			}
			//file ends of knot vector
			for (uint32_t i = l_sz; i < l_sz + CURVE_DEGREE + 1; ++i)
			{
				p_knots[i] = l_t_max;
			}
			//fill knot vector
			for (int i = CURVE_DEGREE + 1; i < p_knots.size() - CURVE_DEGREE; ++i)
			{
				p_knots[i] = ((T)(i - CURVE_DEGREE)) * l_normalize_factor;
			}
			l_t_max = 1;
#ifdef  VF_MATH_DEBUG_BSPLINE
			std::cout << "Generated Knot Vector: [";
			for (int i = 0; i < (int)p_knots.size(); ++i)
			{
				std::cout << p_knots[i] << " ";
			}
			std::cout << "]" << std::endl;
#endif
			return true;
		}
		inline T			get_interval_t_min_i(int p_interval_index) const
		{
			int l_knot_index = p_interval_index + CURVE_DEGREE;
			return m_knot_v[l_knot_index];
		}
		inline T			get_interval_t_max_i(int p_interval_index) const
		{
			int l_knot_index = p_interval_index + CURVE_DEGREE + 1;
			return m_knot_v[l_knot_index];

		}
		inline int			get_interval_index_i(T p_t) const
		{
			for (int i = 1; i < (int)m_knot_v.size() - 2 * (int)CURVE_DEGREE; ++i)
			{
				if (p_t <= m_knot_v[i + CURVE_DEGREE])
				{
					return i - 1;
				}
			}
			return get_interval_count_i() - 1;
		}
		inline uint32_t		get_interval_count_i() const
		{
			return ((int)m_knot_v.size() - 2 * (int)CURVE_DEGREE - 1);
		}
		inline int			get_reqiured_knot_size() const
		{
			return (int)vufCurve<T, V>::m_nodes_pos_v + (int)CURVE_DEGREE + 1;
		}
		inline T			get_basis_at(uint32_t p_node_id, T p_t)					const
		{
			return m_n_v[get_interval_index(p_t)][p_node_id].eval(p_t);
		}
		inline T			get_basis_derivative_at(uint32_t p_node_id, double p_t)	const
		{
			return m_dn_v[get_interval_index(p_t)][p_node_id].eval(p_t);
		}
	public:
		/** If this is our curve and we need auto generation of knot vector */
		bool				init_knot_vector()
		{
			return generate_knot_vector(m_knot_v);
		}
		bool				update_basises_i()
		{
			int l_nodes_sz = (int)vufCurveExplicit<T,V>::m_nodes_pos_v.size();
			int l_knot_sz = (int)m_knot_v.size();
			int l_interval_cnt = l_knot_sz - 2 * (int)CURVE_DEGREE - 1;
			// Check do we have enougth control points
			if (l_nodes_sz < CURVE_DEGREE + 1)
			{
				vufCurve<T, V>::m_valid = false;
				return false;
			}

			// Check do we have correct knot vector size and int if 
			if (l_knot_sz != l_nodes_sz + CURVE_DEGREE + 1)
			{
				vufCurve<T, V>::m_valid = false;
				return false;
			}

			//prepare temporary polinom array to compute basis functions
			vufPolinomCoeff<T, CURVE_DEGREE>* l_n_prev = new vufPolinomCoeff<T, CURVE_DEGREE>[l_knot_sz - 1];
			vufPolinomCoeff<T, CURVE_DEGREE>* l_n_curn = new vufPolinomCoeff<T, CURVE_DEGREE>[l_knot_sz - 1];
			vufPolinomCoeff<T, CURVE_DEGREE>* l_n_swap = nullptr;

			// Allocate memory for curve  [interval basis] members
			VF_RESIZE_ARRAY_IF_DIFFERENT_SIZE(m_n_v, l_interval_cnt);
			VF_RESIZE_ARRAY_IF_DIFFERENT_SIZE(m_dn_v, l_interval_cnt);
			for (int i = 0; i < l_interval_cnt; ++i)
			{
				m_n_v[i].resize(l_nodes_sz);
				m_dn_v[i].resize(l_nodes_sz);
			}

			// For each interval
			//for (int l_interval = CURVE_DEGREE; l_interval < (int)m_knot_v.size() - (int)CURVE_DEGREE - 1; ++l_interval)
			for (int l_interval = 0; l_interval < l_interval_cnt; ++l_interval)
			{
				int l_knot_index = l_interval + CURVE_DEGREE;
				// start initialization
				for (int i = 0; i < m_knot_v.size() - 1; ++i)
				{
					l_n_prev[i] = vufPolinomCoeff<T, CURVE_DEGREE>();
				}
				l_n_prev[l_knot_index].a[0] = 1.;

#ifdef  VF_MATH_DEBUG_BSPLINE
				// Log Debug Info
				std::cout << "------------------------------------------------" << std::endl;
				std::cout << "interval:[" << m_knot_v[l_knot_index] << "," << m_knot_v[l_knot_index + 1] << "]" << std::endl;
				std::cout << "------------------------------------------------" << std::endl;
				std::cout << "start initialization" << std::endl;
				for (int i = 0; i < l_knot_sz - 1; ++i)
				{
					std::cout << l_n_prev[i].to_string() << " ";
				}
				std::cout << std::endl;
#endif // VF_MATH_DEBUG_BSPLINE

				//For each degree
				for (int l_dgr = 1; l_dgr <= CURVE_DEGREE; ++l_dgr)
				{
					// For each node
					for (int i = 0; i < l_knot_sz - 1 - l_dgr; ++i)
					{
						// first sum
						T l_div_1 = m_knot_v[i + l_dgr] - m_knot_v[i];
						auto l_up_1 = vufPolinomCoeff<T, CURVE_DEGREE>();
						l_up_1.a[0] = -m_knot_v[i];
						l_up_1.a[1] = 1;
						//T l_div_2 = m_knot_v[l_dgr +1 + i ] - 
						l_n_curn[i] = vufPolinomCoeff<T, CURVE_DEGREE>();
						if (l_div_1 != 0)
						{
							l_n_curn[i] += (l_up_1 * l_n_prev[i]) * (1. / l_div_1);
						}
						//second sum
						T l_div_2 = m_knot_v[i + l_dgr + 1] - m_knot_v[i + 1];
						auto l_up_2 = vufPolinomCoeff<T, CURVE_DEGREE>();
						l_up_2.a[0] = m_knot_v[i + l_dgr + 1];
						l_up_2.a[1] = -1;
						if (l_div_2 != 0)
						{
							l_n_curn[i] += (l_up_2 * l_n_prev[i + 1]) * (1. / l_div_2);
						}
					}
					l_n_swap = l_n_curn;
					l_n_curn = l_n_prev;
					l_n_prev = l_n_swap;
				}

				// copy computed basises
				//std::cout << "SIZE " << m_knot_v.size() - 2 * CURVE_DEGREE - 1<< std::endl;
				for (int i = 0; i < l_nodes_sz; ++i)
				{
					m_n_v[l_interval][i] = l_n_swap[i];
					m_dn_v[l_interval][i] = l_n_swap[i].get_derivative();
				}
#ifdef VF_MATH_DEBUG_BSPLINE
				//log
				std::cout << "------------------------------------------------" << std::endl;
				for (int i = 0; i < l_nodes_sz; ++i)
				{
					std::cout << i << ": " << m_n_v[l_interval][i].to_string() << std::endl;
				}
#endif // VF_MATH_DEBUG_BSPLINE

			}
			delete[] l_n_prev;
			delete[] l_n_curn;
			return true;
		}
		
		V<T>				get_pos_at_i(T p_t)		const
		{
			if (p_t < 0.0)
			{
				return vufCurveExplicit<T,V>::m_nodes_pos_v[0] - get_tangent_at_i(0) * (-p_t);
				//return	vufCurve<T, V>::m_nodes_pos_v[0] +
				//		vufCurve<T, V>::m_nodes_pos_v[0] * m_dn_v[0][0].eval(0.0) * (-p_t);
			}
			if (p_t > 1.0)
			{
				int l_last_node_id = (int)vufCurveExplicit<T, V>::m_nodes_pos_v.size() - 1;
				return vufCurveExplicit<T, V>::m_nodes_pos_v[l_last_node_id]  + get_tangent_at_i(1) * (p_t - 1);
			}
			V<T>	l_res;
			int		l_node_id = 0;
			int		l_interval_id = get_interval_index(p_t);
			for (int l_dgr = 0; l_dgr < CURVE_DEGREE + 1; ++l_dgr)
			{
				l_node_id = l_interval_id + l_dgr;
				l_res += vufCurveExplicit<T, V>::m_nodes_pos_v[l_node_id] * m_n_v[l_interval_id][l_node_id].eval(p_t);
			}
			return l_res;
		}
		V<T>				get_tangent_at_i(T p_t) const
		{
			if (p_t < 0.0)
			{
				return vufCurveExplicit<T, V>::m_nodes_pos_v[0] * m_n_v[0][0].eval(0.0);
			}
			if (p_t > 1.0)
			{
				int l_last_node_id = (int)vufCurveExplicit<T, V>::m_nodes_pos_v.size() - 1;
				int l_last_interval_id = (int)m_n_v.size() - 1;
				return vufCurveExplicit<T, V>::m_nodes_pos_v[l_last_node_id] * m_n_v[l_last_interval_id][l_last_node_id].eval(1.0);
			}

			V<T>	l_res;
			int		l_node_id = 0;
			int		l_interval_id = get_interval_index(p_t);
			for (int l_dgr = 0; l_dgr < CURVE_DEGREE + 1; ++l_dgr)
			{
				l_node_id = l_interval_id + l_dgr;
				l_res += vufCurveExplicit<T, V>::m_nodes_pos_v[l_node_id] * m_dn_v[l_interval_id][l_node_id].eval(p_t);
			}
			return l_res;
		}
		T					get_closest_point_param_i(const V<T>& p_point, uint32_t p_divisions = 10, T p_percition = 0.00001) const
		{
			T l_dist_prev, l_dist_next, l_closest_dist, l_closest_t = 0;
			T l_dot_prev, l_dot_next;
			T l_prev_t = 0, l_next_t;
			if (gather_info_i(p_point, 0, l_dist_prev, l_dot_prev) == true)
			{
				// this point is on the curve
				return 0;
			}
			l_closest_dist = l_dist_prev;

			T l_interval_step = 1. / (T)p_divisions;
			for (uint32_t i = 1; i <= p_divisions; ++i)
			{
				l_next_t = (T)i * l_interval_step;
				if (gather_info_i(p_point, l_next_t, l_dist_next, l_dot_next) == true)
				{
					// this point is on the curve
					return l_next_t;
				}

				T l_res_t, l_res_dist;
				if (get_closest_point_param_on_interval_i(p_point, l_prev_t, l_next_t,
					l_dist_prev, l_dist_next,
					l_res_t, l_res_dist, p_percition) == true)
				{
					// the point is on the curve
					return l_res_t;
				}
				if (l_res_dist < l_closest_dist)
				{
					l_closest_dist = l_res_dist;
					l_closest_t = l_res_t;
				}
				l_dist_prev = l_dist_next;
				l_prev_t = l_next_t;
			}
			return l_closest_t;
		}
		// return true if point is on the curve
		VF_MATH_CRV_GET_CLOSEST_PARAM_ON_INTERVAL_I_BODY;
		VF_MATH_CRV_GATHER_INFO_I_BODY;

		// inherited virtual methods
		virtual bool			rebuild(uint32_t p_division_count,
										std::vector<T>& p_uniform_to_curve_val_v,
										std::vector<T>& p_curve_to_uniform_val_v,
										std::vector<T>& p_curve_length_to_val_v) const override
		{
			uint32_t	l_total_div = l_crv_ptr->get_interval_count_i() * p_division_count + 1;;
			T			l_curve_length = 0;

			if (p_uniform_to_curve_val_v.size() != l_total_div)
			{
				p_uniform_to_curve_val_v.resize(l_total_div);
				p_curve_to_uniform_val_v.resize(l_total_div);
				p_curve_length_to_val_v.resize(	l_total_div);
			}
			V<T> l_vec_prev = p_curve.get_pos_at_i(0);
			p_uniform_to_curve_val_v[0] = 0;
			p_curve_to_uniform_val_v[0] = 0;
			p_curve_length_to_val_v[0]	= 0;
			// Compute curve length until sample point
			for (uint64_t i = 1; i < l_total_div; ++i)
			{
				T l_crv_val = ((T)i) / (T(m_total_div - 1));
				V<T> l_vec = get_pos_at_i(l_crv_val);
				p_curve_length_to_val_v[i] = p_curve_length_to_val_v[i - 1] + l_vec_prev.distance_to(l_vec);
				l_vec_prev = l_vec;
			}
			l_curve_length = p_curve_length_to_val_v.back();
			T l_u_1 = 0, l_u_2, l_t_1 = 0, l_t_2;
			uint64_t l_ndx = 0;
			T l_step = 1. / (T(l_total_div - 1));
			T l_dnx = .0, l_tetta = .0;
			for (uint64_t i = 1; i < l_total_div; ++i)
			{
				l_u_2 = p_curve_length_to_val_v[i] / l_curve_length;
				p_curve_to_uniform_val_v[i] = l_u_2;

				l_t_2 = (T(i)) / (T(l_total_div - 1));

				while (l_tetta <= l_u_2)
				{
					p_uniform_to_curve_val_v[l_ndx] = (l_t_1 * (l_u_2 - l_tetta) + l_t_2 * (l_tetta - l_u_1)) / (l_u_2 - l_u_1);
					l_ndx++;
					l_dnx++;
					l_tetta = l_dnx * l_step;
				}
				l_u_1 = l_u_2;
				l_t_1 = l_t_2;
			}
			return true;
		}

		virtual T			get_interval_t_min(int p_interval_index) const override { return get_interval_t_min_i(p_interval_index); }
		virtual T			get_interval_t_max(int p_interval_index) const override	{ return get_interval_t_max_i(p_interval_index); }
		virtual int			get_interval_index(T p_t) const							{ return get_interval_index_i(p_t); }
		virtual uint32_t	get_interval_count() const override						{ return get_interval_count_i(); }

		virtual void		set_nodes_count(uint32_t p_count) override
		{
			// reserve elements from start and end
			if (vufCurveExplicit<T, V>::m_nodes_pos_v.size() != p_count)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v.resize(p_count);
				init_knot_vector();
				update_basises_i();
				//update_max_infuences_i();
				vufCurve<T, V>::m_valid = true;
			}
			if (p_count < CURVE_DEGREE + 1)
			{
				vufCurve<T, V>::m_valid = false;
				return;
			}
			vufCurve<T, V>::m_valid = true;
			return;
		}
		virtual uint32_t	get_nodes_count() const override
		{
			return (uint32_t)vufCurveExplicit<T, V>::m_nodes_pos_v.size();
		}
		/*
		virtual uint32_t	get_span_count()	const override
		{
			return (uint32_t)vufCurveExplicit<T, V>::m_nodes_pos_v.size() - 1;
		}
		*/
		virtual void		set_node_at(uint32_t p_index, const V<T>& p_vector) override
		{
			vufCurveExplicit<T, V>::m_nodes_pos_v[p_index] = p_vector;
		}
		virtual V<T>		get_node_at(uint32_t p_index) const
		{
			return vufCurveExplicit<T, V>::m_nodes_pos_v[p_index];
		}
		
		virtual T			get_closest_point_param(const V<T>& p_point, uint32_t p_divisions = 10, T p_percition = 0.00001)  const override
		{
			return get_closest_point_param_i(p_point, p_divisions, p_percition);
		}
		virtual T			get_closest_point_param_on_interval(	const V<T>& p_point, T p_t_1, T p_t_2, T p_percition = 0.00001) const override
		{
			T l_dist_1 = get_pos_at(p_t_1).distance_to(p_point);
			T l_dist_2 = get_pos_at(p_t_2).distance_to(p_point);
			T l_res_t, l_res_dist;
			return get_closest_point_param_on_interval_i(	p_point, 
															p_t_1, p_t_2, 
															l_dist_1, l_dist_2, 
															l_res_t, l_res_dist, p_percition);
		}

		virtual vufCurveType	get_type()											const override
		{
			return vufCurveType::k_open_bspline;
		}
		
		virtual V<T>		get_pos_at(T p_t)										const override
		{
			return get_pos_at_i(p_t);
		}
		virtual V<T>		get_tangent_at(T p_t)									const override
		{
			return get_tangent_at_i(p_t);
		}
		
		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)	const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			l_ss << l_str_offset << "[ General Open BSpline <" << typeid(T).name() << ", " << typeid(V).name() << ", " << CURVE_DEGREE << "> ]" << std::endl;
			l_ss << l_str_offset << "____Controls Count: " << m_nodes_pos_v.size() << std::endl;
			l_ss << l_str_offset << "____Knots: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_knot_v);
			l_ss << std::endl;
			for (int i = 0; i < (int)m_n_v.size(); ++i)
			{
				l_ss << l_str_offset << "____[ " << get_interval_t_min_i(i) << ", " << get_interval_t_max_i(i) << " ] " << std::endl;
				for (int j = 0; j < (int)m_n_v[i].size(); ++j)
				{
					l_ss << l_str_offset << "____[" << j << "] " << m_n_v[i][j].to_string() <<  std::endl;
				}
			}
			return l_ss.str();
		}
		//virtual uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0) override;
		virtual uint64_t		get_binary_size()													const override
		{
//std::cout << "-------------------------------------OPEN_BSPLINE::get_binary_size()--------------------- "<< std::endl;
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
			if (p_offset == 0)
			{
				return 0;
			}
			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0) override
		{
			vufCurve<T, V>::m_valid = false;
			p_offset = vufCurveExplicit<T, V>::from_binary(p_buff, p_offset);
			if (p_offset == 0)
			{
				vufCurve<T, V>::m_valid = false;
				return 0;
			};
			if (init_knot_vector() == true && update_basises_i() == true)
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
		/*
		virtual void		log_me(int p_tab_count = 0) const override
		{
			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_AQUAA, VF_CONSOLE_COLOR_BLACK);

			std::string l_str_offset(p_tab_count * 4, '.');
			//std::cout << l_str_offset <<"-------------------------------------------------------------" << std::endl;
			std::cout << l_str_offset <<"[ General Open BSpline <" << typeid(T).name() << ", " << typeid(V).name() << ", " << CURVE_DEGREE << "> ]" << std::endl;
			std::cout << l_str_offset <<"Control Points Count: " << vufCurveExplicit<T, V>::m_nodes_pos_v.size() << std::endl;
			//To Do
			// Add Control Points Value Info
			std::cout << l_str_offset <<"....Knot Vector [";
			for (auto i : m_knot_v)
			{
				std::cout << " " << i;
			}
			std::cout << " ]" << std::endl;
			std::cout << l_str_offset << "....Segments: " << get_interval_count()<< " [";
			for (uint32_t i = 0; i < get_interval_count(); ++i)
			{
				std::cout << "(" << get_interval_t_min(i) << "," << get_interval_t_max(i) << ")";
			}
			std::cout << " ]" << std::endl;
			VF_CONSOLE_RESET_COLOR();
		}
		*/

		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 1>>		as_open_bspline_mono()	const override { return nullptr; }
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 2>>		as_open_bspline_di()	const override { return nullptr; }
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 3>>		as_open_bspline_tri()	const override { return nullptr; }
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 4>>		as_open_bspline_tetra()	const override { return nullptr; }
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 5>>		as_open_bspline_penta()	const override { return nullptr; }

	private:
		std::vector<T>												m_knot_v;
		std::vector<std::vector<vufPolinomCoeff<T, CURVE_DEGREE>>>	m_n_v;	// [time interval, node index] = basis function
		std::vector<std::vector<vufPolinomCoeff<T, CURVE_DEGREE>>>	m_dn_v;	// [time interval, node index] = basis derivative
		//std::vector<T>		m_nodes_max_influence_t_v;
		
	};

	//specializations
	template<>
	std::shared_ptr<vufCurveOpenBSpline <double, vufVector4, 1>>		vufCurveOpenBSpline <double, vufVector4, 1>::as_open_bspline_mono()	const
	{
		return std::static_pointer_cast<vufCurveOpenBSpline<double, vufVector4,1>>(vufCurve<double,vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveOpenBSpline <double, vufVector4, 2>>		vufCurveOpenBSpline <double, vufVector4, 2>::as_open_bspline_di()		const
	{
		return std::static_pointer_cast<vufCurveOpenBSpline<double, vufVector4, 2>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveOpenBSpline <double, vufVector4, 3>>		vufCurveOpenBSpline <double, vufVector4, 3>::as_open_bspline_tri()	const
	{
		return std::static_pointer_cast<vufCurveOpenBSpline<double, vufVector4, 3>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>	
	std::shared_ptr<vufCurveOpenBSpline <double, vufVector4, 4>>		vufCurveOpenBSpline <double, vufVector4, 4>::as_open_bspline_tetra()	const
	{
		return std::static_pointer_cast<vufCurveOpenBSpline<double, vufVector4, 4>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveOpenBSpline <double, vufVector4, 5>>		vufCurveOpenBSpline <double, vufVector4, 5>::as_open_bspline_penta()	const
	{
		return std::static_pointer_cast<vufCurveOpenBSpline<double, vufVector4, 5>>(vufCurve<double, vufVector4>::m_this.lock());
	}


#pragma region USING
	using vufOpenBSpline_2f = vufCurveOpenBSpline<float,  vufVector2>;
	using vufOpenBSpline_2d = vufCurveOpenBSpline<double, vufVector2>;
	using vufOpenBSpline_3f = vufCurveOpenBSpline<float,  vufVector3>;
	using vufOpenBSpline_3d = vufCurveOpenBSpline<double, vufVector3>;
	using vufOpenBSpline_4f = vufCurveOpenBSpline<float,  vufVector4>;
	using vufOpenBSpline_4d = vufCurveOpenBSpline<double, vufVector4>;
#pragma endregion
}
#endif // !VF_MATH_OPEN_BSPLINE_CRV_H
