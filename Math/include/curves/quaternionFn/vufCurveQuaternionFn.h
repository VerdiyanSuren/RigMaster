#ifndef VF_MATH_CRV_QTRN_FN_H
#define VF_MATH_CRV_QTRN_FN_H

#include <curves/vufCurve.h>
#include <math/vufQuaternion.h>
#include <math/vufMatrix.h>
#include <cmath>

namespace vufMath
{
	template <class T, template<typename> class V>	class vufCurve;
	template <class T, template<typename> class V>	class vufCurveContainer;
	template <class T, template<typename> class V>	class vufCurveQuaternionCloseFn;
	template <class T, template<typename> class V>	class vufCurveQuaternionTransportFn;
	template <class T, template<typename> class V>	class vufCurveQuaternionBlendFn;

#pragma region VF_CURVE_FN_BASE
	enum class vufCurveQuatFnType :uint8_t
	{
		//open curves
		k_none		= 0,
		k_closest	= 1,
		k_transport = 2,
		k_blend		= 3
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
			if (p_type == vufCurveQuatFnType::k_transport)
			{
				return vufCurveQuaternionTransportFn<T, V>::create();
			}
			if (p_type == vufCurveQuatFnType::k_blend)
			{
				return vufCurveQuaternionBlendFn<T, V>::create();
			}
			return nullptr;
		}
		void			is_valid() const { return m_valid; }
		// main work horse
		virtual vufQuaternion<T> get_quaternion_at(const vufCurveContainer<T, V>& p_curve_container, T p_val,T p_rebiuld_val) const = 0;

		virtual vufCurveQuatFnType								get_type() const = 0 { return vufCurveQuatFnType::k_none; }
		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>>	get_copy() const = 0;
		// cast interface
		virtual std::shared_ptr < vufCurveQuaternionCloseFn<T, V> >		as_closest_fn()		const { return nullptr; }
		virtual std::shared_ptr < vufCurveQuaternionTransportFn<T, V> > as_tranport_fn()	const { return nullptr; }
		virtual std::shared_ptr < vufCurveQuaternionBlendFn<T, V> >		as_blend_fn()		const { return nullptr; }

		// serialization interface
		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const = 0
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			l_ss << l_str_offset << "valid..........." << m_valid << std::endl;

			return l_ss.str();
		}
		virtual uint64_t		get_binary_size()														const = 0
		{
			uint64_t l_size = sizeof(uint32_t);
			l_size += sizeof(m_valid);

			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const = 0
		{
			uint32_t l_version	= VF_MATH_VERSION;
			uint64_t l_size		= vufCurveQuaternionFn<T, V>::get_binary_size();

			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			std::memcpy(&p_buff[p_offset], &l_version,			sizeof(l_version));			p_offset += sizeof(l_version);
			std::memcpy(&p_buff[p_offset], &m_valid,			sizeof(m_valid));			p_offset += sizeof(m_valid);

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0) = 0
		{
			uint64_t l_size = vufCurveQuaternionFn<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				return 0;
			}
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, p_version,			sizeof(p_version));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_valid,			sizeof(m_valid));
			//VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_pin_start,		sizeof(m_pin_start));
			//VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_pin_start_value,	sizeof(m_pin_start_value));
			//VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_pin_end,			sizeof(m_pin_end));
			//VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_pin_end_value,	sizeof(m_pin_end_value));
			//VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_offset,			sizeof(m_offset));

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
