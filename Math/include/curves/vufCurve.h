#ifndef VF_MATH_CRVS_INTERFACE_H
#define VF_MATH_CRVS_INTERFACE_H

#include <vector>
#include <string>
#include <sstream>

#include <math/vufVector.h>
#include <math/vufQuaternion.h>
#include <serializer/vufTxtStdVectorSerializerFn.h>
//#include <vufNumericArrayObject.h>


#ifndef vufCurve_kTol
#define vufCurve_kTol 1.0e-10
#endif
namespace vufMath
{
#pragma region VF_CURVE
	
	template <class T, template<typename> class V>				class vufCurveExplicit;
	template <class T, template<typename> class V>				class vufCurveImplicit;

	template <class T, template<typename> class V, uint32_t>	class vufCurveBSplineOpen;
	template <class T, template<typename> class V, uint32_t>	class vufCurveBSplineClose;
	template <class T, template<typename> class V, uint32_t>	class vufCurveBezierOpen;
	template <class T, template<typename> class V, uint32_t>	class vufCurveBezierClose;

	template <class T, template<typename> class V >				class vufCurveBlend;
	template <class T, template<typename> class V >				class vufCurveSlide;
	template <class T, template<typename> class V >				class vufCurveNoise;
	template <class T, template<typename> class V >				class vufCurveCompose;


	/**
	*  This is a base interface class  for all curves
	*  bezier, b-saplines and 
	*/
	enum class vufCurveType :uint8_t
	{
		k_none						= 0,
		//open curves
		k_open_bezier_piecewise		= 1,
		k_close_bezier_piecewise	= 2,

		k_open_bspline				= 3,
		k_close_bspline				= 4,

		k_open_xspline				= 5,
		k_close_xspline				= 6,

		k_blend_curve				= 7,
		k_slide_curve				= 8,

		k_noise_curve				= 9,
		k_compose_curve				= 10
	};
	enum vufCurveCategory
	{
		k_none							= 0,
		k_bspline_category				= VF_BIT(0),
		k_bezier_category				= VF_BIT(1),
		k_xspline_category				= VF_BIT(2),
		k_math_category					= VF_BIT(3),
		k_compound_category				= VF_BIT(4),
		k_piece_line_category			= VF_BIT(5)
	};
	// we supopose that V<T> is vactor 2D or 3D of float or double 
	template <	class T,
				template<typename> class V>
	class vufCurve
	{
	public:
		/** all suppported types of curves*/
		vufCurve()			{}
	public:
		virtual ~vufCurve()	{}
		
		bool		is_valid()		const	{ return m_valid; }
		/** some curves could be  degree 1, 2 or 3 come only degree 2 or 3*/
		bool		has_degree()	const	{ return m_has_degree; }
		uint32_t	get_degree()	const	{ return m_degree; }
		bool		is_explicit()	const	{ return m_explicit; }
		/** suportes open and close */
		bool		is_open()			const	{ return  m_close == false; }
		bool		is_close()			const	{ return  m_close; }
		T			get_domain_min()	const	{ return m_domain_min; }
		T			get_domain_max()	const	{ return m_domain_max; }
		bool		is_in_category(vufCurveCategory p_category)
		{
			return get_category() & p_category;
		}
		virtual vufCurveType	get_curve_type()							const = 0;
		virtual int				get_curve_category()						const = 0;
		/* if curve can easily compute length then compute else return -1  */
		// Actual for math curves
		virtual T				get_length() const
		{
			return -1;
		}
		virtual T				get_length(T p_start,T p_end ) const 
		{ 
			return -1; 
		}
		virtual bool			rebuild(			std::vector<T>& p_uniform_to_curve_val_v,
													std::vector<T>& p_curve_to_uniform_val_v,
													std::vector<T>& p_curve_val_to_length_v,
													uint32_t		p_divisions = 10) const = 0;

