#ifndef VF_MATH_CLOSE_BSPLINE_CRV_H
#define VF_MATH_CLOSE_BSPLINE_CRV_H

#include <curves/vufCurveExplicit.h>
#include <vufNumericArrayObject.h>
#include <vufLog.h>
#include <cmath>

namespace vufMath
{
	template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>
	class vufCloseBSpline : public vufCurveExplicit<T, V>
	{
	private:
	public:
		vufCloseBSpline() :vufCurveExplicit<T, V>()
		{
			vufCurve<T, V>::m_degree = CURVE_DEGREE;
			vufCurve<T, V>::m_close = true;
		}
		virtual ~vufCloseBSpline() {}
		static	std::shared_ptr<vufCloseBSpline> create()
		{
			std::shared_ptr< vufCloseBSpline >  l_ptr = std::shared_ptr<vufCloseBSpline>(new vufCloseBSpline());
			VF_MATH_CURVE_CREATOR_BODY(l_ptr);
			return l_ptr;
		}
		virtual std::shared_ptr<vufCurve<T, V>>  get_copy() const override
		{
			std::shared_ptr< vufCloseBSpline > l_ptr = vufCloseBSpline::create();

			l_ptr->m_degree = vufCurve<T, V>::m_degree;
			l_ptr->m_close = vufCurve<T, V>::m_close;
			l_ptr->m_valid = vufCurve<T, V>::m_valid;

			l_ptr->m_nodes_pos_v = vufCurveExplicit<T, V>::m_nodes_pos_v;
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
		bool				generate_knot_vector(std::vector<T>& p_knots)
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
			/*
			std::cout << "Generated Knot Vector for Periodic BSpline : [";
			for (int i = 0; i < (int)p_knots.size(); ++i)
			{
				std::cout << p_knots[i] << " ";
			}
			std::cout << "]" << std::endl;
			*/
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

		V<T>				get_pos_at_i(T p_t)		const
		{
			if (p_t < 0.0)
			{
				return vufCurveExplicit<T, V>::m_nodes_pos_v[0] - get_tangent_at_i(0) * (-p_t);
				//return	vufCurve<T, V>::m_nodes_pos_v[0] +
				//		vufCurve<T, V>::m_nodes_pos_v[0] * m_dn_v[0][0].eval(0.0) * (-p_t);
			}
			if (p_t > 1.0)
			{
				int l_last_node_id = (int)vufCurveExplicit<T, V>::m_nodes_pos_v.size() - 1;
				return vufCurveExplicit<T, V>::m_nodes_pos_v[l_last_node_id] + get_tangent_at_i(1) * (p_t - 1);
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
		virtual T			get_interval_t_min(int p_interval_index) const override { return get_interval_t_min_i(p_interval_index); }
		virtual T			get_interval_t_max(int p_interval_index) const override { return get_interval_t_max_i(p_interval_index); }
		virtual int			get_interval_index(T p_t) const { return get_interval_index_i(p_t); }
		virtual uint32_t	get_interval_count() const override { return get_interval_count_i(); }

		virtual void		set_nodes_count(uint32_t p_count) override
		{
			if (p_count < CURVE_DEGREE + 1)
			{
				vufCurve<T, V>::m_valid = false;
				return;
			}
			// reserve elements from start and end
			m_node_count = p_count;
			p_count += CURVE_DEGREE;
			if (vufCurveExplicit<T, V>::m_nodes_pos_v.size() != p_count)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v.resize(p_count);
				init_knot_vector();
				update_basises_i();
				//update_max_infuences_i();
			}
			vufCurve<T, V>::m_valid = true;
			return;
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
			vufCurveExplicit<T, V>::m_nodes_pos_v[p_index] = p_vector;
			if (p_index < CURVE_DEGREE)
			{
				vufCurveExplicit<T, V>::m_nodes_pos_v[p_index + m_node_count] = p_vector;
			}
		}
		virtual V<T>		get_node_at(uint32_t p_index) const
		{
			return vufCurveExplicit<T, V>::m_nodes_pos_v[p_index];
		}

		virtual T			get_closest_point_param(const V<T>& p_point, uint32_t p_divisions = 10, T p_percition = 0.00001)  const override
		{
			return get_closest_point_param_i(p_point, p_divisions, p_percition);
		}
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


		virtual vufCurveType	get_type()											const override
		{
			return vufCurveType::k_close_bspline;
		}

		virtual V<T>		get_pos_at(T p_t)										const override
		{
			p_t -= std::floor(p_t);
			return get_pos_at_i(p_t);
		}
		virtual V<T>		get_tangent_at(T p_t)									const override
		{ 
			p_t -= std::floor(p_t);
			return get_tangent_at_i(p_t);
		}
		
		virtual std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			if (p_precision > 0)
			{
				if (p_precision > 64)
				{
					l_ss.precision(64);
				}
				else
				{
					l_ss.precision(p_precision);
				}
			}
			if (p_tab_count > 0)
			{
				for (uint32_t i = 0; i < p_tab_count; ++i)
				{
					l_str_offset = l_str_offset + "____";
				}
			}
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
			// m_knot_v			(array);
			l_size += sizeof(uint64_t) + m_knot_v.size() * sizeof(T);
			// m_n_v			(array(array))
			l_size += sizeof(uint64_t);
			for (uint64_t i = 0; i < m_n_v.size(); ++i)
			{
				l_size += sizeof(uint64_t);
				for (uint64_t j = 0; j < m_n_v[i].size(); ++j)
				{
					l_size += m_n_v[i][j].get_binary_size();
				}
			}
			// m_dn_v			(array(array))
			l_size += sizeof(uint64_t);
			for (uint64_t i = 0; i < m_dn_v.size(); ++i)
			{
				l_size += sizeof(uint64_t);
				for (uint64_t j = 0; j < m_dn_v[i].size(); ++j)
				{
					l_size += m_dn_v[i][j].get_binary_size();
				}
			}
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
			// -- open bspline
			//m_knot_v
			uint64_t l_array_size = m_knot_v.size();
			std::memcpy(&p_buff[p_offset], &l_array_size, sizeof(l_array_size));			p_offset += sizeof(l_array_size);
			std::memcpy(&p_buff[p_offset], m_knot_v.data(), m_knot_v.size() * sizeof(T));	p_offset += m_knot_v.size() * sizeof(T);
			// m_n_v
			l_array_size = m_n_v.size();
			std::memcpy(&p_buff[p_offset], &l_array_size, sizeof(l_array_size));			p_offset += sizeof(l_array_size);
			for (uint64_t i = 0; i < m_n_v.size(); ++i)
			{
				l_array_size = m_n_v[i].size();
				std::memcpy(&p_buff[p_offset], &l_array_size, sizeof(l_array_size));		p_offset += sizeof(l_array_size);
				for (uint64_t j = 0; j < m_n_v[i].size(); ++j)
				{
					p_offset = m_n_v[i][j].to_binary(p_buff, p_offset);
				}
			}
			// m_dn_v
			l_array_size = m_dn_v.size();
			std::memcpy(&p_buff[p_offset], &l_array_size, sizeof(l_array_size));			p_offset += sizeof(l_array_size);
			for (uint64_t i = 0; i < m_dn_v.size(); ++i)
			{
				l_array_size = m_dn_v[i].size();
				std::memcpy(&p_buff[p_offset], &l_array_size, sizeof(l_array_size));		p_offset += sizeof(l_array_size);
				for (uint64_t j = 0; j < m_dn_v[i].size(); ++j)
				{
					p_offset = m_dn_v[i][j].to_binary(p_buff, p_offset);
				}
			}
			std::memcpy(&p_buff[p_offset], &m_node_count, sizeof(m_node_count));			p_offset += sizeof(m_node_count);
			return p_offset;
		}
		virtual uint64_t	from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)
		{
			p_offset = vufCurveExplicit<T, V>::from_binary(p_buff, p_offset);
			if (p_offset == 0)
			{
				return 0;
			};
			//m_knot_v
			uint64_t l_array_size;
			if (p_buff.size() < p_offset + sizeof(l_array_size)) return 0;
			std::memcpy(&l_array_size, &p_buff[p_offset], sizeof(l_array_size));			p_offset += sizeof(l_array_size);
			m_knot_v.resize(l_array_size);
			if (p_buff.size() < p_offset + m_knot_v.size() * sizeof(T)) return 0;
			if (l_array_size > 0)
			{
				std::memcpy(&m_knot_v[0], &p_buff[p_offset], l_array_size * sizeof(T));	p_offset += l_array_size * sizeof(T);
			}
			// m_n_v
			if (p_buff.size() < p_offset + sizeof(l_array_size)) return 0;
			std::memcpy(&l_array_size, &p_buff[p_offset], sizeof(l_array_size));			p_offset += sizeof(l_array_size);
			m_n_v.resize(l_array_size);
			if (l_array_size > 0)
			{
				for (uint64_t i = 0; i < m_n_v.size(); ++i)
				{
					if (p_buff.size() < p_offset + sizeof(l_array_size)) return 0;
					std::memcpy(&l_array_size, &p_buff[p_offset], sizeof(l_array_size));	p_offset += sizeof(l_array_size);
					m_n_v[i].resize(l_array_size);
					for (uint64_t j = 0; j < m_n_v[i].size(); ++j)
					{
						p_offset = m_n_v[i][j].from_binary(p_buff, p_offset);
					}
				}
			}
			// m_dn_v
			if (p_buff.size() < p_offset + sizeof(l_array_size)) return 0;
			std::memcpy(&l_array_size, &p_buff[p_offset], sizeof(l_array_size));			p_offset += sizeof(l_array_size);
			m_dn_v.resize(l_array_size);
			if (l_array_size > 0)
			{
				for (uint64_t i = 0; i < m_dn_v.size(); ++i)
				{
					if (p_buff.size() < p_offset + sizeof(l_array_size)) return 0;
					std::memcpy(&l_array_size, &p_buff[p_offset], sizeof(l_array_size));	p_offset += sizeof(l_array_size);
					m_dn_v[i].resize(l_array_size);
					for (uint64_t j = 0; j < m_dn_v[i].size(); ++j)
					{
						p_offset = m_dn_v[i][j].from_binary(p_buff, p_offset);
					}
				}
			}
			if (p_buff.size() < p_offset + sizeof(m_node_count)) return 0;
			std::memcpy(&m_node_count, &p_buff[p_offset], sizeof(m_node_count));			p_offset += sizeof(m_node_count);
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
		//virtual uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0) override;

		/*
		virtual void		log_me(int p_tab_count = 0) const override
		{
			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_AQUAA, VF_CONSOLE_COLOR_BLACK);

			std::string l_str_offset(p_tab_count * 4, '.');
			//std::cout << l_str_offset <<"-------------------------------------------------------------" << std::endl;
			std::cout << l_str_offset << "[ General Open BSpline <" << typeid(T).name() << ", " << typeid(V).name() << ", " << CURVE_DEGREE << "> ]" << std::endl;
			std::cout << l_str_offset << "Control Points Count: " << vufCurveExplicit<T, V>::m_nodes_pos_v.size() << std::endl;
			//To Do
			// Add Control Points Value Info
			std::cout << l_str_offset << "....Knot Vector [";
			for (auto i : m_knot_v)
			{
				std::cout << " " << i;
			}
			std::cout << " ]" << std::endl;
			std::cout << l_str_offset << "....Segments: " << get_interval_count() << " [";
			for (uint32_t i = 0; i < get_interval_count(); ++i)
			{
				std::cout << "(" << get_interval_t_min(i) << "," << get_interval_t_max(i) << ")";
			}
			std::cout << " ]" << std::endl;
			VF_CONSOLE_RESET_COLOR();

		}
		*/

		// convert to close bspline
		virtual std::shared_ptr<vufCloseBSpline <T, V, 1>>	as_close_bspline_mono()		const { return nullptr; }
		virtual std::shared_ptr<vufCloseBSpline <T, V, 2>>	as_close_bspline_di()		const { return nullptr; }
		virtual std::shared_ptr<vufCloseBSpline <T, V, 3>>	as_close_bspline_tri()		const { return nullptr; }
		virtual std::shared_ptr<vufCloseBSpline <T, V, 4>>	as_close_bspline_tetra()	const { return nullptr; }
		virtual std::shared_ptr<vufCloseBSpline <T, V, 5>>	as_close_bspline_penta()	const { return nullptr; }
	private:
		std::vector<T>												m_knot_v;
		std::vector<std::vector<vufPolinomCoeff<T, CURVE_DEGREE>>>	m_n_v;	// [time interval, node index] = basis function
		std::vector<std::vector<vufPolinomCoeff<T, CURVE_DEGREE>>>	m_dn_v;	// [time interval, node index] = basis derivative
		uint32_t m_node_count = 0;
	};
	//specializations
	template<>
	std::shared_ptr<vufCloseBSpline <double, vufVector4, 1>>		vufCloseBSpline <double, vufVector4, 1>::as_close_bspline_mono()	const
	{
		return std::static_pointer_cast<vufCloseBSpline<double, vufVector4, 1>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCloseBSpline <double, vufVector4, 2>>		vufCloseBSpline <double, vufVector4, 2>::as_close_bspline_di()		const
	{
		return std::static_pointer_cast<vufCloseBSpline<double, vufVector4, 2>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCloseBSpline <double, vufVector4, 3>>		vufCloseBSpline <double, vufVector4, 3>::as_close_bspline_tri()	const
	{
		return std::static_pointer_cast<vufCloseBSpline<double, vufVector4, 3>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCloseBSpline <double, vufVector4, 4>>		vufCloseBSpline <double, vufVector4, 4>::as_close_bspline_tetra()	const
	{
		return std::static_pointer_cast<vufCloseBSpline<double, vufVector4, 4>>(vufCurve<double, vufVector4>::m_this.lock());
	}
	template<>
	std::shared_ptr<vufCloseBSpline <double, vufVector4, 5>>		vufCloseBSpline <double, vufVector4, 5>::as_close_bspline_penta()	const
	{
		return std::static_pointer_cast<vufCloseBSpline<double, vufVector4, 5>>(vufCurve<double, vufVector4>::m_this.lock());
	}
}
#endif !VF_MATH_CLOSE_BSPLINE_CRV_H
