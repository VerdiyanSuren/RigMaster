#ifndef VF_MATH_CRV_QTRN_COMPOSE_FN_H
#define VF_MATH_CRV_QTRN_COMPOSE_FN_H

// compose is default qouternion solver for curves,
// which have input containers and has own impolementation

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;
	template<class T, template<typename> class V>	class vufCurveQuaternionFn;


	template <class T, template<typename> class V>
	class vufCurveQuaternionComposeFn : public vufCurveQuaternionFn<T, V>
	{
	private:
		vufCurveQuaternionComposeFn() :vufCurveQuaternionFn<T, V>() {}
	public:
		/// creator
		static std::shared_ptr<vufCurveQuaternionComposeFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveQuaternionComposeFn>(new vufCurveQuaternionComposeFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}

		virtual vufCurveQuatFnType get_type() const override { return vufCurveQuatFnType::k_compose; }
		virtual vufQuaternion<T> get_quaternion_at(const vufCurveContainer<T, V>& p_curve_container, T p_val, T p_rebiuld_val) const override
		{
			//Change this when close curve will be implemented
			auto l_crv_ptr = p_curve_container.get_curve_ptr();
			if (l_crv_ptr != nullptr && l_crv_ptr->is_valid() == true)
			{
				return l_crv_ptr->get_quaternion_at(p_val);
			}
			return vufQuaternion<T>();
		}

		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>> get_copy() const override
		{
			return create();
		}
		virtual std::shared_ptr < vufCurveQuaternionComposeFn<T, V> > as_compose_fn()	const  override
		{ 
			return std::static_pointer_cast<vufCurveQuaternionComposeFn>(vufCurveQuaternionFn<T, V>::m_this.lock());
		}

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
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
#endif // !VF_MATH_CRV_QTRN_BLEND_FN_H
