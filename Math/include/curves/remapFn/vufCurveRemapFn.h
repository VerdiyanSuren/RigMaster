#ifndef VF_MATH_CRV_REMAP_FN_H
#define VF_MATH_CRV_REMAP_FN_H

namespace vufMath
{

	enum class vufCurveRemapFnType :uint8_t
	{
		k_unknown = 3
	};
	template <class T>
	class vufCurveRemapFn
	{
	public:
		vufCurveRemapFn() {}
		virtual ~vufCurveRemapFn() {}
		static std::shared_ptr<vufCurveRemapFn<T>> create(vufCurveRemapFnType p_type)
		{
			return nullptr;
		}
		virtual vufCurveRemapFnType	get_type() const = 0 { return vufCurveRemapFnType::k_unknown; }
		virtual void			log_me(int p_tab_count = 0) const = 0;


		virtual std::shared_ptr< vufCurveRemapFn<T>> get_copy() const = 0;

		std::weak_ptr <vufCurveRemapFn<T> > m_this = std::weak_ptr <vufCurveRemapFn<T> >();
	};

}
#endif // !VF_MATH_CRV_REMAP_FN_H


 
