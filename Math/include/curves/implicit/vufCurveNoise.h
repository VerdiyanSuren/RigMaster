#ifndef VF_MATH_CRV_NOISE_H
#define VF_MATH_CRV_NOISE_H
#include <curves/vufCurve.h>
#include <noise/vufPelinNoise.h>

namespace vufMath
{
	template<class T, template<typename> class V>		class vufCurveContainer;
	template <	class T, template<typename> class V>	class vufCurve;

	template <class T, template<typename> class V >
	class vufCurveNoise : public vufCurve<T, V>
	{
	private:
		vufCurveNoise() :vufCurve<T, V>()
		{
			vufCurve<T, V>::m_degree = 0;
			vufCurve<T, V>::m_close = false;
			vufCurve<T, V>::m_explicit = false;
			vufCurve<T, V>::m_has_degree = false;
		}
	public:
		virtual ~vufCurveNoise() {}
		static  std::shared_ptr< vufCurveNoise > create()
		{
			std::shared_ptr< vufCurveNoise >  l_ptr = std::shared_ptr<vufCurveNoise>(new vufCurveNoise());
			VF_MATH_CURVE_CREATOR_BODY(l_ptr);
			return l_ptr;
		}
		virtual std::shared_ptr<vufCurve<T, V>>		get_copy() const override
		{
			std::shared_ptr< vufCurveNoise > l_ptr = vufCurveNoise::create();

			l_ptr->m_time		= m_time;
			l_ptr->m_speed		= m_speed;
			l_ptr->m_amount_x	= m_amount_x;
			l_ptr->m_amount_y	= m_amount_y;
			l_ptr->m_amount_z	= m_amount_z;

			l_ptr->m_scale_x	= m_scale_x;
			l_ptr->m_scale_y	= m_scale_y;
			l_ptr->m_scale_z	= m_scale_z;

			l_ptr->m_offset_x = m_offset_x;
			l_ptr->m_offset_y = m_offset_y;
			l_ptr->m_offset_z = m_offset_z;

			return l_ptr;
		}

