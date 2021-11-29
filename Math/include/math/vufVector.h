#ifndef VF_MATH_VECTR_H
#define VF_MATH_VECTR_H
#include <cmath>
#include <vufMathInclude.h>

#include <sstream>
#include <exception>
#include <vector>
#include <type_traits>

#include <serializer/vufTxtSerializer.h>
#include <vufObject.h>
#include <vufObjectArrayFn.h>

/**
vufVector4<T>
vufVectorObject4<T>
vufVectorArrayObject4<T>
*/
#ifndef vufVector4_kTol
	#define vufVector4_kTol 1.0e-10
#endif
//#define vufVector4_kTol 1.0e-10
namespace vufMath
{
	template<typename T>	class vufMatrix4;
//------------------------------------------------------------------------------------------------------------------
// vufVector 2
//------------------------------------------------------------------------------------------------------------------

#pragma region VUF_VECTOR_2
	template<typename T>
	class vufVector2
	{
		static_assert(std::is_floating_point_v<T>, "Vector2 can be float or double");
	public:
		T x;
		T y;

		vufVector2(const vufVector2& p_v) :x(p_v.x), y(p_v.y) {}
		vufVector2(T a = .0, T b = .0) :x(a), y(b) {}
		static vufVector2 random(bool p_all_component = true)
		{
			vufVector2 l_vec;
			l_vec.x = (T)(rand()) / (T)(RAND_MAX);
			if (p_all_component == true)
			{
				l_vec.y = (T)(rand()) / (T)(RAND_MAX);
			}
			return l_vec;
		}


		inline void post_constructor(T a = .0, T b = .0) { x = a;	y = b; }
		inline void set(T a = .0, T b = .0 ) { x = a;	y = b; }

		inline T length()  const { return sqrt(x * x + y * y ); }
		inline T length2() const { return (x * x + y * y ); }

		inline vufVector2<T> get_normalized() const
		{
			const T d = this->length();
			return  (d > VF_MATH_EPSILON ? vufVector2<T>(x / d, y / d ) : *this);
		}
		inline vufVector2<T>& normalize_in_place()
		{
			T d = this->length();
			if (d > VF_MATH_EPSILON)
			{
				x /= d; y /= d; 
			}
			return *this;
		}

