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

		bool	get_clamp_start() const			{ return m_clamp_start; }
		void	set_clamp_start(bool p_val)		{ m_clamp_start = p_val; }
		T		get_clamp_start_value() const	{ return m_clamp_start_value; }
		void	set_clamp_start_value(T p_val)	{ m_clamp_start_value = p_val; }

		bool	get_clamp_end() const			{ return m_clamp_end; }
		void	set_clamp_end(bool p_val)		{ m_clamp_end = p_val; }
		T		get_clamp_end_value() const		{ return m_clamp_end_value; }
		void	set_clamp_end_value(T p_val)	{ m_clamp_end_value = p_val; }

		T		get_offset() const		{ return m_offset; }
		void	set_offset(T p_val)	{ m_offset = p_val; }

		virtual vufCurveRebuildFnType	get_type() const = 0 { return vufCurveRebuildFnType::k_none; }
		
		virtual void		rebuild( const vufCurve<T, V>& p_curve ) = 0;
		virtual T			get_curve_val_by_rebuilded(T p_val) const = 0;
		virtual T			get_rebuilded_val_by_curve(T p_val) const = 0;
		virtual std::shared_ptr< vufCurveRebuildFn<T, V>> get_copy() const = 0;

		virtual std::shared_ptr < vufCurveRebuildUniformFn<T, V> > as_uniform_rebuild_fn() const { return nullptr; }

		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const = 0
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			l_ss << l_str_offset << "clamp start......." << m_clamp_start		<< std::endl;
			l_ss << l_str_offset << "clamp start value." << m_clamp_start_value << std::endl;
			l_ss << l_str_offset << "m_clamp_end......." << m_clamp_end			<< std::endl;
			l_ss << l_str_offset << "m_clamp_end_value." << m_clamp_end_value	<< std::endl;
			l_ss << l_str_offset << "m_offset.........." << m_offset			<< std::endl;
			return l_ss.str();
		}
		virtual uint64_t		get_binary_size()														const = 0
		{
			uint64_t l_size = 0;
			l_size += sizeof(m_clamp_start);
			l_size += sizeof(m_clamp_start_value);
			l_size += sizeof(m_clamp_end);
			l_size += sizeof(m_clamp_end_value);
			l_size += sizeof(m_offset);
			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const = 0
		{
			uint64_t l_size = get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}			
			std::memcpy(&p_buff[p_offset], &m_clamp_start,			sizeof(m_clamp_start));			p_offset += sizeof(m_clamp_start);
			std::memcpy(&p_buff[p_offset], &m_clamp_start_value,	sizeof(m_clamp_start_value));	p_offset += sizeof(m_clamp_start_value);
			std::memcpy(&p_buff[p_offset], &m_clamp_end,			sizeof(m_clamp_end));			p_offset += sizeof(m_clamp_end);
			std::memcpy(&p_buff[p_offset], &m_clamp_end_value,		sizeof(m_clamp_end_value));		p_offset += sizeof(m_clamp_end_value);
			std::memcpy(&p_buff[p_offset], &m_offset,				sizeof(m_offset));				p_offset += sizeof(m_offset);

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0) = 0
		{
			uint64_t l_size = vufCurveRebuildFn<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				return 0;
			}
			std::memcpy( &m_clamp_start,		&p_buff[p_offset],	sizeof(m_clamp_start));			p_offset += sizeof(m_clamp_start);
			std::memcpy( &m_clamp_start_value,	&p_buff[p_offset],	sizeof(m_clamp_start_value));	p_offset += sizeof(m_clamp_start_value);
			std::memcpy( &m_clamp_end,			&p_buff[p_offset],	sizeof(m_clamp_end));			p_offset += sizeof(m_clamp_end);
			std::memcpy( &m_clamp_end_value,	&p_buff[p_offset],	sizeof(m_clamp_end_value));		p_offset += sizeof(m_clamp_end_value);
			std::memcpy( &m_offset,				&p_buff[p_offset],	sizeof(m_offset));				p_offset += sizeof(m_offset);
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
		bool	m_clamp_start		= false;
		T		m_clamp_start_value	= 0.0;
		bool	m_clamp_end			= false;
		T		m_clamp_end_value	= 1.0;
		T		m_offset			= 0;
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
		virtual void	rebuild(const vufCurve<T, V>& p_curve ) override
		{			
			m_total_div = m_div_per_segment;
			if (p_curve.is_explicit() == true)
			{
				m_total_div = get_explicit_samples_i(p_curve);
			}	
			m_curve_length		= 0;

			if (m_uniform_to_curve_val_v.size() != m_total_div)
			{
				m_uniform_to_curve_val_v.resize(m_total_div);
				m_curve_to_uniform_val_v.resize(m_total_div);
				m_curve_length_to_val_v.resize(m_total_div);
			}
			;
			V<T> l_vec_prev = p_curve.get_pos_at(0);
			m_uniform_to_curve_val_v[0]	= 0;
			m_curve_to_uniform_val_v[0]	= 0;
			m_curve_length_to_val_v[0]	= 0;
			// Compute curve length until sample point
			for (uint64_t i = 1; i < m_total_div; ++i)
			{
				T l_crv_val = ((T)i) / (T(m_total_div - 1));
				V<T> l_vec = p_curve.get_pos_at(l_crv_val);
				m_curve_length_to_val_v[i] = m_curve_length_to_val_v[i-1] + l_vec_prev.distance_to(l_vec);
				l_vec_prev = l_vec;
			}
			m_curve_length = m_curve_length_to_val_v.back();
			T l_u_1 = 0 , l_u_2, l_t_1 = 0, l_t_2;
			uint64_t l_ndx = 0;
			T l_step = 1. / (T(m_total_div - 1));
			T l_dnx = .0, l_tetta = .0;
			for (uint64_t i = 1; i < m_total_div; ++i)
			{
				l_u_2 = m_curve_length_to_val_v[i] / m_curve_length;
				m_curve_to_uniform_val_v[i] = l_u_2;

				l_t_2 = (T(i)) / (T(m_total_div-1));
				
				while (l_tetta <= l_u_2)
				{
					m_uniform_to_curve_val_v[l_ndx] = (l_t_1 * (l_u_2 - l_tetta) + l_t_2 * (l_tetta - l_u_1)) / (l_u_2 - l_u_1);
					l_ndx++;
					l_dnx++;
					l_tetta = l_dnx * l_step;
				}
				l_u_1 = l_u_2;
				l_t_1 = l_t_2;
			}			
		}
		virtual T		get_curve_val_by_rebuilded(T p_val) const override
		{
			// clamp and offset input val
			p_val += vufCurveRebuildFn<T, V>::m_offset;
			if (vufCurveRebuildFn<T, V>::m_clamp_start && p_val < vufCurveRebuildFn<T, V>::m_clamp_start_value)
			{
				p_val = vufCurveRebuildFn<T, V>::m_clamp_start_value;
			}
			if (vufCurveRebuildFn<T, V>::m_clamp_end && p_val < vufCurveRebuildFn<T, V>::m_clamp_end_value)
			{
				p_val = vufCurveRebuildFn<T, V>::m_clamp_end_value;
			}
			// match outbound velosities
			if (p_val < .0)
			{	
				// match velocity
				T l_k = (m_uniform_to_curve_val_v[1] - m_uniform_to_curve_val_v[0]) / 
						(m_curve_to_uniform_val_v[1] - m_curve_to_uniform_val_v[0]);
				return p_val * l_k;
			}
			if (p_val > 1.0)
			{
				// match velocity
				auto l_sz = m_uniform_to_curve_val_v.size();
				T l_k = (m_uniform_to_curve_val_v.back() - m_uniform_to_curve_val_v[l_sz-2]) / 
						(m_curve_to_uniform_val_v.back() - m_curve_to_uniform_val_v[l_sz-2]);
				return 1 + (p_val - 1)* l_k;
			}
			int64_t l_sz = m_uniform_to_curve_val_v.size();
			p_val *= (T)(l_sz - 1);
			uint32_t l_ndx_1 = (int)(p_val);
			uint32_t l_ndx_2 = l_ndx_1 + 1;
			p_val -= std::floor(p_val);
			if (l_ndx_1 < 0)
			{				
				return 0;
			}
			if (l_ndx_2 > l_sz - 1)
			{
				return 1;
			}
			return  m_uniform_to_curve_val_v[l_ndx_1] * (1. - p_val) + m_uniform_to_curve_val_v[l_ndx_2] * p_val;

		}
		virtual T		get_rebuilded_val_by_curve(T p_val) const override
		{
			if (p_val < .0)
			{
				return p_val;
			}
			if (p_val > 1.0)
			{
				return p_val;
			}
			int64_t l_sz = m_curve_to_uniform_val_v.size();
			p_val *= (T)(l_sz - 1);
			uint32_t l_ndx_1 = (int)(p_val);
			uint32_t l_ndx_2 = l_ndx_1 + 1;
			p_val -= std::floor(p_val);
			if (l_ndx_1 < 0)
			{
				return 0;
			}
			if (l_ndx_2 > l_sz - 1)
			{
				return 1;
			}
			return  m_curve_to_uniform_val_v[l_ndx_1] * (1. - p_val) + m_curve_to_uniform_val_v[l_ndx_2] * p_val;

			return 0;
		}
		virtual std::shared_ptr <vufCurveRebuildUniformFn<T, V>> as_uniform_rebuild_fn() const override
		{
			return std::static_pointer_cast<vufCurveRebuildUniformFn<T,V> > (vufCurveRebuildFn<T,V>::m_this.lock());
		}

		virtual std::shared_ptr< vufCurveRebuildFn<T, V>> get_copy() const 
		{
			auto l_ptr = create();
			l_ptr->m_div_per_segment	= m_div_per_segment;
			l_ptr->m_total_div			= m_total_div;
			l_ptr->m_curve_length		= m_curve_length;

			l_ptr->m_uniform_to_curve_val_v = m_uniform_to_curve_val_v;
			l_ptr->m_curve_to_uniform_val_v = m_curve_to_uniform_val_v;
			l_ptr->m_curve_length_to_val_v	= m_curve_length_to_val_v;
			return l_ptr;
		}
		
		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ Rebuild Fn < " << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl; 
			l_ss << vufCurveRebuildFn<T, V>::to_string(-1, p_tab_count + 1);
			l_ss << l_str_offset << "____division per segment: "	<< m_div_per_segment << std::endl;
			l_ss << l_str_offset << "____m_total_division: "		<< m_total_div << std::endl;
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
			l_size += sizeof(m_total_div);
			l_size += sizeof(m_curve_length);

			l_size += sizeof(uint64_t) + m_uniform_to_curve_val_v.size() * sizeof(T);
			l_size += sizeof(uint64_t) + m_curve_to_uniform_val_v.size() * sizeof(T);
			l_size += sizeof(uint64_t) + m_curve_length_to_val_v.size()  * sizeof(T);
			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			p_offset  = vufCurveRebuildFn<T, V>::to_binary(p_buff, p_offset);
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_div_per_segment,	sizeof(m_div_per_segment));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_total_div,		sizeof(m_total_div));
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

			l_size = m_curve_length_to_val_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_size, sizeof(l_size));
			if (l_size > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_curve_length_to_val_v[0], l_size * sizeof(T));
			}

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)		override
		{
			p_offset = vufCurveRebuildFn<T, V>::from_binary(p_buff, p_offset);
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_div_per_segment,	sizeof(m_div_per_segment));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_total_div,		sizeof(m_total_div));
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
			m_curve_length_to_val_v.resize(l_size);
			if (l_size > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_curve_length_to_val_v[0], l_size * sizeof(T));
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

		uint32_t m_div_per_segment	= 5;
		uint64_t m_total_div		= 0;
		T		 m_curve_length		= 0;

		std::vector<T>		m_uniform_to_curve_val_v;
		std::vector<T>		m_curve_to_uniform_val_v;
		std::vector<T>		m_curve_length_to_val_v; // original curve length to value. the last one has length of whole curve
	};
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
