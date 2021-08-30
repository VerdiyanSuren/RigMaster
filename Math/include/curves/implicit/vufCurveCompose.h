#ifndef VF_MATH_CRV_COMPOSE_H
#define VF_MATH_CRV_COMPOSE_H

#include <curves/vufCurve.h>
#include <curves/vufCurvesInclude.h>
//#include <curves/vufCurveContatiner.h>
#include <math/vufVector.h>
#include <memory>

namespace vufMath
{
	template<class T, template<typename> class V>		class vufCurveContainer;
	template <	class T, template<typename> class V>	class vufCurve;

	template <class T, template<typename> class V >
	class vufCurveCompose : public vufCurve<T, V>
	{
	private:
		vufCurveCompose() :vufCurve<T, V>()
		{
			vufCurve<T, V>::m_degree = 0;
			vufCurve<T, V>::m_close = false;
			vufCurve<T, V>::m_explicit = false;
			vufCurve<T, V>::m_has_degree = false;
			vufCurve<T, V>::m_valid = false;
		}
	public:
		enum
		{
			k_plus	= 0,
			k_minus = 1,
			k_mult	= 2
		};
		virtual ~vufCurveCompose() {}
		VF_MATH_CURVE_DEFINE_CREATOR(vufCurveCompose);
		VF_MATH_CURVE_DEFINE_TYPE_CATEGORY(k_compose_curve, k_compound_category);
		virtual std::shared_ptr<vufCurve<T, V>>		get_copy() const override
		{
			std::shared_ptr< vufCurveCompose > l_ptr = vufCurveCompose::create();
			return l_ptr;
		}
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
		virtual std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ Noise Curve: " << typeid(T).name() << ", " << typeid(V).name() << std::endl;
			l_ss << l_str_offset << l_str_offset << "Weight_a: " << m_weight_a << std::endl;
			l_ss << l_str_offset << l_str_offset << "Weight_b " << m_weight_b << std::endl;
			l_ss << std::endl;
			return l_ss.str();
		}
		virtual uint64_t	get_binary_size()														const override
		{
			uint64_t l_size = vufCurve<T, V>::get_binary_size();
			l_size += sizeof(l_size) + sizeof(m_weight_a);
			l_size += sizeof(l_size) + sizeof(m_weight_b);

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
			//VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_time, sizeof(m_time));
			

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
			//VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_time, sizeof(m_time));
			

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

		virtual bool			rebuild(std::vector<T>&				p_uniform_to_curve_val_v,
										std::vector<T>&				p_curve_to_uniform_val_v,
										std::vector<T>&				p_curve_val_to_length_v,
										uint32_t					p_divisions = 10,
										T							p_start = 0 /*interval on which we need rebuild*/,
										T							p_end = 1) const override
		{
			return true;
		}

		virtual V<T>			get_closest_point(const V<T>&	p_point,
													T			p_start = 0,
													T			p_end = 1,
													uint32_t	p_divisions = 10,
													T p_percition = vufCurve_kTol) const override
		{
			return V<T>();
		}


		virtual T				get_closest_point_param(const V<T>&		p_point,
														T				p_start = 0,
														T				p_end = 1 /*if p_start == p_end then interval is infinite*/,
														uint32_t		p_divisions = 10,
														T				p_percition = vufCurve_kTol) const override
		{
			return 0;
		}

		virtual T				get_param_by_vector_component(	T			p_value,
																uint32_t	p_component_index = 0/*x by default*/,
																T			p_start = 0,
																T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
																uint32_t	p_divisions = 10,
																T			p_percition = vufCurve_kTol)	const override
		{
			return 0;
		}
		virtual std::shared_ptr<vufCurveCompose <T, V >>	as_curve_compose()	const override
		{
			return std::static_pointer_cast<vufCurveCompose<T, V>>(vufCurveCompose<T, V>::m_this.lock());
		}

		inline V<T>		get_pos_at_i(T p_t)		const
		{
			if (m_operator == k_plus)
			{
				return get_pos_at_plus_i(p_t);
			}
			if (m_operator == k_mult)
			{
				return get_pos_at_mult_i(p_t);
			}
			return V<T>();
		}
		inline V<T>		get_tangent_at_i(T p_t) const
		{
			V<T> l_res = (get_pos_at_i(p_t) - get_pos_at_i(p_t + 0.00001)) * 10000;
			return l_res;
		}

		inline V<T>		get_pos_at_plus_i(T p_t) const
		{
			V<T> l_p_a = m_container_a_ptr == nullptr ? V<T>() : m_container_a_ptr->get_pos_at(p_t);
			V<T> l_p_b = m_container_a_ptr == nullptr ? V<T>() : m_container_b_ptr->get_pos_at(p_t);
			return (l_p_a.get_mult_components( m_weight_a) + l_p_b.get_mult_components(m_weight_b));
		}
		inline V<T>		get_pos_at_mult_i(T p_t) const
		{
			V<T> l_p_a = m_container_a_ptr == nullptr ? V<T>() : m_container_a_ptr->get_pos_at(p_t);
			V<T> l_p_b = m_container_a_ptr == nullptr ? V<T>() : m_container_b_ptr->get_pos_at(p_t);
			l_p_a.get_mult_components_in_place(m_weight_a); 
			l_p_b.get_mult_components_in_place(m_weight_b);
			return l_p_a.get_mult_components(l_p_b);
		}


		void set_container_a_i(std::shared_ptr< vufCurveContainer<T, V> > p_cntnr)
		{
			m_container_a_ptr = p_cntnr;
			vufCurve<T, V>::m_valid = true;
		}
		void set_container_b_i(std::shared_ptr< vufCurveContainer<T, V> > p_cntnr)
		{
			m_container_b_ptr = p_cntnr;
			vufCurve<T, V>::m_valid = false;
		}
		void set_operator_i(uint8_t p_type)
		{
			if (p_type != m_operator)
			{
				m_operator = p_type;
			}
		}
		void set_weight_a_i(V<T> p_val)
		{
			m_weight_a = p_val;
		}
		void set_weight_b_i(V<T> p_val)
		{
			m_weight_b = p_val;
		}


		std::shared_ptr< vufCurveContainer<T, V> > get_container_a_i() const
		{
			return m_container_a_ptr;
		}
		std::shared_ptr< vufCurveContainer<T, V> > get_container_b_i() const
		{
			return m_container_b_ptr;
		}
		uint8_t get_operator_i() const 
		{
			return m_operator;
		}
		T get_weight_a_i() const
		{
			return m_weight_a;
		}
		T get_weight_b_i() const
		{
			return m_weight_b;
		}
	private:
		V<T> m_weight_a		= V<T>(0., 1., .0);
		V<T> m_weight_b		= V<T>(0., 1., .0);
		uint8_t m_operator	= 0;

		std::shared_ptr< vufCurveContainer<T, V> >	m_container_a_ptr = nullptr;
		std::shared_ptr< vufCurveContainer<T, V> >	m_container_b_ptr = nullptr;

	};
}
#endif // !VF_MATH_CRV_COMPOSE_H
