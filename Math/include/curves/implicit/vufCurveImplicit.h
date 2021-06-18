#ifndef VF_MATH_CRV_IMPLICIT_H
#define VF_MATH_CRV_IMPLICIT_H

#include <curves/vufCurve.h>
#include <math/vufVector.h>

namespace vufMath
{
	template <class T, template<typename> class V>
	class vufCurveImplicit : public vufCurve<T, V>
	{
		vufCurveImplicit() : vufCurve<T, V>() { vufCurve<T, V>::m_explicit = false; }
		virtual ~vufCurveImplicit() {}
		bool is_clamped() const { return m_clamped; }

		virtual uint64_t		get_binary_size() const override = 0
		{
			//std::cout << "------------------------------------- EXPLICIT::get_binary_size()--------------------- " << std::endl;
			uint64_t l_array_size = m_nodes_pos_v.size();
			uint64_t l_size = 0;
			l_size += vufCurve < T, V>::get_binary_size();
			l_size += sizeof(m_knot_weighted);
			l_size += sizeof(l_array_size);
			l_size += m_nodes_pos_v.size() * sizeof(V<T>);
			return l_size;
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const override = 0
		{
			//std::cout << "------------------------------------- EXPLICIT::to_binary()--------------------- " << std::endl;

			// resize if needed
			uint64_t l_size = vufCurveExplicit<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			uint64_t l_array_size = m_nodes_pos_v.size();
			p_offset = vufCurve < T, V>::to_binary(p_buff, p_offset);

			std::memcpy(&p_buff[p_offset], &m_knot_weighted, sizeof(m_knot_weighted));				p_offset += sizeof(m_knot_weighted);
			std::memcpy(&p_buff[p_offset], &l_array_size, sizeof(l_array_size));					p_offset += sizeof(l_array_size);
			std::memcpy(&p_buff[p_offset], m_nodes_pos_v.data(), m_nodes_pos_v.size() * sizeof(V<T>));	p_offset += m_nodes_pos_v.size() * sizeof(V<T>);

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0) override = 0
		{
			//std::cout << "------------------------------------- EXPLICIT::from_binary()--------------------- " << std::endl;
			p_offset = vufCurve < T, V>::from_binary(p_buff, p_version, p_offset);
			if (p_offset == 0)
			{
				return 0;
			}
			uint64_t l_array_size;
			if (p_buff.size() < p_offset + sizeof(m_knot_weighted))
			{
				return 0;
			}
			std::memcpy(&m_knot_weighted, &p_buff[p_offset], sizeof(m_knot_weighted));				p_offset += sizeof(m_knot_weighted);

			if (p_buff.size() < p_offset + sizeof(l_array_size))
			{
				return 0;
			}

			std::memcpy(&l_array_size, &p_buff[p_offset], sizeof(l_array_size));					p_offset += sizeof(l_array_size);
			m_nodes_pos_v.resize(l_array_size);
			if (l_array_size == 0)
			{
				return p_offset;
			}
			std::memcpy(&m_nodes_pos_v[0], &p_buff[p_offset], l_array_size * sizeof(V<T>));			p_offset += l_array_size * sizeof(V<T>);

			return p_offset;
		}

		virtual		std::shared_ptr< vufCurveExplicit<T, V> >	as_implicit_curve()	 const override
		{
			return std::static_pointer_cast<vufCurveImplicit<T, V>>(vufCurve<T, V>::m_this.lock());
		}

	protected:
		bool m_is_clamped = false;
	};
}

#endif // !VF_MATH_CRV_IMPLICIT_H
