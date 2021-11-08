#ifndef VF_MATH_SCALE_SLIDE_FN_H
#define VF_MATH_SCALE_SLIDE_FN_H

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurve;
	template<class T, template<typename> class V>	class vufCurveContainer;

	template <class T, template<typename> class V>
	class vufCurveScaleSlideFn : public vufCurveScaleFn<T, V>
	{
	private:
		vufCurveScaleSlideFn() :vufCurveScaleFn<T, V>() {}
	public:
		static std::shared_ptr<vufCurveScaleSlideFn<T, V>> create()
		{
			auto l_ptr = std::shared_ptr< vufCurveScaleSlideFn>(new vufCurveScaleSlideFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual V<T>				get_scale_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const override
		{
			auto	l_crv_ptr		= p_curve_container.get_curve_ptr();
			auto	l_slide_curve	= l_crv_ptr == nullptr ? nullptr : l_crv_ptr->as_curve_slide();

			if (l_slide_curve != nullptr)
			{
				T p_base_val = l_slide_curve->get_param_on_base_i(p_val);
				auto l_cntnr = l_slide_curve->get_container_ptr();
				if (l_cntnr != nullptr)
				{
					return 	l_cntnr->get_scale_at(p_base_val);
				}
			}
			return V<T>();
		}
		virtual vufCurveScaleFnType	get_type() const override { return vufCurveScaleFnType::k_slide; }
		virtual std::shared_ptr< vufCurveScaleFn<T, V>> get_copy() const override
		{
			auto l_ptr = create();
			return l_ptr;
		}
		virtual std::shared_ptr < vufCurveScaleSlideFn<T, V> > as_slide_fn() const  override
		{
			return std::static_pointer_cast<vufCurveScaleSlideFn<T, V>> (vufCurveScaleSlideFn<T, V>::m_this.lock());
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

#endif /!VF_MATH_SCALE_SLIDE_FN_H