		inline vufVector2<T> get_ortho_to(const vufVector2<T>& p_v) const
		{
			T d = this->dot(p_v);
			T l2 = p_v.length2();
			if (l2 < VF_MATH_EPSILON)
			{
				return *this;
			}
			d /= l2;
			return vufVector2<T>(x - p_v.x * d, y - p_v.y * d);
		}
		inline vufVector2<T>& make_ortho_to_in_place(const vufVector2<T>& v)
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON) return *this;
			d /= l2;
			x -= v.x * d;
			y -= v.y * d;
			return *this;
		}

		inline vufVector2<T> get_parallel_to(const vufVector2<T>& v) const
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON) return vufVector2<T>();
			d /= l2;
			return vufVector2<T>(v.x * d, v.y * d );
		}
		inline vufVector2<T>& make_parallel_to_in_place(const vufVector2<T>& v)
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON)
			{
				x = y = .0;
				return *this;
			}
			d /= l2;
			x = v.x * d;
			y = v.y * d;

			return *this;
		}

		inline T distance_to(const vufVector2<T>& v) const
		{
			return sqrt((x - v.x) * (x - v.x) +
				(y - v.y) * (y - v.y));
		}
		inline T distance_to2(const vufVector2<T>& v) const
		{
			return ((x - v.x) * (x - v.x) +
				(y - v.y) * (y - v.y));
		}

		inline  T dot(const vufVector2<T>& v) const
		{
			return (x * v.x + y * v.y );
		}
		
		vufVector2<T> as_linear_combination_of(vufVector2<T>& e1, vufVector2<T>& e2)
		{
			//TO DO 
			//Impolement this
			throw std::invalid_argument("vufVector2<T> as_linear_combination_of Not implemented");
			return vufVector2<T>();
		}
		//-----------------------------------	
		inline vufVector2<T>& add_in_place(const vufVector2<T>& v1, const vufVector2<T>& v2)
		{
			x = v1.x + v2.x;
			y = v1.y + v2.y;
			return *this;
		}
		inline vufVector2<T>& add_in_place(const vufVector2<T>& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		inline vufVector2<T>& substruct_in_place(const vufVector2<T>& v1, const vufVector2<T>& v2)
		{
			x = v1.x - v2.x;
			y = v1.y - v2.y;
			return *this;
		}
		inline vufVector2<T>& substruct_in_place(const vufVector2<T>& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		inline vufVector2<T>& mult_in_place(T d) {
			x *= d;
			y *= d;
			return *this;
		}
		inline vufVector2<T>& set(const vufVector2<T>& v)
		{
			x = v.x; y = v.y;
			return *this;
		}
		vufVector2<T>& linear_comb_in_place(T a1, const vufVector2<T>& v1, T a2, const vufVector2<T>& v2)
		{
			x = a1 * v1.x + a2 * v2.x;
			y = a1 * v1.y + a2 * v2.y;
			return *this;
		}
		vufVector2<T>& linear_comb_in_place(T a1, const vufVector2<T>& v1, T a2, const vufVector2<T>& v2, T a3, const vufVector2<T>& v3)
		{
			x = a1 * v1.x + a2 * v2.x + a3 * v3.x;
			y = a1 * v1.y + a2 * v2.y + a3 * v3.y;
			return *this;
		}

		std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0, bool p_multiline = false) const
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '\t');
			l_ss << l_str_offset << "[" << x << "," << y << "]";
			return l_ss.str();
		}
		/** return new offset in string */
		uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			std::stringstream l_ss;
			uint64_t l_str_pos = p_offset;

			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != '[') return p_offset;
			l_str_pos++;
			
			// read x
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ',')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> x;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ',') return p_offset;
			++l_str_pos;
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);

			// read y
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ']')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> y;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ']') return p_offset;
			++l_str_pos;
			return l_str_pos;
		}
		uint64_t		get_binary_size() const
		{
			return 2 * sizeof(T);
		}

		uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)	const
		{
			unsigned char* l_x = (unsigned char*)this;
			uint64_t l_writen_size = 0;
			uint64_t l_old_buff_sz = p_buff.size();
			p_buff.resize(l_old_buff_sz + 2 * sizeof(T));
			for (uint64_t i = l_old_buff_sz; i < p_buff.size(); ++i)
			{
				p_buff[i] = l_x[l_writen_size++];
			}
			return l_writen_size;
		}
		uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0)
		{
			if (p_buff.size() < p_offset + 2 * sizeof(T))
			{
				return 0;
			}
			unsigned char* l_x = (unsigned char*)this;
			uint64_t l_read_size = 0;
			for (uint64_t i = p_offset; i < p_offset + 2 * sizeof(T); ++i)
			{
				l_x[l_read_size++] = p_buff[i];
			}
			return p_offset + l_read_size;
		}

		//static vufVector4<T> LinearInterpolation(const vufVector4<T>& v1, const vufVector4<T>& v2);
		//----------------------------------
		//operators
		inline vufVector2<T> operator+(const vufVector2<T>& a) const
		{
			return vufVector2<T>(x + a.x, y + a.y);
		}
		inline vufVector2<T> operator-(const vufVector2<T>& a) const
		{
			return vufVector2<T>(x - a.x, y - a.y);
		}
		inline vufVector2<T> operator-() const
		{
			return vufVector2<T>(-x, -y);
		}
		inline vufVector2<T> operator*(T d) const
		{
			return vufVector2<T>(x * d, y * d);
		}
		inline vufVector2<T>& operator-=(const vufVector2<T>& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		inline vufVector2<T>& operator+=(const vufVector2<T>& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		inline vufVector2<T>& operator*=(T d) {
			x *= d;
			y *= d;
			return *this;
		}
		inline bool operator==(const vufVector2<T> v) const
		{
			return x == v.x && y == v.y;
		}
		inline bool operator!=(const vufVector2<T> v) const
		{
			return x != v.x || y != v.y;
		}
		friend std::ostream& operator<<(std::ostream& out, const vufVector2<T>& v)
		{
			out << "[ " << v.x << ", " << v.y << ", " << " ]";
			return out;
		}
		friend inline vufVector2<T>  operator*(T d, const vufVector2<T>& v)
		{
			return vufVector3<T>(v.x * d, v.y * d);
		}
	};

#pragma endregion VUF_VECTOR_2
	//------------------------------------------------------------------------------------------------------------------
	// vufVector 3
	//------------------------------------------------------------------------------------------------------------------
#pragma region VUF_VECTOR_3
	template<typename T>
	class vufVector3
	{
		static_assert(std::is_floating_point_v<T>, "Vector3 can be float or double");
	public:
		T x;
		T y;
		T z;
		vufVector3(const vufVector3& p_v) :x(p_v.x), y(p_v.y), z(p_v.z) {}
		vufVector3( T a = .0, T b = .0, T c = .0 ) :x(a), y(b), z(c) {}
		static vufVector3 random(bool p_all_component = true)
		{
			vufVector3 l_vec;
			l_vec.x = (T)(rand()) / (T)(RAND_MAX);
			l_vec.y = (T)(rand()) / (T)(RAND_MAX);
			if (p_all_component == true)
			{
				l_vec.z = (T)(rand()) / (T)(RAND_MAX);
			}
			return l_vec;
		}

		inline void post_constructor(T a = .0, T b = .0, T c = .0) { x = a;	y = b;	z = c; }
		inline void set(T a = .0, T b = .0, T c = .0) { x = a;	y = b;	z = c; }

		inline T length()  const { return sqrt(x * x + y * y + z * z); }
		inline T length2() const { return (x * x + y * y + z * z); }

		inline vufVector3<T> get_normalized() const
		{
			const T d = this->length();
			return  (d > VF_MATH_EPSILON ? vufVector3<T>(x / d, y / d, z / d) : *this);
		}
		inline vufVector3<T>& normalize_in_place() 
		{
			T d = this->length();
			if (d > VF_MATH_EPSILON)
			{
				x /= d; y /= d; z /= d;
			}
			return *this;
		}

		inline vufVector3<T> get_ortho_to(const vufVector3<T>& p_v) const 
		{
			T d = this->dot(p_v);
			T l2 = p_v.length2();
			if (l2 < VF_MATH_EPSILON)
			{
				return *this;
			}
			d /= l2;
			return vufVector3<T>(x - p_v.x * d, y - p_v.y * d, z - p_v.z * d);
		}
		inline vufVector3<T>& make_ortho_to_in_place(const vufVector3<T>& v) 
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON) return *this;
			d /= l2;
			x -= v.x * d;
			y -= v.y * d;
			z -= v.z * d;
			return *this;
		}

		inline vufVector3<T> get_parallel_to(const vufVector3<T>& v) const 
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON) return vufVector3<T>();
			d /= l2;
			return vufVector3<T>(v.x * d, v.y * d, v.z * d);
		}
		inline vufVector3<T>& make_parallel_to_in_place(const vufVector3<T>& v) 
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON)
			{
				x = y = z = .0;
				return *this;
			}
			d /= l2;
			x = v.x * d;
			y = v.y * d;
			z = v.z * d;

			return *this;
		}

		inline T distance_to(const vufVector3<T>& v) const 
		{
			return sqrt((x - v.x) * (x - v.x) +
				(y - v.y) * (y - v.y) +
				(z - v.z) * (z - v.z));
		}
		inline T distance_to2(const vufVector3<T>& v) const 
		{
			return ((x - v.x) * (x - v.x) +
				(y - v.y) * (y - v.y) +
				(z - v.z) * (z - v.z));
		}

		inline  T dot(const vufVector3<T>& v) const
		{
			return (x * v.x + y * v.y + z * v.z);
		}
		inline vufVector3<T> get_cross(const vufVector3<T>& v) const 
		{
			return vufVector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}
		inline vufVector3<T>& cross_in_place(const vufVector3<T>& v) 
		{
			set(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
			return *this;
		}
		vufVector3<T> as_linear_combination_of(vufVector3<T>& e1, vufVector3<T>& e2, vufVector3<T>& e3)
		{
			T det, detX, detY, detZ;
			{
				det = e1.x * e2.y * e3.z +
					e1.z * e2.x * e3.y +
					e1.y * e2.z * e3.x -

					e1.y * e2.x * e3.z -
					e1.z * e2.y * e3.x -
					e1.x * e2.z * e3.y;
			}
			{
				detX = x * e2.y * e3.z +
					z * e2.x * e3.y +
					y * e2.z * e3.x -

					y * e2.x * e3.z -
					z * e2.y * e3.x -
					x * e2.z * e3.y;
			}
			{
				detY = e1.x * y * e3.z +
					e1.z * x * e3.y +
					e1.y * z * e3.x -

					e1.y * x * e3.z -
					e1.z * y * e3.x -
					e1.x * z * e3.y;
			}
			{
				detZ = e1.x * e2.y * z +
					e1.z * e2.x * y +
					e1.y * e2.z * x -

					e1.y * e2.x * z -
					e1.z * e2.y * x -
					e1.x * e2.z * y;
			}
			if (det < VF_MATH_EPSILON && det > -VF_MATH_EPSILON)
			{
				return vufVector3<T>();
			}
			return vufVector3<T>(detX / det, detY / det, detZ / det);
		}
		//-----------------------------------	
		inline vufVector3<T>& add_in_place(const vufVector3<T>& v1, const vufVector3<T>& v2) 
		{
			x = v1.x + v2.x;
			y = v1.y + v2.y;
			z = v1.z + v2.z;
			return *this;
		}
		inline vufVector3<T>& add_in_place(const vufVector3<T>& v) 
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		inline vufVector3<T>& substruct_in_place(const vufVector3<T>& v1, const vufVector3<T>& v2) 
		{
			x = v1.x - v2.x;
			y = v1.y - v2.y;
			z = v1.z - v2.z;
			return *this;
		}
		inline vufVector3<T>& substruct_in_place(const vufVector3<T>& v) 
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		inline vufVector3<T>& mult_in_place(T d) {
			x *= d;
			y *= d;
			z *= d;
			return *this;
		}
		inline vufVector3<T>& set(const vufVector3<T>& v) 
		{
			x = v.x; y = v.y; z = v.z; 
			return *this;
		}
		vufVector3<T>& linear_comb_in_place(T a1, const vufVector3<T>& v1, T a2, const vufVector3<T>& v2)
		{
			x = a1 * v1.x + a2 * v2.x;
			y = a1 * v1.y + a2 * v2.y;
			z = a1 * v1.z + a2 * v2.z;
			return *this;
		}
		vufVector3<T>& linear_comb_in_place(T a1, const vufVector3<T>& v1, T a2, const vufVector3<T>& v2, T a3, const vufVector3<T>& v3)
		{
			x = a1 * v1.x + a2 * v2.x + a3 * v3.x;
			y = a1 * v1.y + a2 * v2.y + a3 * v3.y;
			z = a1 * v1.z + a2 * v2.z + a3 * v3.z;
			return *this;
		}
		
		std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0, bool p_multiline = false) const
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '\t');
			l_ss << l_str_offset << "[" << x << "," << y << "," << z << "]";
			return l_ss.str();
		}
		uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			std::stringstream l_ss;
			uint64_t l_str_pos = p_offset;

			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != '[') return p_offset;
			l_str_pos++;

			// read x
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ',')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> x;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ',') return p_offset;
			++l_str_pos;
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);

			// read y
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ',')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> y;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ',') return p_offset;
			++l_str_pos;

			// read z
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ']')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> z;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ']') return p_offset;
			++l_str_pos;

			return l_str_pos;
		}
		
		uint64_t		get_binary_size() const
		{
			return 3 * sizeof(T);
		}
		uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)	const
		{
			unsigned char* l_x = (unsigned char*)this;
			uint64_t l_writen_size = 0;
			uint64_t l_old_buff_sz = p_buff.size();
			p_buff.resize(l_old_buff_sz + 3 * sizeof(T));
			for (uint64_t i = l_old_buff_sz; i < p_buff.size(); ++i)
			{
				p_buff[i] = l_x[l_writen_size++];
			}
			return l_writen_size;
		}
		uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0)
		{
			if (p_buff.size() < p_offset + 3 * sizeof(T))
			{
				return 0;
			}
			unsigned char* l_x = (unsigned char*)this;
			uint64_t l_read_size = 0;
			for (uint64_t i = p_offset; i < p_offset + 3 * sizeof(T); ++i)
			{
				l_x[l_read_size++] = p_buff[i];
			}
			return p_offset + l_read_size;
		}


		//static vufVector4<T> LinearInterpolation(const vufVector4<T>& v1, const vufVector4<T>& v2);
		//----------------------------------
		//operators
		inline vufVector3<T> operator+(const vufVector3<T>& a) const
		{
			return vufVector3<T>(x + a.x, y + a.y, z + a.z);
		}
		inline vufVector3<T> operator-(const vufVector3<T>& a) const
		{
			return vufVector3<T>(x - a.x, y - a.y, z - a.z);
		}
		inline vufVector3<T> operator-() const
		{
			return vufVector3<T>(-x, -y, -z);
		}
		inline vufVector3<T> operator*(T d) const
		{
			return vufVector3<T>(x * d, y * d, z * d);
		}
		inline vufVector3<T>& operator-=(const vufVector3<T>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		inline vufVector3<T>& operator+=(const vufVector3<T>& v) 
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		inline vufVector3<T>& operator*=(T d) {
			x *= d;
			y *= d;
			z *= d;
			return *this;
		}
		inline bool operator==(const vufVector3<T> v) const
		{
			return x == v.x && y == v.y && z == v.z;
		}
		inline bool operator!=(const vufVector3<T> v) const
		{
			return x != v.x || y != v.y || z != v.z;
		}
		friend std::ostream& operator<<(std::ostream& out, const vufVector3<T>& v)
		{
			out << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
			return out;
		}
		friend inline vufVector3<T>  operator*(T d, const vufVector3<T>& v)
		{
			return vufVector3<T>(v.x * d, v.y * d, v.z * d);
		}
	};
