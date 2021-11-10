#ifndef VF_MATH_CRV_SCALE_COMPOSE_FN_H
#define VF_MATH_CRV_SCALE_COMPOSE_FN_H


namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurve;
	template<class T, template<typename> class V>	class vufCurveContainer;

	template <class T, template<typename> class V>
	class vufCurveScaleComposeFn : public vufCurveScaleFn<T, V>
	{
	private:
		vufCurveScaleComposeFn() :vufCurveScaleFn<T, V>() {}
	public:
		static std::shared_ptr<vufCurveScaleComposeFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveScaleComposeFn>(new vufCurveScaleComposeFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual V<T>				get_scale_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const override
		{
			auto	l_crv_ptr = p_curve_container.get_curve_ptr();
			if (l_crv_ptr != nullptr && l_crv_ptr->is_valid() == true)
			{
				return l_crv_ptr->get_scale_at(p_val);
			}			
			return V<T>(1,1,1);
		}
		virtual vufCurveScaleFnType	get_type() const override { return vufCurveScaleFnType::k_compose; }
		virtual std::shared_ptr< vufCurveScaleFn<T, V>> get_copy() const override
		{
			auto l_ptr = create();
			return l_ptr;
		}
		virtual std::shared_ptr < vufCurveScaleComposeFn<T, V> > as_compose_fn() const  override
		{
			return std::static_pointer_cast<vufCurveScaleComposeFn<T, V>> (vufCurveScaleComposeFn<T, V>::m_this.lock());
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
			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0)		override
		{
			return p_offset;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			return p_offset;
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			return p_offset;
		}

	};
}
#endif /!VF_MATH_CRV_SCALE_COMPOSE_FN_H