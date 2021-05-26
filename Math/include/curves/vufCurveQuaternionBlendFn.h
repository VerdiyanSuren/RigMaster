#ifndef VF_MATH_CRV_QTRN_BLEND_FN_H
#define VF_MATH_CRV_QTRN_BLEND_FN_H

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;
	template<class T, template<typename> class V>	class vufCurveQuaternionFn;


	template <class T, template<typename> class V>
	class vufCurveQuaternionBlendFn : public vufCurveQuaternionFn<T, V>
	{
	private:
		vufCurveQuaternionBlendFn() :vufCurveQuaternionFn<T, V>() {}
	public:
		/// creator
		static std::shared_ptr<vufCurveQuaternionBlendFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveQuaternionBlendFn>(new vufCurveQuaternionBlendFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}

		virtual vufCurveQuatFnType get_type() const override { return vufCurveQuatFnType::k_blend; }
		virtual void	log_me(int p_tab_count = 0) const override
		{
			std::string l_str_offset(p_tab_count * 4, '.');
			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_GREEN, VF_CONSOLE_COLOR_BLACK);
			std::cout << std::fixed;
			std::cout << l_str_offset << "[ Quaternion Blend Fn ]: " << std::endl;
			VF_CONSOLE_RESET_COLOR();
		}
		virtual void	set_item_count(uint32_t p_count) override	{}
		virtual void	set_item_at(uint32_t p_index, const V<T> p_pos, const vufMatrix4<T>& p_matr) override {}
		/// Compute or set influencer param on the curve
		virtual bool	compute_bind_params(const vufCurveContainer<T, V>& p_curve_container, uint32_t p_division, T p_percition = 0.00001) override { return true; }
		// Make quaternions non flopped
		virtual bool	match_quaternions(const vufCurveContainer<T, V>& p_curve_container) override { return true; }

		virtual vufQuaternion<T> get_quaternion_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const override
		{
			//Change this when close curve will be implemented
			auto	l_crv_ptr		= p_curve_container.get_curve_ptr();
			T		l_crv_val		= p_curve_container.get_curve_val_by_remaped(p_val);
			auto	l_blend_curve	= l_crv_ptr->as_curve_blend();

			if (l_blend_curve != nullptr)
			{
				T	 l_weight	= l_blend_curve->get_weight();
				auto l_cntnr_1	= l_blend_curve->get_container_ptr_1();
				auto l_cntnr_2	= l_blend_curve->get_container_ptr_2();
				V<T> l_tngnt	= l_blend_curve->get_tangent_at(l_crv_val);
				if (l_cntnr_1 != nullptr)
				{
					vufQuaternion<T>	l_q_1 = l_cntnr_1->get_quaternion_at(l_crv_val);
					V<T>				l_v_1(1.0);
					l_v_1	= l_q_1.rotate_vector_by_quaternion(l_v_1);
					l_q_1	= l_q_1.increment_quaternion_with_2vectors(l_v_1, l_tngnt);
					if (l_cntnr_2 != nullptr)
					{
						vufQuaternion<T>	l_q_2 = l_cntnr_2->get_quaternion_at(l_crv_val);
						V<T>				l_v_2(1.0);
						l_v_2 = l_q_2.rotate_vector_by_quaternion(l_v_2);
						l_q_2 = l_q_2.increment_quaternion_with_2vectors(l_v_2, l_tngnt);
						if (l_q_1.dot(l_q_2) < 0.0)
						{
							l_q_2 = -l_q_2;
						}
						vufQuaternion<T> l_res = l_q_1 * (1. - l_weight) + l_q_2 * l_weight;
						l_res.normalize_in_place();
						return l_res;
					}
					return l_q_1.normalize_in_place();
				}
				if (l_cntnr_2 != nullptr)
				{
					vufQuaternion<T>	l_q_2 = l_cntnr_2->get_quaternion_at(l_crv_val);
					V<T>				l_t_2 = l_cntnr_2->get_tangent_at(l_crv_val);
					V<T>				l_v_2(1.0);
					l_v_2 = l_q_2.rotate_vector_by_quaternion(l_v_2);
					l_q_2 = l_q_2.increment_quaternion_with_2vectors(l_v_2, l_t_2);
					return l_q_2.normalize_in_place();
				}
			}
			return vufQuaternion<T>();
		}

		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>> get_copy() const override
		{
			return create();
		}
		virtual std::shared_ptr < vufCurveQuaternionBlendFn<T, V> > as_blend_fn()	const  override
		{ 
			return std::static_pointer_cast<vufCurveQuaternionBlendFn>(vufCurveQuaternionFn<T, V>::m_this.lock());
		}
	};

}
#endif // !VF_MATH_CRV_QTRN_BLEND_FN_H
