#ifndef VF_MATH_CRV_QTRN_SLIDE_FN_H
#define VF_MATH_CRV_QTRN_SLIDE_FN_H

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;
	template<class T, template<typename> class V>	class vufCurveQuaternionFn;
	
	template <class T, template<typename> class V>
	class vufCurveQuaternionSlideFn : public vufCurveQuaternionFn<T, V>
	{
	private:
		vufCurveQuaternionSlideFn() :vufCurveQuaternionFn<T, V>() {}
	public:
		/// creator
		static std::shared_ptr<vufCurveQuaternionSlideFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveQuaternionSlideFn>(new vufCurveQuaternionSlideFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual vufCurveQuatFnType get_type() const override { return vufCurveQuatFnType::k_slide; }
		virtual vufQuaternion<T> get_quaternion_at(const vufCurveContainer<T, V>& p_curve_container, T p_val, T p_rebiuld_val) const override
		{
			//Change this when close curve will be implemented
			auto	l_crv_ptr = p_curve_container.get_curve_ptr();
			auto	l_slide_curve = l_crv_ptr == nullptr? nullptr: l_crv_ptr->as_curve_slide();

			if (l_slide_curve != nullptr)
			{
				T p_base_val	= l_slide_curve->get_param_on_base_i(p_val);
				//std::cout << "val: " << p_val <<  " base val: " << p_base_val << std::endl;
				auto l_cntnr	= l_slide_curve->get_container_ptr();
				if (l_cntnr != nullptr)
				{
					auto l_quat		= l_cntnr->get_quaternion_at(p_base_val);
					V<T> l_tngnt	= l_cntnr->get_tangent_at(p_base_val);
					V<T> l_v_1(1.0);
					l_v_1 = l_quat.rotate_vector_by_quaternion(l_v_1);
					l_quat = l_quat.increment_quaternion_with_2vectors(l_v_1, l_tngnt);
					
					return l_quat.normalize_in_place();
				}				
			}
			return vufQuaternion<T>();
		}
		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>> get_copy()						const override
		{
			return create();
		}
		virtual std::shared_ptr < vufCurveQuaternionSlideFn<T, V> > as_slide_fn()			const  override
		{
			return std::static_pointer_cast<vufCurveQuaternionSlideFn>(vufCurveQuaternionFn<T, V>::m_this.lock());
		}
		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)	const override
		{
			return std::string();
		}
		virtual uint64_t		get_binary_size()														const override
		{
			return 0;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			return 0;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)		override
		{
			//To Do
			return 0;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			return 0;
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		override
		{
			return 0;
		}

	};
}


#endif	//!VF_MATH_CRV_QTRN_SLIDE_FN_H