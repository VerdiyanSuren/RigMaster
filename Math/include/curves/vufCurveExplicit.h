#ifndef VF_MATH_CRV_EXPLCT_H
#define VF_MATH_CRV_EXPLCT_H

#include <curves/vufCurve.h>
#include <vufVector.h>
namespace vufMath
{
	template <	class T, template<typename> class V>
	class vufCurveExplicit :public vufCurve<T, V>
	{
	public:
		vufCurveExplicit() :vufCurve<T, V>(){}
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

		virtual		std::shared_ptr< vufCurveExplicit<T, V> >				as_explicit_curve()	 const override
		{ 
			return std::static_pointer_cast<vufCurveExplicit<T, V>>(vufCurve<T,V>::m_this.lock());
		}

	protected:
		bool				m_knot_weighted = false;
		std::vector<V<T>>	m_nodes_pos_v;

	};
}
#endif // !VF_MATH_CRV_EXPLCT_H
