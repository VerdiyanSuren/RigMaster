#ifndef VF_MATH_CURVE_SCALE_2_FN_H
#define VF_MATH_CURVE_SCALE_2_FN_H

namespace vufMath
{
	template<class T,	template<typename> class V>	class vufCurveContainer;
	template <class T,	template<typename> class V>	class vufCurveScaleFn;

	template <class T, template<typename> class V>
	class vufScaleCloseCurveFn : public vufCurveScaleFn<T, V>
	{
	private:
		vufScaleCloseCurveFn() :vufCurveScaleFn<T, V>() {}
	public:
		/// creator
		static std::shared_ptr<vufScaleCloseCurveFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufScaleCloseCurveFn>(new vufScaleCloseCurveFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		// close curve
		V<T>	get_scale_close_curve_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const
		{
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			p_val = p_val - std::floor(p_val);

			auto l_index_1 = m_scale_indeces_v.front();
			auto l_index_2 = m_scale_indeces_v.back();
			if (p_val < m_scale_param_v[l_index_1] || p_val > m_scale_param_v[l_index_2])
			{
				// we have to handle special case when param is betwean last and first element
				T l_interval_length = m_scale_param_v[l_index_1] - m_scale_param_v[l_index_2] + 1;
				if (p_val < m_scale_param_v[l_index_1])
				{
					p_val += 1;
				}
				T l_w_1 = (p_val - m_scale_param_v[l_index_2]) / l_interval_length;
				T l_w_0 = 1. - l_w_1;
				auto l_res = m_scale_a_v[l_index_2] * l_w_0 + m_scale_b_v[l_index_2] * l_w_1;
				return l_res;
			}
			// as open bspline
			return get_scale_open_curve_at(p_curve_container, p_val);
		}
		V<T>	get_scale_open_curve_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const
		{
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			if (p_val <= 0)
			{
				return m_scale_a_v[m_scale_indeces_v.front()];
			}
			if (p_val >= 1.)
			{
				return m_scale_a_v[m_scale_indeces_v.back()];
			}
			for (uint64_t i = 1; i < m_scale_a_v.size(); ++i)
			{
				auto l_index_1 = m_scale_indeces_v[i - 1];
				auto l_index_2 = m_scale_indeces_v[i];
				if (p_val > m_scale_param_v[l_index_1] && p_val < m_scale_param_v[l_index_2])
				{
					T l_interval_length = m_scale_param_v[l_index_2] - m_scale_param_v[l_index_1];
					if (l_interval_length <= VF_MATH_EPSILON)
					{
						return m_scale_a_v[l_index_2];
					}
					T l_w_1 = (p_val - m_scale_param_v[l_index_1]) / l_interval_length;
					T l_w_0 = 1. - l_w_1;
					return m_scale_a_v[l_index_1] * l_w_0 + m_scale_b_v[l_index_1] * l_w_1;
				}
			}
			return V<T>(1., 1., 1.);
		}

		virtual vufCurveScaleFnType get_type() const override { return vufCurveScaleFnType::k_closest_params; }
		virtual void	log_me(int p_tab_count = 0) const override
		{
			std::string l_str_offset(p_tab_count * 4, '.');
			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_YELLOW, VF_CONSOLE_COLOR_BLACK);
			std::cout << std::fixed;
			std::cout << l_str_offset << "Pin_start: " << vufCurveScaleFn<T, V>::m_pin_start << std::endl;
			std::cout << l_str_offset << "Pin_star_value: " << vufCurveScaleFn<T, V>::m_pin_start_value << std::endl;
			std::cout << l_str_offset << "Pin_end: " << vufCurveScaleFn<T, V>::m_pin_end << std::endl;
			std::cout << l_str_offset << "Pin_end value: " << vufCurveScaleFn<T, V>::m_pin_end_value << std::endl;

			VF_CONSOLE_RESET_COLOR();
		}
		/// Set count of scale influencers
		virtual void	set_item_count(uint32_t p_count) override
		{
			if (m_positon_v.size() != p_count)
			{
				m_positon_v.resize(p_count);
				m_scale_a_v.resize(p_count);
				m_scale_b_v.resize(p_count);
				m_scale_param_v.resize(p_count);
				m_scale_indeces_v.resize(p_count);
				for (uint32_t i = 0; i < m_scale_indeces_v.size(); ++i)
				{
					m_scale_indeces_v[i] = i;
				}
			}
		}
		/// Set influence scale value and position in the world. 
		virtual void	set_item_at(uint32_t p_index, const V<T> p_pos, const V<T>& p_scale) override
		{
			uint32_t l_ndx = m_scale_indeces_v[p_index];
			m_positon_v[l_ndx] = p_pos;
			m_scale_a_v[l_ndx] = p_scale;
		}
		/// Compute or set influencer scale params on the curve
		virtual void	compute_bind_param(const vufCurveContainer<T, V>& p_curve_container, uint32_t p_division = 10, T p_percition = 0.00001) override
		{
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			uint32_t l_start = 0, l_end = (uint32_t)m_scale_param_v.size();

			if (l_crv_ptr->is_close() == false)
			{
				if (vufCurveScaleFn<T, V>::m_pin_start == true)
				{
					m_scale_param_v[0] = vufCurveScaleFn<T, V>::m_pin_start_value;
					++l_start;
				}
				if (vufCurveScaleFn<T, V>::m_pin_end == true)
				{
					m_scale_param_v.back() = vufCurveScaleFn<T, V>::m_pin_end_value;
					--l_end;
				}
			}
			
			//--------------------------------
			//  get curve params for each control by closest point
			for (uint32_t i = l_start; i < l_end; ++i)
			{
				m_scale_param_v[i] = l_crv_ptr->get_closest_point_param(m_positon_v[i], p_division, p_percition);
			}
			// initiale sort
			for (uint32_t i = 0; i < m_scale_indeces_v.size(); ++i)
			{
				m_scale_indeces_v[i] = i;
			}
			// Sort scale controls by curve param
			bool m_need_sort = true;
			while (m_need_sort == true)
			{
				m_need_sort = false;
				for (uint32_t i = 1; i < (uint32_t)m_scale_indeces_v.size(); ++i)
				{
					auto l_index_1 = m_scale_indeces_v[i - 1];
					auto l_index_2 = m_scale_indeces_v[i];
					if (m_scale_param_v[l_index_1] > m_scale_param_v[l_index_2])
					{
						m_need_sort = true;
						m_scale_indeces_v[i] = l_index_1;
						m_scale_indeces_v[i - 1] = l_index_2;
					}
				}
			}
			//return true;
		}		
		virtual bool	match_scales(const vufCurveContainer<T, V>& p_curve_container) override
		{
			// Any time when scale values of contols are updated - call this method to update
			for (uint64_t i = 0; i < m_scale_indeces_v.size() - 1; ++i)
			{
				auto l_index_1 = m_scale_indeces_v[i];
				auto l_index_2 = m_scale_indeces_v[i + 1];
				m_scale_b_v[l_index_1] = m_scale_a_v[l_index_2];
			}
			// for last scale
			auto l_index_1 = m_scale_indeces_v.front();
			auto l_index_2 = m_scale_indeces_v.back();
			m_scale_b_v[l_index_2] = m_scale_a_v[l_index_1];
			return true;
		}


