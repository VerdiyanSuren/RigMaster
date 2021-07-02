#ifndef VF_MATH_QUAT_2_ENDS_FN_H
#define VF_MATH_QUAT_2_ENDS_FN_H

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurve;
	template<class T, template<typename> class V>	class vufCurveContainer;
	template<class T, template<typename> class V>	class vufCurveQuaternionFn;
	template <class T, template<typename> class V>
	class vufCurveQuaternion2EndsFn : public vufCurveQuaternionFn<T, V>
	{
	private:
		vufCurveQuaternion2EndsFn() :vufCurveQuaternionFn<T, V>() {}
	public:
		enum 
		{
			k_transport = 0,
			k_binormal	= 1
		};
		/// creator
		static std::shared_ptr<vufCurveQuaternion2EndsFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveQuaternion2EndsFn>(new vufCurveQuaternion2EndsFn<T, V>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		// close curve
		vufQuaternion<T> get_quaternion_close_curve_at_i(const vufCurve<T, V>& p_curve, T p_val, T p_rebuild_val) const
		{
			// To Do 
			// Implement 
			return vufQuaternion<T>();
		}
		// open  curve
		vufQuaternion<T> get_quaternion_open_curve_at_i(const vufCurve<T, V>& p_curve, T p_val, T p_rebuild_val) const
		{
			// To Do 
			// Implement 
			return vufQuaternion<T>();
		}
		virtual vufCurveQuatFnType	get_type() const override { return vufCurveQuatFnType::k_2_ends; }
		bool						set_division_i(uint32_t p_divisions, std::shared_ptr<vufCurve<T, V> >  p_crv)
		{
			p_divisions++;
			if (p_crv == nullptr || p_crv->is_valid() == false)
			{
				vufCurveQuaternionFn<T, V>::m_valid = false;
				return false;
			}
			
			if (m_tangent_v.size() != p_divisions + 1)
			{
				m_y_axis_v.resize(p_divisions + 1);
				m_tangent_v.resize(p_divisions + 1);
				m_quaternion_a_v.resize(p_divisions + 1);
				m_quaternion_b_v.resize(p_divisions + 1);
				m_quat_param_v.resize(p_divisions + 1);
			}
			uint32_t	l_start = 0;
			uint32_t	l_end = p_divisions + 1;
			m_quat_param_v[0] = 0;
			m_quat_param_v.back() = 1.;
			T l_step = 1.0 / (T)(p_divisions);
			for (uint32_t i = 1; i < p_divisions; ++i)
			{
				m_quat_param_v[i] = m_quat_param_v[0] + l_step * (T)i;
			}
			vufCurveQuaternionFn<T, V>::m_valid = true;
			return true;
		}
	public:
		void						set_root_item_start_i(const vufMatrix4<T>& p_matr)
		{
			m_y_axis_v[0] = V<T>(p_matr[1][0], p_matr[1][1], p_matr[1][2]);
			m_y_axis_v[0].normalize_in_place();
		}
		void						set_root_item_end_i(const vufMatrix4<T>& p_matr)
		{
			m_y_axis_v.back() = V<T>(p_matr[1][0], p_matr[1][1], p_matr[1][2]);
			m_y_axis_v.back().normalize_in_place();
		}
		void						set_fcurve_i(const std::shared_ptr <vufCurve<T, V> > p_crv)
		{
			m_twist_fcurve = p_crv;
		}
		std::shared_ptr <vufCurve<T, V> >	get_f_curve_i() const
		{
			return m_twist_fcurve;
		}
		void						set_twist_start_mult_i(T p_val)
		{
			m_twist_start_mult = p_val;
		}
		T							get_twist_start_mult_i() const
		{
			return m_twist_start_mult;
		}
		void						set_twist_end_mult_i(T p_val)
		{
			m_twist_end_mult = p_val;
		}
		T							get_twist_end_mult_i() const
		{
			return m_twist_end_mult;
		}
		void						set_mode_i(uint8_t p_mode)
		{
			m_mode = p_mode;
		}
		uint8_t						get_mode_i() const
		{
			return m_mode;
		}

		bool						compute_bind_params_i(const std::shared_ptr<vufCurve<T, V> > p_crv_ptr)
		{
			if (p_crv_ptr == nullptr || p_crv_ptr->is_valid() == false)
			{
				vufCurveQuaternionFn<T, V>::m_valid = false;
				return false;
			}
			if (p_crv_ptr->is_open() == true)
			{
				return  m_mode == k_transport ? compute_bind_params_open_transport_i(p_crv_ptr): compute_bind_params_open_binormal_i(p_crv_ptr);
			}
			return compute_bind_params_close_i(p_crv_ptr);
		}
	private:
		bool						compute_bind_params_open_transport_i(const  std::shared_ptr<vufCurve<T, V> > p_crv_ptr)
		{
			//-----------------------------------
			// for first root param start
			m_tangent_v[0] = p_crv_ptr->get_tangent_normalized_at(m_quat_param_v[0]);
			m_y_axis_v[0].make_ortho_to_in_place(m_tangent_v[0]);
			m_y_axis_v[0].normalize_in_place();
			//-----------------------------------
			// for end root param start
			m_tangent_v.back() = p_crv_ptr->get_tangent_normalized_at(m_quat_param_v.back());
			m_y_axis_v.back().make_ortho_to_in_place(m_tangent_v.back());
			m_y_axis_v.back().normalize_in_place();
			//-----------------------------------
			// forward. from root start to end
			{
				for (uint32_t i = 1; i < m_quat_param_v.size(); ++i)
				{
					m_tangent_v[i] = p_crv_ptr->get_tangent_normalized_at(m_quat_param_v[i]);
					V<T> l_cross = m_tangent_v[i - 1].get_cross(m_tangent_v[i]);

					if (l_cross.length2() < 0.000001)
					{
						m_y_axis_v[i] = m_y_axis_v[i - 1];
						continue;
					}
					l_cross.normalize_in_place();
					T l_dot = m_tangent_v[i - 1].dot(m_tangent_v[i]);
					T l_angle = -acos(VF_CLAMP(-1., 1., l_dot));
					vufQuaternion<T> l_rot(l_angle, l_cross);
					m_y_axis_v[i] = l_rot.rotate_vector_by_quaternion(m_y_axis_v[i - 1]);
					m_y_axis_v[i].normalize_in_place();
					if (m_y_axis_v[i].dot(m_y_axis_v[i - 1]) < 0.0)
					{
						m_y_axis_v[i] = -m_y_axis_v[i];
					}
				}
				// convert to quaternion
				for (uint32_t i = 0; i < m_quat_param_v.size(); ++i)
				{
					V<T> l_z = m_tangent_v[i].get_cross(m_y_axis_v[i]);
					l_z.normalize_in_place();
					vufMatrix4<T> l_matr;
					l_matr.set_axis_x(m_tangent_v[i]);
					l_matr.set_axis_y(m_y_axis_v[i]);
					l_matr.set_axis_z(l_z);
					m_quaternion_a_v[i] = l_matr.get_quaternion();
				}
			}
			return true;
		}
		bool						compute_bind_params_close_i(const std::shared_ptr<vufCurve<T, V> > p_crv_ptr)
		{
			return true;
		}
	public:
		bool						match_quaternions_i(const vufCurveContainer<T, V>& p_curve_container)
		{
			return true;
		}

		virtual vufQuaternion<T>	get_quaternion_at(const vufCurveContainer<T, V>& p_curve_container, T p_val, T p_rebuild_val) const override
		{
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			if (l_crv_ptr->is_close() == true)
			{
				return 	get_quaternion_close_curve_at_i(*l_crv_ptr, p_val, p_rebuild_val);
			}
			return get_quaternion_open_curve_at_i(*l_crv_ptr, p_val, p_rebuild_val);
		}

		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>>		get_copy() const override
		{
			auto l_ptr = create();
			l_ptr->m_tangent_v = m_tangent_v;
			l_ptr->m_quaternion_a_v = m_quaternion_a_v;
			l_ptr->m_quaternion_b_v = m_quaternion_b_v;
			l_ptr->m_quat_param_v = m_quat_param_v;
			return l_ptr;
		}
		virtual std::shared_ptr < vufCurveQuaternion2EndsFn<T, V> > as_2ends_fn() const override
		{
			return std::static_pointer_cast<vufCurveQuaternion2EndsFn<T, V>> (vufCurveQuaternionFn<T, V>::m_this.lock());
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ vufCurveQuaternionCloseFn < " << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl;
			l_ss << vufCurveQuaternionFn<T, V>::to_string(-1, p_tab_count + 1);

			l_ss << l_str_offset << "____twist multipier: " << m_twist_multiplier << std::endl;

			l_ss << l_str_offset << "____twist curve: " << (m_twist_fcurve != nullptr) << std::endl;

			l_ss << l_str_offset << "____y_axis: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_y_axis_v);
			l_ss << std::endl;

			l_ss << l_str_offset << "m_tangent_v: ";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_tangent_v);
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
			l_size += sizeof(m_twist_multiplier);
			l_size += sizeof(l_size) + m_y_axis_v.size() * sizeof(V<T>);
			l_size += sizeof(l_size) + m_tangent_v.size() * sizeof(V<T>);
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
			// m_twist_multiplier
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_twist_multiplier, sizeof(m_twist_multiplier));
			// m_y_axis_v
			l_size = m_y_axis_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_y_axis_v[0], l_size * sizeof(V<T>));
			}
			// m_tangent_v
			l_size = m_tangent_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_tangent_v[0], l_size * sizeof(V<T>));
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

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0)		override
		{
			uint64_t l_size;
			p_offset = vufCurveQuaternionFn<T, V>::from_binary(p_buff, p_version, p_offset);
			// m_twist_multiplier
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_twist_multiplier, sizeof(m_twist_multiplier));
			// m_y_axis_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_y_axis_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_y_axis_v[0], l_size * sizeof(V<T>));
			}
			// m_tangent_v
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_tangent_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_tangent_v[0], l_size * sizeof(V<T>));
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
		uint8_t								m_mode				= 0;

		std::vector <V<T>>					m_y_axis_v;			// y axices of nodes nodes array
		std::vector< V<T>>					m_tangent_v;		// quaternion nodes 3d tangents array
		std::vector< vufQuaternion<T>>		m_quaternion_a_v;	// each node has pair of quaternions ( a := my, b := next)
		std::vector< vufQuaternion<T>>		m_quaternion_b_v;
		std::vector<T>						m_quat_param_v;		// parameter on curve

		T									m_twist_start_mult	= 0;
		T									m_twist_end_mult	= 0;
		std::shared_ptr <vufCurve<T, V> >	m_twist_fcurve		= nullptr;
	};
}
#endif // !VF_MATH_QUAT_2_ENDS_FN_H
