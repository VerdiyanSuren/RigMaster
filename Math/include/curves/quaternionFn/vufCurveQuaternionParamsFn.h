#ifndef VF_MATH_QUAT_PARAM_FN_H
#define VF_MATH_QUAT_PARAM_FN_H

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurve;
	template<class T, template<typename> class V>	class vufCurveContainer;
	template<class T, template<typename> class V>	class vufCurveQuaternionFn;
	template <class T, template<typename> class V>
	class vufCurveQuaternionParamFn : public vufCurveQuaternionFn<T, V>
	{
	private:
		vufCurveQuaternionParamFn() :vufCurveQuaternionFn<T, V>() {}
	public:
		static std::shared_ptr<vufCurveQuaternionParamFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveQuaternionParamFn>(new vufCurveQuaternionParamFn<T, V>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		// close curve
		vufQuaternion<T> get_quaternion_close_curve_at_i(const vufCurve<T, V>& p_curve, T p_val) const
		{
			return vufQuaternion<T>();
		}
		// open  curve
		vufQuaternion<T> get_quaternion_open_curve_at_i(const vufCurve<T, V>& p_curve, T p_val) const
		{
			return vufQuaternion<T>();
		}

		void	set_item_count_i(uint32_t p_count)
		{			
		}
		// just update quaternion values
		bool	set_item_at_i(	uint32_t									p_index,
								const	vufMatrix4<T>&						p_matr,
								const	std::shared_ptr<vufCurve<T, V> >	p_crv_ptr)
		{
			
			return true;
		}
		// update quaternions and set curve value
		bool	set_param_item_at_i(uint32_t									p_index,
									T											p_param,
									const	vufMatrix4<T>&						p_matr,
									const	std::shared_ptr<vufCurve<T, V> >	p_crv_ptr)
		{
			return true;
		}
		// update quaternion and call closest point to update param 
		bool	match_quaternions_i()
		{			
			return true;
		}
		std::vector<T>& get_params()
		{
			return m_quat_param_v;
		}
		//virtuals
		virtual vufCurveQuatFnType	get_type() const override { return vufCurveQuatFnType::k_param; }
		virtual vufQuaternion<T>	get_quaternion_at(const vufCurveContainer<T, V>& p_curve_container, T p_val, T p_rebuild_value) const override
		{
			auto l_curve = p_curve_container.get_curve_ptr();
			if (l_curve->is_close() == true)
			{
				return 	get_quaternion_close_curve_at_i(*l_curve, p_val);
			}
			return get_quaternion_open_curve_at_i(*l_curve, p_val);
		}

		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>> get_copy() const override
		{
			auto l_ptr = create();

			l_ptr->m_pin_start = m_pin_start;
			l_ptr->m_pin_start_value = m_pin_start_value;
			l_ptr->m_pin_end = m_pin_end;
			l_ptr->m_pin_end_value = m_pin_end_value;
			l_ptr->m_offset = m_offset;

			l_ptr->m_y_axis_v = m_y_axis_v;
			l_ptr->m_positon_v = m_positon_v;
			l_ptr->m_quaternion_a_v = m_quaternion_a_v;
			l_ptr->m_quaternion_b_v = m_quaternion_b_v;
			l_ptr->m_quat_param_v = m_quat_param_v;
			l_ptr->m_quat_indeces_v = m_quat_indeces_v;
			return l_ptr;
		}

		virtual std::shared_ptr < vufCurveQuaternionParamFn<T, V> > as_params_fn() const override
		{
			return std::static_pointer_cast<vufCurveQuaternionParamFn<T, V>> (vufCurveQuaternionParamFn<T, V>::m_this.lock());
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ vufCurveQuaternionParamFn < " << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl;
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
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0)		override
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
	};
}

#endif // !VF_MATH_QUAT_PARAM_FN_H