		/// get scale interpolated by curve 
		virtual V<T>	get_scale_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const override
		{
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			if (l_crv_ptr->is_close() == true)
			{
				return 	get_scale_close_curve_at(p_curve_container, p_val);
			}
			return get_scale_open_curve_at(p_curve_container, p_val);
		}
		/// Return copy of this curve
		virtual std::shared_ptr< vufCurveScaleFn<T, V>> get_copy() const override
		{
			auto l_ptr = create();
			l_ptr->m_pin_start = vufCurveScaleFn<T, V>::m_pin_start;
			l_ptr->m_pin_start_value = vufCurveScaleFn<T, V>::m_pin_start_value;
			l_ptr->m_pin_end = vufCurveScaleFn<T, V>::m_pin_end;
			l_ptr->m_pin_end_value = vufCurveScaleFn<T, V>::m_pin_end_value;
			l_ptr->m_offset = vufCurveScaleFn<T, V>::m_offset;

			l_ptr->m_positon_v = m_positon_v;
			l_ptr->m_scale_a_v = m_scale_a_v;
			l_ptr->m_scale_b_v = m_scale_b_v;
			l_ptr->m_scale_param_v = m_scale_param_v;
			return l_ptr;
		}
		virtual std::shared_ptr < vufScaleCloseCurveFn<T, V> > as_scale_close_fn() const
		{
			return std::static_pointer_cast<vufScaleCloseCurveFn<T, V>> (vufCurveScaleFn<T, V>::m_this.lock());
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			return std::string();
		}
		virtual uint64_t		get_binary_size()														const override
		{
			uint64_t l_size = vufCurveScaleFn<T,V>::get_binary_size();

			l_size += sizeof(uint64_t) + m_positon_v.size() * sizeof(V<T>);
			l_size += sizeof(uint64_t) + m_scale_a_v.size() * sizeof(V<T>);
			l_size += sizeof(uint64_t) + m_scale_b_v.size() * sizeof(V<T>);
			l_size += sizeof(uint64_t) + m_scale_param_v.size() * sizeof(T);
			l_size += sizeof(uint64_t) + m_scale_indeces_v.size() * sizeof(uint32_t);

			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			p_offset = vufCurveScaleFn<T, V>::to_binary(p_buff, p_offset);
			// m_positon_v
			uint64_t l_size = m_positon_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_positon_v[0], l_size * sizeof(V<T>));
			}
			// m_scale_a_v
			l_size = m_scale_a_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_scale_a_v[0], l_size * sizeof(V<T>));
			}
			// m_scale_b_v
			l_size = m_scale_b_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_scale_b_v[0], l_size * sizeof(V<T>));
			}
			// m_scale_param_v
			l_size = m_scale_param_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_scale_param_v[0], l_size * sizeof(T));
			}
			// m_scale_indeces_v
			l_size = m_scale_indeces_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_scale_indeces_v[0], l_size * sizeof(uint32_t));
			}

			return p_offset;

		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff,uint32_t& p_version, uint64_t p_offset = 0)		override
		{
			p_offset = vufCurveScaleFn<T, V>::from_binary(p_buff, p_version, p_offset);
			uint64_t l_size; 
			// m_positon_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_positon_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_positon_v[0], l_size * sizeof(V<T>));
			}
			// m_scale_a_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_scale_a_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_scale_a_v[0], l_size * sizeof(V<T>));
			}
			// m_scale_b_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_scale_b_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_scale_b_v[0], l_size * sizeof(V<T>));
			}
			// m_scale_param_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_scale_param_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_scale_param_v[0], l_size * sizeof(T));
			}
			// m_scale_indeces_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_scale_indeces_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_scale_indeces_v[0], l_size * sizeof(uint32_t));
			}

			return p_offset;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			return vufCurveScaleFn<T, V>::encode_to_buff(p_buff, p_offset);
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		override
		{
			return vufCurveScaleFn<T, V>::decode_from_buff(p_buff, p_offset);
		}

	private:
		std::vector< V<T>>		m_positon_v;
		std::vector< V<T>>		m_scale_a_v;
		std::vector< V<T>>		m_scale_b_v;
		std::vector<T>			m_scale_param_v;
		std::vector<uint32_t>	m_scale_indeces_v;	// array of indecies, sorted by nodes params on curve
	};
}
#endif // !VF_MATH_CURVE_SCALE_2_FN_H
