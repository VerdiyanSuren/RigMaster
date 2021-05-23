#ifndef VF_MATH_CRVS_INTERFACE_H
#define VF_MATH_CRVS_INTERFACE_H

#include <vector>
#include <string>
#include <sstream>
#include "math/vufVector.h"
#include "math/vufQuaternion.h"
#include "math/vufNumericArrayObject.h"

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

			std::cout << "A: " << l_A_sign << " B: " << l_B_sign << std::endl;

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
		uint64_t		to_binary(std::vector<unsigned char>& p_buff)	const
		{
			std::vector<T> l_vec(MAX_POLYNOM_DEGREE + 1);
			for (uint64_t i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_vec[i] = a[i];
			}
			vufNumericArrayFn<T> l_fn(l_vec);
			return l_fn.to_binary(p_buff);
		}
		uint64_t		from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)
		{
			std::vector<T> l_vec;
			vufNumericArrayFn<T> l_fn(l_vec);
			uint64_t l_offset = l_fn.from_binary(p_buff, p_offset);
			for (uint64_t i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				a[i] = l_vec[i];
			}
			return l_offset;
		}
		std::string		to_canonic_string() const
		{
			std::stringstream l_ss;
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
			double l_t_1 = (l_d - l_dp.a[1]) / (l_dp.a[2] * 2.0);
			double l_t_2 = (l_d + l_dp.a[1]) / (l_dp.a[2] * 2.0);
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
	/**
	*  This is a base interface class  for all curves
	*  bezier, b-saplines and 
	*/
	enum class curveType:uint8_t
	{
		//open curves
		k_open_bezier_piecewise				= 0,
		k_open_bspline						= 1,
		k_open_general_bspline				= 2,
		k_open_xspline						= 3,
		//close curves
		k_close_bezier_piecewise_quadratic	= 6,
		k_open_bezier_piecewise_quadratic	= 7,
		k_close_polyline					= 8,
		k_close_quadratic					= 9,
		k_close_cubic						= 10,
		k_close_xspline						= 11
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
		virtual std::shared_ptr<vufCurve> make_copy() const = 0;
		/** control points count could be less than neccesary and then curve is invalid */
		bool	 is_valid()		const	{ return m_valid; }
		/** some curves could be  degree 1, 2 or 3 come only degree 2 or 3*/
		uint32_t get_degree()	const	{ return m_degree; }
		/** suportes open and close */
		bool	 is_open()		const	{ return  m_close ==false; }
		bool	 is_close()		const	{ return m_close; }
		T		 get_node_max_influence_t(int p_node_index) const { return m_nodes_max_influence_t_v[p_node_index]; }
		
		/** set and retrieve nodes count for control points of curves */
		virtual void		set_nodes_count(uint32_t p_count) = 0
		{
			if (m_nodes_pos_v.size() != p_count)
			{
				m_nodes_pos_v.resize(p_count);
				update_topology();
			}
		}
		virtual uint32_t	get_nodes_count()	const = 0
		{
			return (uint32_t)m_nodes_pos_v.size();
		}
		virtual uint32_t	get_span_count()	const = 0;
		/** set and get control point position*/
		virtual void		set_node_at(uint32_t p_index, const V<T>& p_vector) = 0
		{
			m_nodes_pos_v[p_index] = p_vector;
		}
		virtual V<T>		get_node_at(uint32_t p_index) const = 0
		{
			return m_nodes_pos_v[p_index];
		}
		virtual T			get_closest_point_param(const V<T>& p_point, T p_start_param) = 0;
		virtual int			get_interval_count() const = 0;

		virtual void		make_samples( int p_per_interval_count) = 0;
		virtual void		update_samples() = 0;

		virtual curveType	get_type()					const = 0;
		virtual T			get_length()				const = 0;
		virtual V<T>		get_pos_at(T p_t)			const = 0;
		virtual V<T>		get_tangent_at(T p_t)		const = 0;
		virtual T			get_basis_at(uint32_t p_node_id, T p_t)				const = 0;
		virtual T			get_basis_derivative_at(uint32_t p_node_id, T p_t)	const = 0;

		virtual bool		update_topology()	= 0;
		virtual void		log_me(int p_tab_count = 0) const		= 0;

	protected:
		uint32_t	m_degree	= 0;
		bool		m_close		= false;
		bool		m_valid		= false;
		
		std::vector<V<T>>	m_nodes_pos_v;
		std::vector<T>		m_nodes_max_influence_t_v;
		uint32_t			m_samples_per_segment;
		std::vector<V<T>>	m_pos_samples_v;
		std::vector<V<T>>	m_div_samples_v;
		std::vector<T>		m_param_samples_v;
		
		//std::vector<vufQuaternion<T>> m_source_quaternion_v;
		//std::vector<std::vector<vufPolinomCoeff>>	m_n_v;	// [time interval, node index] = basis function
		//std::vector<std::vector<vufPolinomCoeff>>	m_dn_v;	// [time interval, node index] = basis derivative

		std::weak_ptr<vufCurve> m_this = std::weak_ptr<vufCurve>();
	};
#pragma endregion VF_CURVE
#pragma region USING_NAMES
	using vufCurve4d = vufCurve<double, vufVector4>;
#pragma endregion
}
#endif // !VF_MATH_CRVS_INTERFACE_H
