#ifndef VF_MATH_QTRN_H
#define VF_MATH_QTRN_H

#include "vufMathInclude.h"
#include "vufVector.h"
//#include "vufObject.h"
//#include "vufObjectArrayFn.h"


namespace vuf
{
	//------------------------------------------------------------------------------------------------------------------
	// vufQuaternion
	//------------------------------------------------------------------------------------------------------------------
#pragma region VUF_QUATERNION
	template<typename T>
	class vufQuaternion
	{
	public:
		T x = .0;
		T y = .0;
		T z = .0;
		T w = 1.;

		vufQuaternion(const vufQuaternion& other) :x(other.x), y(other.y), z(other.z), w(other.w) {}
		vufQuaternion(T a = .0, T b = .0, T c = .0, T d = 1.) :x(a), y(b), z(c), w(d) {}
		vufQuaternion(const vufVector3<T>& v) :x(v.x), y(v.y), z(v.z), w(0.0) {}
		vufQuaternion(const vufVector4<T>& v) :x(v.x), y(v.y), z(v.z), w(v.w) {}
		vufQuaternion(const T angle, const vufVector4<T>& axis)
		{
			vufVector4<T> ax = axis.normalize();
			w = cos(angle / 2.);
			x = sin(angle / 2);
			y = x * ax.y;
			z = x * ax.z;
			x *= ax.x;
		}
		vufQuaternion(const T angle, const vufVector3<T>& axis)
		{
			vufVector3<T> ax = axis.normalize();
			w = cos(angle / 2.);
			x = sin(angle / 2);
			y = x * ax.y;
			z = x * ax.z;
			x *= ax.x;
		}

		static vufQuaternion<T> random_quaternion(bool p_all_component = true)
		{
			vufQuaternion l_q;
			l_q.x = (T)(rand()) / (T)(RAND_MAX);
			l_q.y = (T)(rand()) / (T)(RAND_MAX);
			l_q.z = (T)(rand()) / (T)(RAND_MAX);
			l_q.w = (T)(rand()) / (T)(RAND_MAX);
			l_q.normalize_in_place();
			return l_q;
		}
		T			operator()(unsigned int p_index) const
		{
			return ((T*)&x)[p_index];
		}
		const T operator[](unsigned int p_index) const
		{
			return ((T*)&x)[p_index];
		}
		T& operator[](unsigned int p_index)
		{
			return ((T*)&x)[p_index];
		}

		inline void post_constructor(T a = .0, T b = .0, T c = .0, T d = 1.)
		{
			x = a;
			y = b;
			z = c;
			w = d;
		}
		inline void set(T a = .0, T b = .0, T c = .0, T d = 1.)
		{
			x = a;
			y = b;
			z = c;
			w = d;
		}

		inline void  set_by_angle_and_axis_in_place(const T angle, const vufVector4<T>& p_axis)
		{
			vufVector4<T> l_ax = p_axis.get_normalized();
			w = cos(angle / 2.);
			x = sin(angle / 2);
			y = x * l_ax.y;
			z = x * l_ax.z;
			x *= l_ax.x;
		}
		inline void  set_by_angle_and_axis_in_place(const T angle, const vufVector3<T>& p_axis)
		{
			vufVector3<T> l_ax = p_axis.get_normalized();
			w = cos(angle / 2.);
			x = sin(angle / 2);
			y = x * l_ax.y;
			z = x * l_ax.z;
			x *= l_ax.x;
		}