		virtual V<T>			get_closest_point(			const V<T>& p_point,
															T			p_start		= 0, 
															T			p_end		= 1,
															uint32_t	p_divisions = 10,
															T p_percition			= vufCurve_kTol) const = 0;


		virtual T				get_closest_point_param(	const V<T>& p_point, 
															T p_start				= 0, 
															T p_end					= 1 /*if p_start == p_end then interval is infinite*/,
															uint32_t p_divisions	= 10,
															T p_percition = vufCurve_kTol) const = 0;

		virtual T				get_param_by_vector_component(	T			p_value, 
																uint32_t	p_component_index	= 0/*x by default*/,
																T			p_start				= 0,
																T			p_end				= 1 /*if p_start == p_end then interval is infinite*/,
																uint32_t	p_divisions			= 10,
																T			p_percition			= vufCurve_kTol)	const = 0;
		virtual V<T>				get_pos_at(T p_t)					const = 0;
		virtual V<T>				get_tangent_at(T p_t)				const = 0;
		virtual V<T>				get_tangent_normalized_at(T p_t)	const
		{
			// To Do 
			// make abstract
			return V<T>();
		}
		virtual vufQuaternion<T>	get_quaternion_at(T p_t)			const
		{
			V<T> l_vec(1.0);
			V<T> l_tng = get_tangent_normalized_at(p_t);
			vufQuaternion<T> l_res = vufQuaternion<T>();
			return l_res.increment_quaternion_with_2vectors(l_vec, l_tng);
		}
		virtual V<T>				get_scale_at(T p_t)					const
		{
			return V<T>(1.0, 1.0, 1.0);
		}
		/// Get copy of this curve.	Original curve is unchenged
		virtual std::shared_ptr<vufCurve>	get_copy() const = 0;
		virtual std::string					to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const	= 0;
		virtual uint64_t					get_binary_size() const = 0
		{
			return  sizeof(uint32_t) +	//version
					sizeof(m_valid) + 
					sizeof(m_has_degree) + 
					sizeof(m_degree) + 
					sizeof(m_explicit) + 
					sizeof(m_close) + 
					sizeof(m_domain_min) +
					sizeof(m_domain_max);
		}
		virtual uint64_t					to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const = 0
		{
			// resize if needed
			uint32_t l_version = VF_MATH_VERSION;
			uint64_t l_size = vufCurve<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			std::memcpy(&p_buff[p_offset], &l_version,		sizeof(l_version));		p_offset += sizeof(l_version);
			std::memcpy(&p_buff[p_offset], &m_valid,		sizeof(m_valid));		p_offset += sizeof(m_valid);
			std::memcpy(&p_buff[p_offset], &m_has_degree,	sizeof(m_has_degree));	p_offset += sizeof(m_has_degree);
			std::memcpy(&p_buff[p_offset], &m_degree,		sizeof(m_degree));		p_offset += sizeof(m_degree);
			std::memcpy(&p_buff[p_offset], &m_explicit,		sizeof(m_explicit));	p_offset += sizeof(m_explicit);
			std::memcpy(&p_buff[p_offset], &m_close,		sizeof(m_close));		p_offset += sizeof(m_close);
			std::memcpy(&p_buff[p_offset], &m_domain_min,	sizeof(m_domain_min));	p_offset += sizeof(m_domain_min);
			std::memcpy(&p_buff[p_offset], &m_domain_max,	sizeof(m_domain_max));	p_offset += sizeof(m_domain_max);
			return p_offset;
		}
		virtual uint64_t					from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0) = 0
		{
			if (p_buff.size() < p_offset + vufCurve<T, V>::get_binary_size())
			{
				return 0;
			}
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, p_version,		sizeof(p_version));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_valid,		sizeof(m_valid));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_has_degree,	sizeof(m_has_degree));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_degree,		sizeof(m_degree));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_explicit,		sizeof(m_explicit));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_close,		sizeof(m_close));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_domain_min,	sizeof(m_domain_min));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_domain_max,	sizeof(m_domain_max));

			return p_offset;
		}
		virtual uint64_t					encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const = 0
		{
			uint64_t l_size = get_binary_size();
			std::vector<char> l_buff(l_size);
			to_binary(l_buff);
			vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);
			p_offset = l_serializer.encode_to_buff(p_buff, p_offset);
			return p_offset;
		}
		virtual uint64_t					decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0) = 0
		{			
			uint32_t l_version;
			std::vector<char> l_buff;
			vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);
			p_offset = l_serializer.decode_from_buff(p_buff, p_offset);
			from_binary(l_buff, l_version);	
			return p_offset;
		}

		// convert to explicit
		virtual	std::shared_ptr< vufCurveExplicit<T,V> >			as_explicit_curve()		const { return nullptr; }
		virtual	std::shared_ptr< vufCurveImplicit<T,V> >			as_implicit_curve()		const { return nullptr; }
		// convert to open bspline
		virtual std::shared_ptr<vufCurveBSplineOpen <T, V, 1>>		as_open_bspline_mono()	const {	return nullptr;	}
		virtual std::shared_ptr<vufCurveBSplineOpen <T, V, 2>>		as_open_bspline_di()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveBSplineOpen <T, V, 3>>		as_open_bspline_tri()	const { return nullptr; }

		// convert to close bspline
		virtual std::shared_ptr<vufCurveBSplineClose <T, V, 1>>		as_close_bspline_mono()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveBSplineClose <T, V, 2>>		as_close_bspline_di()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveBSplineClose <T, V, 3>>		as_close_bspline_tri()	const { return nullptr; }

		// convert to open bezier
		virtual std::shared_ptr<vufCurveBezierOpen <T, V, 1>>		as_open_bezier_mono()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveBezierOpen <T, V, 2>>		as_open_bezier_di()		const { return nullptr; }
		virtual std::shared_ptr<vufCurveBezierOpen <T, V, 3>>		as_open_bezier_tri()	const { return nullptr; }

		// convert to close bezier
		virtual std::shared_ptr<vufCurveBezierClose <T, V, 1>>		as_close_bezier_mono()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveBezierClose <T, V, 2>>		as_close_bezier_di()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveBezierClose <T, V, 3>>		as_close_bezier_tri()	const { return nullptr; }


		virtual std::shared_ptr<vufCurveBlend <T, V >>		as_curve_blend()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveSlide <T, V >>		as_curve_slide()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveNoise <T, V >>		as_curve_noise()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveCompose <T, V >>	as_curve_compose()	const { return nullptr; }

		void copy_members_from_i(std::shared_ptr<vufCurve> p_crv)
		{
			m_valid			= p_crv->m_valid;
			m_has_degree	= p_crv->m_has_degree;
			m_degree		= p_crv->m_degree;
			m_explicit		= p_crv->m_explicit;
			m_close			= p_crv->m_close;
			m_domain_min	= p_crv->m_domain_min;
			m_domain_max	= p_crv->m_domain_max;
		}
	protected:
		bool		m_valid			= false;	// if inherited curve is valid 
		bool		m_has_degree	= false;
		uint32_t	m_degree		= 0;		// degree of curve. 0 if therre is n't degree
		bool		m_explicit		= true;		// has control pointas or not
		bool		m_close			= false;	//
		T			m_domain_min	= 0.0;
		T			m_domain_max	= 1.0;

		std::weak_ptr<vufCurve> m_this = std::weak_ptr<vufCurve>();
	};
#pragma endregion VF_CURVE
#pragma region USING_NAMES
	using vufCurve_2f = vufCurve<float, vufVector2>;
	using vufCurve_2d = vufCurve<double, vufVector2>;
	using vufCurve_3f = vufCurve<float, vufVector3>;
	using vufCurve_3d = vufCurve<double, vufVector3>;
	using vufCurve_4f = vufCurve<float, vufVector4>;
	using vufCurve_4d = vufCurve<double, vufVector4>;
#pragma endregion
}
#endif // !VF_MATH_CRVS_INTERFACE_H
