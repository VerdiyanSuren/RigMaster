#ifndef VF_MATH_CRV_CONTAINER_H
#define VF_MATH_CRV_CONTAINER_H

#include <curves/vufCurve.h>
#include <math/vufQuaternion.h>
#include <curves/vufOpenBSpline.h>
#include <curves/vufCloseBSpline.h>
#include <curves/vufCurveBlend.h>
#include <curves/rebuildFn/vufCurveRebuildFn.h>

#include <curves/quaternionFn/vufCurveQuaternionFn.h>
#include <curves/quaternionFn/vufCurveQuaternionCloseFn.h>
#include <curves/quaternionFn/vufCurveQuaternionBlendFn.h>

#include <curves/scaleFn/vufCurveScaleFn.h>
#include <curves/scaleFn/vufCurveScaleCloseFn.h>

#include <curves/remapFn/vufCurveRemapFn.h>
#include <math/vufQuaternion.h>
#include <vufLog.h>
#include <memory>
#include <iostream>
#include <sstream>

namespace vufMath
{
	template <class T, template<typename> class V>	class vufCurve;
	template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>	class vufOpenBSpline;
	template <class T, template<typename> class V >	class vufCurveBlend;
	//template <class T, template<typename> class V>	class vufCurveFn;

	template <class T>	class vufRemapCurveFn;		/* Fn Set to Remap Curve parameter */
	template <class T, template<typename> class V>	class vufCurveRebuildFn;	/* Fn Set to rebuild curve */
	template <class T, template<typename> class V>	class vufCurveQuaternionFn; /* Fn Set to Interpolaete Quaternions along curve */
	template <class T, template<typename> class V>	class vufCurveScaleFn;		/* Fn Set to Scale Curve parameter */
	template <class T>								class vufCurveRemapFn;		/* Fn Set to Remap parameter */

#pragma region VF_CURVE_CONTAINER
	template<class T, template<typename> class V>
	class vufCurveContainer
	{
	public:
		vufCurveContainer()	{}
		vufCurveContainer(const vufCurveContainer& p_other)
			:m_curve_ptr(p_other.m_m_curve_ptr)
			,m_rebuild_fn(p_other.m_rebuild_fn)
			,m_remap_fn(p_other.m_remap_fn)
			,m_quatrernion_fn(p_other.m_quatrernion_fn)
		{}
		static std::shared_ptr < vufCurveContainer<T, V> > create()
		{
			return std::shared_ptr< vufCurveContainer<T, V> >(new vufCurveContainer<T, V>());
		}
		std::shared_ptr<vufCurveContainer> get_copy() const
		{
			auto l_ptr = vufCurveContainer<T, V>::create();
			if (m_curve_ptr != nullptr)
			{
				l_ptr->m_curve_ptr = m_curve_ptr->get_copy();
			}
			if (m_rebuild_fn != nullptr)
			{
				l_ptr->m_rebuild_fn = m_rebuild_fn->get_copy();
			}
			if (m_remap_fn != nullptr)
			{
				l_ptr->m_remap_fn = m_remap_fn->get_copy();
			}
			if (m_quatrernion_fn != nullptr)
			{
				l_ptr->m_quatrernion_fn = m_quatrernion_fn->get_copy();
			}
			if (m_scale_fn != nullptr)
			{
				l_ptr->m_scale_fn = m_scale_fn->get_copy();
			}

			return l_ptr;
		}
		void set_all_to_nullptr()
		{
			m_curve_ptr			= nullptr;
			m_rebuild_fn		= nullptr;
			m_remap_fn			= nullptr;
			m_quatrernion_fn	= nullptr;
		}
		
