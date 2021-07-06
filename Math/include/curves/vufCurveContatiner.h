#ifndef VF_MATH_CRV_CONTAINER_H
#define VF_MATH_CRV_CONTAINER_H

#include <curves/vufCurve.h>
#include <math/vufQuaternion.h>
#include <curves/explicit/vufCurveOpenBSpline.h>
#include <curves/explicit/vufCurveCloseBSpline.h>
#include <curves/implicit/vufCurveBlend.h>
#include <curves/rebuildFn/vufCurveRebuildFn.h>

#include <curves/quaternionFn/vufCurveQuaternionFn.h>

#include <curves/quaternionFn/vufCurveQuaternionCloseFn.h>
#include <curves/quaternionFn/vufCurveQuaternionTransportFn.h>
#include <curves/quaternionFn/vufCurveQuaternion2EndsFn.h>
#include <curves/quaternionFn/vufCurveQuaternionParamsFn.h>
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
	//template <class T, template<typename> class V, uint32_t CURVE_DEGREE = 2>	class vufCurveOpenBSpline;
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
			if (m_quaternion_fn != nullptr)
			{
				l_ptr->m_quaternion_fn = m_quaternion_fn->get_copy();
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
			if (m_curve_ptr == nullptr || m_curve_ptr->get_degree() != p_degree || m_curve_ptr->get_curve_type() != p_type )
			{
				if (p_type == vufCurveType::k_none)
				{
					m_curve_ptr = nullptr;
					return true;
				}
				// handle open b spline
				if (p_type == vufCurveType::k_open_bspline)
				{
					if (p_degree == 1)
					{
						m_curve_ptr = vufCurveOpenBSpline<T, V, 1>::create();
						return true;
					}
					if (p_degree == 2)
					{
						m_curve_ptr = vufCurveOpenBSpline<T, V,2>::create();
						return true;
					}
					if (p_degree == 3)
					{
						m_curve_ptr = vufCurveOpenBSpline<T, V,3>::create();
						return true;
					}
					if (p_degree == 4)
					{
						m_curve_ptr = vufCurveOpenBSpline<T, V, 4>::create();
						return true;
					}
					if (p_degree == 5)
					{
						m_curve_ptr = vufCurveOpenBSpline<T, V, 5>::create();
						return true;
					}
				}
				if (p_type == vufCurveType::k_close_bspline)
				{
					if (p_degree == 1)
					{
						m_curve_ptr = vufCurveCloseBSpline<T, V, 1>::create();
						return true;
					}
					if (p_degree == 2)
					{
						m_curve_ptr = vufCurveCloseBSpline<T, V, 2>::create();
						return true;
					}
					if (p_degree == 3)
					{
						m_curve_ptr = vufCurveCloseBSpline<T, V, 3>::create();
						return true;
					}
					if (p_degree == 4)
					{
						m_curve_ptr = vufCurveCloseBSpline<T, V, 4>::create();
						return true;
					}
					if (p_degree == 5)
					{
						m_curve_ptr = vufCurveCloseBSpline<T, V, 5>::create();
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
		bool								switch_rebuild_fn(vufCurveRebuildFnType p_type)
		{
			if (m_rebuild_fn == nullptr || m_rebuild_fn->get_type() != p_type)
			{
				//To Do 
				// Create from base rebuild class as quaternion fn
				if (p_type == vufCurveRebuildFnType::k_none)
				{
					m_rebuild_fn = nullptr;
					return true;
				}
				if (p_type == vufCurveRebuildFnType::k_constant_step)
				{
					m_rebuild_fn = vufCurveRebuildUniformFn<T,V>::create();
					return true;
				}
			}
			return false;
		}
		/// Return true if new remap has fn been created and assigned to container 
		bool								switch_remap_fn(vufCurveRemapFnType p_type)
		{
			if (m_remap_fn == nullptr || m_remap_fn->get_type() != p_type)
			{
				//To Do 
				// Create from base rebuild class as quaternion fn
				/*
				if (p_type == vufCurveRemapFnType::k_none)
				{
					m_remap_fn = nullptr;
					return true;
				}
				*/
			}
			m_remap_fn = nullptr;
			return false;
		}

		///  Return true if new quaternion fn has been created and assigned to container 
		bool								switch_quaternion_fn(vufCurveQuatFnType p_type)
		{
			if ( m_quaternion_fn == nullptr || m_quaternion_fn->get_type() != p_type )
			{				
				if (p_type == vufCurveQuatFnType::k_none)
				{
					m_quaternion_fn = nullptr;
					return true;
				}
				m_quaternion_fn = vufCurveQuaternionFn<T, V>::create(p_type);
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
				if (p_type == vufCurveScaleFnType::k_none)
				{
					m_scale_fn = nullptr;
					return true;
				}
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
			m_quaternion_fn = p_quat_ptr;
		}
		/// Get assigned to container quaternion fn
		std::shared_ptr<vufCurveQuaternionFn<T, V>>	get_quaternion_fn_ptr() const
		{
			return m_quaternion_fn;
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
		std::shared_ptr<vufRemapCurveFn<T>>		get_remap_fn_ptr()
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

		// rebuild interface
		void rebuild()
		{
			if (m_rebuild_fn != nullptr && m_curve_ptr != nullptr)
			{
				m_rebuild_fn->rebuild(*m_curve_ptr);
			}
		}
		// quaternion intrerface
		bool	compute_bind_params(uint32_t p_divisions = 10, T p_percition = 0.00001)
		{
			if (m_quaternion_fn != nullptr)
			{
				return m_quaternion_fn->compute_bind_params(*this, p_divisions, p_percition);
			}
			return false;
		}
		// Make quaternions non flipped
		bool	match_quaternions() 
		{
			if (m_quaternion_fn != nullptr)
			{
				return m_quaternion_fn->match_quaternions(*this);
			}
			return false;
		}
		/* -> remap -> rebuild */
		T		get_curve_val_by_remaped(T p_val)		const
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
		V<T>	get_pos_at(T p_val)						const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				T l_new_val = get_curve_val_by_remaped(p_val);
				return m_curve_ptr->get_pos_at(l_new_val);
			}
			return V<T>();
		}
		T		get_param_by_value(T p_value, uint32_t p_componenet_index, uint32_t p_componenet_index_return, T p_start, T p_end, uint32_t p_division, T p_percition = vufCurve_kTol)
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				//T l_new_val = get_curve_val_by_remaped(p_val);
				T l_val =  m_curve_ptr->get_param_by_vector_component(p_value, p_componenet_index, p_start, p_end, p_division,  p_percition);
				return m_curve_ptr->get_pos_at(l_val)[p_componenet_index_return];
			}
			return 0.0;
		}
		V<T>	get_tangent_at(T p_val)					const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				T l_new_val = get_curve_val_by_remaped(p_val);
				return m_curve_ptr->get_tangent_at(l_new_val);
			}
			return V<T>();
		}
		V<T>	get_normal_at(T p_val)					const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				T l_new_val = get_curve_val_by_remaped(p_val);
				return m_curve_ptr->get_normal_at(l_new_val);
			}
			return V<T>();
		}
		V<T>	get_scale_at(T p_val)					const
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
		vufQuaternion<T> get_quaternion_at(T p_val)	const
		{
			if (m_curve_ptr != nullptr && m_curve_ptr->is_valid() == true)
			{
				if (m_quaternion_fn != nullptr)
				{
					T l_new_val = get_curve_val_by_remaped(p_val);
					return m_quaternion_fn->get_quaternion_at(*this, l_new_val,p_val);
				}
			}
			return vufQuaternion<T>();
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

	private:
		// Curve
		std::shared_ptr<vufCurve<T, V>>						m_curve_ptr			= nullptr; 
		// Function Sets
		std::shared_ptr<vufCurveRebuildFn<T,V> >			m_rebuild_fn		= nullptr;
		std::shared_ptr<vufCurveRemapFn<T> >				m_remap_fn			= nullptr;
		std::shared_ptr<vufCurveQuaternionFn<T, V> >		m_quaternion_fn		= nullptr;
		std::shared_ptr< vufCurveScaleFn<T, V>	 >			m_scale_fn			= nullptr;
	public:
		std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0)	const
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ Curve Container: <" << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl;
			// curve info
			l_ss << l_str_offset << "____" << "Curve: >" << std::endl;
			if (m_curve_ptr == nullptr)
			{
				l_ss << l_str_offset << "________" << "Null" << std::endl;
			}
			else
			{
				l_ss << m_curve_ptr->to_string(p_precision, p_tab_count + 2)  << std::endl;
			}
			// rebuild info
			l_ss << l_str_offset << "____" << "Rebuild: >" << std::endl;
			if (m_rebuild_fn == nullptr)
			{
				l_ss << l_str_offset << "________" << "Null" << std::endl;
			}
			else
			{
				l_ss << m_rebuild_fn->to_string(p_precision, p_tab_count + 2) << std::endl;
			}
			//remap info
			l_ss << l_str_offset << "____" << "Remap: >" << std::endl;
			if (m_remap_fn == nullptr)
			{
				l_ss << l_str_offset << "________" << "Null" << std::endl;
			}
			else
			{
				l_ss << m_remap_fn->to_string(p_precision, p_tab_count + 2) << std::endl;
			}
			// quaternion info
			l_ss << l_str_offset << "____" << "Quaternion: >" << std::endl;
			if (m_quaternion_fn == nullptr)
			{
				l_ss << l_str_offset << "________" << "Null" << std::endl;
			}
			else
			{
				l_ss << m_quaternion_fn->to_string(p_precision, p_tab_count + 2) << std::endl;
			}
			// scale info
			l_ss << l_str_offset << "____" << "Scale: >" << std::endl;
			if (m_scale_fn == nullptr)
			{
				l_ss << l_str_offset << "________" << "Null" << std::endl;
			}
			else
			{
				l_ss << m_scale_fn->to_string(p_precision, p_tab_count + 2) << std::endl;
			}
			return l_ss.str();
		}
		uint64_t	get_binary_size()											const
		{
			uint64_t l_size = sizeof(uint32_t);;
			// curve
			l_size += sizeof(uint8_t);	// type_of_curve;
			l_size += sizeof(uint32_t);	// degree
			if (m_curve_ptr != nullptr)
			{
				l_size += m_curve_ptr->get_binary_size();
			}
			// rebuild
			l_size += sizeof(uint8_t);//type_of_rebuild;
			if (m_rebuild_fn != nullptr)
			{
				l_size += m_rebuild_fn->get_binary_size();
			}
			// remap
			l_size += sizeof(uint8_t);//type_of_remap;
			if (m_remap_fn != nullptr)
			{
				l_size += m_remap_fn->get_binary_size();
			}
			// quaternion
			l_size += sizeof(uint8_t);//type_of_quaternion;
			if (m_quaternion_fn != nullptr)
			{
				l_size += m_quaternion_fn->get_binary_size();
			}
			// scale
			l_size += sizeof(uint8_t);//type_of_scale;
			if (m_scale_fn != nullptr)
			{
				l_size += m_scale_fn->get_binary_size();
			}
			return l_size;
		}
		uint64_t	to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)	const 
		{
			uint64_t l_container_size = get_binary_size();
			uint32_t l_version = VF_MATH_VERSION;
			//------------------------------------------------------
			// resize if needed
			if (p_buff.size() < p_offset + l_container_size)
			{
				p_buff.resize(p_offset + l_container_size);
			}
			
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_version, sizeof(l_version));
			//------------------------------------------------------
			// curve
			if (m_curve_ptr == nullptr)
			{
				uint8_t l_none = (uint8_t)vufCurveType::k_none;
				uint32_t l_degree = 0;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_none,		sizeof(l_none));
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_degree,	sizeof(l_degree));
			}
			else
			{
				uint8_t l_type		= (uint8_t)m_curve_ptr->get_curve_type();
				uint32_t l_degree	= m_curve_ptr->get_degree();
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_type,		sizeof(l_type));
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_degree,	sizeof(l_degree));
				p_offset = m_curve_ptr->to_binary(p_buff, p_offset);
			}
			// rebuild
			if (m_rebuild_fn == nullptr)
			{
				uint8_t l_none = (uint8_t)vufCurveRebuildFnType::k_none;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_none, sizeof(l_none));
			}
			else
			{
				uint8_t l_type = (uint8_t)m_rebuild_fn->get_type();
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_type, sizeof(l_type));
				p_offset = m_rebuild_fn->to_binary(p_buff, p_offset);
			}
			// remap
			if (m_remap_fn == nullptr)
			{
				uint8_t l_none = (uint8_t)vufCurveRemapFnType::k_none;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_none, sizeof(l_none));
			}
			else
			{
				uint8_t l_type = (uint8_t)m_remap_fn->get_type();
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_type, sizeof(l_type));
				p_offset = m_remap_fn->to_binary(p_buff, p_offset);
			}
			// quaternion
			if (m_quaternion_fn == nullptr)
			{
				uint8_t l_none = (uint8_t)vufCurveQuatFnType::k_none;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_none, sizeof(l_none));
			}
			else
			{
				uint8_t l_type = (uint8_t)m_quaternion_fn->get_type();
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_type, sizeof(l_type));
				p_offset = m_quaternion_fn->to_binary(p_buff, p_offset);
			}
			// scale
			if (m_scale_fn == nullptr)
			{
				uint8_t l_none = (uint8_t)vufCurveScaleFnType::k_none;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_none, sizeof(l_none));
			}
			else
			{
				uint8_t l_type = (uint8_t)m_scale_fn->get_type();
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_type, sizeof(l_type));
				p_offset = m_scale_fn->to_binary(p_buff, p_offset);
			}
			return p_offset;
		}
		uint64_t	from_binary(const std::vector<char>& p_buff, uint32_t& l_version, uint64_t p_offset = 0)
		{
			uint8_t l_type;
			uint32_t l_degree;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_version, sizeof(l_version));
			// curve
			uint32_t l_curve_version;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_type, sizeof(l_type));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_degree, sizeof(l_degree));
			switch_curve(l_degree, (vufCurveType)l_type);
			if (m_curve_ptr != nullptr)
			{
				p_offset = m_curve_ptr->from_binary(p_buff, l_curve_version, p_offset);
			}
			// rebuild
			uint32_t l_rebuild_version;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_type, sizeof(l_type));
			switch_rebuild_fn(vufCurveRebuildFnType(l_type));
			if (m_rebuild_fn != nullptr)
			{
				p_offset = m_rebuild_fn->from_binary(p_buff, l_rebuild_version, p_offset);
			}
			// remap
			uint32_t l_remap_version;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_type, sizeof(l_type));
			switch_remap_fn(vufCurveRemapFnType(l_type));
			if (m_remap_fn != nullptr)
			{
				p_offset = m_remap_fn->from_binary(p_buff, l_remap_version, p_offset);
			}
			// quaternion
			uint32_t l_quaternion_version;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_type, sizeof(l_type));
			switch_quaternion_fn(vufCurveQuatFnType(l_type));
			if (m_quaternion_fn != nullptr)
			{
				p_offset = m_quaternion_fn->from_binary(p_buff, l_quaternion_version, p_offset);
			}
			// scale
			uint32_t l_scale_version;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_type, sizeof(l_type));
			switch_scale_fn(vufCurveScaleFnType(l_type));
			if (m_scale_fn != nullptr)
			{
				p_offset = m_scale_fn->from_binary(p_buff, l_scale_version, p_offset);
			}
			return p_offset;
		}

		uint64_t	encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)	const 
		{
			uint64_t l_size = get_binary_size();
			std::vector<char> l_buff(l_size);
			to_binary(l_buff);
			vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);
			p_offset = l_serializer.encode_to_buff(p_buff, p_offset);
			return p_offset;
		}
		uint64_t	decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			std::vector<char> l_buff;
			uint32_t l_version;
			vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);
			p_offset = l_serializer.decode_from_buff(p_buff, p_offset);
			from_binary(l_buff, l_version);
			return p_offset;
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