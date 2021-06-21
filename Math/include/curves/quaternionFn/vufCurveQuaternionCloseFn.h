#ifndef VF_MATH_CRV_QTRN_CLS_FN_H
#define VF_MATH_CRV_QTRN_CLS_FN_H


namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;
	template<class T, template<typename> class V>	class vufCurveQuaternionFn;

	template <class T, template<typename> class V>
	class vufCurveQuaternionCloseFn : public vufCurveQuaternionFn<T, V>
	{
	private:
		vufCurveQuaternionCloseFn() :vufCurveQuaternionFn<T, V>() {}
	public:
		/// creator
		static std::shared_ptr<vufCurveQuaternionCloseFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveQuaternionCloseFn>(new vufCurveQuaternionCloseFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		// close curve
		vufQuaternion<T> get_quaternion_close_curve_at_i(const vufCurveContainer<T, V>& p_curve_container, T p_val) const
		{
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			p_val = p_val - std::floor(p_val);
			
			auto l_index_1 = m_quat_indeces_v.front();
			auto l_index_2 = m_quat_indeces_v.back();
			if (p_val < m_quat_param_v[l_index_1] || p_val > m_quat_param_v[l_index_2])
			{
				V<T> l_vec(1.0);
				V<T> l_tng;
				vufQuaternion_d l_quat;
				// we have to handle special case when param is betwean last and first element
				T l_interval_length = m_quat_param_v[l_index_1] - m_quat_param_v[l_index_2] + 1;
				if (p_val < m_quat_param_v[l_index_1])
				{
					p_val += 1;
				}
				T l_w_1 = (p_val - m_quat_param_v[l_index_2]) / l_interval_length;
				T l_w_0 = 1. - l_w_1;
				vufQuaternion<T> l_res = m_quaternion_a_v[l_index_2] * l_w_0 + m_quaternion_b_v[l_index_2] * l_w_1;
				l_res.normalize_in_place();
				//drop on axis
				l_tng = l_crv_ptr->get_tangent_at(p_val);
				l_vec = l_res.rotate_vector_by_quaternion(l_vec);
				return l_res.increment_quaternion_with_2vectors(l_vec, l_tng);
			}
			
			// as open bspline
			return get_quaternion_open_curve_at(p_curve_container, p_val);
		}
		// open  curve
		vufQuaternion<T> get_quaternion_open_curve_at_i(const vufCurveContainer<T, V>& p_curve_container, T p_val) const
		{
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			V<T> l_vec(1.0);
			V<T> l_tng;
			vufQuaternion_d l_quat;
			if (p_val <= 0)
			{
				l_tng = l_crv_ptr->get_tangent_at(0.);
				l_quat = m_quaternion_a_v[m_quat_indeces_v.front()];
				l_vec = l_quat.rotate_vector_by_quaternion(l_vec);
				return l_quat.increment_quaternion_with_2vectors(l_vec, l_tng);
			}
			if (p_val >= 1.)
			{
				l_tng = l_crv_ptr->get_tangent_at(1.);
				l_quat = m_quaternion_a_v[m_quat_indeces_v.back()];
				l_vec = l_quat.rotate_vector_by_quaternion(l_vec);
				return l_quat.increment_quaternion_with_2vectors(l_vec, l_tng);
			}
			for (uint64_t i = 1; i < m_quaternion_a_v.size(); ++i)
			{
				auto l_index_1 = m_quat_indeces_v[i - 1];
				auto l_index_2 = m_quat_indeces_v[i];
				if (p_val => m_quat_param_v[l_index_1] && p_val =< m_quat_param_v[l_index_2])
				{
					T l_interval_length = m_quat_param_v[l_index_2] - m_quat_param_v[l_index_1];
					if (l_interval_length <= VF_MATH_EPSILON)
					{
						return m_quaternion_a_v[l_index_2];
					}
					T l_w_1 = (p_val - m_quat_param_v[l_index_1]) / l_interval_length;
					T l_w_0 = 1. - l_w_1;
					vufQuaternion<T> l_res = m_quaternion_a_v[l_index_1] * l_w_0 + m_quaternion_b_v[l_index_1] * l_w_1;
					l_res.normalize_in_place();
					//drop on axis
					l_tng = l_crv_ptr->get_tangent_at(p_val);
					l_vec = l_res.rotate_vector_by_quaternion(l_vec);
					return l_res.increment_quaternion_with_2vectors(l_vec, l_tng);
				}
			}
			return vufQuaternion<T>();
		}
		
		void	set_item_count_i(uint32_t p_count)
		{
			if (m_positon_v.size() != p_count)
			{
				m_y_axis_v.resize(p_count);
				m_positon_v.resize(p_count);
				m_quaternion_a_v.resize(p_count);
				m_quaternion_b_v.resize(p_count);
				m_quat_param_v.resize(p_count);
				m_quat_indeces_v.resize(p_count);

				for (uint32_t i = 0; i < m_quat_indeces_v.size(); ++i)
				{
					m_quat_indeces_v[i] = i;
				}
				T l_step = 1.0 / (T)(p_count - 1);
				for (uint32_t i = 0; i < p_count; ++i)
				{
					m_quat_param_v[i] = ((T)i) * l_step;
				}
			}
		}
		// just update quaternion values
		void	set_item_at_i(uint32_t p_index, const vufMatrix4<T>& p_matr)
		{
			uint32_t l_ndx		= m_quat_indeces_v[p_index];
			m_positon_v[l_ndx]	= V<T>(p_matr[3][0], p_matr[3][1], p_matr[3][2]);
			m_y_axis_v[l_ndx]	= V<T>(p_matr[1][0], p_matr[1][1], p_matr[1][2]);
			m_y_axis_v[l_ndx].normalize_in_place();
			V<T> l_tangent		= l_crv_ptr->get_tangent_normalized_at(m_quat_param_v[l_ndx]);
			m_y_axis_v[l_ndx].make_ortho_to_in_place(l_tangent);
			m_y_axis_v[l_ndx].normalize_in_place();
			vufMatrix4<T> l_matr;
			l_matr.set_axis_x(l_tangent);
			l_matr.set_axis_y(m_y_axis_v[l_ndx]);
			l_matr.set_axis_z(l_z);

			m_quaternion_a_v[i] = l_matr.get_quaternion();
		}
		// update quaternions and set curve value
		void	set_item_at_i(uint32_t p_index, T p_param, const vufMatrix4<T>& p_matr)
		{
			uint32_t l_ndx			= m_quat_indeces_v[p_index];
			m_quat_param_v[l_ndx]	= p_param;
			m_positon_v[l_ndx]		= V<T>(p_matr[3][0], p_matr[3][1], p_matr[3][2]);
			m_y_axis_v[l_ndx]		= V<T>(p_matr[1][0], p_matr[1][1], p_matr[1][2]);
			m_y_axis_v[l_ndx].normalize_in_place();

			V<T> l_tangent = l_crv_ptr->get_tangent_normalized_at(m_quat_param_v[l_ndx]);
			m_y_axis_v[l_ndx].make_ortho_to_in_place(l_tangent);
			m_y_axis_v[l_ndx].normalize_in_place();
			vufMatrix4<T> l_matr;
			l_matr.set_axis_x(l_tangent);
			l_matr.set_axis_y(m_y_axis_v[l_ndx]);
			l_matr.set_axis_z(l_z);

			m_quaternion_a_v[i] = l_matr.get_quaternion();
		}
		// update quaternion and call closest point to update param 
		void	set_item_at_i(			uint32_t					p_index, 
								const	vufMatrix4<T>&				p_matr, 
								const	vufCurveContainer<T, V>&	p_curve_container, 
										uint32_t					p_divisions = 10, 
										T p_percition = vufCurve_kTol)
		{
			uint32_t l_ndx		= m_quat_indeces_v[p_index];
			m_positon_v[l_ndx]	= V<T>(p_matr[3][0], p_matr[3][1], p_matr[3][2]);
			m_y_axis_v[l_ndx]	= V<T>(p_matr[1][0], p_matr[1][1],p_matr[1][2]);
			m_y_axis_v[l_ndx].normalize_in_place();

			m_quat_param_v[l_ndx]	= l_crv_ptr->get_closest_point_param(m_positon_v[l_ndx], p_divisions, p_percition);
			V<T> l_tangent			= l_crv_ptr->get_tangent_normalized_at(m_quat_param_v[l_ndx]);
			m_y_axis_v[l_ndx].make_ortho_to_in_place(l_tangent);
			m_y_axis_v[l_ndx].normalize_in_place();
			vufMatrix4<T> l_matr;
			l_matr.set_axis_x(l_tangent);
			l_matr.set_axis_y(m_y_axis_v[l_ndx]);
			l_matr.set_axis_z(l_z);

			m_quaternion_a_v[i] = l_matr.get_quaternion();
		}
		bool	sort_params_i(bool p_pin_start = true, bool p_pin_end = true)
		{			
			//--------------------------------
			// Sort them by param
			bool m_need_sort = true;
			while (m_need_sort == true)
			{
				m_need_sort = false;
				for (uint32_t i = 1; i < (uint32_t)m_quat_indeces_v.size(); ++i)
				{
					auto l_index_1 = m_quat_indeces_v[i-1];
					auto l_index_2 = m_quat_indeces_v[i];
					if (m_quat_param_v[l_index_1] > m_quat_param_v[l_index_2])
					{
						m_need_sort = true;
						m_quat_indeces_v[i]		= l_index_1;
						m_quat_indeces_v[i-1]	= l_index_2;
					}
				}
			}
			return true;
		}
		bool	match_quaternions_i(const vufCurveContainer<T, V>& p_curve_container)
		{
			if (m_quat_indeces_v.size() == 0)
			{
				vufCurveQuaternionFn<T, V>::m_valid = false;
				return false;
			}
			for (uint64_t i = 0; i < m_quat_indeces_v.size() - 1; ++i)
			{
				auto l_index_1 = m_quat_indeces_v[i]; 
				auto l_index_2 = m_quat_indeces_v[i+1];
				m_quaternion_b_v[l_index_1] = m_quaternion_a_v[l_index_2];
				if (m_quaternion_a_v[l_index_1].dot(m_quaternion_a_v[l_index_2]) < 0.0)
				{
					m_quaternion_b_v[l_index_1] = -m_quaternion_b_v[l_index_1];
				}
			}
			auto l_index_1 = m_quat_indeces_v.front();
			auto l_index_2 = m_quat_indeces_v.back();
			m_quaternion_b_v[l_index_2] = m_quaternion_a_v[l_index_1];
			if (m_quaternion_a_v[l_index_2].dot(m_quaternion_a_v[l_index_1]) < .0)
			{
				m_quaternion_b_v[l_index_2] = -m_quaternion_b_v[l_index_2];
			}
			vufCurveQuaternionFn<T, V>::m_valid = true;
			return true;
		}

		virtual vufCurveQuatFnType	get_type() const override { return vufCurveQuatFnType::k_closest; }
		virtual vufQuaternion<T>	get_quaternion_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const override
		{
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			if (l_crv_ptr->is_close() == true)
			{
				return 	get_quaternion_close_curve_at(p_curve_container, p_val);
			}
			return get_quaternion_open_curve_at(p_curve_container, p_val);
		}

		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>> get_copy() const override
		{
			auto l_ptr = create();
			l_ptr->m_pin_start = vufCurveQuaternionFn<T, V>::m_pin_start;
			//l_ptr->m_pin_start_value = vufCurveQuaternionFn<T, V>::m_pin_start_value;
			l_ptr->m_pin_end = vufCurveQuaternionFn<T, V>::m_pin_end;
			//l_ptr->m_pin_end_value = vufCurveQuaternionFn<T, V>::m_pin_end_value;
			//l_ptr->m_offset = vufCurveQuaternionFn<T, V>::m_offset;
			l_ptr->m_y_axis_v		= m_y_axis_v;
			l_ptr->m_positon_v		= m_positon_v;
			l_ptr->m_quaternion_a_v = m_quaternion_a_v;
			l_ptr->m_quaternion_b_v = m_quaternion_b_v;
			l_ptr->m_quat_param_v	= m_quat_param_v;
			l_ptr->m_quat_indeces_v = m_quat_indeces_v;
			return l_ptr;
		}

		virtual std::shared_ptr < vufCurveQuaternionCloseFn<T, V> > as_closest_fn() const override
		{
			return std::static_pointer_cast<vufCurveQuaternionCloseFn<T, V>> (vufCurveQuaternionCloseFn<T, V>::m_this.lock());
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ vufCurveQuaternionCloseFn < " << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl;
			l_ss << vufCurveQuaternionFn<T, V>::to_string(-1, p_tab_count + 1);
			
			l_ss << l_str_offset << "____y_axis: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_y_axis_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "____positon_v: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_positon_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "____quaternion_a: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_quaternion_a_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "____quaternion_b: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_quaternion_b_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "____quat_param_v: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_quat_param_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "____quat_indeces_v: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_quat_indeces_v);
			l_ss << std::endl;

			return l_ss.str();
		}
		virtual uint64_t		get_binary_size()														const override
		{
			uint64_t l_size = vufCurveQuaternionFn<T, V>::get_binary_size();
			l_size += sizeof(l_size) + m_y_axis_v.size() * sizeof(V<T>);
			l_size += sizeof(l_size) + m_positon_v.size() * sizeof(V<T>);
			l_size += sizeof(l_size) + m_quaternion_a_v.size() * sizeof(vufQuaternion<T>);
			l_size += sizeof(l_size) + m_quaternion_b_v.size() * sizeof(vufQuaternion<T>);
			l_size += sizeof(l_size) + m_quat_param_v.size() * sizeof(T);
			l_size += sizeof(l_size) + m_quat_indeces_v.size() * sizeof(uint32_t);

			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			uint64_t l_size = get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			p_offset = vufCurveQuaternionFn<T, V>::to_binary(p_buff, p_offset);
			// m_y_axis_v
			l_size = m_y_axis_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_y_axis_v[0], l_size * sizeof(V<T>));
			}
			// m_positon_v
			l_size = m_positon_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_positon_v[0], l_size * sizeof(V<T>));
			}
			// m_quaternion_a_v
			l_size = m_quaternion_a_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_quaternion_a_v[0], l_size * sizeof(vufQuaternion<T>));
			}
			// m_quaternion_b_v
			l_size = m_quaternion_b_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_quaternion_b_v[0], l_size * sizeof(vufQuaternion<T>));
			}
			// m_quat_param_v
			l_size = m_quat_param_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_quat_param_v[0], l_size * sizeof(T));
			}
			// m_quat_indeces_v
			l_size = m_quat_indeces_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_quat_indeces_v[0], l_size * sizeof(uint32_t));
			}

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff,uint32_t& p_version,  uint64_t p_offset = 0)		override
		{
			uint64_t l_size;
			p_offset = vufCurveQuaternionFn<T, V>::from_binary(p_buff, p_version, p_offset);
			// m_y_axis_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_y_axis_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_y_axis_v[0], l_size * sizeof(V<T>));
			}
			// m_positon_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_positon_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_positon_v[0], l_size * sizeof(V<T>));
			}
			// m_quaternion_a_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_quaternion_a_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_quaternion_a_v[0], l_size * sizeof(vufQuaternion<T>));
			}
			// m_quaternion_b_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_quaternion_b_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_quaternion_b_v[0], l_size * sizeof(vufQuaternion<T>));
			}
			// m_quat_param_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_quat_param_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_quat_param_v[0], l_size * sizeof(T));
			}
			// m_quat_param_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_quat_indeces_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_quat_indeces_v[0], l_size * sizeof(uint32_t));
			}
			
			return p_offset;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			return vufCurveQuaternionFn<T, V>::encode_to_buff(p_buff, p_offset);
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			return vufCurveQuaternionFn<T, V>::decode_from_buff(p_buff, p_offset);
		}
	private:
		std::vector <V<T>>				m_y_axis_v;			// y axices of nodes nodes array
		std::vector< V<T>>				m_positon_v;		// quaternion nodes 3d position array
		std::vector< vufQuaternion<T>>	m_quaternion_a_v;	// each node has pair of quaternions ( a := my, b := next)
		std::vector< vufQuaternion<T>>	m_quaternion_b_v;
		std::vector<T>					m_quat_param_v;		// parameter on curve
		std::vector<uint32_t>			m_quat_indeces_v;	// array of indecies, sorted by nodes params on curve
	};

}
#endif // !VF_MATH_CRV_QTRN_CLS_FN_H
