#ifndef VF_MATH_CRV_SCL_FN_H
#define VF_MATH_CRV_SCL_FN_H

#include <curves/vufCurve.h>
#include <math/vufVector.h>

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;
	template <class T, template<typename> class V>	class vufScaleCloseCurveFn;
#pragma region VF_SCALE_FN_BASE

	enum class vufCurveScaleFnType :uint8_t
	{
		//open curves
		k_none				= 0,
		k_closest_params	= 1,
		k_fixed_params		= 2,
		k_ends				= 3
	};

	// functions set to work with specific part 
	template <class T, template<typename> class V>
	class vufCurveScaleFn
	{
	public:
		vufCurveScaleFn() {}
		virtual ~vufCurveScaleFn() {}
		static std::shared_ptr<vufCurveScaleFn<T,V>> create(vufCurveScaleFnType p_type)
		{
			if (p_type == vufCurveScaleFnType::k_closest_params)
			{
				return vufScaleCloseCurveFn<T,V>::create();
			}
			return nullptr;
		}
		bool	get_pin_start() const		{ return m_pin_start; }
		void	set_pin_start(bool p_val)	{ m_pin_start = p_val; }
		T		get_pin_start_value() const { return m_pin_start_value; }
		void	set_pin_start_value(T p_val){ m_pin_start_value = p_val; }

		bool	get_pin_end() const			{ return m_pin_end; }
		void	set_pin_end(bool p_val)		{ m_pin_end = p_val; }
		T		get_pin_end_value() const	{ return m_pin_end_value; }
		void	set_pin_end_value(T p_val)	{ m_pin_end_value = p_val; }

		T		get_offset() const			{ return m_offset; }
		void	set_offset(T p_val)			{ m_offset = p_val; }

		virtual vufCurveScaleFnType get_type() const = 0 { return vufCurveScaleFnType::k_none; }
		virtual void	log_me(int p_tab_count = 0) const = 0;
		/// Set count of scale influencers
		virtual void	set_item_count(uint32_t p_count) = 0;
		/// Set influence scale value and position in the world. 
		virtual void	set_item_at(uint32_t p_index, const V<T> p_pos, const V<T>& p_scale) = 0;
		/// Compute or set influencer param on the curve
		virtual void	compute_bind_param(  const vufCurveContainer<T,V>& p_curve_container, uint32_t p_divisions = 10, T p_percition = 0.00001) = 0;
		/// match scale b by scale a
		virtual bool	match_scales(const vufCurveContainer<T, V>& p_curve_container) = 0;

		/// Get scale interpolated by curve   
		virtual V<T>	get_scale_at( const vufCurveContainer<T,V>& p_curve_container, T p_val) const = 0;
		
		/// Return copy of this curve
		virtual std::shared_ptr< vufCurveScaleFn<T, V>> get_copy() const = 0;		
		/// return casted pointer if inherited class if possible exept nullptr
		virtual std::shared_ptr < vufScaleCloseCurveFn<T, V> > as_scale_close_fn() const { return nullptr; }

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const = 0;
		virtual uint64_t		get_binary_size()														const = 0
		{
			uint64_t l_size = sizeof(uint32_t);
			l_size += sizeof(m_pin_start);
			l_size += sizeof(m_pin_start_value);
			l_size += sizeof(m_pin_end);
			l_size += sizeof(m_pin_end_value);
			l_size += sizeof(m_offset);
			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const = 0
		{
			uint64_t l_size = get_binary_size();
			uint32_t l_version = VF_MATH_VERSION;
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			std::memcpy(&p_buff[p_offset], &l_version,			sizeof(l_version));				p_offset += sizeof(l_version);
			std::memcpy(&p_buff[p_offset], &m_pin_start,		sizeof(m_pin_start));			p_offset += sizeof(m_pin_start);
			std::memcpy(&p_buff[p_offset], &m_pin_start_value,	sizeof(m_pin_start_value));		p_offset += sizeof(m_pin_start_value);
			std::memcpy(&p_buff[p_offset], &m_pin_end,			sizeof(m_pin_end));				p_offset += sizeof(m_pin_end);
			std::memcpy(&p_buff[p_offset], &m_pin_end_value,	sizeof(m_pin_end_value));		p_offset += sizeof(m_pin_end_value);
			std::memcpy(&p_buff[p_offset], &m_offset,			sizeof(m_offset));				p_offset += sizeof(m_offset);

			return p_offset;

		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff,uint32_t& p_version, uint64_t p_offset = 0) = 0
		{
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, p_version,			sizeof(p_version));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_pin_start,		sizeof(m_pin_start));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_pin_start_value,	sizeof(m_pin_start_value));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_pin_end,			sizeof(m_pin_end));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_pin_end_value,	sizeof(m_pin_end_value));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_offset,			sizeof(m_offset));
			
			return p_offset;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const = 0
		{
			VF_ENCODE_FOR_BASE();
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0) = 0
		{
			VF_DECODE_FOR_BASE();
		}

	protected:
		bool	m_pin_start = false;
		T		m_pin_start_value = 0.0;
		bool	m_pin_end = false;
		T		m_pin_end_value = 1.0;
		T		m_offset = 0;

		std::weak_ptr <vufCurveScaleFn<T, V> > m_this;
	};
#pragma endregion

#pragma region USING_NAMES
	using vufCurveScaleFn_2f = vufCurveScaleFn<float,  vufVector2>;
	using vufCurveScaleFn_2d = vufCurveScaleFn<double, vufVector2>;
	using vufCurveScaleFn_3f = vufCurveScaleFn<float,  vufVector3>;
	using vufCurveScaleFn_3d = vufCurveScaleFn<double, vufVector3>;
	using vufCurveScaleFn_4f = vufCurveScaleFn<float,  vufVector4>;
	using vufCurveScaleFn_4d = vufCurveScaleFn<double, vufVector4>;

#pragma endregion
}

#endif // !VF_MATH_CRV_SCL_FN_H
