#ifndef VF_MATH_CRV_QTRN_FN_H
#define VF_MATH_CRV_QTRN_FN_H

#include <curves/vufCurve.h>
#include <math/vufQuaternion.h>
#include <math/vufMatrix.h>
#include <cmath>

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;
	template <class T, template<typename> class V>	class vufCurveQuaternionCloseFn;
	template <class T, template<typename> class V>	class vufCurveQuaternionBlendFn;

#pragma region VF_CURVE_FN_BASE
	enum class vufCurveQuatFnType :uint8_t
	{
		//open curves
		k_none		= 0,
		k_closest	= 1,
		k_blend		= 2
	};

	// functions set to work with specific part 
	template <class T, template<typename> class V>	
	class vufCurveQuaternionFn
	{
	public:
		vufCurveQuaternionFn() {}
		virtual ~vufCurveQuaternionFn() {}
		static std::shared_ptr< vufCurveQuaternionFn<T, V>> create(vufCurveQuatFnType p_type)
		{
			if (p_type == vufCurveQuatFnType::k_closest)
			{
				return vufCurveQuaternionCloseFn<T, V>::create();
			}
			if (p_type == vufCurveQuatFnType::k_blend)
			{
				return vufCurveQuaternionBlendFn<T, V>::create();
			}
			return nullptr;
		}

		bool	get_pin_start() const { return m_pin_start; }
		void	set_pin_start(bool p_val) { m_pin_start = p_val; }
		T		get_pin_start_value() const { return m_pin_start_value; }
		void	set_pin_start_value(T p_val) { m_pin_start_value = p_val; }

		bool	get_pin_end() const { return m_pin_end; }
		void	set_pin_end(bool p_val) { m_pin_end = p_val; }
		T		get_pin_end_value() const { return m_pin_end_value; }
		void	set_pin_end_value(T p_val) { m_pin_end_value = p_val; }

		T		get_offset() const { return m_offset; }
		void	set_offset(T p_val) { m_offset = p_val; }

		void			is_valid() const { return m_valid; }
		virtual vufCurveQuatFnType get_type() const = 0 { return vufCurveQuatFnType::k_none; }
		virtual void	set_item_count(uint32_t p_count) = 0;
		virtual void	set_item_at(uint32_t p_index, const V<T> p_pos, const vufMatrix4<T>& p_qmatr) = 0;
		/// Compute or set influencer param on the curve
		virtual bool	compute_bind_params(const vufCurveContainer<T, V>& p_curve_container, uint32_t p_divisions = 10, T p_percition = 0.00001) = 0;
		// Make quaternions non flipped
		virtual bool	match_quaternions(const vufCurveContainer<T, V>& p_curve_container) = 0;

		virtual vufQuaternion<T> get_quaternion_at(const vufCurveContainer<T, V>& p_curve_container, T p_val) const = 0;

		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>> get_copy() const = 0;

		virtual std::shared_ptr < vufCurveQuaternionCloseFn<T, V> > as_closest_fn() const { return nullptr; }
		virtual std::shared_ptr < vufCurveQuaternionBlendFn<T, V> > as_blend_fn()	const { return nullptr; }

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const = 0
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			l_ss << l_str_offset << "valid..........." << m_valid << std::endl;
			l_ss << l_str_offset << "pin start......." << m_pin_start << std::endl;
			l_ss << l_str_offset << "pin start value." << m_pin_start_value << std::endl;
			l_ss << l_str_offset << "pin end........." << m_pin_end << std::endl;
			l_ss << l_str_offset << "pin end value..." << m_pin_end_value << std::endl;
			l_ss << l_str_offset << "offset.........." << m_offset << std::endl;
			return l_ss.str();
		}
		virtual uint64_t		get_binary_size()														const = 0
		{
			uint64_t l_size = 0;
			l_size += sizeof(m_valid);
			l_size += sizeof(m_pin_start);
			l_size += sizeof(m_pin_start_value);
			l_size += sizeof(m_pin_end);
			l_size += sizeof(m_pin_end_value);
			l_size += sizeof(m_offset);

			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const = 0
		{
			uint64_t l_size = vufCurveQuaternionFn<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			std::memcpy(&p_buff[p_offset], &m_valid,			sizeof(m_valid));			p_offset += sizeof(m_valid);
			std::memcpy(&p_buff[p_offset], &m_pin_start,		sizeof(m_pin_start));		p_offset += sizeof(m_pin_start);
			std::memcpy(&p_buff[p_offset], &m_pin_start_value,	sizeof(m_pin_start_value));	p_offset += sizeof(m_pin_start_value);
			std::memcpy(&p_buff[p_offset], &m_pin_end,			sizeof(m_pin_end));			p_offset += sizeof(m_pin_end);
			std::memcpy(&p_buff[p_offset], &m_pin_end_value,	sizeof(m_pin_end_value));	p_offset += sizeof(m_pin_end_value);
			std::memcpy(&p_buff[p_offset], &m_offset,			sizeof(m_offset));			p_offset += sizeof(m_offset);

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0) = 0
		{
			uint64_t l_size = vufCurveQuaternionFn<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				return 0;
			}
			std::memcpy(&m_valid,			&p_buff[p_offset], sizeof(m_valid));			p_offset += sizeof(m_valid);
			std::memcpy(&m_pin_start,		&p_buff[p_offset], sizeof(m_pin_start));		p_offset += sizeof(m_pin_start);
			std::memcpy(&m_pin_start_value, &p_buff[p_offset], sizeof(m_pin_start_value));	p_offset += sizeof(m_pin_start_value);
			std::memcpy(&m_pin_end,			&p_buff[p_offset], sizeof(m_pin_end));			p_offset += sizeof(m_pin_end);
			std::memcpy(&m_pin_end_value,	&p_buff[p_offset], sizeof(m_pin_end_value));	p_offset += sizeof(m_pin_end_value);
			std::memcpy(&m_offset,			&p_buff[p_offset], sizeof(m_offset));			p_offset += sizeof(m_offset);

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

		bool m_valid = false;
		std::weak_ptr <vufCurveQuaternionFn<T, V> > m_this;
	};
#pragma endregion
#pragma region USING_NAMES
	using vufCurveQuaternionFn_2f = vufCurveQuaternionFn<float,  vufVector2>;
	using vufCurveQuaternionFn_2d = vufCurveQuaternionFn<double, vufVector2>;
	using vufCurveQuaternionFn_3f = vufCurveQuaternionFn<float,  vufVector3>;
	using vufCurveQuaternionFn_3d = vufCurveQuaternionFn<double, vufVector3>;
	using vufCurveQuaternionFn_4f = vufCurveQuaternionFn<float,  vufVector4>;
	using vufCurveQuaternionFn_4d = vufCurveQuaternionFn<double, vufVector4>;
#pragma endregion

}
#endif // !VF_MATH_CRV_QTRN_RBLD_FN_H
