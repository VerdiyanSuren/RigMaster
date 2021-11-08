#ifndef VF_MATH_CRV_RBLD_FN_H
#define VF_MATH_CRV_RBLD_FN_H

#include <curves/vufCurve.h>
#include <vufLog.h>
#include <memory>
#include <vector>
#include <cmath>

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;
	template <class T, template<typename> class V>	class vufCurveRebuildUniformFn;

#pragma region VF_CURVE_FN_BASE
	enum class vufCurveRebuildFnType :uint8_t
	{
		//open curves
		k_none			= 0,
		k_constant_step = 1,
	};

	// functions set to work with specific part 
	template <class T, template<typename> class V>
	class vufCurveRebuildFn
	{
	public:
		vufCurveRebuildFn() {}
		virtual ~vufCurveRebuildFn() {}
		static std::shared_ptr<vufCurveRebuildFn<T, V>> create(vufCurveRebuildFnType p_type)
		{
			if ( p_type == vufCurveRebuildFnType::k_constant_step )
			{
				return vufCurveRebuildUniformFn<T,V>::create();
			}
			return nullptr;
		}
		bool							is_valid()		const { return m_valid; }
		virtual vufCurveRebuildFnType	get_type() const = 0 { return vufCurveRebuildFnType::k_none; }
		
		virtual bool		rebuild(const vufCurve<T, V>& p_curve) = 0;

		virtual T			get_curve_val_by_rebuilded(T p_val) const = 0;
		virtual T			get_rebuilded_val_by_curve(T p_val) const = 0;
		virtual T			get_rebuilded_val_by_length(T p_length) const = 0;
		virtual T			get_length_of_rebuilded() const = 0;
		virtual T			get_length(T p_start = 0.0, T p_end = 1.0) const = 0;
		virtual std::shared_ptr< vufCurveRebuildFn<T, V>> get_copy() const = 0;

		virtual std::shared_ptr < vufCurveRebuildUniformFn<T, V> > as_uniform_rebuild_fn() const { return nullptr; }

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const = 0;
		virtual uint64_t		get_binary_size()														const = 0
		{
			uint64_t l_size = sizeof(uint32_t);//version
			l_size += sizeof(m_valid);
			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const = 0
		{
			uint32_t l_version = VF_MATH_VERSION;
			uint64_t l_size = get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			std::memcpy(&p_buff[p_offset], &l_version,	sizeof(l_version));		p_offset += sizeof(l_version);
			std::memcpy(&p_buff[p_offset], &m_valid, sizeof(m_valid));			p_offset += sizeof(m_valid);
			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0) = 0
		{
			uint64_t l_size = vufCurveRebuildFn<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				return 0;
			}
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, p_version,	sizeof(p_version));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_valid,	sizeof(m_valid));

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
		bool									m_valid = false;
		std::weak_ptr <vufCurveRebuildFn<T,V> > m_this = std::weak_ptr <vufCurveRebuildFn<T, V> >();
	};

#pragma endregion