		/// Return true if new curve has been allocated and assigned to container
		bool								switch_curve( uint32_t p_degree, vufCurveType p_type)
		{
			if (m_curve_ptr == nullptr || m_curve_ptr->get_degree() != p_degree || m_curve_ptr->get_type() != p_type )
			{
				// handle open b spline
				if (p_type == vufCurveType::k_open_bspline)
				{
					if (p_degree == 1)
					{
						m_curve_ptr = vufOpenBSpline<T, V, 1>::create();
						return true;
					}
					if (p_degree == 2)
					{
						m_curve_ptr = vufOpenBSpline<T, V,2>::create();
						return true;
					}
					if (p_degree == 3)
					{
						m_curve_ptr = vufOpenBSpline<T, V,3>::create();
						return true;
					}
					if (p_degree == 4)
					{
						m_curve_ptr = vufOpenBSpline<T, V, 4>::create();
						return true;
					}
					if (p_degree == 5)
					{
						m_curve_ptr = vufOpenBSpline<T, V, 5>::create();
						return true;
					}
				}
				if (p_type == vufCurveType::k_close_bspline)
				{
					if (p_degree == 1)
					{
						m_curve_ptr = vufCloseBSpline<T, V, 1>::create();
						return true;
					}
					if (p_degree == 2)
					{
						m_curve_ptr = vufCloseBSpline<T, V, 2>::create();
						return true;
					}
					if (p_degree == 3)
					{
						m_curve_ptr = vufCloseBSpline<T, V, 3>::create();
						return true;
					}
					if (p_degree == 4)
					{
						m_curve_ptr = vufCloseBSpline<T, V, 4>::create();
						return true;
					}
					if (p_degree == 5)
					{
						m_curve_ptr = vufCloseBSpline<T, V, 5>::create();
						return true;
					}
				}
				if (p_type == vufCurveType::k_blend_curve)
				{
					m_curve_ptr = vufCurveBlend<T, V>::create();
					return true;
				}
				m_curve_ptr = nullptr;
				return true;
			}
			return false;
		}
		/// Return true if new rebuild has fn been created and assigned to container 
		bool								switch_rebuild_fn(vufCurveRebuildType p_type)
		{
			if (m_rebuild_fn == nullptr || m_rebuild_fn->get_type() != p_type)
			{
				//To Do 
				// Create from base rebuild class as quaternion fn
				if (p_type == vufCurveRebuildType::k_constant_step)
				{
					m_rebuild_fn = vufCurveRebuildUniformFn<T,V>::create();
					return true;
				}
			}
			return false;
		}
		///  Return true if new quaternion fn has been created and assigned to container 
		bool								switch_quaternion_fn(vufCurveQuatFnType p_type)
		{
			if ( m_quatrernion_fn == nullptr || m_quatrernion_fn->get_type() != p_type )
			{				
				m_quatrernion_fn = vufCurveQuaternionFn<T, V>::create(p_type);
				return true;
			}
			return false;
		}
		///  Return true if new scale fn has been created		
		bool								switch_scale_fn(vufCurveScaleFnType p_type)
		{
			//To Do 
			// Create from base scale class as quaternion fn

			if (m_scale_fn == nullptr || m_scale_fn->get_type() != p_type)
			{	
				m_scale_fn = vufCurveScaleFn<T,V>::create(p_type);
				return true;
			}
			return false;
		}
		


		/// Assign given curve to container
		void								set_curve_ptr(std::shared_ptr<vufCurve<T, V>> p_curve_ptr)
		{
			m_curve_ptr = p_curve_ptr;
		}
		/// Get assigned to container curve  
		std::shared_ptr<vufCurve<T, V>>		get_curve_ptr() const 
		{ 
			return m_curve_ptr; 
		}
		/// Make copy  of assigned curve and assign to container
		void								make_curve_unique()
		{
			if (m_curve_ptr != nullptr)
			{
				m_curve_ptr = m_curve_ptr->get_copy();
			}
		}
		
		
		/// Assign rebuilded fn to container 
		void										set_rebuild_fn_ptr(std::shared_ptr<vufCurveRebuildFn<T,V>> p_rebuild_ptr)
		{
			m_rebuild_fn = p_rebuild_ptr;
		}
		/// Get assigned to container rebuild fn
		std::shared_ptr<vufCurveRebuildFn<T,V>>		get_rebuild_fn_ptr() const
		{
			return m_rebuild_fn;
		}
		// Make copy  of assigned rebuild fn and assign to container
		void										make_rebuild_fn_unique()
		{
			if (m_rebuild_fn != nullptr)
			{
				m_rebuild_fn = m_rebuild_fn->make_copy();
			}
		}
		
		
		/// Assign quaternion fn to container 
		void										set_quaternion_fn_ptr(std::shared_ptr<vufCurveQuaternionFn<T, V>> p_quat_ptr)
		{
			m_quatrernion_fn = p_quat_ptr;
		}
		/// Get assigned to container quaternion fn
		std::shared_ptr<vufCurveQuaternionFn<T, V>>	get_quaternion_fn_ptr() const
		{
			return m_quatrernion_fn;
		}
		// Make copy  of assigned quaternion fn and assign to container
		void										make_quaternion_fn_fn_unique()
		{
			if (m_quatrernion_fn != nullptr)
			{
				m_quatrernion_fn = m_quatrernion_fn->make_copy();
			}
		}
		

		/// Assign scale fn to container 
		void										set_scale_fn_ptr(std::shared_ptr<vufCurveScaleFn<T, V>> p_scl_fn_ptr)
		{
			m_scale_fn = p_scl_fn_ptr;
		}
		/// Get assigned to container scale fn
		std::shared_ptr<vufCurveScaleFn<T, V>>		get_scale_fn_ptr() const
		{
			return m_scale_fn;
		}
		/// Make copy  of assigned scale fn and assign to container
		void										make_scale_fn_fn_unique()
		{
			if (m_scale_fn != nullptr)
			{
				m_scale_fn = m_scale_fn->get_copy();
			}
		}


		// Remap
		void									set_remap_fn_ptr(std::shared_ptr<vufRemapCurveFn<T>> p_remap_ptr)
		{
			m_remap_fn = p_remap_ptr;
		}
		std::shared_ptr<vufRemapCurveFn<T>>		get_remap_fn_ptr() const
		{
			return m_remap_fn;
		}
		void									make_remap_fn_unique()
		{
			if (m_remap_fn != nullptr)
			{
				m_remap_fn = m_remap_fn->make_copy();
			}
		}


