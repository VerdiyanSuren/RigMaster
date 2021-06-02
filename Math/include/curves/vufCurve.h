#ifndef VF_MATH_CRVS_INTERFACE_H
#define VF_MATH_CRVS_INTERFACE_H

#include <vector>
#include <string>
#include <sstream>
#include <math/vufVector.h>
#include <math/vufQuaternion.h>
#include <serializer/vufTxtStdVectorSerializerFn.h>
//#include <vufNumericArrayObject.h>

#define VF_MATH_CURVE_CREATOR_BODY(SHARED_CRV) \
		SHARED_CRV->m_this = std::static_pointer_cast<vufCurve<T, V>>(SHARED_CRV);

namespace vufMath
{
#pragma region VF_POLINOM
	template<class T, uint32_t  MAX_POLYNOM_DEGREE >
	class vufPolinomCoeff
	{
	public:
		vufPolinomCoeff()
		{
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				a[i] = .0;
			}
		}
		vufPolinomCoeff(const vufPolinomCoeff& p_other)
		{
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				a[i] = p_other.a[i];
			}
		}
		static vufPolinomCoeff<T, MAX_POLYNOM_DEGREE> random_polinom()
		{
			vufPolinomCoeff<T, MAX_POLYNOM_DEGREE> l_polinom;
			for (uint64_t i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_polinom.a[i] = (T)(rand()) / (T)(RAND_MAX);
			}
			return l_polinom;
		}
		vufPolinomCoeff operator*(const vufPolinomCoeff& p_other)
		{
			vufPolinomCoeff l_plmn;
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				for (int j = 0; j < MAX_POLYNOM_DEGREE + 1; ++j)
				{
					if (i + j > MAX_POLYNOM_DEGREE + 1)
					{
						continue;
					}
					l_plmn.a[i + j] += a[i] * p_other.a[j];
				}
			}
			return l_plmn;
		}
		vufPolinomCoeff operator+(const vufPolinomCoeff& other) const
		{
			vufPolinomCoeff l_plnm;
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_plnm.a[i] = a[i] + other.a[i];
			}
			return l_plnm;
		}
		vufPolinomCoeff& operator+=(const vufPolinomCoeff& other)
		{
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				a[i] += other.a[i];
			}
			return *this;
		}
		vufPolinomCoeff operator*(T p_val) const
		{
			vufPolinomCoeff l_plnm;
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_plnm.a[i] = a[i] * p_val;
			}
			return l_plnm;
		}
		vufPolinomCoeff& operator*=(T p_val)
		{
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				a[i] *= p_val;
			}
			return *this;
		}
		bool operator==(const vufPolinomCoeff& p_other) const
		{
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				if (a[i] != p_other.a[i])
				{
					return false;
				}
			}
			return true;
		}
		bool operator!=(const vufPolinomCoeff& p_other) const
		{
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				if (a[i] != p_other.a[i])
				{
					return true;
				}
			}
			return false;
		}
		T eval(T t) const
		{
			T l_sum = 0;
			for (int i = 0; i < MAX_POLYNOM_DEGREE +1; ++i)
			{
				l_sum +=a[i] * pow(t,i);
			}
			return l_sum;
		}
		vufPolinomCoeff get_derivative() const
		{
			vufPolinomCoeff l_plnm;
			for (int i = 0; i < MAX_POLYNOM_DEGREE; ++i)
			{
				l_plnm.a[i] = a[i + 1] * (i + 1);
			}
			return l_plnm;
		}
		/** Find param at max value oon interval*/
		T				find_max_on_interval(T p_min, T p_max) const
		{			
			// TO DO fix this for degree > 3
			vufPolinomCoeff<T, MAX_POLYNOM_DEGREE> l_dp = get_derivative();
			T l_A = l_dp.eval(p_min);
			T l_B = l_dp.eval(p_max);
			T l_C;

			T l_AV = eval(p_min);
			T l_BV = eval(p_max);

			int l_A_sign = VF_SIGN(l_A);
			int l_B_sign = VF_SIGN(l_B);
			int l_C_sign;

			T l_t = p_min;
			T l_t_a = p_min;
			T l_t_b = p_max;
			T l_t_c;

			//std::cout << "A: " << l_A_sign << " B: " << l_B_sign << std::endl;

			// both derivatives has the same sign
			//(-,-)(+,+)
			if (l_A_sign * l_B_sign == 1)
			{
				if (l_AV > l_BV)
				{
					return l_t_a;
				}
				return l_t_b;
			}
			//(-,-0)
			if (l_A_sign == -1 && l_B_sign < 1)
			{
				return l_t_a;
			}
			//(+0,+)
			if (l_A_sign > -1 &&l_B_sign == 1 )
			{
				return l_t_b;
			}


			//int l_iter = 1;
			while ( l_t_b - l_t_a > VF_MATH_EPSILON )
			{
				l_t_c	= (l_t_a + l_t_b) * 0.5;
				l_C		= l_dp.eval(l_t_c);
				l_C_sign = VF_SIGN(l_C);
				if (l_A_sign * l_C_sign == -1)
				{
					l_t_b = l_t_c;
					l_B = l_C;
					l_B_sign = l_C_sign;
					continue;
				}
				if (l_B_sign * l_C_sign == -1)
				{
					l_t_a = l_t_c;
					l_A = l_C;
					l_A_sign = l_C_sign;
					continue;
				}
				return l_t_c;
			}
			return l_t_a;
		}
		std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0) const
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '_');
			l_ss << l_str_offset;
			bool l_record_exist = false;
			for (int i = MAX_POLYNOM_DEGREE; i >= 0; --i)
			{
				if (a[i] != 0)
				{
					if (a[i] > 0)
					{
						if (l_record_exist == true)
						{
							l_ss << "+";
						}
					}
					l_record_exist = true;
					if (i == 0)
					{
						l_ss << a[i];
						continue;
					}
					if (a[i] == 1)
					{
						if (i == 1)
						{
							l_ss << "x";
							continue;
						}
						l_ss << "x^" << i;
						continue;
					}
					if (a[i] == -1)
					{
						if (i == 1)
						{
							l_ss << "-x";
							continue;
						}

						l_ss << "-x^" << i;
						continue;
					}
					if (i == 1)
					{
						l_ss <<a[i] << "*x";
						continue;
					}
					l_ss <<a[i] <<"*x^" << i;
					
				}
			}
			if (l_record_exist == false)
			{
				l_ss << 0;
			}
			return l_ss.str();
		}
		/*
		std::string		to_string() const
		{
			std::vector<T> l_vec(MAX_POLYNOM_DEGREE +1);
			for (uint64_t i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_vec[i] = a[i];
			}
			vufNumericArrayFn<T> l_fn(l_vec);
			return l_fn.to_string();
		}
		uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0)
		{			
			std::vector<T> l_vec;
			vufNumericArrayFn<T> l_fn(l_vec);
			uint64_t l_offset = l_fn.from_string(p_str, p_offset);
			for (uint64_t i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				a[i] = l_vec[i];
			}
			return l_offset;
		}
		*/
		uint64_t		get_binary_size() const
		{
			return sizeof(T) * (MAX_POLYNOM_DEGREE + 1);
		}
		uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)	const
		{
			if (p_buff.size() < p_offset + sizeof(T) * (MAX_POLYNOM_DEGREE + 1))
			{
				p_buff.resize(p_offset + sizeof(T) * (MAX_POLYNOM_DEGREE + 1));
			}
			std::memcpy(&p_buff[p_offset], a, sizeof(T) * (MAX_POLYNOM_DEGREE + 1));
			return  p_offset + sizeof(T) * (MAX_POLYNOM_DEGREE + 1);
		}
		uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)
		{
			if (p_buff.size() < p_offset + sizeof(T) * (MAX_POLYNOM_DEGREE + 1))
			{
				return 0;
			}
			std::memcpy(a, &p_buff[p_offset], sizeof(T) * (MAX_POLYNOM_DEGREE + 1));
			return p_offset + sizeof(T) * (MAX_POLYNOM_DEGREE + 1);
		}
		
		uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			return vuf::txtSerializer::encode_to_buff(a, (MAX_POLYNOM_DEGREE + 1) * sizeof(T), p_buff, p_offset);
		}
		uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			return vuf::txtSerializer::decode_from_buff(a, (MAX_POLYNOM_DEGREE + 1) * sizeof(T), p_buff, p_offset);
		}

		friend std::ostream& operator<<(std::ostream& p_out, const vufPolinomCoeff<T, MAX_POLYNOM_DEGREE>& p_polinom)
		{
			p_out << "[ " << p_polinom.to_string() << " ]";
			return p_out;
		}

		T a[MAX_POLYNOM_DEGREE +1];
	};
	//specialization for linear
	template<>
	double vufPolinomCoeff<double, 1>::find_max_on_interval(double p_min, double p_max) const
	{
		double l_A = eval(p_min);
		double l_B = eval(p_max);
		return l_A > l_B ? p_min : p_max;
	}
	// specialization for quadratic
	template<>
	double vufPolinomCoeff<double, 2>::find_max_on_interval(double p_min, double p_max) const
	{
		double l_A		= eval(p_min);
		double l_B		= eval(p_max);
		double l_MAX	= l_A;
		double l_MAX_t	= p_min;
		if (l_B > l_MAX)
		{
			l_MAX = l_B;
			l_MAX_t = p_max;
		}
		vufPolinomCoeff<double, 2> l_dp = get_derivative();
		if (abs(l_dp.a[1]) > VF_MATH_EPSILON)
		{
			double l_extrm_t = -l_dp.a[0] / l_dp.a[1];
			if (l_extrm_t > p_min && l_extrm_t < p_max)
			{
				return eval(l_extrm_t) > l_MAX ? l_extrm_t : l_MAX_t;
			}
		}
		return l_MAX_t;
	}
	//sppecialization for cubic
	template<>
	double vufPolinomCoeff<double, 3>::find_max_on_interval(double p_min, double p_max) const
	{
		if (abs(a[3]) < VF_MATH_EPSILON) //if this is quadratic polynom
		{
			vufPolinomCoeff<double, 2> l_p_2;
			l_p_2.a[0] = a[0];
			l_p_2.a[1] = a[1];
			l_p_2.a[2] = a[2];
			return l_p_2.find_max_on_interval(p_min,p_max);
		}
		// cubic polyniom
		double l_A = eval(p_min);
		double l_B = eval(p_max);
		double l_MAX = l_A;
		double l_MAX_t = p_min;
		if (l_B > l_MAX)
		{
			l_MAX = l_B;
			l_MAX_t = p_max;
		}
		
		vufPolinomCoeff<double, 3> l_dp = get_derivative();
		double l_d = l_dp.a[1] * l_dp.a[1] - 4.0 * l_dp.a[2] * l_dp.a[0];
		if (l_d >= 0.0)
		{
			l_d = sqrt(l_d);
			double l_t_1 = ( - l_dp.a[1] + l_d) / (l_dp.a[2] * 2.0);
			double l_t_2 = ( - l_dp.a[1] - l_d) / (l_dp.a[2] * 2.0);
			if (l_t_1 > p_min && l_t_1 < p_max)
			{
				double l_val = eval(l_t_1);
				if (l_val > l_MAX)
				{
					l_MAX = l_val;
					l_MAX_t = l_t_1;
				}
			}
			if (l_t_2 > p_min && l_t_2 < p_max)
			{
				double l_val = eval(l_t_2);
				if (l_val > l_MAX)
				{
					l_MAX = l_val;
					l_MAX_t = l_t_2;
				}
			}
		}
		return l_MAX_t;
	}


