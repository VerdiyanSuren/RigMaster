#ifndef VF_MATH_CRV_SLIDE_H
#define VF_MATH_CRV_SLIDE_H

#include <curves/vufCurve.h>
#include <curves/vufCurvesInclude.h>


namespace vufMath
{
	template<class T, template<typename> class V>		class vufCurveContainer;
	template <	class T, template<typename> class V>	class vufCurve;

	template <class T, template<typename> class V >
	class vufCurveSlide : public vufCurve<T, V>
	{
	private:
		vufCurveSlide() :vufCurve<T, V>()
		{
			vufCurve<T, V>::m_degree = 0;
			vufCurve<T, V>::m_close = false;
			vufCurve<T, V>::m_explicit = false;
			vufCurve<T, V>::m_has_degree = false;
			vufCurve<T, V>::m_valid = true;
		}
	public:
		virtual ~vufCurveSlide() {}
		VF_MATH_CURVE_DEFINE_CREATOR(vufCurveSlide);
		VF_MATH_CURVE_DEFINE_TYPE_CATEGORY(k_slide_curve, k_compound_category);
		VF_MATH_CRV_REBUILD;
		VF_MATH_CRV_DEFINE_CLOSEST_POINTS;
		VF_MATH_DEFINE_PARAM_COMPONENT;
		virtual V<T>		get_pos_at(T p_t)										const override
		{
			return get_pos_at_i(p_t);
		}
		virtual V<T>		get_tangent_at(T p_t)									const override
		{
			return get_tangent_at_i(p_t);
		}
		virtual V<T>		get_tangent_normalized_at(T p_t)						const override
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}
		virtual std::shared_ptr<vufCurve<T, V>>		get_copy() const override
		{
			std::shared_ptr< vufCurveSlide > l_ptr = vufCurveSlide::create();
			l_ptr->copy_members_from_i(std::static_pointer_cast<vufCurveSlide>(m_this.lock()));
			return l_ptr;
		}
		virtual std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ Slide Curve: <" << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Hardness:     " << m_hardness		<< std::endl;
			l_ss << l_str_offset << l_str_offset << "____Offset:       " << m_offset		<< std::endl;
			l_ss << l_str_offset << l_str_offset << "____Start Param:  " << m_start_value	<< std::endl;
			l_ss << l_str_offset << l_str_offset << "____Start Weight: " << m_start_weight	<< std::endl;
			l_ss << l_str_offset << l_str_offset << "____End Param:    " << m_end_value		<< std::endl;
			l_ss << l_str_offset << l_str_offset << "____End Weight:   " << m_end_weight	<< std::endl;
			l_ss << l_str_offset << l_str_offset << "____Length:       " << m_length		<< std::endl;
			l_ss << l_str_offset << l_str_offset << "____Use Length:   " << m_use_length	<< std::endl;
			l_ss << l_str_offset << "____Input Curve: " << (m_container_ptr == nullptr ? "Absent" : m_container_ptr->to_string(p_precision, p_tab_count + 1)) << std::endl;
			l_ss << std::endl;
			return l_ss.str();
		}
		virtual uint64_t	get_binary_size()														const override
		{
			uint64_t l_size = vufCurve<T, V>::get_binary_size();
			l_size += sizeof(l_size) + sizeof(m_hardness);
			l_size += sizeof(l_size) + sizeof(m_offset);
			l_size += sizeof(l_size) + sizeof(m_start_value);
			l_size += sizeof(l_size) + sizeof(m_start_res);
			l_size += sizeof(l_size) + sizeof(m_start_weight);
			l_size += sizeof(l_size) + sizeof(m_end_value);
			l_size += sizeof(l_size) + sizeof(m_end_res);
			l_size += sizeof(l_size) + sizeof(m_end_weight);
			l_size += sizeof(l_size) + sizeof(m_length);
			l_size += sizeof(l_size) + sizeof(m_use_length);
			l_size += sizeof(bool);
			if (m_should_serialize_container == true && m_container_ptr != nullptr)
			{
				l_size += m_container_ptr->get_binary_size();
			}
			return l_size;
		}
		virtual uint64_t	to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override
		{
			uint64_t l_curve_size = get_binary_size();
			//------------------------------------------------------
			// resize if needed
			if (p_buff.size() < p_offset + l_curve_size)
			{
				p_buff.resize(p_offset + l_curve_size);
			}
			//------------------------------------------------------
			p_offset = vufCurve<T, V>::to_binary(p_buff, p_offset);
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_hardness,		sizeof(m_hardness));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_offset,		sizeof(m_offset));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_start_value,	sizeof(m_start_value));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_start_res,	sizeof(m_start_res));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_start_weight, sizeof(m_start_weight));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_end_value,	sizeof(m_end_value));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_end_res,		sizeof(m_end_res));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_end_weight,	sizeof(m_end_weight));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_length,		sizeof(m_length));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_use_length,	sizeof(m_use_length));

			if (m_container_ptr != nullptr && m_should_serialize_container == true)
			{
				bool l_save = true;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
				p_offset = m_container_ptr->to_binary(p_buff, p_offset);
			}
			else
			{
				bool l_save = false;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
			}

			return p_offset;
		}
		virtual uint64_t	from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0)
		{
			vufCurve<T, V>::m_valid = false;
			p_offset = vufCurve<T, V>::from_binary(p_buff, p_version, p_offset);
			if (p_offset == 0)
			{
				vufCurve<T, V>::m_valid = false;
				return 0;
			};
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_hardness,		sizeof(m_hardness));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_offset,		sizeof(m_offset));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_start_value,	sizeof(m_start_value));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_start_res,	sizeof(m_start_res));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_start_weight, sizeof(m_start_weight));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_end_value,	sizeof(m_end_value));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_end_res,		sizeof(m_end_res));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_end_weight,	sizeof(m_end_weight));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_length,		sizeof(m_length));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_use_length,	sizeof(m_use_length));

			bool l_load;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_load, sizeof(l_load));
			if (l_load == true)
			{
				m_should_serialize_container = true;
				m_container_ptr = vufCurveContainer<T, V>::create();
				p_offset = m_container_ptr->from_binary(p_buff, p_version, p_offset);
			}
			else
			{
				m_should_serialize_container = false;
				m_container_ptr = nullptr;
			}

			return p_offset;
		}
		virtual uint64_t	encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const override
		{
			return vufCurve<T, V>::encode_to_buff(p_buff, p_offset);
		}
		virtual uint64_t	decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		override
		{
			return vufCurve<T, V>::decode_from_buff(p_buff, p_offset);
		}

		virtual std::shared_ptr<vufCurveSlide <T, V >>		as_curve_slide()	const override
		{
			return std::static_pointer_cast<vufCurveSlide<T, V>>(vufCurveSlide<T, V>::m_this.lock());
		}

		inline void		update()
		{
			std::cout << "is percent: " << m_percent << std::endl;
			if (m_percent == false)
			{
				m_start_value	= m_container_ptr->get_param_by_length(m_start_value);
				m_end_value		= m_container_ptr->get_param_by_length(m_end_value);
				m_offset		= m_container_ptr->get_param_by_length(m_offset);
			}
			if (m_use_length == false)
			{
				m_start_res = m_start_value;
				m_end_res	= m_end_value;
				return;
			}
			T l_delta_hardness = m_hardness - std::floor(m_hardness);
			for (int i = 0; i < m_hardness; ++i)
			{
				T l_length = m_container_ptr->get_length(m_start_value, m_end_value);
				T l_delta = (m_start_value - m_end_value) * (1. - m_length / l_length );
				l_delta *= 0.5;
				m_start_value	-= l_delta * m_start_weight;
				m_end_value		+= l_delta * m_end_weight;
			}
			m_start_res = m_offset + m_start_value;
			m_end_res	= m_offset + m_end_value;
		}
		inline T		get_param_on_base_i(T p_val) const
		{
			return m_start_res* (1.0 - p_t) + m_end_res * p_t;
		}
		inline V<T>		get_pos_at_i(T p_t)		const
		{
			if ( m_container_ptr != nullptr )
			{
				T p_param = m_start_res * (1.0 - p_t) + m_end_res * p_t;
				return m_container_ptr->get_pos_at(p_param);
			}
			return V<T>();
		}
		inline V<T>		get_tangent_at_i(T p_t)	const
		{
			if ( m_container_ptr != nullptr )
			{
				T p_param = m_start_res * (1.0 - p_t) + m_end_res * p_t;
				return m_container_ptr->get_tangent_at(p_param);
			}
			return V<T>();
		}
		inline V<T>		get_tangent_normalized_at_i(T p_t) const
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}
		
		void			make_selializable_input(bool p_val) { m_should_serialize_container = p_val; }
		bool			is_serializable_input() const { return m_should_serialize_container; }

		inline bool		get_percent_mode()	const { return p_percent; }
		inline T		get_hardness()		const { return m_hardness; }
		inline T		get_offset()		const { return m_offset; }
		inline T		get_start_param()	const { return m_start_value; }
		inline T		get_start_weight()  const { return m_start_weight; }
		inline T		get_end_param()		const { return m_end_value; }
		inline T		get_end_weight()	const { return m_end_weight; }
		inline T		get_length()		const { return m_length; }
		inline bool		get_use_lenght()	const { return m_use_length; }
		std::shared_ptr < vufCurveContainer<T, V>>		get_container_ptr() const
		{
			return m_container_ptr;
		}

		inline void set_percent_mode(bool p_percent) { m_percent = p_percent; }
		inline void set_hardness(T p_val)			{ m_hardness = p_val; }
		inline void set_offset(T p_val)				{ m_offset = p_val; }
		inline void set_start_param(T p_val)		{ m_start_value = p_val; }
		inline void set_start_weight(T p_val)		{ m_start_weight = p_val; }
		inline void set_end_param(T p_val)			{ m_end_value = p_val; }
		inline void set_end_weight(T p_val)			{ m_end_weight = p_val; }
		inline void set_length(T p_val)				{ m_length = p_val; }
		inline void set_use_lenght(bool p_val)		{ m_use_length = p_val; }
		void		set_container_ptr(std::shared_ptr < vufCurveContainer<T, V>> p_ptr)
		{
			m_container_ptr = p_ptr;
		}
	private:
		bool m_percent		= true;	// start, end values as percent or length
		T m_hardness		= 10;	// how elasticc is our curve
		T m_start_value		= 0.0;	
		T m_start_res		= 0.0;	//start value after solving
		T m_start_weight	= 1.0;
		T m_end_value		= 1.0;
		T m_end_res			= 1.0;	// end value after solving
		T m_end_weight		= 1.0;
		T m_length			= 1;	// desired lenght of slide curve
		bool m_use_length	= true; // set curve as trim or classic slide curve
		T m_offset			=	.0; // offset slide curve on base curve after solving
		bool										m_should_serialize_container = false;
		std::shared_ptr< vufCurveContainer<T, V> >	m_container_ptr = nullptr;

	};
}
#endif //!VF_MATH_CRV_SLIDE_H