		/* -> remap -> rebuild */
		T	 get_curve_val_by_remaped(T p_val)		const
		{
			T l_val = p_val;
			/*
			if (m_remap_fn != nullptr)
			{
				l_val = m_remap_fn.get_val_by(l_val);
			}
			*/
			if (m_rebuild_fn != nullptr)
			{
				l_val = m_rebuild_fn->get_curve_val_by_rebuilded(l_val);
			}

			return l_val;
		}
		V<T> get_pos_at(T p_val)					const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				T l_new_val = get_curve_val_by_remaped(p_val);
				return m_curve_ptr->get_pos_at(l_new_val);
			}
			return V<T>();
		}
		/// No rebild no remap
		V<T> get_pos_at_by_curve_param(T p_val)		const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				return m_curve_ptr->get_pos_at(p_val);
			}
			return V<T>();
		}
		V<T> get_tangent_at(T p_val)				const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				T l_new_val = get_curve_val_by_remaped(p_val);
				return m_curve_ptr->get_tangent_at(l_new_val);
			}
			return V<T>();
		}
		vufQuaternion<T> get_quaternion_at(T p_val)	const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				if (m_quatrernion_fn != nullptr)
				{
					T l_new_val = get_curve_val_by_remaped(p_val);
					return m_quatrernion_fn->get_quaternion_at(*this, l_new_val);
				}
			}
			return vufQuaternion<T>();
		}
		V<T> get_scale_at(T p_val)					const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				if (m_scale_fn != nullptr)
				{
					T l_new_val = get_curve_val_by_remaped(p_val);
					return m_scale_fn->get_scale_at(*this, l_new_val);
				}
			}
			return V<T>(1., 1., 1.);
		}

	private:
		// Curve
		std::shared_ptr<vufCurve<T, V>>						m_curve_ptr			= nullptr; 
		// Function Sets
		std::shared_ptr<vufCurveRebuildFn<T,V> >			m_rebuild_fn		= nullptr;
		std::shared_ptr<vufCurveRemapFn<T> >				m_remap_fn			= nullptr;
		std::shared_ptr<vufCurveQuaternionFn<T, V> >		m_quatrernion_fn	= nullptr;
		std::shared_ptr< vufCurveScaleFn<T, V>	 >			m_scale_fn			= nullptr;
	public:
		std::string		to_string() const
		{
			return std::string();
		}
		uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			return p_offset;
		}
		uint64_t		to_binary(std::vector<unsigned char>& p_buff)	const
		{
			return 0;
		}
		uint64_t		from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)
		{			
			return p_offset;
		}
		void log_me(int p_tab_count = 0) const
		{
			std::string l_str_offset( p_tab_count*4,'.');
			//VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_AQUAA, VF_CONSOLE_COLOR_BLACK);
			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_LIGHT_PURPLE, VF_CONSOLE_COLOR_BLACK);
         	std::cout << l_str_offset << "[ Curve Container ]:> " <<std::endl;
			VF_CONSOLE_RESET_COLOR();
			// log 
			if (m_curve_ptr == nullptr)
			{
				std::cout << l_str_offset << l_str_offset << "[ Curve: NULL ]" << std::endl;
			}
			else
			{
				m_curve_ptr->log_me(p_tab_count + 1);
			}
			if (m_rebuild_fn == nullptr)
			{
				std::cout << l_str_offset << l_str_offset << "[ Rebuild Fn: NULL ]" << std::endl;
			}
			else
			{
				m_rebuild_fn->log_me(p_tab_count + 1);
			}
			if (m_quatrernion_fn == nullptr)
			{
				std::cout << l_str_offset << l_str_offset << "[ Quaternion Fn: NULL ]" << std::endl;
			}
			else
			{
				m_quatrernion_fn->log_me(p_tab_count + 1);
			}

			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_LIGHT_PURPLE, VF_CONSOLE_COLOR_BLACK);
			std::cout << l_str_offset << "[ Curve Container ]: " << std::endl;
			VF_CONSOLE_RESET_COLOR();
		}
		friend std::ostream& operator<<(std::ostream& out, const vufCurveContainer<T,V>& v)
		{
			out << "[ Curve Container ]: " << std::endl;
			return out;
		}

	};
#pragma endregion VF_CURVE_CONTAINER
#pragma region USING_NAMES
	using vufCurveContainer_2f = vufCurveContainer<float,  vufVector2>;
	using vufCurveContainer_2d = vufCurveContainer<double, vufVector2>;
	using vufCurveContainer_3f = vufCurveContainer<float,  vufVector3>;
	using vufCurveContainer_3d = vufCurveContainer<double, vufVector3>;
	using vufCurveContainer_4f = vufCurveContainer<float,  vufVector4>;
	using vufCurveContainer_4d = vufCurveContainer<double, vufVector4>;
#pragma endregion

}

#endif // ! VF_MATH_CRV_CONTAINER_H