#pragma endregion VUF_VECTOR_3
	//------------------------------------------------------------------------------------------------------------------
	// vufVector 4
	//------------------------------------------------------------------------------------------------------------------
#pragma region VUF_VECTOR_4
	template<typename T>
	class vufVector4
	{
		static_assert(std::is_floating_point_v<T>,"Vector4 can be float or double");
	public:
		T x = .0;
		T y = .0;
		T z = .0;
		T w = 1.;
		vufVector4(const vufVector4& p_v) :x(p_v.x), y(p_v.y), z(p_v.z), w(p_v.w) {}
		vufVector4(T a = .0, T b = .0, T c = .0, T d = 1.) :x(a), y(b), z(c), w(d) {}
		static vufVector4 random(bool p_all_component = true)
		{
			vufVector4 l_vec;
			l_vec.x = (T)(rand()) / (T)(RAND_MAX);
			l_vec.y = (T)(rand()) / (T)(RAND_MAX);
			l_vec.z = (T)(rand()) / (T)(RAND_MAX);
			if (p_all_component == true)
			{
				l_vec.w = (T)(rand()) / (T)(RAND_MAX);
			}
			return l_vec;
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

		inline void post_constructor(T a = .0, T b = .0, T c = .0, T d = 1.) { x = a;	y = b;	z = c;	w = d; }
		inline vufVector4<T>& set(T a = .0, T b = .0, T c = .0, T d = 1.)
		{
			x = a;
			y = b;
			z = c;
			w = d;
			return *this;
		}
		inline vufVector4<T>& set(const vufVector4<T>& v)
		{
			x = v.x; y = v.y; z = v.z; w = v.w;
			return *this;
		}
		//methods
		//bool save(FILE*) const;
		//bool load(FILE*);

		inline T length()  const { return sqrt(x * x + y * y + z * z); }
		inline T length2() const { return (x * x + y * y + z * z); }

		inline vufVector4<T>  get_normalized() const
		{
			const T d = this->length();
			return  (d > VF_MATH_EPSILON ? vufVector4<T>(x / d, y / d, z / d) : *this);
		}
		inline vufVector4<T>& normalize_in_place()
		{
			T d = this->length();
			if (d > VF_MATH_EPSILON)
			{
				x /= d; y /= d; z /= d;
			}
			return *this;
		}

		inline vufVector4<T> get_ortho_to(const vufVector4<T>& p_v) const
		{
			T d = this->dot(p_v);
			T l2 = p_v.length2();
			if (l2 < VF_MATH_EPSILON) return *this;
			d /= l2;
			return vufVector4<T>(x - p_v.x * d, y - p_v.y * d, z - p_v.z * d);
		}
		inline vufVector4<T>& make_ortho_to_in_place(const vufVector4<T>& v)
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON) return *this;
			d /= l2;
			x -= v.x * d;
			y -= v.y * d;
			z -= v.z * d;
			return *this;
		}

		inline vufVector4<T> get_parallel_to(const vufVector4<T>& v) const
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON) return vufVector4<T>();
			d /= l2;
			return vufVector4<T>(v.x * d, v.y * d, v.z * d);
		}
		inline vufVector4<T>& make_parallel_to_in_place(const vufVector4<T>& v)
		{
			T d = this->dot(v);
			T l2 = v.length2();
			if (l2 < VF_MATH_EPSILON)
			{
				x = y = z = .0;
				return *this;
			}
			d /= l2;
			x = v.x * d;
			y = v.y * d;
			z = v.z * d;

			return *this;
		}

		inline T distance_to(const vufVector4<T>& v) const
		{
			return sqrt((x - v.x) * (x - v.x) +
				(y - v.y) * (y - v.y) +
				(z - v.z) * (z - v.z));
		}
		inline T distance_to2(const vufVector4<T>& v) const
		{
			return ((x - v.x) * (x - v.x) +
				(y - v.y) * (y - v.y) +
				(z - v.z) * (z - v.z));
		}

		inline  T dot(const vufVector4<T>& v) const
		{
			return (x * v.x + y * v.y + z * v.z);
		}
		inline vufVector4<T> get_cross(const vufVector4<T>& v) const
		{
			return vufVector4<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}
		inline vufVector4<T>& cross_in_place(const vufVector4<T>& v) {
			set(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
			return *this;
		}
		inline vufVector4<T> get_mult_components(const vufVector4<T>& v) const
		{
			return vufVector4<T>(x * v.x, y * v.y, z * v.z);
		}
		inline vufVector4<T>& get_mult_components_in_place(const vufVector4<T>& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}
		vufVector4<T> as_linear_combination_of(vufVector4<T>& e1, vufVector4<T>& e2, vufVector4<T>& e3)
		{
			T det, detX, detY, detZ;
			{
				det = e1.x * e2.y * e3.z +
					e1.z * e2.x * e3.y +
					e1.y * e2.z * e3.x -

					e1.y * e2.x * e3.z -
					e1.z * e2.y * e3.x -
					e1.x * e2.z * e3.y;
			}
			{
				detX = x * e2.y * e3.z +
					z * e2.x * e3.y +
					y * e2.z * e3.x -

					y * e2.x * e3.z -
					z * e2.y * e3.x -
					x * e2.z * e3.y;
			}
			{
				detY = e1.x * y * e3.z +
					e1.z * x * e3.y +
					e1.y * z * e3.x -

					e1.y * x * e3.z -
					e1.z * y * e3.x -
					e1.x * z * e3.y;
			}
			{
				detZ = e1.x * e2.y * z +
					e1.z * e2.x * y +
					e1.y * e2.z * x -

					e1.y * e2.x * z -
					e1.z * e2.y * x -
					e1.x * e2.z * y;
			}
			if (det < VF_MATH_EPSILON && det > -VF_MATH_EPSILON)
			{
				return vufVector4<T>();
			}
			return vufVector4<T>(detX / det, detY / det, detZ / det);
		}
		//-----------------------------------	
		inline vufVector4<T>& add_in_place(const vufVector4<T>& v1, const vufVector4<T>& v2)
		{
			x = v1.x + v2.x;
			y = v1.y + v2.y;
			z = v1.z + v2.z;
			return *this;
		}
		inline vufVector4<T>& add_in_place(const vufVector4<T>& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		inline vufVector4<T>& substruct_in_place(const vufVector4<T>& v1, const vufVector4<T>& v2)
		{
			x = v1.x - v2.x;
			y = v1.y - v2.y;
			z = v1.z - v2.z;
			return *this;
		}
		inline vufVector4<T>& substruct_in_place(const vufVector4<T>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		inline vufVector4<T>& mult_in_place(T d)
		{
			x *= d;
			y *= d;
			z *= d;
			return *this;
		}
		vufVector4<T>&	linear_comb_in_place(T a1, const vufVector4<T>& v1, T a2, const vufVector4<T>& v2)
		{
			x = a1 * v1.x + a2 * v2.x;
			y = a1 * v1.y + a2 * v2.y;
			z = a1 * v1.z + a2 * v2.z;
			return *this;
		}
		vufVector4<T>&	linear_comb_in_place(T a1, const vufVector4<T>& v1, T a2, const vufVector4<T>& v2, T a3, const vufVector4<T>& v3)
		{
			x = a1 * v1.x + a2 * v2.x + a3 * v3.x;
			y = a1 * v1.y + a2 * v2.y + a3 * v3.y;
			z = a1 * v1.z + a2 * v2.z + a3 * v3.z;
			return *this;
		}
		//static vufVector4<T> LinearInterpolation(const vufVector4<T>& v1, const vufVector4<T>& v2);
		// serialization
		std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0, bool p_multiline = false) const
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '\t');
			l_ss << l_str_offset << "[" << x << "," << y << "," << z <<"," <<w << "]";
			return l_ss.str();
		}
		/** patse string for offest return new offset in string*/
		uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			std::stringstream l_ss;
			uint64_t l_str_pos = p_offset;

			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != '[') return p_offset;
			l_str_pos++;

			// read x
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ',')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> x;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ',') return p_offset;
			++l_str_pos;
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);

			// read y
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ',')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> y;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ',') return p_offset;
			++l_str_pos;

			// read z
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ',')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> z;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ',') return p_offset;
			++l_str_pos;

			// read w
			l_ss.clear();
			l_ss.str("");
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ']')
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> w;
			if (l_ss.fail() == true) return p_offset;
			if (p_str[l_str_pos] != ']') return p_offset;
			++l_str_pos;

			return l_str_pos;
		}
		uint64_t		get_binary_size() const
		{
			return 4 * sizeof(T);
		}

		/** write into bytes array return size of array of byties*/
		uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)	const
		{
			if (p_buff.size() < p_offset + sizeof(T) * 4 )
			{
				p_buff.resize(p_offset + sizeof(T) * 4);
			}
			char* l_data = (char*)&x;
			std::memcpy(&p_buff[p_offset], l_data, sizeof(T) * 4);
			return p_offset + sizeof(T) * 4;
		}
		/** read vector from binary return size of readed */
		uint64_t		from_binary(const std::vector<char>& p_buff, uint32_t& p_version, uint64_t p_offset = 0)
		{
			if (p_buff.size() < p_offset + 4 * sizeof(T))
			{
				return 0;
			}
			char* l_data = (char*)&x;	
			std::memcpy(l_data, &p_buff[p_offset], sizeof(T) * 4);
			return  p_offset + 4 * sizeof(T);
		}

		uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0) const
		{
			return vuf::txtSerializer::encode_to_buff(&x, 4 * sizeof(T), p_buff, p_offset);
		}
		uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			return vuf::txtSerializer::decode_from_buff(&x, 4 * sizeof(T), p_buff, p_offset);
		}
		//----------------------------------
		inline bool		is_equivalent(const vufVector4& p_other, T p_tolerance = vufVector4_kTol)
		{
			return	abs(x - p_other.x) < p_tolerance &&
					abs(y - p_other.y) < p_tolerance &&
					abs(z - p_other.z) < p_tolerance &&
					abs(w - p_other.w) < p_tolerance;
		}

		//operators
		inline vufVector4<T> operator+(const vufVector4<T>& a) const
		{
			return vufVector4<T>(x + a.x, y + a.y, z + a.z);
		}
		inline vufVector4<T> operator-(const vufVector4<T>& a) const
		{
			return vufVector4<T>(x - a.x, y - a.y, z - a.z);
		}
		inline vufVector4<T> operator-() const
		{
			return vufVector4<T>(-x, -y, -z);
		}
		inline vufVector4<T> operator*(T d) const
		{
			return vufVector4<T>(x * d, y * d, z * d);
		}
		inline vufVector4<T> operator*(const vufMatrix4<T>& p_matr) const
		{
			vufVector4<T> l_res;
			l_res.x = x * p_matr[0][0] + y * p_matr[1][0] + z * p_matr[2][0] + w * p_matr[3][0];
			l_res.y = x * p_matr[0][1] + y * p_matr[1][1] + z * p_matr[2][1] + w * p_matr[3][1];
			l_res.z = x * p_matr[0][2] + y * p_matr[1][2] + z * p_matr[2][2] + w * p_matr[3][2];
			l_res.w = x * p_matr[0][3] + y * p_matr[1][3] + z * p_matr[2][3] + w * p_matr[3][3];
			return l_res;
		}
		inline vufVector4<T>& operator-=(const vufVector4<T>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		inline vufVector4<T>& operator+=(const vufVector4<T>& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		inline vufVector4<T>& operator*=(T d) {
			x *= d;
			y *= d;
			z *= d;
			return *this;
		}
		inline vufVector4<T>& operator*=(const vufMatrix4<T>& p_matr)
		{
			T l_x = x * p_matr[0][0] + y * p_matr[1][0] + z * p_matr[2][0] + w * p_matr[3][0];
			T l_y = x * p_matr[0][1] + y * p_matr[1][1] + z * p_matr[2][1] + w * p_matr[3][1];
			T l_z = x * p_matr[0][2] + y * p_matr[1][2] + z * p_matr[2][2] + w * p_matr[3][2];
			T l_w = x * p_matr[0][3] + y * p_matr[1][3] + z * p_matr[2][3] + w * p_matr[3][3];
			x = l_x;
			y = l_y;
			z = l_z;
			w = l_w;
			return *this;
		}
		inline bool operator==(const vufVector4<T> v) const
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		inline bool operator!=(const vufVector4<T> v) const
		{
			return x != v.x || y != v.y || z != v.z || w != v.w;
		}
		friend std::ostream& operator<<(std::ostream& p_out, const vufVector4<T>& p_v)
		{
			p_out << "[ " << p_v.x << ", " << p_v.y << ", " << p_v.z << ", " << p_v.w << " ]";
			return p_out;
		}
		friend inline vufVector4<T>  operator*( T d, const vufVector4<T>& v)
		{
			return vufVector4<T>(v.x * d, v.y * d, v.z * d);
		}
	};
#pragma endregion VUF_VECTOR_4
	//------------------------------------------------------------------------------------------------------------------
	// vufVector Object
	//------------------------------------------------------------------------------------------------------------------
	/*
#pragma region VUF_VECTOR_OBJECT
	template<typename T, template <typename> class VECTOR >
	class vufVectorObject :public vufObject
	{
		//private:
	public:
		vufVectorObject() {}
		vufVectorObject(const vufVectorObject& p_other) : vufObject(), m_vector(p_other.m_vector) {}
		vufVectorObject(const VECTOR<T>& p_vec) : vufObject(), m_vector(p_vec) {}
	public:
		static std::shared_ptr < vufVectorObject<T, VECTOR> >	creator()
		{
			std::shared_ptr<vufVectorObject<T, VECTOR>> l_ptr = std::shared_ptr< vufVectorObject<T, VECTOR>>(new vufVectorObject<T, VECTOR>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual std::shared_ptr<vufObject>				make_copy() const override
		{
			std::shared_ptr<vufVectorObject<T, VECTOR>> l_ptr = std::shared_ptr< vufVectorObject<T, VECTOR>>(new vufVectorObject<T, VECTOR>(*this));
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
			return m_vector.to_string();
		}
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) override
		{
			return m_vector.from_string(p_str, p_offset);
		}
		virtual uint64_t	to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)	const override
		{
			return m_vector.to_binary(p_buff,p_offset);
		}
		virtual uint64_t	from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)  override
		{
			return m_vector.from_binary(p_buff, p_offset);
		}

		virtual vufObjectType get_type()	const override
		{
			return vufObjectType::k_null_object;
		}
		virtual std::shared_ptr< vufVectorObject<float, vufVector2> > as_vector_object_2f()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<float, vufVector3> > as_vector_object_3f()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<float, vufVector4> > as_vector_object_4f()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<double,vufVector2> > as_vector_object_2d()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<double,vufVector3> > as_vector_object_3d()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<double,vufVector4> > as_vector_object_4d()	const override { return nullptr; }
		
		VECTOR<T> m_vector;
	};
	// <float,vector 2>  object specializations
	template<>
	vufObjectType	vufVectorObject<float, vufVector2>::get_type()	const
	{
		return vufObjectType::k_vector_2f_object;
	}
	template<>
	std::shared_ptr< vufVectorObject<float, vufVector2> >		vufVectorObject<float, vufVector2>::as_vector_object_2f()	const
	{
		return std::static_pointer_cast<vufVectorObject<float, vufVector2>>( vufObject::m_this.lock() );
	}
	// <float,vector 3>  object specializations
	template<>
	vufObjectType	vufVectorObject<float, vufVector3>::get_type()	const
	{
		return vufObjectType::k_vector_3f_object;
	}
	template<>
	std::shared_ptr< vufVectorObject<float, vufVector3> >		vufVectorObject<float, vufVector3>::as_vector_object_3f()	const
	{
		return std::static_pointer_cast<vufVectorObject<float, vufVector3>>(vufObject::m_this.lock());
	}
	// <float,vector 4>  object specializations
	template<>
	vufObjectType	vufVectorObject<float, vufVector4>::get_type()	const
	{
		return vufObjectType::k_vector_4f_object;
	}
	template<>
	std::shared_ptr< vufVectorObject<float, vufVector4> >		vufVectorObject<float, vufVector4>::as_vector_object_4f()	const
	{
		return std::static_pointer_cast<vufVectorObject<float, vufVector4>>(vufObject::m_this.lock());
	}
	// <double,vector 2>  object specializations
	template<>
	vufObjectType	vufVectorObject<double, vufVector2>::get_type()	const
	{
		return vufObjectType::k_vector_2d_object;
	}
	template<>
	std::shared_ptr< vufVectorObject<double, vufVector2> >		vufVectorObject<double, vufVector2>::as_vector_object_2d()	const
	{
		return std::static_pointer_cast<vufVectorObject<double, vufVector2>>(vufObject::m_this.lock());
	}
	// <double,vector 3>  object specializations
	template<>
	vufObjectType	vufVectorObject<double, vufVector3>::get_type()	const
	{
		return vufObjectType::k_vector_3d_object;
	}
	template<>
	std::shared_ptr< vufVectorObject<double, vufVector3> >		vufVectorObject<double, vufVector3>::as_vector_object_3d()	const
	{
		return std::static_pointer_cast<vufVectorObject<double, vufVector3>>(vufObject::m_this.lock());
	}
	// <double,vector 4>  object specializations
	template<>
	vufObjectType	vufVectorObject<double, vufVector4>::get_type()	const
	{
		return vufObjectType::k_vector_4d_object;
	}
	template<>
	std::shared_ptr< vufVectorObject<double, vufVector4> >		vufVectorObject<double, vufVector4>::as_vector_object_4d()	const
	{
		return std::static_pointer_cast<vufVectorObject<double, vufVector4>>(vufObject::m_this.lock());
	}
#pragma endregion VUF_VECTOR_OBJECT
	//------------------------------------------------------------------------------------------------------------------
	// vufVector Object Array
	//------------------------------------------------------------------------------------------------------------------
#pragma region VUF_VECTOR_OBJECT_ARRAY
	template<typename T, template <typename> class VECTOR >
	class vufVectorArrayObject :public vufObject
	{
	public:
		vufVectorArrayObject() {}
		vufVectorArrayObject(const vufVectorArrayObject& p_other) : vufObject(), m_vector_array_v(p_other.m_vector_array_v) {}
		vufVectorArrayObject(const std::vector<VECTOR<T>>& p_matr) : vufObject(), m_vector_array_v(p_matr) {}
	public:
		static std::shared_ptr < vufVectorArrayObject<T, VECTOR> >	creator()
		{
			std::shared_ptr<vufVectorArrayObject<T, VECTOR>> l_ptr =
				std::shared_ptr< vufVectorArrayObject<T, VECTOR>>(new vufVectorArrayObject<T, VECTOR>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual std::shared_ptr<vufObject>					make_copy() const override
		{
			std::shared_ptr<vufVectorArrayObject<T, VECTOR>> l_ptr = std::shared_ptr< vufVectorArrayObject<T, VECTOR>>(new vufVectorArrayObject<T, VECTOR>(*this));
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
			uint64_t l_sz = m_vector_array_v.size();
			std::stringstream l_ss;
			l_ss.precision(64);
			l_ss << '[' << l_sz;
			for (uint64_t i = 0; i < l_sz; ++i)
			{
				l_ss << m_vector_array_v[i].to_string();
			}
			l_ss << ']';
			return l_ss.str();
		}
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) override
		{
			uint64_t l_str_pos = p_offset;
			uint64_t l_vec_sz = 0;
			try
			{
				while (p_str[l_str_pos] != '[')
				{
					++l_str_pos;
				}
				std::stringstream l_ss;
				++l_str_pos;
				while (p_str[l_str_pos] != '[')
				{
					l_ss << p_str[l_str_pos++];
				}
				l_ss >> l_vec_sz;
				m_vector_array_v.resize(l_vec_sz);
				for (uint64_t i = 0; i < l_vec_sz; ++i)
				{
					l_str_pos = m_vector_array_v[i].from_string(p_str, l_str_pos);
				}
				while (p_str[l_str_pos] != ']')
				{
					l_ss << p_str[l_str_pos++];
				}
				return l_str_pos;
			}
			catch (const std::exception& l_err)
			{
				std::cout << "Error: " << l_err.what() << __LINE__ << " " << __FILE__ << std::endl;
				return p_offset;
			}
			return p_offset;
		}
		virtual uint64_t	to_binary(std::vector<unsigned char>& p_buff)	const override
		{
			uint64_t l_sz = m_vector_array_v.size();
			uint64_t l_data_sz = sizeof(uint64_t) + l_sz * sizeof(VECTOR<T>);
			uint64_t l_old_buff_sz = p_buff.size();
			p_buff.resize(l_old_buff_sz + l_data_sz);

			//write size
			unsigned char* l_uint = (unsigned char*)&l_sz;
			uint64_t l_index = l_old_buff_sz;
			for (uint64_t i = 0; i < sizeof(uint64_t); ++i)
			{
				p_buff[l_index++] = l_uint[i];
			}
			//wrrite vectors
			for (uint64_t i = 0; i < l_sz; ++i)
			{
				unsigned char* l_x = (unsigned char*)&m_vector_array_v[i];
				for (uint64_t j = 0; j < sizeof(VECTOR<T>); ++j)
				{
					p_buff[l_index++] = l_x[j];
				}
			}
			return l_index - l_old_buff_sz;
		}
		virtual uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)  override
		{
			uint64_t l_sz;
			uint64_t l_buff_offset = p_offset;
			//read size of vector
			unsigned char* l_uint = (unsigned char*)&l_sz;
			for (uint64_t i = 0; i < sizeof(uint64_t); ++i)
			{
				l_uint[i] = p_buff[l_buff_offset++];
			}
			m_vector_array_v.resize(l_sz);
			for (uint64_t i = 0; i < l_sz; ++i)
			{
				unsigned char* l_x = (unsigned char*)&m_vector_array_v[i];
				for (uint64_t j = 0; j < sizeof(VECTOR<T>); ++j)
				{
					l_x[j] = p_buff[l_buff_offset++];
				}
			}

			return l_buff_offset;
		}

		virtual vufObjectType get_type()	const override
		{
			return vufObjectType::k_null_object;
		}
		virtual std::shared_ptr< vufVectorArrayObject<float, vufVector2>>	as_vector_array_object_2f()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<float, vufVector3>>	as_vector_array_object_3f()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<float, vufVector4>>	as_vector_array_object_4f()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<double, vufVector2>>	as_vector_array_object_2d()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<double, vufVector3>>	as_vector_array_object_3d()	const override { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<double, vufVector4>>	as_vector_array_object_4d()	const override { return nullptr; }

		std::vector< VECTOR<T> > m_vector_array_v;
	};
	// <float,vector 2>  object array specializations
	template<>
	vufObjectType	vufVectorArrayObject<float, vufVector2>::get_type()	const
	{
		return vufObjectType::k_vector_2f_array_object;
	}
	template<>
	std::shared_ptr<vufVectorArrayObject<float, vufVector2>>	vufVectorArrayObject<float, vufVector2>::as_vector_array_object_2f()	const
	{
		return std::static_pointer_cast<vufVectorArrayObject<float, vufVector2>>(vufObject::m_this.lock());
	}
	// <float,vector 3>  object array specializations
	template<>
	vufObjectType	vufVectorArrayObject<float, vufVector3>::get_type()	const
	{
		return vufObjectType::k_vector_3f_array_object;
	}
	template<>
	std::shared_ptr<vufVectorArrayObject<float, vufVector3>>	vufVectorArrayObject<float, vufVector3>::as_vector_array_object_3f()	const
	{
		return std::static_pointer_cast<vufVectorArrayObject<float, vufVector3>>(vufObject::m_this.lock());
	}
	// <float,vector 4>  object array specializations
	template<>
	vufObjectType	vufVectorArrayObject<float, vufVector4>::get_type()	const
	{
		return vufObjectType::k_vector_4f_array_object;
	}
	template<>
	std::shared_ptr<vufVectorArrayObject<float, vufVector4>>	vufVectorArrayObject<float, vufVector4>::as_vector_array_object_4f()	const
	{
		return std::static_pointer_cast<vufVectorArrayObject<float, vufVector4>>(vufObject::m_this.lock());
	}

	// <double,vector 2>  object array specializations
	template<>
	vufObjectType	vufVectorArrayObject<double, vufVector2>::get_type()	const
	{
		return vufObjectType::k_vector_2f_array_object;
	}
	template<>
	std::shared_ptr<vufVectorArrayObject<double, vufVector2>>	vufVectorArrayObject<double, vufVector2>::as_vector_array_object_2d()	const
	{
		return std::static_pointer_cast<vufVectorArrayObject<double, vufVector2>>(vufObject::m_this.lock());
	}
	// <double,vector 3>  object array specializations
	template<>
	vufObjectType	vufVectorArrayObject<double, vufVector3>::get_type()	const
	{
		return vufObjectType::k_vector_3f_array_object;
	}
	template<>
	std::shared_ptr<vufVectorArrayObject<double, vufVector3>>	vufVectorArrayObject<double, vufVector3>::as_vector_array_object_3d()	const
	{
		return std::static_pointer_cast<vufVectorArrayObject<double, vufVector3>>(vufObject::m_this.lock());
	}
	// <double,vector 4>  object array specializations
	template<>
	vufObjectType	vufVectorArrayObject<double, vufVector4>::get_type()	const
	{
		return vufObjectType::k_vector_4f_array_object;
	}
	template<>
	std::shared_ptr<vufVectorArrayObject<double, vufVector4>>	vufVectorArrayObject<double, vufVector4>::as_vector_array_object_4d()	const
	{
		return std::static_pointer_cast<vufVectorArrayObject<double, vufVector4>>(vufObject::m_this.lock());
	}
#pragma endregion VUF_VECTOR_OBJECT_ARRAY
	//-------------------------------------------------------------------------------------------------------------------------
	// USING NAMES
	//-------------------------------------------------------------------------------------------------------------------------
	*/
#pragma region USING_NAMES
	using vufVector_2f = vufVector2<float>; 
	using vufVector_3f = vufVector3<float>;
	using vufVector_4f = vufVector4<float>;
	using vufVector_2d = vufVector2<double>;
	using vufVector_3d = vufVector3<double>;
	using vufVector_4d = vufVector4<double>;
	/*
	using vufVectorObject_2f = vufVectorObject<float,  vufVector2>;
	using vufVectorObject_3f = vufVectorObject<float,  vufVector3>;
	using vufVectorObject_4f = vufVectorObject<float,  vufVector4>;
	using vufVectorObject_2d = vufVectorObject<double, vufVector2>;
	using vufVectorObject_3d = vufVectorObject<double, vufVector3>;
	using vufVectorObject_4d = vufVectorObject<double, vufVector4>;

	using vufVectorArrayObject_2d = vufVectorArrayObject<double, vufVector2>;
	using vufVectorArrayObject_3d = vufVectorArrayObject<double, vufVector3>;
	using vufVectorArrayObject_4d = vufVectorArrayObject<double, vufVector4>;
	using vufVectorArrayObject_2f = vufVectorArrayObject<float,  vufVector2>;
	using vufVectorArrayObject_3f = vufVectorArrayObject<float,  vufVector3>;
	using vufVectorArrayObject_4f = vufVectorArrayObject<float,  vufVector4>;

	using vufVectorArrayFn_2d = vufObjectArrayFn<double, vufVector2>;
	using vufVectorArrayFn_3d = vufObjectArrayFn<double, vufVector3>;
	using vufVectorArrayFn_4d = vufObjectArrayFn<double, vufVector4>;
	using vufVectorArrayFn_2f = vufObjectArrayFn<float,  vufVector2>;
	using vufVectorArrayFn_3f = vufObjectArrayFn<float,  vufVector3>;
	using vufVectorArrayFn_4f = vufObjectArrayFn<float,  vufVector4>;
	*/

#pragma endregion USING_NAMES 

}
#endif // !VF_MATH_VECTR_H
