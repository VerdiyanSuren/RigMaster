#ifndef VF_MATH_CRV_CONTAINER_H
#define VF_MATH_CRV_CONTAINER_H

#include "math/curves/vufCurve.h"
#include "math/curves/vufRebuildCurveFn.h"
#include "math/curves/vufQuaternionCurveFn.h"
#include "math/vufQuaternion.h"
#include "math/vufConsoleInclude.h"
#include <memory>
#include <iostream>
#include <sstream>

namespace vufMath
{
	template <class T, template<typename> class V>	class vufCurve;
	//template <class T, template<typename> class V>	class vufCurveFn;

	template <class T>	class vufRemapCurveFn;		/* Fn Set to Remap Curve parameter */
	template <class T, template<typename> class V>	class vufRebuildCurveFn;	/* Fn Set to rebuild curve */
	template <class T, template<typename> class V>	class vufQuaternionCurveFn; /* Fn Set to Interpolaete Quaternions along curve */
	template <class T, template<typename> class V>	class vufScaleCurveFn;		/* Fn Set to Remap Curve parameter */

#pragma region VF_CURVE_CONTAINER
	template<class T, template<typename> class V>
	class vufCurveContainer
	{
	public:
		vufCurveContainer()
		{
			set_all_to_nullptr();
		}
		vufCurveContainer(const vufCurveContainer& p_other)
			:m_curve_ptr(p_other.m_m_curve_ptr)
			,m_rebuild_fn(p_other.m_rebuild_fn)
			,m_remap_fn(p_other.m_remap_fn)
			,m_quatrernion_fn(p_other.m_quatrernion_fn)
		{}
		void set_all_to_nullptr()
		{
			m_curve_ptr			= nullptr;
			m_rebuild_fn		= nullptr;
			m_remap_fn			= nullptr;
			m_quatrernion_fn	= nullptr;
		}
		// Curves
		void								set_curve_ptr(std::shared_ptr<vufCurve<T, V>> p_curve_ptr)
		{
			m_curve_ptr = p_curve_ptr;
		}
		std::shared_ptr<vufCurve<T, V>>		get_curve_ptr() const 
		{ 
			return m_curve_ptr; 
		}
		void								make_curve_unique()
		{
			if (m_curve_ptr != nullptr)
			{
				m_curve_ptr = m_curve_ptr->make_copy();
			}
		}
		// Rebuild
		void										set_rebuild_fn_ptr(std::shared_ptr<vufRebuildCurveFn<T,V>> p_rebuild_ptr)
		{
			m_rebuild_fn = p_rebuild_ptr;
		}
		std::shared_ptr<vufRebuildCurveFn<T,V>>		get_rebuild_fn_ptr() const
		{
			return m_rebuild_fn;
		}
		void										make_rebuild_fn_unique()
		{
			if (m_rebuild_fn != nullptr)
			{
				m_rebuild_fn = m_rebuild_fn->make_copy();
			}
		}
		// Quaternion
		void										set_quaternion_fn_ptr(std::shared_ptr<vufQuaternionCurveFn<T, V>> p_quat_ptr)
		{
			m_quatrernion_fn = p_quat_ptr;
		}
		std::shared_ptr<vufQuaternionCurveFn<T, V>>	get_quaternion_fn_ptr() const
		{
			return m_quatrernion_fn;
		}
		void										make_quaternion_fn_fn_unique()
		{
			if (m_quatrernion_fn != nullptr)
			{
				m_quatrernion_fn = m_quatrernion_fn->make_copy();
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
			if (m_remap_fn != nullptr)
			{
				l_val = m_remap_fn.get_val_by(l_val);
			}
			if (m_rebuild_fn != nullptr)
			{
				l_val = m_rebuild_fn(l_val);
			}
			return l_val;
		}
		V<T> get_pos_at(T p_val)					const;	/* curve */
		V<T> get_tangent(T p_val)					const;	/* curve  */
		vufQuaternion<T> get_quaternion(T p_val)	const;	/* curve quaternion*/
		V<T> get_scale_at(T p_val)					const;	/* curve scale */

	private:
		// Curve
		std::shared_ptr<vufCurve<T, V>>						m_curve_ptr			= nullptr; 
		// Function Sets
		std::shared_ptr<vufRebuildCurveFn<T,V> >			m_rebuild_fn		= nullptr;
		std::shared_ptr<vufRemapCurveFn<T> >				m_remap_fn			= nullptr;
		std::shared_ptr<vufQuaternionCurveFn<T, V> >		m_quatrernion_fn	= nullptr;
		std::shared_ptr< vufScaleCurveFn<T, V>	 >			m_scale_fn			= nullptr;
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
			out << "[ Curve Container ]: ";			
			return out;
		}

	};
#pragma endregion VF_CURVE_CONTAINER

}

#endif // ! VF_MATH_CRV_CONTAINER_H
