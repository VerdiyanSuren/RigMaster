#ifndef VF_MATH_OPEN_BSPLINE_CRV_H
#define VF_MATH_OPEN_BSPLINE_CRV_H

#include <typeinfo>
#include <iostream>
#include <curves/vufCurve.h>
//#include <vufConsoleInclude.h>

//#define VF_MATH_DEBUG_BSPLINE
namespace vufMath
{
	template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>
	class vufOpenBSpline : public vufCurve<T, V>
	{
	private:
	public:
		vufOpenBSpline() :vufCurve<T, V>()
		{
			vufCurve<T, V>::m_degree = CURVE_DEGREE;
			vufCurve<T, V>::m_close = false;
		}
		virtual ~vufOpenBSpline() {}
		static  std::shared_ptr<vufCurve<T, V>> create()
		{
			std::shared_ptr< vufOpenBSpline >  l_ptr = std::shared_ptr<vufOpenBSpline>(new vufOpenBSpline());
			VF_MATH_CURVE_CREATOR_BODY(l_ptr);
			return l_ptr;
		}
		virtual std::shared_ptr<vufCurve<T, V>> make_copy() const override
		{
			std::shared_ptr< vufOpenBSpline > l_ptr = std::static_pointer_cast<vufOpenBSpline>(create());

			l_ptr->m_degree = vufCurve<T, V>::m_degree;
			l_ptr->m_close = vufCurve<T, V>::m_close;
			l_ptr->m_valid = vufCurve<T, V>::m_valid;

			l_ptr->m_nodes_pos_v = vufCurve<T, V>::m_nodes_pos_v;

			l_ptr->m_pos_samples_v = vufCurve<T, V>::m_pos_samples_v;
			l_ptr->m_div_samples_v = vufCurve<T, V>::m_div_samples_v;
			l_ptr->m_param_samples_v = vufCurve<T, V>::m_param_samples_v;

			l_ptr->m_knot_v = m_knot_v;
			l_ptr->m_n_v = m_n_v;
			l_ptr->m_dn_v = m_dn_v;

			return l_ptr;
		}
		//non virtual methods
		/** If we get knot custom vector. From external application, for example*/
	private:
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
			int l_sz = (int)vufCurve<T, V>::m_nodes_pos_v.size();
			if (l_sz < (int)CURVE_DEGREE + 1)
			{
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
		inline int			get_interval_index(T p_t) const
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
		inline int			get_interval_count_i() const
		{
			return ((int)m_knot_v.size() - 2 * (int)CURVE_DEGREE - 1);
		}
		inline int			get_reqiured_knot_size() const
		{
			return (int)vufCurve<T, V>::m_nodes_pos_v + (int)CURVE_DEGREE + 1;
		}
	public:
		/** If this is our curve and we need auto generation of knot vector */
		bool				init_knot_vector()
		{
			return generate_knot_vector(m_knot_v);
		}
		bool				update_basises_i()
		{
			int l_nodes_sz = (int)vufCurve<T, V>::m_nodes_pos_v.size();
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
			m_n_v.resize(l_interval_cnt);
			m_dn_v.resize(l_interval_cnt);
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
		bool				update_max_infuences_i()
		{
			int l_knot_sz = (int)m_knot_v.size();
			int l_nodes_sz = (int)vufCurve<T, V>::m_nodes_pos_v.size();
			int l_interval_cnt = l_knot_sz - 2 * (int)CURVE_DEGREE - 1;

			std::vector<T> l_max_value_v;
			l_max_value_v.resize(l_nodes_sz);
			VF_RESIZE_ARRAY_IF_DIFFERENT_SIZE((vufCurve<T, V>::m_nodes_max_influence_t_v), l_nodes_sz);
			//vufCurve<T, V>::m_nodes_max_influence_t_v.resize(l_nodes_sz);

			for (int i = 0; i < l_nodes_sz; ++i)
			{
				l_max_value_v[i] = 0.0;
			}
			for (int l_interval = 0; l_interval < l_interval_cnt; ++l_interval)
			{

				T l_t_min = m_knot_v[l_interval + CURVE_DEGREE];
				T l_t_max = m_knot_v[l_interval + 1 + CURVE_DEGREE];

				for (int l_dgr = 0; l_dgr < CURVE_DEGREE + 1; ++l_dgr)
				{
					int l_node_index = l_interval + l_dgr;
					T l_param = m_n_v[l_interval][l_node_index].find_max_on_interval(l_t_min, l_t_max);
					T l_value = m_n_v[l_interval][l_node_index].eval(l_param);
					if (l_value > l_max_value_v[l_node_index])
					{
						l_max_value_v[l_node_index] = l_value;
						vufCurve<T, V>::m_nodes_max_influence_t_v[l_node_index] = l_param;
					}
				}
			}
#ifdef VF_MATH_DEBUG_BSPLINE
			std::cout << "---------------------------------------------------" << std::endl;
			std::cout << "Max Influence Array: [";
			for (int i = 0; i < l_nodes_sz; ++i)
			{
				std::cout << vufCurve<T, V>::m_nodes_max_influence_t_v[i] << " ";
			}
			std::cout << "]" << std::endl;
			std::cout << "---------------------------------------------------" << std::endl;
#endif
			return true;
		}
		V<T>				get_pos_at_i(T p_t)		const
		{
			if (p_t < 0.0)
			{
				return	vufCurve<T, V>::m_nodes_pos_v[0] * m_n_v[0][0].eval(0.0) +
					vufCurve<T, V>::m_nodes_pos_v[0] * m_dn_v[0][0].eval(0.0) * (-p_t);
			}
			if (p_t > 1.0)
			{
				int l_last_node_id = (int)vufCurve<T, V>::m_nodes_pos_v.size() - 1;
				int l_last_interval_id = (int)m_n_v.size() - 1;

				return	vufCurve<T, V>::m_nodes_pos_v[l_last_node_id] * m_n_v[l_last_interval_id][l_last_node_id].eval(1.0) +
					vufCurve<T, V>::m_nodes_pos_v[l_last_node_id] * m_dn_v[l_last_interval_id][l_last_node_id].eval(1.0) * (1.0 - p_t);
			}
			V<T>	l_res;
			int		l_node_id = 0;
			int		l_interval_id = get_interval_index(p_t);
			for (int l_dgr = 0; l_dgr < CURVE_DEGREE + 1; ++l_dgr)
			{
				l_node_id = l_interval_id + l_dgr;
				l_res += vufCurve<T, V>::m_nodes_pos_v[l_node_id] * m_n_v[l_interval_id][l_node_id].eval(p_t);
			}
			return l_res;
		}
		V<T>				get_tangent_at_i(T p_t) const
		{
			if (p_t < 0.0)
			{
				return vufCurve<T, V>::m_nodes_pos_v[0] * m_n_v[0][0].eval(0.0);
			}
			if (p_t > 1.0)
			{
				int l_last_node_id = (int)vufCurve<T, V>::m_nodes_pos_v.size() - 1;
				int l_last_interval_id = (int)m_n_v.size() - 1;
				return vufCurve<T, V>::m_nodes_pos_v[l_last_node_id] * m_n_v[l_last_interval_id][l_last_node_id].eval(1.0);
			}

			V<T>	l_res;
			int		l_node_id = 0;
			int		l_interval_id = get_interval_index(p_t);
			for (int l_dgr = 0; l_dgr < CURVE_DEGREE + 1; ++l_dgr)
			{
				l_node_id = l_interval_id + l_dgr;
				l_res += vufCurve<T, V>::m_nodes_pos_v[l_node_id] * m_dn_v[l_interval_id][l_node_id].eval(p_t);
			}
			return l_res;
		}
		T					get_closest_point_param_i(const V<T>& p_point, T p_start_param) 
		{
			make_samples(10);
			V<T> l_p  = vufCurve<T, V>::m_pos_samples_v[0];
			T l_dist = (p_point - l_p).length2();
			int l_short_index = 0;
			for (int i = 1; i < (int)vufCurve<T, V>::m_pos_samples_v.size(); ++i)
			{
				l_p = vufCurve<T, V>::m_pos_samples_v[i];
				if (l_dist > (p_point - l_p).length2())
				{
					l_short_index = i;
				}
			}
			// Try to move right
			//if ()
			// Try to move left

			return  vufCurve<T, V>::m_param_samples_v[l_short_index];;
		}

		// inherited virtual methods
		virtual void		set_nodes_count(uint32_t p_count) override
		{
			// reserve elements from start and end
			if (vufCurve<T, V>::m_nodes_pos_v.size() != p_count)
			{
				vufCurve<T, V>::m_nodes_pos_v.resize(p_count);
				init_knot_vector();
				update_basises_i();
				update_max_infuences_i();
				vufCurve<T, V>::m_valid = true;
			}
		}
		virtual uint32_t	get_nodes_count() const override
		{
			return (uint32_t)vufCurve<T, V>::m_nodes_pos_v.size();
		}
		virtual uint32_t	get_span_count()	const override
		{
			return (uint32_t)vufCurve<T, V>::m_nodes_pos_v.size() - 1;
		}
		virtual void		set_node_at(uint32_t p_index, const V<T>& p_vector) override
		{
			vufCurve<T, V>::m_nodes_pos_v[p_index] = p_vector;
		}
		virtual V<T>		get_node_at(uint32_t p_index) const
		{
			return vufCurve<T, V>::m_nodes_pos_v[p_index];
		}
		virtual T			get_closest_point_param(const V<T>& p_point, T p_start_param)  override
		{
			return get_closest_point_param_i(p_point, p_start_param);
		}
		virtual int			get_interval_count() const override
		{
			return get_interval_count_i();
		}

		virtual void		make_samples(int p_per_interval_count) override
		{
			int l_all_samples = get_interval_count_i() * p_per_interval_count;
			T l_step = 1.0 / (T)l_all_samples;
			++l_all_samples;

			vufCurve<T, V>::m_pos_samples_v.resize(l_all_samples);
			vufCurve<T, V>::m_div_samples_v.resize(l_all_samples);
			vufCurve<T, V>::m_param_samples_v.resize(l_all_samples);

			for (int i = 0; i < l_all_samples; ++i)
			{
				T l_param = l_step * (T)i;
				vufCurve<T, V>::m_pos_samples_v[i] = get_pos_at_i(l_param);
				vufCurve<T, V>::m_div_samples_v[i] = get_tangent_at_i(l_param);
				vufCurve<T, V>::m_param_samples_v[i] = l_param;
			}
		}
		virtual void		update_samples() override
		{
			for (int i = 0; i < vufCurve<T, V>::m_param_samples_v.size(); ++i)
			{
				T l_param = vufCurve<T, V>::m_param_samples_v[i];
				vufCurve<T, V>::m_pos_samples_v[i] = get_pos_at_i(l_param);
				vufCurve<T, V>::m_div_samples_v[i] = get_tangent_at_i(l_param);
			}
		}

		virtual curveType	get_type()												const override
		{
			return curveType::k_open_general_bspline;
		}
		virtual T			get_length()											const override
		{
			//To Do 
			// Implement this
			return 0;
		}
		virtual T			get_basis_at(uint32_t p_node_id, T p_t)					const override
		{
			return m_n_v[get_interval_index(p_t)][p_node_id].eval(p_t);
		}
		virtual T			get_basis_derivative_at(uint32_t p_node_id, double p_t)	const override
		{
			return m_dn_v[get_interval_index(p_t)][p_node_id].eval(p_t);
		}
		virtual V<T>		get_pos_at(T p_t)										const override
		{
			return get_pos_at_i(p_t);
		}
		virtual V<T>		get_tangent_at(T p_t)									const override
		{
			return get_tangent_at_i(p_t);
		}
		virtual bool		update_topology()										override
		{
			return update_basises_i();
		}
		virtual void		log_me(int p_tab_count = 0) const override
		{
			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_AQUAA, VF_CONSOLE_COLOR_BLACK);

			std::string l_str_offset(p_tab_count * 4, '.');
			//std::cout << l_str_offset <<"-------------------------------------------------------------" << std::endl;
			std::cout << l_str_offset <<"[ General Open BSpline <" << typeid(T).name() << ", " << typeid(V).name() << ", " << CURVE_DEGREE << "> ]" << std::endl;
			std::cout << l_str_offset <<"Control Points Count: " << (vufCurve<T, V>::m_nodes_pos_v.size() == 0 ? 0 : vufCurve<T, V>::m_nodes_pos_v.size() ) << std::endl;
			//To Do
			// Add Control Points Value Info
			std::cout << l_str_offset <<"....Knot Vector [";
			for (auto i : m_knot_v)
			{
				std::cout << " " << i;
			}
			std::cout << " ]" << std::endl;
			VF_CONSOLE_RESET_COLOR();

			/*
			std::cout << l_str_offset << "-------------------------------------------------------------" << std::endl;
			*/
		}

	private:
		std::vector<T>												m_knot_v;
		std::vector<std::vector<vufPolinomCoeff<T, CURVE_DEGREE>>>	m_n_v;	// [time interval, node index] = basis function
		std::vector<std::vector<vufPolinomCoeff<T, CURVE_DEGREE>>>	m_dn_v;	// [time interval, node index] = basis derivative
	};
}
#endif // !VF_MATH_OPEN_BSPLINE_CRV_H
