#ifndef VF_MATH_CRV_REMAP_FN_H
#define VF_MATH_CRV_REMAP_FN_H

namespace vufMath
{

	enum class vufCurveRemapFnType :uint8_t
	{
		k_none = 0
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
		virtual vufCurveRemapFnType	get_type() const = 0 { return vufCurveRemapFnType::k_none; }
		virtual void			log_me(int p_tab_count = 0) const = 0;


		virtual std::shared_ptr< vufCurveRemapFn<T>> get_copy() const = 0;
		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const = 0;
		virtual uint64_t		get_binary_size()														const = 0;
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const = 0;
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)		= 0;
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const = 0;
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		= 0;


		std::weak_ptr <vufCurveRemapFn<T> > m_this = std::weak_ptr <vufCurveRemapFn<T> >();
	};

}
#endif // !VF_MATH_CRV_REMAP_FN_H


 