		inline T length2() const { return (x * x + y * y + z * z + w * w); }
		inline T length()  const { return sqrt(x * x + y * y + z * z + w * w); }
		inline vufQuaternion get_normalized() const
		{
			T d = this->length();
			return  (d > VF_MATH_EPSILON ? vufQuaternion(x / d, y / d, z / d, w / d) : *this);
		};
		inline vufQuaternion& normalize_in_place()
		{
			T d = this->length();
			if (d > VF_MATH_EPSILON)
			{
				x /= d;
				y /= d;
				z /= d;
				w /= d;
			}
			return *this;
		}
		inline vufQuaternion get_conjugated() const { return vufQuaternion(-x, -y, -z, w); }
		inline vufQuaternion& conjugate_in_place()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}
		inline vufQuaternion get_inverted() const
		{
			T d = w * w + x * x + y * y + z * z;
			return  (d > VF_MATH_EPSILON ? vufQuaternion(-x / d, -y / d, -z / d, w / d) : *this);

		}
		inline vufQuaternion& invert_in_place()
		{
			T d = w * w + x * x + y * y + z * z;
			if (d > VF_MATH_EPSILON)
			{
				x = -x / d;
				y = -y / d;
				z = -z / d;
				w /= d;
			}
			return *this;
		}
		inline T dot(const vufQuaternion& q) const
		{
			return x * q.x + y * q.y + z * q.z + w * q.w;
		}
		inline T get_angle() const
		{
			return acos(VF_CLAMP(-1., 1, w)) * 2.;
		}
		inline vufVector3<T> get_axis_as_v3() const
		{
			vufVector3<T> l_v(x, y, z);
			l_v.normalize_in_place();
			return l_v;
		}
		inline vufVector4<T> get_axis_as_v4() const
		{
			vufVector4<T> l_v(x, y, z);
			l_v.normalize_in_place();
			return l_v;
		}
		//----------------------------------
		//operators
		inline vufQuaternion operator+(const vufQuaternion& a) const
		{
			return vufQuaternion(x + a.x, y + a.y, z + a.z, w + a.w);
		}
		inline vufQuaternion operator-(const vufQuaternion& a) const
		{
			return vufQuaternion(x - a.x, y - a.y, z - a.z, w - a.w);
		}
		inline vufQuaternion operator-() const
		{
			return vufQuaternion(-x, -y, -z, -w);
		}
		inline vufQuaternion operator*(const vufQuaternion& a) const
		{
			return vufQuaternion(x * a.w + y * a.z - z * a.y + w * a.x,
				-x * a.z + y * a.w + z * a.x + w * a.y,
				x * a.y - y * a.x + z * a.w + w * a.z,
				-x * a.x - y * a.y - z * a.z + w * a.w);
		}
		inline vufQuaternion operator*(const T d) const
		{
			return vufQuaternion(x * d, y * d, z * d, w * d);
		}
		inline vufQuaternion& operator+=(const vufQuaternion& q)
		{
			w += q.w;
			x += q.x;
			y += q.y;
			z += q.z;
			return *this;

		}
		inline vufQuaternion& operator*=(const T d)
		{
			w *= d;
			x *= d;
			y *= d;
			z *= d;
			return *this;
		}
		inline vufQuaternion& operator*=(const vufQuaternion& a)
		{
			this->set(x * a.w + y * a.z - z * a.y + w * a.x,
				-x * a.z + y * a.w + z * a.x + w * a.y,
				x * a.y - y * a.x + z * a.w + w * a.z,
				-x * a.x - y * a.y - z * a.z + w * a.w);
		}
		inline bool operator==(const vufQuaternion<T> v) const
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		inline bool operator!=(const vufQuaternion<T> v) const
		{
			return x != v.x || y != v.y || z != v.z || w != v.w;
		}

		inline vufVector3<T> rotate_vector_by_quaternion(const vufVector3<T>& a) const
		{
			vufQuaternion v(a.x, a.y, a.z, .0);
			vufQuaternion conj = this->get_conjugated();
			vufQuaternion p;

			//p = ( *this )*(-v)*conj;
			p = conj * v * (*this);
			return vufVector3<T>(p.x, p.y, p.z);
		}
		inline vufVector4<T> rotate_vector_by_quaternion(const vufVector4<T>& a) const
		{
			vufQuaternion v(a.x, a.y, a.z, .0);
			vufQuaternion conj = this->get_conjugated();
			vufQuaternion p;

			//p = ( *this )*(-v)*conj;
			p = conj * v * (*this);
			return vufVector4<T>(p.x, p.y, p.z);
		}
		inline vufQuaternion increment_quaternion_with_2vectors(const vufVector3<T>& vFrom, const vufVector3<T>& vTo) const
		{
			vufVector3<T> v1 = vFrom.get_normalized();
			vufVector3<T> v2 = vTo.get_normalized();

			vufQuaternion q = vufQuaternion::rotate_arc(v1, v2);
			q = (*this) * q;// *q;
			//q *=( *this );
			q.normalizeInPlace();
			return q;
		}
		inline vufQuaternion increment_quaternion_with_2vectors(const vufVector4<T>& vFrom, const vufVector4<T>& vTo) const
		{
			vufVector4<T> v1 = vFrom.get_normalized();
			vufVector4<T> v2 = vTo.get_normalized();

			vufQuaternion q = vufQuaternion::rotate_arc(v1, v2);
			q = (*this) * q;// *q;
			//q *=( *this );
			q.normalize_in_place();
			return q;
		}
		