		virtual vufCurveType	get_curve_type()					const override
		{
			return vufCurveType::k_noise_curve;
		}
		virtual int				get_curve_category()				const override
		{
			return vufCurveCategory::k_math_category;
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
			l_ss << l_str_offset << l_str_offset << "Time: " << m_time << std::endl;
			l_ss << l_str_offset << l_str_offset << "Speed " << m_speed << std::endl;
			l_ss << l_str_offset << l_str_offset << "Amount_x " << m_amount_x << std::endl;
			l_ss << l_str_offset << l_str_offset << "Amount_y " << m_amount_y << std::endl;
			l_ss << l_str_offset << l_str_offset << "Amount_z " << m_amount_z << std::endl;
			l_ss << l_str_offset << l_str_offset << "Scale_x  " << m_scale_x << std::endl;
			l_ss << l_str_offset << l_str_offset << "Scale_y  " << m_scale_y << std::endl;
			l_ss << l_str_offset << l_str_offset << "Scale_z  " << m_scale_z << std::endl;
			l_ss << l_str_offset << l_str_offset << "Offset_x " << m_offset_x << std::endl;
			l_ss << l_str_offset << l_str_offset << "Offset_y " << m_offset_y << std::endl;
			l_ss << l_str_offset << l_str_offset << "Offset_z " << m_offset_z << std::endl;
			l_ss << std::endl;
			return l_ss.str();
		}
		virtual uint64_t	get_binary_size()														const override
		{
			uint64_t l_size = vufCurve<T, V>::get_binary_size();
			l_size += sizeof(l_size) + sizeof(m_time);
			l_size += sizeof(l_size) + sizeof(m_speed);
			l_size += sizeof(l_size) + sizeof(m_amount_x);
			l_size += sizeof(l_size) + sizeof(m_amount_y);
			l_size += sizeof(l_size) + sizeof(m_amount_z);
			l_size += sizeof(l_size) + sizeof(m_scale_x);
			l_size += sizeof(l_size) + sizeof(m_scale_y);
			l_size += sizeof(l_size) + sizeof(m_scale_z);
			l_size += sizeof(l_size) + sizeof(m_offset_x);
			l_size += sizeof(l_size) + sizeof(m_offset_y);
			l_size += sizeof(l_size) + sizeof(m_offset_z);

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
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_time, sizeof(m_time));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_speed, sizeof(m_speed));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_amount_x, sizeof(m_amount_x));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_amount_y, sizeof(m_amount_y));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_amount_z, sizeof(m_amount_z));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_scale_x, sizeof(m_scale_x));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_scale_y, sizeof(m_scale_y));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_scale_z, sizeof(m_scale_z));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_offset_x, sizeof(m_offset_x));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_offset_y, sizeof(m_offset_y));
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_offset_z, sizeof(m_offset_z));

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
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_time, sizeof(m_time));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_speed, sizeof(m_speed));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_amount_x, sizeof(m_amount_x));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_amount_y, sizeof(m_amount_y));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_amount_z, sizeof(m_amount_z));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_scale_x, sizeof(m_scale_x));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_scale_y, sizeof(m_scale_y));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_scale_z, sizeof(m_scale_z));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_offset_x, sizeof(m_offset_x));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_offset_y, sizeof(m_offset_y));
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_offset_z, sizeof(m_offset_z));

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

		virtual bool			rebuild(std::vector<T>& p_uniform_to_curve_val_v,
			std::vector<T>& p_curve_to_uniform_val_v,
			std::vector<T>& p_curve_val_to_length_v,
			uint32_t					p_divisions = 10,
			T							p_start = 0 /*interval on which we need rebuild*/,
			T							p_end = 1) const override
		{
			return true;
		}
		virtual bool			rebuild_along_axis(const V<T>&		p_axis/*project curve on this axis*/,
													std::vector<T>& p_uniform_to_curve_val_v,
													std::vector<T>& p_curve_to_uniform_val_v,
													std::vector<T>& p_curve_val_to_length_v,
													uint32_t		p_division_count = 10,
													T				p_start = 0 /*interval on which we need rebuild*/,
													T				p_end = 1) const override
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


		virtual T				get_closest_point_param(const V<T>& p_point,
														T				p_start = 0,
														T				p_end = 1 /*if p_start == p_end then interval is infinite*/,
														uint32_t		p_divisions = 10,
														T				p_percition = vufCurve_kTol) const override
		{
			return 0;
		}

		virtual T				get_param_by_vector_component(	T	p_value,
																uint32_t	p_component_index = 0/*x by default*/,
																T			p_start = 0,
																T			p_end = 1 /*if p_start == p_end then interval is infinite*/,
																uint32_t	p_divisions = 10,
																T			p_percition = vufCurve_kTol)	const override
		{
			return 0;
		}

		virtual std::shared_ptr<vufCurveNoise <T, V >>		as_curve_noise()	const override
		{ 
			return std::static_pointer_cast<vufCurveNoise<T, V>>(vufCurveNoise<T, V>::m_this.lock());
		}

		inline V<T>			get_pos_at_i(T p_t)		const
		{			
			V<T>	l_res;
			
			l_res.x = m_noise.simplex_4d( (p_t + m_offset_x)* m_scale_x,.0,								.0,								m_speed) * m_amount_x;
			l_res.y = m_noise.simplex_4d(.0,							(p_t + m_offset_y)* m_scale_y,  .0,								m_speed) * m_amount_y;
			l_res.z = m_noise.simplex_4d(.0,							.0,								(p_t + m_offset_z) * m_scale_z, m_speed) * m_amount_z;
			return l_res;
		}
		inline V<T>			get_tangent_at_i(T p_t) const
		{
			V<T>	l_res;
			l_res = (get_pos_at_i(p_t) - get_pos_at_i(p_t + 0.00001)) * 10000;
			return l_res;
		}
		inline bool get_use_quat_i()	const { return m__use_frame_quat; }
		inline bool get_use_scale_i()	const { return m_use_frame_scale; }
		inline T get_time_i()		const { return m_time; }
		inline T get_speed_i()		const { return m_speed; }
		inline T get_amount_x_i()	const { return m_amount_x; }
		inline T get_amount_y_i()	const { return m_amount_y; }
		inline T get_amount_z_i()	const {	return m_amount_z; }
		inline T get_scale_x_i()	const { return m_scale_x; }
		inline T get_scale_y_i()	const { return m_scale_y; }
		inline T get_scale_z_i()	const { return m_scale_z; }
		inline T get_offset_x_i()	const { return m_offset_x; }
		inline T get_offset_y_i()	const { return m_offset_y; }
		inline T get_offset_z_i()	const { return m_offset_z; }
		inline std::shared_ptr< vufCurveContainer<T, V> > get_container() const { return m_container_ptr; }

		inline void set_use_quat_i(bool p_val)	 { m__use_frame_quat = p_val; }
		inline void set_use_scale_i(bool p_val)	 { m_use_frame_scale = p_val; }
		inline void set_time_i(T p_val)			 { m_time		= p_val; }
		inline void set_speed_i(T p_val)		 { m_speed		= p_val; }
		inline void set_amount_x_i(T p_val)		 { m_amount_x	= p_val; }
		inline void set_amount_y_i(T p_val)		 { m_amount_y	= p_val; }
		inline void set_amount_z_i(T p_val)		 { m_amount_z	= p_val; }
		inline void set_scale_x_i(T p_val)		 { m_scale_x	= p_val; }
		inline void set_scale_y_i(T p_val)		 { m_scale_y	= p_val; }
		inline void set_scale_z_i(T p_val)		 { m_scale_z	= p_val; }
		inline void set_offset_x_i(T p_val)		 { m_offset_x	= p_val; }
		inline void set_offset_y_i(T p_val)		 { m_offset_y	= p_val; }
		inline void set_offset_z_i(T p_val)		 { m_offset_z	= p_val; }
		inline void set_container(std::shared_ptr< vufCurveContainer<T, V> > p_cntnr) 
		{ 
			m_container_ptr = p_cntnr;
		}

	private:
		bool m__use_frame_quat = false;
		bool m_use_frame_scale = false;
		T m_time		= 0;
		T m_speed		= 1;
		T m_amount_x	= 1;
		T m_amount_y	= 1;
		T m_amount_z	= 1;

		T m_scale_x		= 1;
		T m_scale_y		= 1;
		T m_scale_z		= 1;

		T m_offset_x	= 0;
		T m_offset_y	= 0;
		T m_offset_z	= 0;

		std::shared_ptr< vufCurveContainer<T, V> >	m_container_ptr = nullptr;
		vufPelinNoise<T> m_noise;
	};
}
#endif // !VF_MATH_CRV_NOISE_H
