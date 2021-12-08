#ifndef VF_MATH_SCALE_PARAM_FN_H
#define VF_MATH_SCALE_PARAM_FN_H

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurve;
	template<class T, template<typename> class V>	class vufCurveContainer;
	template<class T, template<typename> class V>	class vufCurveQuaternionFn;
	template <class T, template<typename> class V>
	class vufCurveScaleParamFn : public vufCurveScaleFn<T, V>
	{
	private:
		vufCurveScaleParamFn() :vufCurveScaleFn<T, V>() {}
	public:
		static std::shared_ptr<vufCurveScaleParamFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveScaleParamFn>(new vufCurveScaleParamFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}

		// close curve
		V<T>	get_scale_close_curve_at_i(const vufCurve<T, V>& p_curve, T p_val) const
		{
			p_val = p_val - std::floor(p_val);

			auto l_index_1 = m_scale_indeces_v.front();
			auto l_index_2 = m_scale_indeces_v.back();
			if (p_val < m_scale_param_v[l_index_1] || p_val > m_scale_param_v[l_index_2])
			{
				V<T> l_vec(1.0);
				V<T> l_tng;
				vufQuaternion_d l_quat;
				// we have to handle special case when param is betwean last and first element
				T l_interval_length = m_scale_param_v[l_index_1] - m_scale_param_v[l_index_2] + 1.;
				if (p_val < m_scale_param_v[l_index_1])
				{
					p_val += 1;
				}
				T l_w_1 = (p_val - m_scale_param_v[l_index_2]) / l_interval_length;
				T l_w_0 = 1. - l_w_1;
				auto l_res = m_scale_a_v[l_index_2] * l_w_0 + m_scale_b_v[l_index_2] * l_w_1;
				return l_res;
			}

			// as open spline
			return get_scale_open_curve_at_i(p_curve, p_val);
		}
		// open
		V<T>	get_scale_open_curve_at_i(const vufCurve<T, V>& p_curve, T p_val) const
		{
			V<T> l_vec(1.0);
			V<T> l_tng;
			vufQuaternion_d l_quat;
			if (p_val <= m_scale_param_v[m_scale_indeces_v.front()])
			{
				return m_scale_a_v[m_scale_indeces_v.front()];;
			}
			if (p_val >= m_scale_param_v[m_scale_indeces_v.back()])
			{
				return m_scale_a_v[m_scale_indeces_v.back()];
			}
			for (uint64_t i = 1; i < m_scale_a_v.size(); ++i)
			{
				auto l_index_1 = m_scale_indeces_v[i - 1];
				auto l_index_2 = m_scale_indeces_v[i];
				if (p_val >= m_scale_param_v[l_index_1] && p_val <= m_scale_param_v[l_index_2])
				{
					T l_interval_length = m_scale_param_v[l_index_2] - m_scale_param_v[l_index_1];
					if (l_interval_length <= VF_MATH_EPSILON)
					{
						return (m_scale_a_v[l_index_1] + m_scale_b_v[l_index_1])*0.5;
					}
					T l_w_1 = (p_val - m_scale_param_v[l_index_1]) / l_interval_length;
					T l_w_0 = 1. - l_w_1;
					auto l_res = m_scale_a_v[l_index_1] * l_w_0 + m_scale_b_v[l_index_1] * l_w_1;
					return l_res;
				}
			}
			return V<T>(1,1,1);
		}
		void	set_item_count_i(uint32_t p_count)
		{
			if (m_scale_param_v.size() != p_count)
			{
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
		bool	set_item_at_i(	uint32_t				p_index,
								T						p_param,/*orginal curve param*/
								const	vufMatrix4<T>&	p_matr)
		{
			m_scale_param_v[p_index]	= p_param;
			m_scale_a_v[p_index].x		= p_matr.get_scale_x();
			m_scale_a_v[p_index].y		= p_matr.get_scale_y();
			m_scale_a_v[p_index].z		= p_matr.get_scale_z();
			return true;
		}
		bool	set_item_at_i(uint32_t					p_index,
								const	vufMatrix4<T>&	p_matr)
		{
			m_scale_a_v[p_index].x = p_matr.get_scale_x();
			m_scale_a_v[p_index].y = p_matr.get_scale_y();
			m_scale_a_v[p_index].z = p_matr.get_scale_z();
			return true;
		}
		bool	sort_params_i()
		{
			// Sort them by param
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
						m_scale_indeces_v[i]		= l_index_1;
						m_scale_indeces_v[i - 1]	= l_index_2;
					}
				}
			}
			return true;
		}
		bool	match_scales_i()
		{
			if (m_scale_param_v.size() == 0)
			{
				vufCurveScaleFn<T, V>::m_valid = false;
				return false;
			}
			for (uint64_t i = 0; i < m_scale_param_v.size() - 1; ++i)
			{
				m_scale_b_v[i] = m_scale_a_v[i + 1];
			}
			auto l_index_2 = m_scale_param_v.size() - 1;
			m_scale_b_v[l_index_2] = m_scale_a_v[0];
			vufCurveScaleFn<T, V>::m_valid = true;
			return true;

		}
		std::vector<T>& get_params_i()
		{
			return m_quat_param_v;
		}

		//virtuals
		virtual V<T>				get_scale_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const override
		{
			auto l_curve = p_curve_container.get_curve_ptr();
			if (l_curve->is_close() == true)
			{
				return 	get_scale_close_curve_at_i(*l_curve, p_val);
			}
			return get_scale_open_curve_at_i(*l_curve, p_val);
		}

		virtual vufCurveScaleFnType	get_type() const override { return vufCurveScaleFnType::k_params; }
		virtual std::shared_ptr< vufCurveScaleFn<T, V>> get_copy() const override
		{
			auto l_ptr = create();
			l_ptr->m_valid				= m_valid;
			l_ptr->m_scale_a_v			= m_scale_a_v;
			l_ptr->m_scale_b_v			= m_scale_b_v;
			l_ptr->m_scale_param_v		= m_scale_param_v;
			l_ptr->m_scale_indeces_v	= m_scale_indeces_v;
			return l_ptr;
		}
		virtual std::shared_ptr < vufCurveScaleParamFn<T, V> > as_params_fn() const  override
		{ 
			return std::static_pointer_cast<vufCurveScaleParamFn<T, V>> (vufCurveScaleFn<T, V>::m_this.lock());
		}
		
		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ vufCurveScaleParamFn < " << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl;
			l_ss << vufCurveScaleFn<T, V>::to_string(-1, p_tab_count + 1);

			l_ss << l_str_offset << "____scale_a: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_scale_a_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "____scale_b: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_scale_b_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "____scale_param_v: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_scale_param_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "____scale_indeces_v: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_scale_indeces_v);
			l_ss << std::endl;

			return l_ss.str();
		}
		virtual uint64_t		get_binary_size()														const override
		{
			uint64_t l_size = vufCurveScaleFn<T, V>::get_binary_size();

			l_size += sizeof(l_size) + m_scale_a_v.size() * sizeof(vufQuaternion<T>);
			l_size += sizeof(l_size) + m_scale_b_v.size() * sizeof(vufQuaternion<T>);
			l_size += sizeof(l_size) + m_scale_param_v.size() * sizeof(T);

			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			uint64_t l_size = get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			p_offset = vufCurveScaleFn<T, V>::to_binary(p_buff, p_offset);

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
			// m_quat_param_v
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
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)		override
		{
			uint64_t l_size;
			p_offset = vufCurveScaleFn<T, V>::from_binary(p_buff, p_offset);

			// m_scale_a_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_scale_a_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_scale_a_v[0], l_size * sizeof(V<T>));
			}
			// m_scale_v
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
			// m_scale_param_indeces_v
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
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			return vufCurveScaleFn<T, V>::decode_from_buff(p_buff, p_offset);
		}

	private:
		std::vector<uint32_t>	m_scale_indeces_v;
		std::vector< V<T>>		m_scale_a_v;
		std::vector< V<T>>		m_scale_b_v;
		std::vector<T>			m_scale_param_v; // parameter on curve

	};
}

#endif // !VF_MATH_SCALE_PARAM_FN_H