#pragma endregion VF_POLINOM
#pragma region VF_CURVE
	
	template <class T, template<typename> class V>				class vufCurveExplicit;

	template <class T, template<typename> class V, uint32_t>	class vufCurveOpenBSpline;
	template <class T, template<typename> class V, uint32_t>	class vufCurveCloseBSpline;
	template <class T, template<typename> class V, uint32_t>	class vufOpenBezier;
	template <class T, template<typename> class V, uint32_t>	class vufCloseBezier; 

	template <class T, template<typename> class V>				class vufOpenXSpline;
	template <class T, template<typename> class V>				class vufCloseXSpline;

	template <class T, template<typename> class V >				class vufCurveBlend;
	template <class T, template<typename> class V >				class vufCurveSlide;

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
		bool		is_open()		const	{ return  m_close == false; }
		bool		is_close()		const	{ return  m_close; }
		//T		 get_node_max_influence_t(int p_node_index) const { return m_nodes_max_influence_t_v[p_node_index]; }

		//virtual int			get_interval_count() const = 0;
		virtual vufCurveType	get_type()					const = 0;
		virtual V<T>			get_pos_at(T p_t)			const = 0;
		virtual V<T>			get_tangent_at(T p_t)		const = 0;

		virtual T	get_closest_point_param(const V<T>& p_point, uint32_t p_divisions = 10,		T p_percition = 0.00001) const= 0;
		virtual T	get_closest_point_param_on_interval( const V<T>& p_point, T p_t_1, T p_t_2, T p_percition = 0.00001) const = 0;
		
		/// Get copy of this curve.	Original curve is unchenged
		virtual std::shared_ptr<vufCurve> get_copy() const = 0;
		virtual std::string		to_string(int p_precision = -1, uint32_t p_tab_count = 0)				const	= 0;
		virtual uint64_t		get_binary_size() const = 0
		{
			return  sizeof(m_valid) + 
					sizeof(m_has_degree) + 
					sizeof(m_degree) + 
					sizeof(m_explicit) + 
					sizeof(m_close);
		}
		virtual uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const = 0
		{
			// resize if needed
			uint64_t l_size = vufCurve<T, V>::get_binary_size();
			if (p_buff.size() < p_offset + l_size)
			{
				p_buff.resize(p_offset + l_size);
			}
			std::memcpy(&p_buff[p_offset], &m_valid,		sizeof(m_valid));		p_offset += sizeof(m_valid);
			std::memcpy(&p_buff[p_offset], &m_has_degree,	sizeof(m_has_degree));	p_offset += sizeof(m_has_degree);
			std::memcpy(&p_buff[p_offset], &m_degree,		sizeof(m_degree));		p_offset += sizeof(m_degree);
			std::memcpy(&p_buff[p_offset], &m_explicit,		sizeof(m_explicit));	p_offset += sizeof(m_explicit);
			std::memcpy(&p_buff[p_offset], &m_close,		sizeof(m_close));		p_offset += sizeof(m_close);
			return p_offset;
		}
		virtual uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0) = 0
		{
			if (p_buff.size() < p_offset + vufCurve<T, V>::get_binary_size())
			{
				return 0;
			}
			std::memcpy(&m_valid,		&p_buff[p_offset], sizeof(m_valid));		p_offset += sizeof(m_valid);
			std::memcpy(&m_has_degree,	&p_buff[p_offset], sizeof(m_has_degree));	p_offset += sizeof(m_has_degree);
			std::memcpy(&m_degree,		&p_buff[p_offset], sizeof(m_degree));		p_offset += sizeof(m_degree);
			std::memcpy(&m_explicit,	&p_buff[p_offset], sizeof(m_explicit));		p_offset += sizeof(m_explicit);
			std::memcpy(&m_close,		&p_buff[p_offset], sizeof(m_close));		p_offset += sizeof(m_close);
			return p_offset;
		}
		virtual uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const = 0
		{
			uint64_t l_size = get_binary_size();
			std::vector<char> l_buff(l_size);
			to_binary(l_buff);
			vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);
			p_offset = l_serializer.encode_to_buff(p_buff, p_offset);
			return p_offset;
		}
		virtual uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0) = 0
		{
			std::vector<char> l_buff;
			vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);
			p_offset = l_serializer.decode_from_buff(p_buff, p_offset);
			from_binary(l_buff);
			return p_offset;
		}

		// convert to explicit
		virtual	std::shared_ptr< vufCurveExplicit<T,V> >		as_explicit_curve()		const { return nullptr; }
		//virtual	std::shared_ptr< vufCurveImplicit<T, V>>		as_implicit_curve()		const { return nullptr; }
		// convert to open bspline
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 1>>		as_open_bspline_mono()	const {	return nullptr;	}
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 2>>		as_open_bspline_di()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 3>>		as_open_bspline_tri()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 4>>		as_open_bspline_tetra()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveOpenBSpline <T, V, 5>>		as_open_bspline_penta()	const { return nullptr; }

		// convert to close bspline
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 1>>		as_close_bspline_mono()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 2>>		as_close_bspline_di()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 3>>		as_close_bspline_tri()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 4>>		as_close_bspline_tetra()const { return nullptr; }
		virtual std::shared_ptr<vufCurveCloseBSpline <T, V, 5>>		as_close_bspline_penta()const { return nullptr; }

		// convert to open bezier
		virtual std::shared_ptr<vufOpenBezier <T, V, 1>>		as_open_bezier_mono()	const { return nullptr; }
		virtual std::shared_ptr<vufOpenBezier <T, V, 2>>		as_open_bezier_di()		const { return nullptr; }
		virtual std::shared_ptr<vufOpenBezier <T, V, 3>>		as_open_bezier_tri()	const { return nullptr; }
		virtual std::shared_ptr<vufOpenBezier <T, V, 4>>		as_open_bezier_tetra()	const { return nullptr; }
		virtual std::shared_ptr<vufOpenBezier <T, V, 5>>		as_open_bezier_penta()	const { return nullptr; }

		// convert to close bezier
		virtual std::shared_ptr<vufCloseBezier <T, V, 1>>		as_close_bezier_mono()	const { return nullptr; }
		virtual std::shared_ptr<vufCloseBezier <T, V, 2>>		as_close_bezier_di()	const { return nullptr; }
		virtual std::shared_ptr<vufCloseBezier <T, V, 3>>		as_close_bezier_tri()	const { return nullptr; }
		virtual std::shared_ptr<vufCloseBezier <T, V, 4>>		as_close_bezier_tetra()	const { return nullptr; }
		virtual std::shared_ptr<vufCloseBezier <T, V, 5>>		as_close_bezier_penta()	const { return nullptr; }

		// convert to open xspline
		virtual std::shared_ptr<vufOpenXSpline  <T, V>>	as_open_xspline_4d()		const { return nullptr; }
		virtual std::shared_ptr<vufCloseXSpline <T, V>>	as_close_xspline_4d()		const { return nullptr; }


		virtual std::shared_ptr<vufCurveBlend <T, V >>		as_curve_blend()	const { return nullptr; }
		virtual std::shared_ptr<vufCurveSlide <T, V >>		as_curve_slide()	const { return nullptr; }

	protected:
		bool		m_valid			= false;	// if inherited curve is valid 
		bool		m_has_degree	= false;
		uint32_t	m_degree		= 0;		// degree of curve. 0 if therre is n't degree
		bool		m_explicit		= true;		// has control pointas or not
		bool		m_close			= false;	//

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
