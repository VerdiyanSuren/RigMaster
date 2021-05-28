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
		k_unknown	= 0,
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
		static std::shared_ptr< vufCurveQuaternionFn<T,V>> create(vufCurveQuatFnType p_type)
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
		
		bool	get_pin_start() const		{ return m_pin_start; }
		void	set_pin_start(bool p_val)	{ m_pin_start = p_val; }
		T		get_pin_start_value() const { return m_pin_start_value; }
		void	set_pin_start_value(T p_val){ m_pin_start_value = p_val; }

		bool	get_pin_end() const			{ return m_pin_end; }
		void	set_pin_end(bool p_val)		{ m_pin_end = p_val; }
		T		get_pin_end_value() const	{ return m_pin_end_value; }
		void	set_pin_end_value(T p_val)	{ m_pin_end_value = p_val; }

		T		get_offset() const	{ return m_offset; }
		void	set_offset(T p_val) { m_offset = p_val; }

		void			is_valid() const { return m_valid; }
		virtual vufCurveQuatFnType get_type() const = 0 { return vufCurveQuatFnType::k_unknown; }
		virtual void	log_me(int p_tab_count = 0) const = 0;
		virtual void	set_item_count(uint32_t p_count) = 0;
		virtual void	set_item_at(uint32_t p_index, const V<T> p_pos, const vufMatrix4<T>& p_qmatr) = 0;
		/// Compute or set influencer param on the curve
		virtual bool	compute_bind_params(const vufCurveContainer<T, V>& p_curve_container, uint32_t p_divisions = 10, T p_percition = 0.00001) = 0;
		// Make quaternions non flipped
		virtual bool	match_quaternions(const vufCurveContainer<T, V>& p_curve_container) = 0;
		
		virtual vufQuaternion<T> get_quaternion_at( const vufCurveContainer<T, V>& p_curve_container, T p_val) const = 0;

		virtual std::shared_ptr< vufCurveQuaternionFn<T, V>> get_copy() const = 0;
								  
		virtual std::shared_ptr < vufCurveQuaternionCloseFn<T, V> > as_closest_fn() const { return nullptr; }
		virtual std::shared_ptr < vufCurveQuaternionBlendFn<T, V> > as_blend_fn()	const { return nullptr; }

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
