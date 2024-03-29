#ifndef VF_MATH_CRV_NOISE_H
#define VF_MATH_CRV_NOISE_H
#include <curves/vufCurve.h>
#include <curves/vufCurvesInclude.h>
#include <math/vufMatrix.h>
#include <noise/vufPelinNoise.h>
#include <memory>

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
			vufCurve<T, V>::m_valid = true;
		}
	public:
		virtual ~vufCurveNoise() {}
		VF_MATH_CURVE_DEFINE_CREATOR(vufCurveNoise);
		VF_MATH_CURVE_DEFINE_TYPE_CATEGORY(k_noise_curve, k_compound_category);
		VF_MATH_CRV_REBUILD;
		VF_MATH_CRV_DEFINE_CLOSEST_POINTS;
		VF_MATH_CRV_DEFINE_PARAM_COMPONENT;
		virtual V<T>		get_pos_at(T p_t)							const override
		{
			return get_pos_at_i(p_t);
		}
		virtual V<T>		get_tangent_at(T p_t)						const override
		{
			return get_tangent_at_i(p_t);
		}
		virtual V<T>		get_tangent_normalized_at(T p_t)			const override
		{
			return get_tangent_at_i(p_t).normalize_in_place();
		}
		virtual vufQuaternion<T>	get_quaternion_at(T p_t)			const override
		{
			if (m_container_ptr == nullptr)
			{
				V<T> l_vec(1.0);
				V<T> l_tng = get_tangent_normalized_at(p_t);
				vufQuaternion<T> l_res = vufQuaternion<T>();
				return l_res.increment_quaternion_with_2vectors(l_vec, l_tng);
			}
			V<T> l_tng_1 = m_container_ptr->get_tangent_at(p_t);
			l_tng_1.normalize_in_place();
			vufQuaternion<T> l_res = m_container_ptr->get_quaternion_at(p_t);
			for (T l_step = p_rotate_step; l_step <= 1.0; l_step+= p_rotate_step)
			{
				V<T> l_tng_2 = get_tangent_normalized_at_i(p_t, l_step);
				V<T> l_cross = l_tng_1.get_cross(l_tng_2);
				if (l_cross.length2() < 0.000001)
				{
					continue;
				}
				l_cross.normalize_in_place();
				T l_dot		= l_tng_1.dot(l_tng_2);
				T l_angle	= -acos(VF_CLAMP(-1., 1., l_dot));
				vufQuaternion<T> l_rot(l_angle, l_cross);
				l_res *= l_rot;
				l_tng_1 = l_tng_2;
			}
			//------------------------
			V<T> l_vec(1.0);
			l_vec = l_res.rotate_vector_by_quaternion(l_vec);
			return l_res.increment_quaternion_with_2vectors(l_vec, l_tng_1);
		}
		virtual V<T>				get_scale_at(T p_t)					const override
		{
			if (m_container_ptr == nullptr)
			{				
				return m_container_ptr->get_scale_at(p_t);
			}
			return V<T>(1.0, 1.0, 1.0);
		}
		virtual std::shared_ptr<vufCurve<T, V>>		get_copy() const override
		{
			std::shared_ptr< vufCurveNoise > l_ptr = vufCurveNoise::create();
			l_ptr->copy_members_from_i(std::static_pointer_cast<vufCurveNoise>(m_this.lock()));
			return l_ptr;
		}

		virtual std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const override
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');

			l_ss << l_str_offset << "[ Noise Curve: <" << typeid(T).name() << ", " << typeid(V).name() << "> ]" << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Time: " << m_time << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Speed " << m_speed << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Amount_x " << m_amount_x << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Amount_y " << m_amount_y << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Amount_z " << m_amount_z << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Scale_x  " << m_scale_x << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Scale_y  " << m_scale_y << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Scale_z  " << m_scale_z << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Offset_x " << m_offset_x << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Offset_y " << m_offset_y << std::endl;
			l_ss << l_str_offset << l_str_offset << "____Offset_z " << m_offset_z << std::endl;
			l_ss << l_str_offset << "____Input Curve: " << (m_container_ptr == nullptr ? "Absent" : m_container_ptr->to_string(p_precision, p_tab_count + 1)) << std::endl;
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
			
			l_size += sizeof(bool);
			if (m_should_serialize_container == true && m_container_ptr != nullptr)
			{
				l_size += m_container_ptr->get_binary_size();
			}
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
			
			if (m_container_ptr != nullptr && m_should_serialize_container == true)
			{
				bool l_save = true;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
				p_offset = m_container_ptr->to_binary(p_buff, p_offset);
			}
			else
			{
				bool l_save = false;
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_save, sizeof(l_save));
			}

			return p_offset;
		}
		virtual uint64_t	from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)
		{
			vufCurve<T, V>::m_valid = false;
			p_offset = vufCurve<T, V>::from_binary(p_buff, p_offset);
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
			
			bool l_load;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_load, sizeof(l_load));
			if (l_load == true)
			{
				m_should_serialize_container = true;
				m_container_ptr = vufCurveContainer<T, V>::create();
				p_offset = m_container_ptr->from_binary(p_buff, p_offset);
			}
			else
			{
				m_should_serialize_container = false;
				m_container_ptr = nullptr;
			}

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

		virtual std::shared_ptr<vufCurveNoise <T, V >>		as_curve_noise()	const override
		{ 
			return std::static_pointer_cast<vufCurveNoise<T, V>>(vufCurveNoise<T, V>::m_this.lock());
		}

		inline V<T>			get_pos_at_i(T p_t)							const
		{
			if (m_container_ptr == nullptr)
			{
				V<T>	l_res;
				l_res.x = m_noise.simplex_4d(p_t * m_scale_x + m_offset_x,
					.0,
					.0,
					m_speed * m_time) * m_amount_x;
				l_res.y = m_noise.simplex_4d(.0,
					p_t * m_scale_y + m_offset_y,
					.0,
					m_speed * m_time) * m_amount_y;
				l_res.z = m_noise.simplex_4d(.0,
					.0,
					p_t * m_scale_z + m_offset_z,
					m_speed * m_time) * m_amount_z;
				return l_res;
			}
			V<T>	l_res = m_container_ptr->get_pos_at(p_t);
			
			T l_x = m_noise.simplex_4d( p_t * m_scale_x + m_offset_x,
										.0,
										.0,
										m_speed * m_time) * m_amount_x;
			T l_y = m_noise.simplex_4d(.0,
										p_t * m_scale_y + m_offset_y,
										.0,
										m_speed * m_time) * m_amount_y;
			T l_z = m_noise.simplex_4d(.0,
										.0,
										p_t  * m_scale_z + m_offset_z,
										m_speed * m_time)  * m_amount_z;
			
			if (m_use_frame_scale == true)
			{
				V<T> l_scale = m_container_ptr->get_scale_at(p_t);
				l_x *= l_scale.x;
				l_y *= l_scale.y;
				l_z *= l_scale.z;
			}
			if (m_use_frame_quat == true)
			{
				auto l_quat = m_container_ptr->get_quaternion_at(p_t);
				vufMatrix_4d l_matr = vufMatrix_4d();
				l_matr.set_quaternion(l_quat);
				l_res += l_matr.get_axis_x_4() * l_x + 
						 l_matr.get_axis_y_4() * l_y +
						 l_matr.get_axis_z_4() * l_z;
				return l_res;
			}

			l_res.x += l_x;
			l_res.y += l_y;
			l_res.z += l_z;

			return l_res;
		}
		inline V<T>			get_pos_at_i(T p_t, T p_amount_weight)		const
		{
			if (m_container_ptr == nullptr)
			{
				V<T>	l_res;
				l_res.x = m_noise.simplex_4d(p_t * m_scale_x + m_offset_x,
					.0,
					.0,
					m_speed * m_time) * m_amount_x * p_amount_weight;
				l_res.y = m_noise.simplex_4d(.0,
					p_t * m_scale_y + m_offset_y,
					.0,
					m_speed * m_time) * m_amount_y * p_amount_weight;
				l_res.z = m_noise.simplex_4d(.0,
					.0,
					p_t * m_scale_z + m_offset_z,
					m_speed * m_time) * m_amount_z * p_amount_weight;
				return l_res;
			}
			V<T>	l_res = m_container_ptr->get_pos_at(p_t);

			T l_x = m_noise.simplex_4d(p_t * m_scale_x + m_offset_x,
				.0,
				.0,
				m_speed * m_time) * m_amount_x * p_amount_weight;
			T l_y = m_noise.simplex_4d(.0,
				p_t * m_scale_y + m_offset_y,
				.0,
				m_speed * m_time) * m_amount_y * p_amount_weight;
			T l_z = m_noise.simplex_4d(.0,
				.0,
				p_t * m_scale_z + m_offset_z,
				m_speed * m_time) * m_amount_z * p_amount_weight;
			
			if (m_use_frame_scale == true)
			{
				V<T> l_scale = m_container_ptr->get_scale_at(p_t);
				l_x *= l_scale.x;
				l_y *= l_scale.y;
				l_z *= l_scale.z;
			}
			if (m_use_frame_quat == true)
			{
				auto l_quat = m_container_ptr->get_quaternion_at(p_t);
				vufMatrix_4d l_matr = vufMatrix_4d();
				l_matr.set_quaternion(l_quat);
				l_res += l_matr.get_axis_x_4() * l_x +
					l_matr.get_axis_y_4() * l_y +
					l_matr.get_axis_z_4() * l_z;
				return l_res;
			}
			
			l_res.x += l_x;
			l_res.y += l_y;
			l_res.z += l_z;

			return l_res;
		}
		inline V<T>			get_tangent_at_i(T p_t)						const
		{
			//V<T>	l_res = m_container_ptr->get_tangent_at(p_t);
			V<T>	l_res = (get_pos_at_i(p_t + 0.00001) - get_pos_at_i(p_t)) * 10000;
			return	l_res;
		}
		inline V<T>			get_tangent_normalized_at_i(T p_t, T p_amount_weight)	const
		{
			V<T>	l_res = (get_pos_at_i(p_t + 0.00001, p_amount_weight) - get_pos_at_i(p_t, p_amount_weight)) * 10000;
			return	l_res.normalize_in_place();
		}

		inline bool get_use_quat_i()	const { return m_use_frame_quat; }
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
		inline std::shared_ptr< vufCurveContainer<T, V> > get_container_i() const { return m_container_ptr; }

		inline void set_use_quat_i(bool p_val)	 { m_use_frame_quat = p_val; }
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
		inline void set_container_i(std::shared_ptr< vufCurveContainer<T, V> > p_cntnr) 
		{ 
			m_container_ptr = p_cntnr;
		}


		void		copy_members_from_i(std::shared_ptr<vufCurveNoise> p_crv)
		{
			vufCurve<T, V>::copy_members_from_i(p_crv);

			m_container_ptr = p_crv->m_container_ptr == nullptr ? nullptr : p_crv->m_container_ptr->get_copy();

			m_use_frame_quat	= p_crv->m_use_frame_quat;
			m_use_frame_scale	= p_crv->m_use_frame_scale;

			m_time		= p_crv->m_time;
			m_speed		= p_crv->m_speed;
			m_amount_x	= p_crv->m_amount_x;
			m_amount_y	= p_crv->m_amount_y;
			m_amount_z	= p_crv->m_amount_z;

			m_scale_x	= p_crv->m_scale_x;
			m_scale_y	= p_crv->m_scale_y;
			m_scale_z	= p_crv->m_scale_z;

			m_offset_x	= p_crv->m_offset_x;
			m_offset_y	= p_crv->m_offset_y;
			m_offset_z	= p_crv->m_offset_z;

		}

	private:
		const T p_rotate_step = 0.2;

		bool m_use_frame_quat = false;
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

		bool										m_should_serialize_container = false;
		std::shared_ptr< vufCurveContainer<T, V> >	m_container_ptr = nullptr;
		vufPelinNoise<T> m_noise;
	};
}
#endif // !VF_MATH_CRV_NOISE_H