#pragma region VF_CURVE_REBUILD_FN
	template <class T, template<typename> class V>
	class vufCurveRebuildUniformFn :public vufCurveRebuildFn<T,V>
	{
	public:
		vufCurveRebuildUniformFn():vufCurveRebuildFn<T,V>(){}
		static std::shared_ptr<vufCurveRebuildUniformFn<T,V>> create()
		{
			auto l_ptr =  std::shared_ptr< vufCurveRebuildUniformFn<T,V>>(new vufCurveRebuildUniformFn<T, V>);
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		uint32_t get_explicit_samples_i(const vufCurve<T, V>& p_curve) const
		{
			auto l_crv_ptr = p_curve.as_explicit_curve();
			uint32_t l_sp_count = l_crv_ptr->get_interval_count();
			return l_sp_count * m_div_per_segment + 1;
		}

		virtual vufCurveRebuildFnType get_type() const override
		{ 
			return vufCurveRebuildFnType::k_constant_step;
		}
		virtual bool	rebuild(const vufCurve<T, V>& p_curve) override
		{
			if (p_curve.rebuild(m_uniform_to_curve_val_v,
								m_curve_to_uniform_val_v,
								m_curve_val_to_length_v,
								m_div_per_segment) == true)
			{
				m_curve_length	= m_curve_val_to_length_v.empty() == true ? -1 : m_curve_val_to_length_v.back();
				m_samples		= m_uniform_to_curve_val_v.empty() == true ? 0 : m_uniform_to_curve_val_v.size() - 1;
				m_valid = true;
				m_close = p_curve.is_close();
				return true;
			}
			m_uniform_to_curve_val_v.clear();
			m_curve_to_uniform_val_v.clear();
			m_curve_val_to_length_v.clear();
			m_curve_length = -1;
			m_samples = 0;
			m_valid = false;
			return false;
		}
		virtual T		get_curve_val_by_rebuilded(T p_val) const override
		{
			if (m_valid == false) return p_val;
			T l_offset = 0.;
			if (m_close == true)
			{
				l_offset = std::floor(p_val);
				p_val	-= l_offset;
			}
			/*
			* Let Rebuild domain is [s_0,s_k] and c is uniform to curve array then
			* k = size of c
			* if t in [s_0,s_k]
			* T = (t - s_0)/(s_k - s_0)	// shift and rescale to [0,1]
			* i = floor(T*k)
			* w_0 = i + 1 - T*k
			* w_1 = T*k - i or w_1 = 1.0 - w_0
			* R = c[i] * w_0 + c[i + 1] * w_1
			*
			* if  t < s_0 then
			* from eq x/(t-s_0) = k*(c[1] - c[0])/(s_k - s_0) => x = (t - s_0) * k * (c[1] - c[0])/(s_k - s_0)
			* R = s_0 + x
			*
			* if t > s_k
			* x = (t - s_k)*k * (c[k] - c[k-1])/(s_k - s_0)
			*/
			int	l_k = (int)(m_uniform_to_curve_val_v.size() - 1);
			if (p_val < 0.0)
			{
				T l_x = p_val *((T)l_k) * (m_uniform_to_curve_val_v[1]);
				return l_offset + l_x;
			}
			if (p_val > 1.0)
			{
				T l_x = p_val - 1;
				l_x *= ((T)l_k) * (1 - m_uniform_to_curve_val_v[l_k - 1]);
				return l_offset + 1.0 + l_x;
			}
			T l_T = p_val * ((T)l_k);
			int l_ndx_0 = (int)std::floor(l_T);
			int l_ndx_1 = l_ndx_0 + 1;
			if (l_ndx_0 < 0)
			{
				return 0.0;
			}
			if (l_ndx_1 > l_k)
			{
				return 1.0;
			}
			T	l_w0 = ((T)l_ndx_0) + 1.0 - l_T;// *l_k;
			return  l_offset + m_uniform_to_curve_val_v[l_ndx_0] * (l_w0)+m_uniform_to_curve_val_v[l_ndx_1] * (1.0 - l_w0);
		}
		virtual T		get_rebuilded_val_by_curve(T p_val) const override
		{
			if (m_valid == false) return p_val;
			T l_offset = 0.;
			if (m_close == true)
			{
				l_offset = std::floor(p_val);
				p_val -= l_offset;
			}
			int	l_k = (int)(m_curve_to_uniform_val_v.size() - 1);
			if (p_val < 0.0)
			{
				T l_x = p_val * ((T)l_k) * (m_curve_to_uniform_val_v[1]);
				return l_offset + l_x;
			}
			if (p_val > 1.0)
			{
				T l_x = p_val - 1;
				l_x *= ((T)l_k) * (1 - m_curve_to_uniform_val_v[l_k - 1]);
				return l_offset + 1.0 + l_x;
			}
			T l_T = p_val * ((T)l_k);
			int l_ndx_0 = (int)std::floor(l_T);
			int l_ndx_1 = l_ndx_0 + 1;
			if (l_ndx_0 < 0)
			{
				return 0.0;
			}
			if (l_ndx_1 > l_k)
			{
				return 1.0;
			}
			T	l_w0 = ((T)l_ndx_0) + 1.0 - l_T;// *l_k;
			return  l_offset + m_curve_to_uniform_val_v[l_ndx_0] * (l_w0)+m_curve_to_uniform_val_v[l_ndx_1] * (1.0 - l_w0);
		}
		virtual T		get_rebuilded_val_by_length(T p_length) const override
		{
			if (m_valid == false)	return 0;
			return p_length / m_curve_length;
		}

		virtual T		get_length_of_rebuilded() const override
		{
			return m_curve_length;
		}
		virtual T		get_length(T p_start = 0.0, T p_end = 1.0) const override
		{
			if (m_valid == true)
			{
				return std::abs((p_end - p_start) * m_curve_length);
			}
			return -1;
		}
		virtual std::shared_ptr <vufCurveRebuildUniformFn<T, V>> as_uniform_rebuild_fn() const override
		{
			return std::static_pointer_cast<vufCurveRebuildUniformFn<T,V> > (vufCurveRebuildFn<T,V>::m_this.lock());
		}

		virtual std::shared_ptr< vufCurveRebuildFn<T, V>> get_copy() const 
		{
			auto l_ptr = create();
			l_ptr->m_div_per_segment	= m_div_per_segment;
			l_ptr->m_samples			= m_samples;
			l_ptr->m_close				= m_close;
			l_ptr->m_curve_length		= m_curve_length;
			l_ptr->m_valid				= m_valid;

			l_ptr->m_uniform_to_curve_val_v = m_uniform_to_curve_val_v;
			l_ptr->m_curve_to_uniform_val_v = m_curve_to_uniform_val_v;
			l_ptr->m_curve_val_to_length_v  = m_curve_val_to_length_v;

			return l_ptr;
		}
		
		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ Rebuild Fn Uniform < " << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl; 
			l_ss << l_str_offset << "____valid: "					<< m_valid << std::endl;
			l_ss << l_str_offset << "____division per segment: "	<< m_div_per_segment << std::endl;
			l_ss << l_str_offset << "____smaple points: "			<< m_samples << std::endl;
			l_ss << l_str_offset << "____clase: "					<< m_close << std::endl;
			l_ss << l_str_offset << "____curve length: "			<< m_curve_length << std::endl;
			l_ss << l_str_offset << "____uniform to curve array: " << std::endl;
			l_ss << l_str_offset << "____"; 
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_uniform_to_curve_val_v);
			l_ss << std::endl;
			l_ss << l_str_offset << "____curve to uniform array: " << std::endl;
			l_ss << l_str_offset << "____";
			VF_NUMERIC_ARRAY_TO_STRING(l_ss, m_curve_to_uniform_val_v);
			l_ss << std::endl;
			return l_ss.str();
		}
		virtual uint64_t		get_binary_size()														const override
		{
			uint64_t l_size = vufCurveRebuildFn<T,V>::get_binary_size();
			l_size += sizeof(m_div_per_segment);
			l_size += sizeof(m_samples);
			l_size += sizeof(m_close);
			l_size += sizeof(m_curve_length);

			l_size += sizeof(uint64_t) + m_uniform_to_curve_val_v.size() * sizeof(T);
			l_size += sizeof(uint64_t) + m_curve_to_uniform_val_v.size() * sizeof(T);
			l_size += sizeof(uint64_t) + m_curve_val_to_length_v.size()  * sizeof(T);
			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			p_offset  = vufCurveRebuildFn<T, V>::to_binary(p_buff, p_offset);
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_div_per_segment,	sizeof(m_div_per_segment));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_samples,			sizeof(m_samples));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_close,			sizeof(m_close));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_curve_length,		sizeof(m_curve_length));
			
			uint64_t l_size = m_uniform_to_curve_val_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_uniform_to_curve_val_v[0], l_size * sizeof(T));
			}

			l_size = m_curve_to_uniform_val_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_curve_to_uniform_val_v[0], l_size * sizeof(T));
			}

			l_size = m_curve_val_to_length_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_curve_val_to_length_v[0], l_size * sizeof(T));
			}

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff,uint32_t& p_version, uint64_t p_offset = 0)		override
		{
			p_offset = vufCurveRebuildFn<T, V>::from_binary(p_buff, p_version, p_offset);
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_div_per_segment,	sizeof(m_div_per_segment));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_samples,			sizeof(m_samples));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_close,			sizeof(m_close));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_curve_length,		sizeof(m_curve_length));
			uint64_t l_size;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_uniform_to_curve_val_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_uniform_to_curve_val_v[0], l_size * sizeof(T));
			}

			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_curve_to_uniform_val_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_curve_to_uniform_val_v[0], l_size * sizeof(T));
			}

			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_size, sizeof(l_size));
			m_curve_val_to_length_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_curve_val_to_length_v[0], l_size * sizeof(T));
			}
			return p_offset;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			return vufCurveRebuildFn<T, V>::encode_to_buff(p_buff, p_offset);
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		override
		{
			return vufCurveRebuildFn<T, V>::decode_from_buff(p_buff, p_offset);
		}

		uint32_t	get_total_samples() const { return m_total_div + 1; }
		
		uint32_t	get_segment_divisions_i() const { return m_div_per_segment; }
		void		set_segment_divisions_i(uint32_t p_val) { m_div_per_segment = p_val; }
	private:
		uint32_t	m_div_per_segment	= 5;	// curve segment devide into m_div_per_segment count
		uint64_t	m_samples			= 0;	// number of smaples points on curve [m_start, m_end]
		T			m_curve_length		= -1;	// length of this segment [m_start,m_end]
		bool		m_close				= false;
		std::vector<T>		m_uniform_to_curve_val_v;
		std::vector<T>		m_curve_to_uniform_val_v;
		std::vector<T>		m_curve_val_to_length_v; // original curve value to length. 
		static uint64_t g_gl_id;
	};
	template <>
	uint64_t vufCurveRebuildUniformFn<double, vufVector4>::g_gl_id = 0;
#pragma endregion

#pragma region USING_NAMES
	using vufCurveRebuildFn_2f = vufCurveRebuildFn<float,  vufVector2>;
	using vufCurveRebuildFn_2d = vufCurveRebuildFn<double, vufVector2>;
	using vufCurveRebuildFn_3f = vufCurveRebuildFn<float,  vufVector3>;
	using vufCurveRebuildFn_3d = vufCurveRebuildFn<double, vufVector3>;
	using vufCurveRebuildFn_4f = vufCurveRebuildFn<float,  vufVector4>;
	using vufCurveRebuildFn_4d = vufCurveRebuildFn<double, vufVector4>;

#pragma endregion
}

#endif // !VF_MATH_CRV_FN_H
