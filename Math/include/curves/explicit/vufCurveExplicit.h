#ifndef VF_MATH_CRV_EXPLCT_H
#define VF_MATH_CRV_EXPLCT_H

#include <curves/vufCurve.h>
#include <math/vufVector.h>
namespace vufMath
{
	template <	class T, template<typename> class V>
	class vufCurveExplicit :public vufCurve<T, V>
	{
	public:
		vufCurveExplicit() :vufCurve<T, V>() 
		{
			vufCurve<T, V>::m_explicit = true;
		}
		virtual ~vufCurveExplicit() {}
		bool	 is_weighted()  const { return m_knot_weighted; }
		/** set and retrieve nodes count for control points of curves.
		called only for explicit curves*/
		virtual void		set_nodes_count(uint32_t p_count)	= 0;
		virtual uint32_t	get_nodes_count()	const			= 0;
		/** set and get control point position*/
		virtual void		set_node_at(uint32_t p_index, const V<T>& p_vector) = 0;
		virtual V<T>		get_node_at(uint32_t p_index) const					= 0;

		virtual T			get_interval_t_min(int p_interval_index) const = 0;
		virtual T			get_interval_t_max(int p_interval_index) const = 0;
		virtual int			get_interval_index(T p_t) const = 0;
		virtual uint32_t	get_interval_count() const = 0;

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

			std::memcpy(&p_buff[p_offset], &m_knot_weighted,		sizeof(m_knot_weighted));				p_offset += sizeof(m_knot_weighted);
			std::memcpy(&p_buff[p_offset], &l_array_size,			sizeof(l_array_size));					p_offset += sizeof(l_array_size);
			std::memcpy(&p_buff[p_offset], m_nodes_pos_v.data(),	m_nodes_pos_v.size() * sizeof(V<T>));	p_offset += m_nodes_pos_v.size() * sizeof(V<T>);

			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0) override= 0
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
			std::memcpy(&m_knot_weighted, &p_buff[p_offset],  sizeof(m_knot_weighted));				p_offset += sizeof(m_knot_weighted);
			
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

		virtual		std::shared_ptr< vufCurveExplicit<T, V> >	as_explicit_curve()	 const override
		{ 
			return std::static_pointer_cast<vufCurveExplicit<T, V>>(vufCurve<T,V>::m_this.lock());
		}
	protected:
		bool				m_knot_weighted = false;
		std::vector<V<T>>	m_nodes_pos_v;
	};
}
#endif // !VF_MATH_CRV_EXPLCT_H