		std::string		to_string() const
		{
			std::stringstream l_ss;
			l_ss.precision(64);
			l_ss << "[" << x << "," << y << "," << z << "," << w << "]";
			return l_ss.str();
		}
		/** patse string for offest return new offset in string*/
		uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			try
			{
				T l_x[4];
				std::stringstream l_ss;
				uint64_t l_index = 0;
				uint64_t l_str_pos = p_offset;
				for (uint64_t i = p_offset; i < p_str.size(); ++i)
				{
					l_str_pos = i;
					if (p_str[l_str_pos] == '[' || p_str[l_str_pos] == ' ' || p_str[l_str_pos] == '\t')
					{
						continue;
					}
					if (p_str[l_str_pos] == ',' || p_str[l_str_pos] == ']')
					{
						l_ss >> l_x[l_index++];
						l_ss.clear();
						if (p_str[l_str_pos] == ']')
						{
							++l_str_pos;
							break;
						}
						continue;
					}
					l_ss << p_str[l_str_pos];
				}
				if (l_index == 4)
				{
					x = l_x[0];
					y = l_x[1];
					z = l_x[2];
					w = l_x[3];
					return l_str_pos;
				}
				return p_offset;
			}
			catch (const std::exception& l_err)
			{
				std::cout << "Error: " << l_err.what() << std::endl;
				return p_offset;
			}
			return p_offset;
		}
		/** write into bytes array return size of array of baties*/
		uint64_t		to_binary(std::vector<unsigned char>& p_buff)	const
		{
			unsigned char* l_x = (unsigned char*)this;
			uint64_t l_writen_size = 0;
			uint64_t l_old_buff_sz = p_buff.size();
			p_buff.resize(l_old_buff_sz + 4 * sizeof(T));
			for (uint64_t i = l_old_buff_sz; i < p_buff.size(); ++i)
			{
				p_buff[i] = l_x[l_writen_size++];
			}
			return l_writen_size;
		}
		/** read vector from binary return size of readed */
		uint64_t		from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)
		{
			if (p_buff.size() < p_offset + 4 * sizeof(T))
			{
				return 0;
			}
			unsigned char* l_x = (unsigned char*)this;
			uint64_t l_read_size = 0;
			for (uint64_t i = p_offset; i < p_offset + 4 * sizeof(T); ++i)
			{
				l_x[l_read_size++] = p_buff[i];
			}
			return p_offset + l_read_size;
		}

		
		static inline vufQuaternion set_by_angle_and_axis(const T p_angle, const vufVector3<T>& p_axis)
		{
			vufVector3<T> l_ax = p_axis.get_normalized();
			vufQuaternion l_q;
			l_q.w = cos(p_angle / 2.);
			l_q.x = sin(p_angle / 2);
			l_q.y = l_q.x * l_ax.y;
			l_q.z = l_q.x * l_ax.z;
			l_q.x *= l_ax.x;
			return  l_q;
		}
		static inline vufQuaternion set_by_angle_and_axis(const T p_angle, const vufVector4<T>& p_axis)
		{
			vufVector4<T> l_ax = p_axis.get_normalized();
			vufQuaternion l_q;
			l_q.w = cos(p_angle / 2.);
			l_q.x = sin(p_angle / 2);
			l_q.y = l_q.x * l_ax.y;
			l_q.z = l_q.x * l_ax.z;
			l_q.x *= l_ax.x;
			return  l_q;
		}
		static inline vufQuaternion rotate_arc(const vufVector3<T>& vFrom, const vufVector3<T>& vTo)
		{
			//assume vectors are normalized
			//rotate v1 vector to v2
			vufVector3<T> v1 = vFrom.get_normalized();
			vufVector3<T> v2 = vTo.get_normalized();
			vufQuaternion res;
			vufVector3<T> crs;

			crs = v2.get_cross(v1);
			T dt = v1.dot(v2) + 1.;
			if (fabs(dt) < VF_MATH_EPSILON)
			{
				//v1 and v2 are parrallel and look in oposite dirrection
				vufVector3<T> x(1., .0, .0);
				vufVector3<T> y(.0, 1., .0);

				vufVector3<T> vr;
				vr = v1.get_cross(x);
				if (vr.length() < VF_MATH_EPSILON)
					vr = v1.get_cross(y);
				res.set_by_angle_and_axis_in_place(VF_MATH_PI, vr);
				return res;
			}
			// main routine
			//crs.normalizeInPlace();
			dt *= 2.;
			dt = sqrt(dt);
			crs *= 1. / dt;
			res.w = dt / 2.;
			res.x = crs.x;
			res.y = crs.y;
			res.z = crs.z;
			res.normalize_in_place();
			return res;
		}
		static inline vufQuaternion rotate_arc(const vufVector4<T>& vFrom, const vufVector4<T>& vTo)
		{
			//assume vectors are normalized
			//rotate v1 vector to v2
			vufVector4<T> v1 = vFrom.get_normalized();
			vufVector4<T> v2 = vTo.get_normalized();
			vufQuaternion res;
			vufVector4<T> crs;

			crs = v2.get_cross(v1);
			T dt = v1.dot(v2) + 1.;
			if (fabs(dt) < VF_MATH_EPSILON)
			{
				//v1 and v2 are parrallel and look in oposite dirrection
				vufVector4<T> x(1., .0, .0);
				vufVector4<T> y(.0, 1., .0);

				vufVector4<T> vr;
				vr = v1.get_cross(x);
				if (vr.length() < VF_MATH_EPSILON)
					vr = v1.get_cross(y);
				res.set_by_angle_and_axis_in_place(VF_MATH_PI, vr);
				return res;
			}
			// main routine
			//crs.normalizeInPlace();
			dt *= 2.;
			dt = sqrt(dt);
			crs *= 1. / dt;
			res.w = dt / 2.;
			res.x = crs.x;
			res.y = crs.y;
			res.z = crs.z;
			res.normalize_in_place();
			return res;
		}
		//void RotateArcInPlace ( const vufVector& from, const vufVector& to );
		// increment_quaternion_with_2vectors_in_place
		static inline void slerp_720(const vufQuaternion& q1, const vufQuaternion q2, T u, vufQuaternion& res)
		{
			T d = q1.dot(q2);
			if (d > .0)
			{
				res = q1 * (1. - u) + q2 * u;
				res.normalized_in_place();
				return;
			}
			d = VF_CLAMP(-1., 1., d);
			T alpha = acos(d);
			T s = sin(alpha);
			T factor = .0;

			if (abs(s) > VF_MATH_EPSILON)
			{
				factor = 1. / s;
				alpha = -1.;
			}
			res = q1 * (sin((1. - u) * alpha) * factor) + q2 * (sin(u * alpha) * factor);
			res.normalize_in_place();
		}
		static inline vufQuaternion slerp_720(const vufQuaternion& q1, const vufQuaternion q2, T u )
		{
			T d = q1.dot(q2);
			if (d > .0)
			{
				auto res = q1 * (1. - u) + q2 * u;
				res.normalized_in_place();
				return res;
			}
			d = VF_CLAMP(-1., 1., d);
			T alpha = acos(d);
			T s = sin(alpha);
			T factor = .0;

			if (abs(s) > VF_MATH_EPSILON)
			{
				factor = 1. / s;
				alpha = -1.;
			}
			auto res = q1 * (sin((1. - u) * alpha) * factor) + q2 * (sin(u * alpha) * factor);
			res.normalized_in_place();
			return res;
		}
		static inline void slerp_360(const vufQuaternion& q1, const vufQuaternion q2, T u, vufQuaternion& res)
		{
			T d = q1.dotMult(q2);
			vufQuaternion Q1(q1);
			if (d < .0)
				Q1 = -Q1;

			res = Q1 * (1. - u) + q2 * u;
			res.normalizeInPlace();
		}
		static inline vufQuaternion slerp_360(const vufQuaternion& q1, const vufQuaternion q2, T u)
		{
			T d = q1.dot(q2);
			vufQuaternion Q1(q1);
			if (d < .0)
				Q1 = -Q1;

			auto l_res = Q1 * (1. - u) + q2 * u;
			l_res.normalizeInPlace();
			return l_res;
		}

		friend std::ostream& operator<<(std::ostream& out, const vufQuaternion& v)
		{
			out << "[ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " ]";
			return out;
		}
		/*
		*/
	};
