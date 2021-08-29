#ifndef VF_MATH_CLOSE_BSPLINE_CRV_H
#define VF_MATH_CLOSE_BSPLINE_CRV_H

#include <curves/explicit/vufCurveExplicit.h>
#include <vufNumericArrayObject.h>
#include <vufLog.h>
#include <cmath>

namespace vufMath
{
	template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>
	class vufCurveCloseBSpline : public vufCurveExplicit<T, V>
	{
	private:
	public:
		vufCurveCloseBSpline() :vufCurveExplicit<T, V>()
		{
			vufCurve<T, V>::m_has_degree	= true;
			vufCurve<T, V>::m_degree		= CURVE_DEGREE;
			vufCurve<T, V>::m_close			= true;
		}
		virtual ~vufCurveCloseBSpline() {}
		static	std::shared_ptr<vufCurveCloseBSpline>	create()
		{
			std::shared_ptr< vufCurveCloseBSpline >  l_ptr = std::shared_ptr<vufCurveCloseBSpline>(new vufCurveCloseBSpline());
			VF_MATH_CURVE_CREATOR_BODY(l_ptr);
			return l_ptr;
		}
		virtual std::shared_ptr<vufCurve<T, V>>			get_copy() const override
		{
			std::shared_ptr< vufCurveCloseBSpline > l_ptr = vufCurveCloseBSpline::create();

			l_ptr->m_degree = vufCurve<T, V>::m_degree;
			l_ptr->m_close = vufCurve<T, V>::m_close;
			l_ptr->m_valid = vufCurve<T, V>::m_valid;

			l_ptr->m_nodes_pos_v	= vufCurveExplicit<T, V>::m_nodes_pos_v;
			l_ptr->m_pos_offset		= vufCurveExplicit<T, V>::m_pos_offset;
			//l_ptr->m_nodes_max_influence_t_v = m_nodes_max_influence_t_v;

			l_ptr->m_knot_v = m_knot_v;
			l_ptr->m_n_v = m_n_v;
			l_ptr->m_dn_v = m_dn_v;
			return l_ptr;
		}
	private:
		//non virtual methods
		/** If we get knot custom vector. From external application, for example*/
		bool				set_knots_vector_by_other(const T* p_arr, uint64_t p_count)
		{
			// To Do 
			// Add implementation here
		}
		bool				set_knots_vector_by_other(const std::vector<T> p_other)
		{
			return set_knots_vector_by_other(p_other.data(), p_other.size());
		}
		bool				generate_knot_vector_i(std::vector<T>& p_knots)
		{
			int l_sz = (int)vufCurveExplicit<T, V>::m_nodes_pos_v.size();
			if (l_sz < (int)CURVE_DEGREE + 1)
			{
				return false;
			}
			p_knots.resize(l_sz + CURVE_DEGREE + 1);
			T l_normalize_factor = 1. / (T)(l_sz - CURVE_DEGREE );
			T l_t_min = -(T)CURVE_DEGREE * l_normalize_factor;
			for (uint32_t i = 0; i < l_sz + CURVE_DEGREE + 1; ++i)
			{
				p_knots[i] = l_t_min + ((T)(i)) * l_normalize_factor;
			}

#ifdef  VF_MATH_DEBUG_BSPLINE
std::cout << "Generated Knot Vector for Periodic BSpline : [";
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
		bool				init_knot_vector_i()
		{
			return generate_knot_vector_i(m_knot_v);
		}
		bool				update_basises_i()
		{
			int l_nodes_sz = (int)vufCurveExplicit<T, V>::m_nodes_pos_v.size();
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

		inline V<T>			get_pos_at_i(T p_t)		const
		{
			p_t -= std::floor(p_t);
			if (p_t < 0.0)
			{
				return vufCurveExplicit<T, V>::m_nodes_pos_v[0] + get_tangent_at_i(0) * (p_t);
				//return	vufCurve<T, V>::m_nodes_pos_v[0] +
				//		vufCurve<T, V>::m_nodes_pos_v[0] * m_dn_v[0][0].eval(0.0) * (-p_t);
			}
			if (p_t > 1.0)
			{
				int l_last_node_id = (int)vufCurveExplicit<T, V>::m_nodes_pos_v.size() - 1;
				return vufCurveExplicit<T, V>::m_nodes_pos_v[l_last_node_id] + get_tangent_at_i(1) * (p_t - 1);
			}
			V<T>	l_res  = vufCurveExplicit<T, V>::m_pos_offset;
			int		l_node_id = 0;
			int		l_interval_id = get_interval_index(p_t);
			for (int l_dgr = 0; l_dgr < CURVE_DEGREE + 1; ++l_dgr)
			{
				l_node_id = l_interval_id + l_dgr;
				l_res += vufCurveExplicit<T, V>::m_nodes_pos_v[l_node_id] * m_n_v[l_interval_id][l_node_id].eval(p_t);
			}
			return l_res;
		}
		// for closest point to minimize numerical errors
		inline V<T>			get_pos_no_offset_i(T p_t)		const
		{
			p_t -= std::floor(p_t);
			if (p_t < 0.0)
			{
				return get_tangent_at_i(0) * p_t;
				//return vufCurveExplicit<T,V>::m_nodes_pos_v[0] + get_tangent_at_i(0) * p_t;
			}
			if (p_t > 1.0)
			{
				int l_last_node_id = (int)vufCurveExplicit<T, V>::m_nodes_pos_v.size() - 1;
				return vufCurveExplicit<T, V>::m_nodes_pos_v[l_last_node_id] + get_tangent_at_i(1) * (p_t - 1);
				//return vufCurveExplicit<T, V>::m_nodes_pos_v[l_last_node_id]  + get_tangent_at_i(1) * (p_t - 1);
			}
			V<T>	l_res;// = vufCurveExplicit<T, V>::m_pos_offset;
			int		l_node_id = 0;
			int		l_interval_id = get_interval_index_i(p_t);

			for (int l_dgr = 0; l_dgr < CURVE_DEGREE + 1; ++l_dgr)
			{
				l_node_id = l_interval_id + l_dgr;
				l_res += vufCurveExplicit<T, V>::m_nodes_pos_v[l_node_id] * m_n_v[l_interval_id][l_node_id].eval(p_t);
			}
			return l_res;
		}

		inline V<T>			get_tangent_at_i(T p_t) const
		{
			p_t -= std::floor(p_t);
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
		inline V<T>			get_tangent_normalized_at_i(T p_t)	const
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}

		T					get_closest_point_param_i(const V<T>& p_point,T p_start, T p_end, uint32_t p_divisions = 10, T p_percition = vufCurve_kTol) const
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
				//l_next_t = get_interval_t_max(i);
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
		virtual bool		rebuild(std::vector<T>& p_uniform_to_curve_val_v,
									std::vector<T>& p_curve_to_uniform_val_v,
									std::vector<T>& p_curve_val_to_length_v,
									uint32_t		p_division_count = 10,
									T				p_start = 0 /*interval on which we need rebuild*/,
									T				p_end = 1) const override
		{
			uint32_t	l_total_div = get_interval_count_i() * p_division_count + 1;;
			T			l_curve_length = 0;

			if (p_uniform_to_curve_val_v.size() != l_total_div)
			{
				p_uniform_to_curve_val_v.resize(l_total_div);
				p_curve_to_uniform_val_v.resize(l_total_div);
				p_curve_val_to_length_v.resize(l_total_div);
			}
			V<T> l_vec_prev = get_pos_at_i(0);
			p_uniform_to_curve_val_v[0] = 0;
			p_curve_to_uniform_val_v[0] = 0;
			p_curve_val_to_length_v[0] = 0;
			// Compute curve length until sample point
			for (uint64_t i = 1; i < l_total_div; ++i)
			{
				T l_crv_val = ((T)i) / (T(l_total_div - 1));
				V<T> l_vec = get_pos_at_i(l_crv_val);
				p_curve_val_to_length_v[i] = p_curve_val_to_length_v[i - 1] + l_vec_prev.distance_to(l_vec);
				l_vec_prev = l_vec;
			}
			l_curve_length = p_curve_val_to_length_v.back();
			T l_u_1 = 0, l_u_2, l_t_1 = 0, l_t_2;
			uint64_t l_ndx = 0;
			T l_step = 1. / (T(l_total_div - 1));
			T l_dnx = .0, l_tetta = .0;
			for (uint64_t i = 1; i < l_total_div; ++i)
			{
				l_u_2 = p_curve_val_to_length_v[i] / l_curve_length;
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

		virtual bool		rebuild_along_axis(const V<T>& p_axis/*project curve on this axis*/,
												std::vector<T>& p_uniform_to_curve_val_v,
												std::vector<T>& p_curve_to_uniform_val_v,
												std::vector<T>& p_curve_val_to_length_v,
												uint32_t		p_division_count = 10,
												T				p_start = 0 /*interval on which we need rebuild*/,
												T				p_end = 1) const override
		{
			// Tp Do implement this
			return false;
		}
		
		virtual V<T>		get_closest_point(	const V<T>& p_point,
												T			p_start		= 0,
												T			p_end		= 1,
												uint32_t	p_divisions = 10,
												T			p_percition = vufCurve_kTol) const override
		{
			return get_pos_at_i(get_closest_point_param(p_point, p_start, p_end, p_divisions, p_percition));
		}

		virtual T			get_closest_point_param(const V<T>& p_point, 
													T p_start = 0,
													T p_end = 1 /*if p_start == p_end then interval is infinite*/, 
													uint32_t p_divisions = 10, 
													T p_percition = vufCurve_kTol)  const override
		{
			return get_closest_point_param_i(p_point, p_start, p_end, p_divisions, p_percition);
		}
		virtual T			get_param_by_vector_component(	T			p_value,
															uint32_t	p_component_index = 0/*x by default*/,
															T			p_start = 0,
															T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
															uint32_t	p_divisions = 10,
															T			p_percition = vufCurve_kTol)	const  override
		{
			// To avoid crush we increment p_division. Never divide by zero.
			p_divisions++;
			T l_interval_step = (p_end - p_start) / (T)p_divisions;
			T l_res_param = p_start;
			T l_close_value = get_pos_at_i(l_res_param)[p_component_index];
			T l_err = abs(p_value - l_close_value);
			if (l_err < p_percition)
			{
				return  l_res_param;
			}

			for (uint32_t i = 0; i < p_divisions; ++i)
			{
				T l_param_1 = p_start + ((T)i) * l_interval_step;
				T l_param_2 = p_start + ((T)(i + 1)) * l_interval_step;
				T l_param_21 = (l_param_1 + l_param_2) * 0.5;

				T l_x_1 = get_pos_at_i(l_param_1)[p_component_index];
				T l_x_2 = get_pos_at_i(l_param_2)[p_component_index];
				T l_x_21 = get_pos_at_i(l_param_21)[p_component_index];

				T l_err_1 = abs(l_x_1 - p_value);
				T l_err_2 = abs(l_x_2 - p_value);
				T l_err_21 = abs(l_x_21 - p_value);

				if (l_err_1 < l_err)
				{
					l_err = l_err_1;
					l_res_param = l_param_1;
				}
				if (l_err_2 < l_err)
				{
					l_err = l_err_2;
					l_res_param = l_param_2;
				}
				if (l_err_21 < l_err)
				{
					l_err = l_err_21;
					l_res_param = l_param_21;
				}

				if (l_err < p_percition)
				{
					return l_res_param;
				}

				if ((p_value < l_x_1 && p_value < l_x_2) || (l_x_2 < p_value && l_x_1 < p_value))
				{
					continue;
				}
				// check are these point satisfy
				if ((l_x_1 < p_value && p_value < l_x_21) || (l_x_21 < p_value && p_value < l_x_1))
				{
					return get_param_by_vector_component_i(p_value, p_component_index, l_x_1, l_param_1, l_x_21, l_param_21, p_percition);
				}
				if ((l_x_21 < p_value && p_value < l_x_2) || (l_x_2 < p_value && p_value < l_x_21))
				{
					return get_param_by_vector_component_i(p_value, p_component_index, l_x_21, l_param_21, l_x_2, l_param_2, p_percition);
				}
			}
			return l_res_param;
		}
		T					get_param_by_vector_component_i(T			p_value,
															uint32_t	p_component_index = 0/*x by default*/,
															T			p_x_1 = 0,
															T			p_t_1 = 0,
															T			p_x_2 = 1,
															T			p_t_2 = 1,
															T			p_percition = vufCurve_kTol)	const
		{
			T l_middle_param = (p_t_1 + p_t_2) * 0.5;
			T l_middle_value = get_pos_at_i(l_middle_param)[p_component_index];
			if (abs(l_middle_value - p_value) < p_percition)
			{
				return l_middle_param;
			}
			if ((p_x_1 < p_value && p_value < l_middle_value) || (l_middle_value < p_value && p_value < p_x_1))
			{
				return  get_param_by_vector_component_i(p_value, p_component_index, p_x_1, p_t_1, l_middle_value, l_middle_param, p_percition);
			}
			if ((l_middle_value < p_value && p_value < p_x_2) || (p_x_2 < p_value && p_value < l_middle_value))
			{
				return get_param_by_vector_component_i(p_value, p_component_index, l_middle_value, l_middle_param, p_x_2, p_t_2, p_percition);
			}
			return 0;
		}

		virtual T			get_interval_t_min(int p_interval_index) const override { return get_interval_t_min_i(p_interval_index); }
		virtual T			get_interval_t_max(int p_interval_index) const override { return get_interval_t_max_i(p_interval_index); }
		virtual int			get_interval_index(T p_t) const { return get_interval_index_i(p_t); }
		virtual uint32_t	get_interval_count() const override { return get_interval_count_i(); }

		virtual bool		set_nodes_count(uint32_t p_count) override
		{
			if (p_count < CURVE_DEGREE + 1)
			{
				vufCurve<T, V>::m_valid = false;
				return false;
			}
			// reserve elements from start and end
			m_node_count = p_count;
			p_count += CURVE_DEGREE;
			if (vufCurveExplicit<T, V>::m_nodes_pos_v.size() != p_count)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v.resize(p_count);
				init_knot_vector_i();
				update_basises_i();
				//update_max_infuences_i();
			}
			vufCurve<T, V>::m_valid = true;
			return true;
		}
		virtual uint32_t	get_nodes_count() const override
		{
			return m_node_count;
		}
		/*
		virtual uint32_t	get_span_count()	const override
		{
			return (uint32_t)vufCurveExplicit<T, V>::m_nodes_pos_v.size() - 1;
		}
		*/
		virtual void		set_node_at(uint32_t p_index, const V<T>& p_vector) override
		{
			/*
			vufCurveExplicit<T, V>::m_nodes_pos_v[p_index] = p_vector;
			if (p_index < CURVE_DEGREE)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v[p_index + m_node_count] = p_vector;
			}
			*/
			if (p_index == 0)
			{
				vufCurveExplicit<T, V>::m_pos_offset = p_vector;
			}
			vufCurveExplicit<T, V>::m_nodes_pos_v[p_index + m_indent_start] = p_vector - vufCurveExplicit<T, V>::m_pos_offset;
			if (p_index < m_indent_end)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v[p_index + m_indent_start + m_node_count] = p_vector - vufCurveExplicit<T, V>::m_pos_offset;
				return;
			}
			if (p_index > m_node_count -1 - m_indent_start)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v[m_indent_start - (m_node_count - p_index) ] = p_vector - vufCurveExplicit<T, V>::m_pos_offset;
			}
		}
		virtual V<T>		get_node_at(uint32_t p_index) const
		{
			return vufCurveExplicit<T, V>::m_nodes_pos_v[p_index + m_indent_start] + m_pos_offset;
		}

		/*
		virtual T			get_closest_point_param_on_interval(const V<T>& p_point, T p_t_1, T p_t_2, T p_percition = 0.00001) const override
		{
			T l_dist_1 = get_pos_at(p_t_1).distance_to(p_point);
			T l_dist_2 = get_pos_at(p_t_2).distance_to(p_point);
			T l_res_t, l_res_dist;
			return get_closest_point_param_on_interval_i(	p_point,
															p_t_1, p_t_2,
															l_dist_1, l_dist_2,
															l_res_t, l_res_dist, p_percition);
		}
		*/

		virtual vufCurveType	get_curve_type()					const override
		{
			return vufCurveType::k_close_bspline;
		}
		virtual int				get_curve_category()				const override
		{
			return vufCurveCategory::k_bspline_category;
		}

		virtual V<T>		get_pos_at(T p_t)										const override
		{			
			return get_pos_at_i(p_t);
		}
		virtual V<T>		get_tangent_at(T p_t)									const override
		{ 
			return get_tangent_at_i(p_t);
		}
		virtual V<T>		get_tangent_normalized_at(T p_t)						const override
		{
			return get_tangent_normalized_at_i(p_t);
		}

		virtual std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ General Open BSpline: " << typeid(T).name() << ", " << typeid(V).name() << ", degree: " << CURVE_DEGREE << " ]" << std::endl;
			l_ss << l_str_offset << l_str_offset << "Controls count: "	<< m_nodes_pos_v.size() << "]" << std::endl;
			l_ss << l_str_offset << l_str_offset << "Nodes count: "		<< m_node_count << std::endl;
			l_ss << l_str_offset << l_str_offset << "Knot array count:";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_knot_v);
			l_ss<< std::endl;
			l_ss << l_str_offset << l_str_offset << "Nodes count: " << m_node_count << std::endl;
			return l_ss.str();
		}
		virtual uint64_t	get_binary_size()														const override
		{
			//std::cout << "-------------------------------------OPEN_BSPLINE::get_binary_size()--------------------- "<< std::endl;
			uint64_t l_size = 0;
			l_size += vufCurveExplicit<T, V>::get_binary_size();
			l_size += sizeof(m_node_count);
			return  l_size;
		}
		virtual uint64_t	to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			uint64_t l_curve_size = get_binary_size();
			//------------------------------------------------------
			// resize if needed
			if (p_buff.size() < p_offset + l_curve_size)
			{
				p_buff.resize(p_offset + l_curve_size);
			}
			//------------------------------------------------------
			p_offset = vufCurveExplicit<T, V>::to_binary(p_buff, p_offset);
			//std::cout << "vufCurveExplicit<T, V>::to_binary->offset " << p_offset << std::endl;
			if (p_offset == 0)
			{
				return 0;
			}
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_node_count, sizeof(m_node_count));
			return p_offset;
		}
		virtual uint64_t	from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0)
		{
			vufCurve<T, V>::m_valid = false;
			p_offset = vufCurveExplicit<T, V>::from_binary(p_buff, p_version, p_offset);
			if (p_offset == 0)
			{
				vufCurve<T, V>::m_valid = false;
				return 0;
			};
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_node_count, sizeof(m_node_count));
			if (init_knot_vector_i() == true && update_basises_i() == true)
			{
				vufCurve<T, V>::m_valid = true;
			}
			return p_offset;
		}

		virtual uint64_t	encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			return vufCurve<T, V>::encode_to_buff(p_buff, p_offset);
		}
		virtual uint64_t	decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		override
		{
			return vufCurve<T, V>::decode_from_buff(p_buff, p_offset);
		}

		// convert to close bspline
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 1>>	as_close_bspline_mono()		const { return nullptr; }
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 2>>	as_close_bspline_di()		const { return nullptr; }
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 3>>	as_close_bspline_tri()		const { return nullptr; }
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 4>>	as_close_bspline_tetra()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 5>>	as_close_bspline_penta()	const { return nullptr; }
	private:
		std::vector<T>													m_knot_v;
		std::vector<std::vector<vufPolinomCoeff<T, CURVE_DEGREE>>>		m_n_v;	// [time interval, node index] = basis function
		std::vector<std::vector<vufPolinomCoeff<T, CURVE_DEGREE-1>>>	m_dn_v;	// [time interval, node index] = basis derivative
		uint32_t m_node_count = 0;
		uint32_t m_indent_start = CURVE_DEGREE % 2 == 0 ? CURVE_DEGREE/2: (CURVE_DEGREE-1)/2;
		uint32_t m_indent_end	= CURVE_DEGREE % 2 == 0 ? CURVE_DEGREE / 2 : 1 + (CURVE_DEGREE - 1) / 2;
	};
	//specializations
	template<>
	std::shared_ptr<vufCurveCloseBSpline <double, vufVector4, 1>>		vufCurveCloseBSpline <double, vufVector4, 1>::as_close_bspline_mono()	const
	{
		return std::static_pointer_cast<vufCurveCloseBSpline<double, vufVector4, 1>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveCloseBSpline <double, vufVector4, 2>>		vufCurveCloseBSpline <double, vufVector4, 2>::as_close_bspline_di()		const
	{
		return std::static_pointer_cast<vufCurveCloseBSpline<double, vufVector4, 2>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveCloseBSpline <double, vufVector4, 3>>		vufCurveCloseBSpline <double, vufVector4, 3>::as_close_bspline_tri()	const
	{
		return std::static_pointer_cast<vufCurveCloseBSpline<double, vufVector4, 3>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveCloseBSpline <double, vufVector4, 4>>		vufCurveCloseBSpline <double, vufVector4, 4>::as_close_bspline_tetra()	const
	{
		return std::static_pointer_cast<vufCurveCloseBSpline<double, vufVector4, 4>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCurveCloseBSpline <double, vufVector4, 5>>		vufCurveCloseBSpline <double, vufVector4, 5>::as_close_bspline_penta()	const
	{
		return std::static_pointer_cast<vufCurveCloseBSpline<double, vufVector4, 5>>(vufCurve<double, vufVector4>::m_this.lock());
	}

#pragma region CLOSEST_POINTS
	template<>
	double		vufCurveCloseBSpline <double, vufVector4, 1>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
	{
		auto l_point = p_point - vufCurveExplicit<double, vufVector4>::m_pos_offset;
		double l_dist_min = (get_pos_no_offset_i(p_start) - l_point).length2();//std::numeric_limits<double>::max();
		double l_param = p_start;
		double l_temp_dist = (get_pos_no_offset_i(p_end) - l_point).length2();
		if (l_temp_dist < l_dist_min)
		{
			l_dist_min = l_temp_dist;
			l_param = p_end;
		}
		uint32_t l_cnt = get_interval_count_i();
		for (uint32_t l_interval_id = 0; l_interval_id < l_cnt; ++l_interval_id)
		{
			vufPolinomCoeff<double, 1> l_px1, l_py1, l_pz1;
			vufPolinomCoeff<double, 0> l_tx1, l_ty1, l_tz1;
			vufPolinomCoeff<double, 1> l_p;
			double l_min = get_interval_t_min_i(l_interval_id);
			double l_max = get_interval_t_max_i(l_interval_id);
			l_temp_dist = (get_pos_no_offset_i(l_max) - l_point).length2();
			if (l_temp_dist < l_dist_min)
			{
				l_dist_min = l_temp_dist;
				l_param = l_max;
			}
			double p_arr[1];
			for (uint32_t l_dgr = 0; l_dgr < 2; ++l_dgr)
			{
				uint32_t l_node_id = l_interval_id + l_dgr;
				l_px1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].x;
				l_py1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].y;
				l_pz1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].z;
				l_tx1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].x;
				l_ty1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].y;
				l_tz1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].z;
			}
			l_px1.a[0] -= l_point.x;
			l_py1.a[0] -= l_point.y;
			l_pz1.a[0] -= l_point.z;
			l_p = l_px1.mult(l_tx1) + l_py1.mult(l_ty1) + l_pz1.mult(l_tz1);

			auto l_solve_count = l_p.find_root_on_interval(l_min, l_max, p_arr, p_percition);
			if (l_solve_count == 0) continue;
			double l_dist = (get_pos_no_offset_i(p_arr[0]) - l_point).length2();
			if (l_dist < l_dist_min)
			{
				l_dist_min = l_dist;
				l_param = p_arr[0];
			}
		}
		return l_param;
	}
	template<>
	double		vufCurveCloseBSpline <double, vufVector4, 2>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
	{
		auto l_point = p_point - vufCurveExplicit<double, vufVector4>::m_pos_offset;
		double l_dist_min = (get_pos_no_offset_i(p_start) - l_point).length2();//std::numeric_limits<double>::max();
		double l_param = p_start;
		double l_temp_dist = (get_pos_no_offset_i(p_end) - l_point).length2();
		if (l_temp_dist < l_dist_min)
		{
			l_dist_min = l_temp_dist;
			l_param = p_end;
		}
		uint32_t l_cnt = get_interval_count_i();
		for (uint32_t l_interval_id = 0; l_interval_id < l_cnt; ++l_interval_id)
		{
			vufPolinomCoeff<double, 2> l_px1, l_py1, l_pz1;
			vufPolinomCoeff<double, 1> l_tx1, l_ty1, l_tz1;
			vufPolinomCoeff<double, 3> l_p;
			double l_min = get_interval_t_min_i(l_interval_id);
			double l_max = get_interval_t_max_i(l_interval_id);
			l_temp_dist = (get_pos_no_offset_i(l_max) - l_point).length2();
			if (l_temp_dist < l_dist_min)
			{
				l_dist_min = l_temp_dist;
				l_param = l_max;
			}

			double p_arr[3];
			for (uint32_t l_dgr = 0; l_dgr < 3; ++l_dgr)
			{
				uint32_t l_node_id = l_interval_id + l_dgr;
				l_px1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].x;
				l_py1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].y;
				l_pz1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].z;
				l_tx1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].x;
				l_ty1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].y;
				l_tz1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].z;
			}
			l_px1.a[0] -= l_point.x;
			l_py1.a[0] -= l_point.y;
			l_pz1.a[0] -= l_point.z;
			l_p = l_px1.mult(l_tx1) + l_py1.mult(l_ty1) + l_pz1.mult(l_tz1);
			//std::cout << l_p.to_string() << std::endl;
			auto l_solve_count = l_p.find_root_on_interval(l_min, l_max, p_arr, p_percition);
			if (l_solve_count == 0) continue;
			for (int i = 0; i < l_solve_count; ++i)
			{
				double l_dist = (get_pos_no_offset_i(p_arr[i]) - l_point).length2();
				if (l_dist < l_dist_min)
				{
					l_dist_min = l_dist;
					l_param = p_arr[i];
				}
			}
		}
		return l_param;
	}
	template<>
	double		vufCurveCloseBSpline <double, vufVector4, 3>::get_closest_point_param_i(const vufVector4<double>& p_point, double p_start, double p_end, uint32_t p_divisions, double p_percition) const
	{
		//vuf::vufTimer l_timer("Closest Point ");
		auto l_point = p_point - vufCurveExplicit<double, vufVector4>::m_pos_offset;
		double l_dist_min = (get_pos_no_offset_i(p_start) - l_point).length2();//std::numeric_limits<double>::max();
		double l_param = p_start;
		double l_temp_dist = (get_pos_no_offset_i(p_end) - l_point).length2();
		if (l_temp_dist < l_dist_min)
		{
			l_dist_min = l_temp_dist;
			l_param = p_end;
		}

		uint32_t l_cnt = get_interval_count_i();
		for (uint32_t l_interval_id = 0; l_interval_id < l_cnt; ++l_interval_id)
		{
			vufPolinomCoeff<double, 3> l_px1, l_py1, l_pz1;
			vufPolinomCoeff<double, 2> l_tx1, l_ty1, l_tz1;
			vufPolinomCoeff<double, 5> l_p; // polinom for (p-p(t)).dot(tangent(t))
			double l_min = get_interval_t_min_i(l_interval_id);
			double l_max = get_interval_t_max_i(l_interval_id);
			l_temp_dist = (get_pos_no_offset_i(l_max) - l_point).length2();
			if (l_temp_dist < l_dist_min)
			{
				l_dist_min = l_temp_dist;
				l_param = l_max;
			}
			double p_arr[3];
			for (uint32_t l_dgr = 0; l_dgr < 4; ++l_dgr)
			{
				uint32_t l_node_id = l_interval_id + l_dgr;
				l_px1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].x;
				l_py1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].y;
				l_pz1 += m_n_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].z;
				l_tx1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].x;
				l_ty1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].y;
				l_tz1 += m_dn_v[l_interval_id][l_node_id] * vufCurveExplicit<double, vufVector4>::m_nodes_pos_v[l_node_id].z;
			}
			l_px1.a[0] -= l_point.x;
			l_py1.a[0] -= l_point.y;
			l_pz1.a[0] -= l_point.z;
			l_p = l_px1.mult(l_tx1) + l_py1.mult(l_ty1) + l_pz1.mult(l_tz1);
			//std::cout << l_p.to_string() << std::endl;
			auto l_solve_count = l_p.find_root_on_interval(l_min, l_max, p_arr, p_percition);
			if (l_solve_count == 0) continue;
			//std::cout << "	finded interval: " << l_interval_id << " " << l_min << " " << l_max << " " << l_solve_count << std::endl;
			for (int i = 0; i < l_solve_count; ++i)
			{
				double l_dist = (get_pos_no_offset_i(p_arr[i]) - l_point).length2();
				if (l_dist < l_dist_min)
				{
					l_dist_min = l_dist;
					l_param = p_arr[i];
				}
			}
		}
		//std::cout << " param: " << l_param << std::endl;
		return l_param;
	}

#pragma endregion
#pragma region USING
	using vufCloseBSpline_2f = vufCurveCloseBSpline<float, vufVector2>;
	using vufCloseBSpline_2d = vufCurveCloseBSpline<double, vufVector2>;
	using vufCloseBSpline_3f = vufCurveCloseBSpline<float, vufVector3>;
	using vufCloseBSpline_3d = vufCurveCloseBSpline<double, vufVector3>;
	using vufCloseBSpline_4f = vufCurveCloseBSpline<float, vufVector4>;
	using vufCloseBSpline_4d = vufCurveCloseBSpline<double, vufVector4>;
#pragma endregion



}
#endif !VF_MATH_CLOSE_BSPLINE_CRV_H
