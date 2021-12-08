#ifndef VF_MATH_CRV_SCL_FN_H
#define VF_MATH_CRV_SCL_FN_H

#include <curves/vufCurve.h>
#include <math/vufVector.h>

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;
	template <class T, template<typename> class V>	class vufScaleCloseCurveFn;
	template <class T, template<typename> class V>	class vufCurveScaleParamFn;
	template <class T, template<typename> class V>	class vufCurveScaleComposeFn;
	template <class T, template<typename> class V>	class vufCurveScaleSlideFn;
#pragma region VF_SCALE_FN_BASE

	enum class vufCurveScaleFnType :uint8_t
	{
		//open curves
		k_none				= 0,
		k_closest_params	= 1,
		k_params			= 2,
		k_ends				= 3,
		k_compose			= 4,
		k_slide				= 5
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
			if (p_type == vufCurveScaleFnType::k_params)
			{
				return vufCurveScaleParamFn<T, V>::create();
			}
			if (p_type == vufCurveScaleFnType::k_compose)
			{
				return vufCurveScaleComposeFn<T, V>::create();
			}
			if (p_type == vufCurveScaleFnType::k_slide)
			{
				return vufCurveScaleSlideFn<T, V>::create();
			}
			return nullptr;
		}

		// main work horse
		virtual V<T>									get_scale_at( const vufCurveContainer<T,V>& p_curve_container, T p_val) const = 0;
		
		virtual vufCurveScaleFnType						get_type() const = 0 { return vufCurveScaleFnType::k_none; }
		virtual std::shared_ptr< vufCurveScaleFn<T, V>> get_copy() const = 0;
		// cast interface
		virtual std::shared_ptr < vufScaleCloseCurveFn<T, V> >		as_close_fn() const { return nullptr; }
		virtual std::shared_ptr < vufCurveScaleParamFn<T, V> >		as_params_fn() const { return nullptr; }
		virtual std::shared_ptr < vufCurveScaleComposeFn<T, V> >	as_compose_fn() const { return nullptr; }
		virtual std::shared_ptr < vufCurveScaleSlideFn<T, V> >		as_slide_fn() const { return nullptr; }

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
			uint64_t l_size = vufCurveScaleFn<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			std::memcpy(&p_buff[p_offset], &m_version, sizeof(m_version));		p_offset += sizeof(m_version);
			std::memcpy(&p_buff[p_offset], &m_valid, sizeof(m_valid));			p_offset += sizeof(m_valid);

			return p_offset;

		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0) = 0
		{
			uint64_t l_size = vufCurveScaleFn<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				return 0;
			}
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_version, sizeof(m_version));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_valid, sizeof(m_valid));
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
		bool m_valid		= false;
		uint32_t m_version	= VF_MATH_VERSION;
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