#pragma endregion VUF_QUATERNION
	/*
	//------------------------------------------------------------------------------------------------------------------
	// vufQuaternion Object
	//------------------------------------------------------------------------------------------------------------------
#pragma region VUF_QUATERNION_OBJECT
	template<typename T>
	class vufQuaternionObject :public vufObject
	{
		//private:
	public:
		vufQuaternionObject() {}
		vufQuaternionObject(const vufQuaternionObject& p_other) : vufObject(), m_quaternion(p_other.m_quaternion) {}
		vufQuaternionObject(const vufQuaternion<T>& p_vec) : vufObject(), m_quaternion(p_vec) {}
	public:
		static std::shared_ptr < vufQuaternionObject<T> >	creator()
		{
			std::shared_ptr<vufQuaternionObject<T>> l_ptr = std::shared_ptr< vufQuaternionObject<T>>(new vufQuaternionObject<T>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual std::shared_ptr<vufObject>				make_copy() const override
		{
			std::shared_ptr<vufQuaternionObject<T>> l_ptr = std::shared_ptr< vufQuaternionObject<T>>(new vufQuaternionObject<T>(*this));
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual uint64_t						get_detailed_count()				const override
		{
			return 0;
		}
		virtual std::shared_ptr<vufObject>		get_detailed_at(uint64_t p_index)	const override
		{
			return nullptr;
		}

		virtual bool		is_array()					const override
		{
			return false;
		}
		virtual bool		is_numeric()				const override
		{
			return false;
		}
		virtual std::string to_string()					const override
		{
			return m_quaternion.to_string();
		}
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) override
		{
			return m_quaternion.from_string(p_str, p_offset);
		}
		virtual uint64_t	to_binary(std::vector<unsigned char>& p_buff)		const override
		{
			return m_quaternion.to_binary(p_buff);
		}
		virtual uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)  override
		{
			return m_quaternion.from_binary(p_buff, p_offset);
		}

		virtual vufObjectType get_type()	const override
		{
			return vufObjectType::k_null_object;
		}
		virtual std::shared_ptr< vufQuaternionObject<float> >	as_quaternion_object_f()	const override { return nullptr; }
		virtual std::shared_ptr< vufQuaternionObject<double> >	as_quaternion_object_d()	const override { return nullptr; }

		vufQuaternion<T> m_quaternion;
	};
	// <float>  object specializations
	template<>
	vufObjectType	vufQuaternionObject<float>::get_type()	const
	{
		return vufObjectType::k_quaternion_f_object;
	}
	template<>
	std::shared_ptr<vufQuaternionObject<float>>	vufQuaternionObject<float>::as_quaternion_object_f()	const
	{
		return std::static_pointer_cast<vufQuaternionObject<float>>(vufObject::m_this.lock());
	}

	// <double>  object specializations
	template<>
	vufObjectType	vufQuaternionObject<double>::get_type()	const
	{
		return vufObjectType::k_quaternion_d_object;
	}
	template<>
	std::shared_ptr<vufQuaternionObject<double>>	vufQuaternionObject<double>::as_quaternion_object_d()	const
	{
		return std::static_pointer_cast<vufQuaternionObject<double>>(vufObject::m_this.lock());
	}
#pragma endregion VUF_QUATERNION_OBJECT
	//------------------------------------------------------------------------------------------------------------------
	// vufQuaternion Object Array
	//------------------------------------------------------------------------------------------------------------------
#pragma region VUF_QUATERNION_OBJECT_ARRAY
	template<typename T>
	class vufQuaternionArrayObject :public vufObject
	{
	public:
		vufQuaternionArrayObject() {}
		vufQuaternionArrayObject(const vufQuaternionArrayObject& p_other) : vufObject(), m_quaternion_array_v(p_other.m_quaternion_array_v) {}
		vufQuaternionArrayObject(const std::vector<vufQuaternion<T>>& p_matr) : vufObject(), m_quaternion_array_v(p_matr) {}
	public:
		static std::shared_ptr < vufQuaternionArrayObject<T> >	creator()
		{
			std::shared_ptr<vufQuaternionArrayObject<T>> l_ptr =
				std::shared_ptr< vufQuaternionArrayObject<T>>(new vufQuaternionArrayObject<T>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual std::shared_ptr<vufObject>					make_copy() const override
		{
			std::shared_ptr<vufQuaternionArrayObject<T>> l_ptr = std::shared_ptr< vufQuaternionArrayObject<T>>(new vufQuaternionArrayObject<T>(*this));
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}

		virtual uint64_t								get_detailed_count()				const override
		{
			return 0;
		}
		virtual std::shared_ptr<vufObject>				get_detailed_at(uint64_t p_index)	const override
		{
			return nullptr;
		}
		virtual bool		is_array()					const override
		{
			return true;
		}
		virtual bool		is_numeric()				const override
		{
			return false;
		}
		virtual std::string to_string()					const override
		{
			VF_OBJECTS_ARRAY_TO_STRNG_AND_RETURN_IT(m_quaternion_array_v);
		}
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) override
		{
			VF_OBJECTS_ARRAY_FROM_STRING_AND_RETURN_OFFSET(p_str, p_offset, m_quaternion_array_v);
		}
		virtual uint64_t	to_binary(std::vector<unsigned char>& p_buff)	const override
		{
			VF_OBJECTS_ARRAY_TO_BINARY_AND_RETURN_SIZE(p_buff, m_quaternion_array_v);
		}
		virtual uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)  override
		{
			VF_OBJECTS_ARRAY_FROM_BINARY_AND_RETURN_OFFSET(p_buff, p_offset, m_quaternion_array_v);
		}

		virtual vufObjectType get_type()	const override
		{
			return vufObjectType::k_null_object;
		}
		virtual std::shared_ptr<vufQuaternionArrayObject<float>>			as_quaternion_array_object_f()	const override { return nullptr; }
		virtual std::shared_ptr<vufQuaternionArrayObject<double>>			as_quaternion_array_object_d()	const override { return nullptr; }

		std::vector<vufQuaternion<T>> m_quaternion_array_v;
	};
	// <float>  object array specializations
	template<>
	vufObjectType	vufQuaternionArrayObject<float>::get_type()	const
	{
		return vufObjectType::k_quaternion_f_array_object;
	}
	template<>
	std::shared_ptr<vufQuaternionArrayObject<float> >	vufQuaternionArrayObject<float>::as_quaternion_array_object_f()	const
	{
		return std::static_pointer_cast<vufQuaternionArrayObject<float>>(vufObject::m_this.lock());
	}
	
	// <double>  object array specializations
	template<>
	vufObjectType	vufQuaternionArrayObject<double>::get_type()	const
	{
		return vufObjectType::k_quaternion_d_array_object;
	}	
	template<>
	std::shared_ptr<vufQuaternionArrayObject<double>>	vufQuaternionArrayObject<double>::as_quaternion_array_object_d()	const
	{
		return std::static_pointer_cast<vufQuaternionArrayObject<double>>(vufObject::m_this.lock());
	}
#pragma endregion VUF_QUATERNION_OBJECT_ARRAY

#pragma region USING_NAMES
	using vufQuaternion_f = vufQuaternion<float>;
	using vufQuaternion_d = vufQuaternion<double>;

	using vufQuaternionObject_f = vufQuaternionObject<float>;
	using vufQuaternionObject_d = vufQuaternionObject<float>;

	using vufQuaternionArrayObject_f = vufQuaternionArrayObject<float>;
	using vufQuaternionArrayObject_d = vufQuaternionArrayObject<double>;

	using vufQuaternionArrayFn_f = vufObjectArrayFn<float,  vufQuaternion>;
	using vufQuaternionArrayFn_d = vufObjectArrayFn<double, vufQuaternion>;
#pragma endregion
	*/
}
#endif // !VF_MATH_QTRN_H
