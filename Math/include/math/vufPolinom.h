#ifndef VF_MATH_POLINOM_H
#define VF_MATH_POLINOM_H

#include <vector>

namespace vufMath
{
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
		/// get max non zero coeficient degree
		uint32_t get_degree() const
		{
			for (uint32_t i = MAX_POLYNOM_DEGREE; i > 0; --i)
			{
				if (std::abs(a[i]) > VF_MATH_EPSILON)
				{
					return i;
				}
			}
			return 0;
		}
		/// multiply two polinoms of different degrees
		template<typename T, uint32_t OTHER_DEGREE>
		vufPolinomCoeff<T, MAX_POLYNOM_DEGREE + OTHER_DEGREE> mult(const vufPolinomCoeff<T, OTHER_DEGREE>& p_other) const
		{
			vufPolinomCoeff<T, MAX_POLYNOM_DEGREE + OTHER_DEGREE> l_res;
			for (uint32_t i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				for (uint32_t j = 0; j < OTHER_DEGREE + 1; ++j)
				{
					l_res.a[i + j] += a[i] * p_other.a[j];
				}
			}
			return l_res;
		}
		/// divide one polinom by other
		template<typename T, uint32_t OTHER_DEGREE>
		bool div(	const	vufPolinomCoeff<T, OTHER_DEGREE>&						p_divider,
							vufPolinomCoeff<T, MAX_POLYNOM_DEGREE - OTHER_DEGREE>&	p_res,
							vufPolinomCoeff<T, OTHER_DEGREE - 1>&					p_reminder) const
		{
			//std::cout << "-------------------------------------------\n";
			vufPolinomCoeff l_temp;
			uint32_t l_main_degree = get_degree();
			uint32_t l_divider_degree = p_divider.get_degree();
			if (l_main_degree < l_divider_degree)
			{
				return false;
			}
			uint32_t l_mult_degree = l_main_degree - l_divider_degree;
			p_res *= 0;
			p_res.a[l_mult_degree] = a[l_main_degree] / p_divider.a[l_divider_degree];
			l_temp = sub(p_res.mult(p_divider));
			//std::cout << "1:l_mult_degree " << l_mult_degree << std::endl;
			//std::cout << "1:RES   " << p_res.to_string() << std::endl;
			//std::cout << "1:REM   " << l_temp.to_string() << std::endl;
			//std::cout << "1:CHECK " << (p_divider.mult(p_res).add(l_temp)).to_string() << std::endl << std::endl;

			while (true)
			{
				l_main_degree = l_temp.get_degree();
				//std::cout << l_temp << std::endl;
				if (l_main_degree < l_divider_degree)
				{
					//std::cout << "-------------------------------------------\n";
					l_temp.to_other_degree(p_reminder);
					return true;
				}
				l_mult_degree = l_main_degree - l_divider_degree;
				//std::cout << "2:l_mult_degree " << l_mult_degree << std::endl;
				p_res.a[l_mult_degree] = l_temp.a[l_main_degree] / p_divider.a[l_divider_degree];
				l_temp = sub(p_res.mult(p_divider));
				//std::cout << "2:RES   " << p_res.to_string() << std::endl;
				//std::cout << "2:REM  " << l_temp.to_string() << std::endl;
				//std::cout << "2:CHECK " << (p_divider.mult(p_res).add(l_temp)).to_string() << std::endl;
				//std::cout << std::endl;
			}
			l_temp.to_other_degree(p_reminder);
			//std::cout << "-------------------------------------------\n";
			return true;
		}

		template<typename T, uint32_t OTHER_DEGREE>
		bool to_other_degree(vufPolinomCoeff < T, OTHER_DEGREE>& p_other) const
		{
			uint32_t l_min_degree = VF_MIN(OTHER_DEGREE, MAX_POLYNOM_DEGREE);
			for (uint32_t i = 0; i < l_min_degree + 1; ++i)
			{
				p_other.a[i] = a[i];
			}
			for (uint32_t i = l_min_degree + 1; i < OTHER_DEGREE + 1; ++i)
			{
				p_other.a[i] = 0;
			}
			return true;
		}

		template<typename T, uint32_t OTHER_DEGREE>
		vufPolinomCoeff < T, MAX_POLYNOM_DEGREE> sub(const vufPolinomCoeff<T, OTHER_DEGREE>& p_other) const
		{
			vufPolinomCoeff l_res(*this);
			for (uint32_t i = 0; i < OTHER_DEGREE + 1; ++i)
			{
				l_res.a[i] = a[i] - p_other.a[i];
			}
			return l_res;
		}

		template<typename T, uint32_t OTHER_DEGREE>
		vufPolinomCoeff < T, MAX_POLYNOM_DEGREE> add(const vufPolinomCoeff<T, OTHER_DEGREE>& p_other) const
		{
			vufPolinomCoeff l_res(*this);
			for (uint32_t i = 0; i < OTHER_DEGREE + 1; ++i)
			{
				l_res.a[i] = a[i] + p_other.a[i];
			}
			return l_res;
		}

		int32_t find_root_on_interval(T p_start, T p_end, std::vector<T>& p_res, T p_precision = VF_MATH_EPSILON)
		{
			return 0;
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
		vufPolinomCoeff operator-() const
		{
			vufPolinomCoeff l_plnm;
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_plnm.a[i] = -a[i];
			}
			return l_plnm;
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
			T l_sum = a[0];
			T l_t = t;
			for (int i = 1; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_sum += a[i] * l_t;
				l_t *= t;
			}
			return l_sum;
			/*
			T l_sum = 0;
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_sum += a[i] * pow(t, i);
			}
			return l_sum;
			*/
		}

		vufPolinomCoeff<T, MAX_POLYNOM_DEGREE - 1> get_derivative() const
		{
			vufPolinomCoeff<T, MAX_POLYNOM_DEGREE - 1> l_plnm;
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
			if (l_A_sign > -1 && l_B_sign == 1)
			{
				return l_t_b;
			}


			//int l_iter = 1;
			while (l_t_b - l_t_a > VF_MATH_EPSILON)
			{
				l_t_c = (l_t_a + l_t_b) * 0.5;
				l_C = l_dp.eval(l_t_c);
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
						l_ss << a[i] << "*x";
						continue;
					}
					l_ss << a[i] << "*x^" << i;

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

		T a[MAX_POLYNOM_DEGREE + 1];
	};
	//specialization for linear
	template<>
	double vufPolinomCoeff<double, 1>::find_max_on_interval(double p_min, double p_max) const
	{
		double l_A = eval(p_min);
		double l_B = eval(p_max);
		return l_A > l_B ? p_min : p_max;
	}
	template<>
	int32_t vufPolinomCoeff<double, 1>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res, double p_precision)
	{
		if (std::abs(a[1]) < VF_MATH_EPSILON)
		{
			if (std::abs(a[0]) < VF_MATH_EPSILON)
			{
				return -1; //infinitive
			}
			return 0;
		}
		if (p_res.size() < 1)
		{
			p_res.resize(1);
		}
		p_res[0] = -a[0] / a[1];
		return 1;
	}

	// specialization for quadratic
	template<>
	double vufPolinomCoeff<double, 2>::find_max_on_interval(double p_min, double p_max) const
	{
		double l_A = eval(p_min);
		double l_B = eval(p_max);
		double l_MAX = l_A;
		double l_MAX_t = p_min;
		if (l_B > l_MAX)
		{
			l_MAX = l_B;
			l_MAX_t = p_max;
		}
		vufPolinomCoeff<double, 1> l_dp = get_derivative();
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
	template<>
	int32_t vufPolinomCoeff<double, 2>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res, double p_precision)
	{
		if (a[2] == 0.0)
		{
			vufPolinomCoeff<double, 1> l_polynom;
			to_other_degree(l_polynom);
			return l_polynom.find_root_on_interval(p_start, p_end, p_res);
		}
		double l_determinant = a[1] * a[1] - 4. * a[0] * a[2];
		if (l_determinant < 0.0)
		{
			return 0;
		}
		if (std::abs(l_determinant) < VF_MATH_EPSILON)
		{
			if (p_res.size() < 1)
			{
				p_res.resize(1);
			}
			p_res[0] = (- a[1]) / (2.0 * a[2]);
			return 1;
		}
		if (p_res.size() < 2)
		{
			p_res.resize(2);
		}
		p_res[0] = (-a[1] + sqrt(l_determinant)) / (2.0 * a[2]);
		p_res[1] = (-a[1] - sqrt(l_determinant)) / (2.0 * a[2]);
		return 2;
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
			return l_p_2.find_max_on_interval(p_min, p_max);
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

		vufPolinomCoeff<double, 2> l_dp = get_derivative();
		double l_d = l_dp.a[1] * l_dp.a[1] - 4.0 * l_dp.a[2] * l_dp.a[0];
		if (l_d >= 0.0)
		{
			l_d = sqrt(l_d);
			double l_t_1 = (-l_dp.a[1] + l_d) / (l_dp.a[2] * 2.0);
			double l_t_2 = (-l_dp.a[1] - l_d) / (l_dp.a[2] * 2.0);
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
	

#pragma region Sturm Chain Degree 3
	template<typename T>
	int sturm_chain_3(	const	vufPolinomCoeff<T, 3>& p_p3,
								vufPolinomCoeff<T, 2>& p_p2,
								vufPolinomCoeff<T, 1>& p_p1,
								vufPolinomCoeff<T, 0>& p_p0 )
	{
		vufPolinomCoeff<T, 1> l_p2;
		vufPolinomCoeff<T, 1> l_p1;
		p_p2 = p_p3.get_derivative();
		p_p3.div(p_p2, l_p2, p_p1); p_p1 = -p_p1; if (p_p1.get_degree() == 0) return 2;
		p_p2.div(p_p1, l_p1, p_p0);	p_p0 = -p_p0; if (p_p0.get_degree() == 0) return 3;
		return 4;
	}
	template<typename T>
	int sturm_sign_changed_3(	const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T p_t)
	{
		int l_root_count = 0;
		int l_sign_3 = VF_SIGN(p_p3.eval(p_t));
		int l_sign_2 = VF_SIGN(p_p2.eval(p_t));
		int l_sign_1 = VF_SIGN(p_p1.eval(p_t));
		int l_sign_0 = VF_SIGN(p_p0.eval(p_t));
		int l_sign_start_changed = 0;
		if (l_sign_3 * l_sign_2 < 0.0)  l_sign_start_changed++;
		if (l_sign_2 * l_sign_1 < 0.0)  l_sign_start_changed++;
		if (l_sign_1 * l_sign_0 < 0.0)  l_sign_start_changed++;
		//std::cout << "signes: " << l_sign_3 << " " << l_sign_2 << " " << l_sign_1 << " " << l_sign_0 << " " << l_sign_start_changed << std::endl;
		return l_sign_start_changed;
	}
	template<typename T>
	int sturm_root_count_3(const	vufPolinomCoeff<T, 3>& p_p3,
							const	vufPolinomCoeff<T, 2>& p_p2,
							const	vufPolinomCoeff<T, 1>& p_p1,
							const	vufPolinomCoeff<T, 0>& p_p0,
							T p_start, T p_end)
	{
		int l_sign_start	= sturm_sign_changed_3<T>(p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_3<T>(p_p3, p_p2, p_p1, p_p0, p_end);
		return std::abs(l_sign_start - l_sign_end);
	}
	template<typename T>
	bool sturm_isolate_root_3(const	vufPolinomCoeff<T, 3>& p_p3,
							const	vufPolinomCoeff<T, 2>& p_p2,
							const	vufPolinomCoeff<T, 1>& p_p1,
							const	vufPolinomCoeff<T, 0>& p_p0,
							T& p_start, T& p_end)
	{	
		//if (std::abs(p_p3.eval(p_end)) < VF_MATH_EPSILON) return p_end;
		int l_sign_start	= sturm_sign_changed_3<double>(p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_3<double>(p_p3, p_p2, p_p1, p_p0, p_end);
		int l_root_count	= std::abs(l_sign_start - l_sign_end);		
		T l_temp = p_start;
		if (l_root_count == 0)	return false;
		while (l_root_count > 1)
		{
			l_temp = (p_start + p_end)*0.5;
			//if (std::abs(p_p3.eval(l_temp)) < VF_MATH_EPSILON) return l_temp;
			//std::cout << l_temp << std::endl;
			int l_sign = sturm_sign_changed_3<double>(p_p3, p_p2, p_p1, p_p0, l_temp);
			//std::cout << l_root_count << " " << l_temp <<  std::endl;
			if (std::abs(l_sign_start - l_sign) == 0)
			{
				p_start = l_temp;
				l_sign_start = l_sign;
				l_root_count = std::abs(l_sign_start - l_sign_end);
				continue;
			}			
			p_end = l_temp;
			l_sign_end = l_sign;
			l_root_count = std::abs(l_sign_start - l_sign_end);
		}
		return true;
	}
	template<>
	int32_t vufPolinomCoeff<double, 3>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res, double p_precision)
	{
		if (p_res.size() < 3) p_res.resize(3);
		vufPolinomCoeff<double, 2> l_p2;
		vufPolinomCoeff<double, 1> l_p1;
		vufPolinomCoeff<double, 0> l_p0;
		sturm_chain_3<double>(*this, l_p2, l_p1, l_p0);
		int l_root_count = sturm_root_count_3<double>(*this, l_p2, l_p1, l_p0, p_start, p_end);
		if (l_root_count == 0) return 0;
		double l_start	= p_start;
		double l_end	= p_end;
		for (int i = 0; i < l_root_count; ++i)
		{
			sturm_isolate_root_3<double>(*this, l_p2, l_p1, l_p0, l_start, l_end);
			double l_s = l_start;
			double l_e = l_end;
			while ( (l_e - l_s) > p_precision)
			{
				double l_middle = (l_s + l_e) * 0.5;
				if (sturm_root_count_3<double>(*this, l_p2, l_p1, l_p0, l_s, l_middle) > 0)
				{
					l_e = l_middle;
					continue;
				}
				l_s = l_middle;
			}
			p_res[i] = l_s;
			l_start = l_end;
			l_end = p_end;
		}
		return l_root_count;
	}
#pragma endregion
#pragma region Sturm Chain Degree 4
	template<typename T>
	int sturm_chain_4(	const	vufPolinomCoeff<T, 4>& p_p4,
								vufPolinomCoeff<T, 3>& p_p3,
								vufPolinomCoeff<T, 2>& p_p2,
								vufPolinomCoeff<T, 1>& p_p1,
								vufPolinomCoeff<T, 0>& p_p0)
	{
		vufPolinomCoeff<T, 1> l_res;
		
		p_p3 = p_p4.get_derivative();
		p_p4.div(p_p3, l_res, p_p2); p_p2 = -p_p2; if (p_p2.get_degree() == 0) return 2;
		p_p3.div(p_p2, l_res, p_p1); p_p1 = -p_p1; if (p_p1.get_degree() == 0) return 3;
		p_p2.div(p_p1, l_res, p_p0); p_p0 = -p_p0; if (p_p0.get_degree() == 0) return 4;
		return 5;
	}
	template<typename T>
	int sturm_sign_changed_4(	const	vufPolinomCoeff<T, 4>& p_p4,
								const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T p_t)
	{
		int l_root_count = 0;
		int l_sign_4 = VF_SIGN(p_p4.eval(p_t));
		int l_sign_3 = VF_SIGN(p_p3.eval(p_t));
		int l_sign_2 = VF_SIGN(p_p2.eval(p_t));
		int l_sign_1 = VF_SIGN(p_p1.eval(p_t));
		int l_sign_0 = VF_SIGN(p_p0.eval(p_t));		
		int l_sign_start_changed = 0;
		if (l_sign_4 * l_sign_3 < 0.0)  l_sign_start_changed++;
		if (l_sign_3 * l_sign_2 < 0.0)  l_sign_start_changed++;
		if (l_sign_2 * l_sign_1 < 0.0)  l_sign_start_changed++;
		if (l_sign_1 * l_sign_0 < 0.0)  l_sign_start_changed++;
		//std::cout << "signes: " << l_sign_3 << " " << l_sign_2 << " " << l_sign_1 << " " << l_sign_0 << " " << l_sign_start_changed << std::endl;
		return l_sign_start_changed;
	}
	template<typename T>
	int sturm_root_count_4(	const	vufPolinomCoeff<T, 4>& p_p4,
							const	vufPolinomCoeff<T, 3>& p_p3,
							const	vufPolinomCoeff<T, 2>& p_p2,
							const	vufPolinomCoeff<T, 1>& p_p1,
							const	vufPolinomCoeff<T, 0>& p_p0,
							T p_start, T p_end)
	{
		int l_sign_start	= sturm_sign_changed_4<T>(p_p4, p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_4<T>(p_p4, p_p3, p_p2, p_p1, p_p0, p_end);
		return std::abs(l_sign_start - l_sign_end);
	}
	template<typename T>
	bool sturm_isolate_root_4(	const	vufPolinomCoeff<T, 4>& p_p4,
								const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T& p_start, T& p_end)
	{
		//if (std::abs(p_p3.eval(p_end)) < VF_MATH_EPSILON) return p_end;
		int l_sign_start	= sturm_sign_changed_4<double>(p_p4, p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_4<double>(p_p4, p_p3, p_p2, p_p1, p_p0, p_end);
		int l_root_count = std::abs(l_sign_start - l_sign_end);
		T l_temp = p_start;
		if (l_root_count == 0)	return false;
		while (l_root_count > 1)
		{
			l_temp = (p_start + p_end) * 0.5;
			//if (std::abs(p_p3.eval(l_temp)) < VF_MATH_EPSILON) return l_temp;
			//std::cout << l_temp << std::endl;
			int l_sign = sturm_sign_changed_4<double>(p_p4, p_p3, p_p2, p_p1, p_p0, l_temp);
			//std::cout << l_root_count << " " << l_temp <<  std::endl;
			if (std::abs(l_sign_start - l_sign) == 0)
			{
				p_start = l_temp;
				l_sign_start = l_sign;
				l_root_count = std::abs(l_sign_start - l_sign_end);
				continue;
			}
			p_end = l_temp;
			l_sign_end = l_sign;
			l_root_count = std::abs(l_sign_start - l_sign_end);
		}
		return true;
	}
	template<>
	int32_t vufPolinomCoeff<double, 4>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res, double p_precision)
	{
		if (p_res.size() < 4) p_res.resize(4);
		vufPolinomCoeff<double, 3> l_p3;
		vufPolinomCoeff<double, 2> l_p2;
		vufPolinomCoeff<double, 1> l_p1;
		vufPolinomCoeff<double, 0> l_p0;
		sturm_chain_4<double>(*this, l_p3, l_p2, l_p1, l_p0);
		int l_root_count = sturm_root_count_4<double>(*this, l_p3, l_p2, l_p1, l_p0, p_start, p_end);
		if (l_root_count == 0) return 0;
		double l_start = p_start;
		double l_end = p_end;
		for (int i = 0; i < l_root_count; ++i)
		{
			sturm_isolate_root_4<double>(*this, l_p3, l_p2, l_p1, l_p0, l_start, l_end);
			double l_s = l_start;
			double l_e = l_end;
			while ((l_e - l_s) > p_precision)
			{
				double l_middle = (l_s + l_e) * 0.5;
				if (sturm_root_count_4<double>(*this, l_p3, l_p2, l_p1, l_p0, l_s, l_middle) > 0)
				{
					l_e = l_middle;
					continue;
				}
				l_s = l_middle;
			}
			p_res[i] = l_s;
			l_start = l_end;
			l_end = p_end;
		}
		return l_root_count;
	}
#pragma endregion
#pragma region Sturm Chain Degree 5
	template<typename T>
	int sturm_chain_5(const	vufPolinomCoeff<T, 5>& p_p5,
							vufPolinomCoeff<T, 4>& p_p4,
							vufPolinomCoeff<T, 3>& p_p3,
							vufPolinomCoeff<T, 2>& p_p2,
							vufPolinomCoeff<T, 1>& p_p1,
							vufPolinomCoeff<T, 0>& p_p0)
	{
		vufPolinomCoeff<T, 1> l_res;
		vufPolinomCoeff<T, 1> l_p1;
		p_p4 = p_p5.get_derivative();
		p_p5.div(p_p4, l_res, p_p3); p_p3 = -p_p3; if (p_p3.get_degree() == 0) return 2;
		p_p4.div(p_p3, l_res, p_p2); p_p2 = -p_p2; if (p_p2.get_degree() == 0) return 3;
		p_p3.div(p_p2, l_res, p_p1); p_p1 = -p_p1; if (p_p1.get_degree() == 0) return 4;
		p_p2.div(p_p1, l_res, p_p0); p_p0 = -p_p0; if (p_p0.get_degree() == 0) return 5;
		return 6;
	}
	template<typename T>
	int sturm_sign_changed_5(	const	vufPolinomCoeff<T, 5>& p_p5,
								const	vufPolinomCoeff<T, 4>& p_p4,
								const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T p_t)
	{
		int l_root_count = 0;
		int l_sign_5 = VF_SIGN(p_p5.eval(p_t));
		int l_sign_4 = VF_SIGN(p_p4.eval(p_t));
		int l_sign_3 = VF_SIGN(p_p3.eval(p_t));
		int l_sign_2 = VF_SIGN(p_p2.eval(p_t));
		int l_sign_1 = VF_SIGN(p_p1.eval(p_t));
		int l_sign_0 = VF_SIGN(p_p0.eval(p_t));
		int l_sign_start_changed = 0;
		if (l_sign_5 * l_sign_4 < 0.0)  l_sign_start_changed++;
		if (l_sign_4 * l_sign_3 < 0.0)  l_sign_start_changed++;
		if (l_sign_3 * l_sign_2 < 0.0)  l_sign_start_changed++;
		if (l_sign_2 * l_sign_1 < 0.0)  l_sign_start_changed++;
		if (l_sign_1 * l_sign_0 < 0.0)  l_sign_start_changed++;
		//std::cout << "pt: " << p_t << " signes: " << l_sign_5 << l_sign_4 << l_sign_3 << " " << l_sign_2 << " " << l_sign_1 << " " << l_sign_0 << " " << l_sign_start_changed << std::endl;
		return l_sign_start_changed;
	}
	template<typename T>
	int sturm_root_count_5(	const	vufPolinomCoeff<T, 5>& p_p5,
							const	vufPolinomCoeff<T, 4>& p_p4,
							const	vufPolinomCoeff<T, 3>& p_p3,
							const	vufPolinomCoeff<T, 2>& p_p2,
							const	vufPolinomCoeff<T, 1>& p_p1,
							const	vufPolinomCoeff<T, 0>& p_p0,
							T p_start, T p_end)
	{
		int l_sign_start	= sturm_sign_changed_5<T>(p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_5<T>(p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_end);
		return std::abs(l_sign_start - l_sign_end);
	}
	template<typename T>
	bool sturm_isolate_root_5(	const	vufPolinomCoeff<T, 5>& p_p5,
								const	vufPolinomCoeff<T, 4>& p_p4,
								const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T& p_start, T& p_end)
	{
		//if (std::abs(p_p3.eval(p_end)) < VF_MATH_EPSILON) return p_end;
		int l_sign_start	= sturm_sign_changed_5<double>(p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_5<double>(p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_end);
		int l_root_count = std::abs(l_sign_start - l_sign_end);
		T l_temp = p_start;
		if (l_root_count == 0)	return false;
		while (l_root_count > 1)
		{
			l_temp = (p_start + p_end) * 0.5;
			//if (std::abs(p_p3.eval(l_temp)) < VF_MATH_EPSILON) return l_temp;
			//std::cout << l_temp << std::endl;
			int l_sign = sturm_sign_changed_5<double>(p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, l_temp);
			//std::cout << l_root_count << " " << l_temp <<  std::endl;
			if (std::abs(l_sign_start - l_sign) == 0)
			{
				p_start = l_temp;
				l_sign_start = l_sign;
				l_root_count = std::abs(l_sign_start - l_sign_end);
				continue;
			}
			p_end = l_temp;
			l_sign_end = l_sign;
			l_root_count = std::abs(l_sign_start - l_sign_end);
		}
		return true;
	}
	template<>
	int32_t vufPolinomCoeff<double, 5>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res, double p_precision)
	{
		if (p_res.size() < 5) p_res.resize(5);
		vufPolinomCoeff<double, 4> l_p4;
		vufPolinomCoeff<double, 3> l_p3;
		vufPolinomCoeff<double, 2> l_p2;
		vufPolinomCoeff<double, 1> l_p1;
		vufPolinomCoeff<double, 0> l_p0;
		sturm_chain_5<double>(*this, l_p4, l_p3, l_p2, l_p1, l_p0);
		int l_root_count = sturm_root_count_5<double>(*this, l_p4, l_p3, l_p2, l_p1, l_p0, p_start, p_end);
		if (l_root_count == 0) return 0;
		double l_start = p_start;
		double l_end = p_end;
		for (int i = 0; i < l_root_count; ++i)
		{
			sturm_isolate_root_5<double>(*this, l_p4, l_p3, l_p2, l_p1, l_p0, l_start, l_end);
			double l_s = l_start;
			double l_e = l_end;
			while ((l_e - l_s) > p_precision)
			{
				double l_middle = (l_s + l_e) * 0.5;
				if (sturm_root_count_5<double>(*this, l_p4, l_p3, l_p2, l_p1, l_p0, l_s, l_middle) > 0)
				{
					l_e = l_middle;
					continue;
				}
				l_s = l_middle;
			}
			p_res[i] = l_s;
			l_start = l_end;
			l_end = p_end;
		}
		return l_root_count;
	}
#pragma endregion
#pragma region Sturm Chain Degree 6
	template<typename T>
	int sturm_chain_6(const	vufPolinomCoeff<T, 6>& p_p6,
							vufPolinomCoeff<T, 5>& p_p5,
							vufPolinomCoeff<T, 4>& p_p4,
							vufPolinomCoeff<T, 3>& p_p3,
							vufPolinomCoeff<T, 2>& p_p2,
							vufPolinomCoeff<T, 1>& p_p1,
							vufPolinomCoeff<T, 0>& p_p0)
	{
		vufPolinomCoeff<T, 1> l_res;
		vufPolinomCoeff<T, 1> l_p1;
		p_p5 = p_p6.get_derivative();
		p_p6.div(p_p5, l_res, p_p4); p_p4 = -p_p4; if (p_p4.get_degree() == 0) return 2;
		p_p5.div(p_p4, l_res, p_p3); p_p3 = -p_p3; if (p_p3.get_degree() == 0) return 3;
		p_p4.div(p_p3, l_res, p_p2); p_p2 = -p_p2; if (p_p2.get_degree() == 0) return 4;
		p_p3.div(p_p2, l_res, p_p1); p_p1 = -p_p1; if (p_p1.get_degree() == 0) return 5;
		p_p2.div(p_p1, l_res, p_p0); p_p0 = -p_p0; if (p_p0.get_degree() == 0) return 6;
		return 7;
	}
	template<typename T>
	int sturm_sign_changed_6(	const	vufPolinomCoeff<T, 6>& p_p6,
								const	vufPolinomCoeff<T, 5>& p_p5,
								const	vufPolinomCoeff<T, 4>& p_p4,
								const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T p_t)
	{
		int l_root_count = 0;
		int l_sign_6 = VF_SIGN(p_p6.eval(p_t));
		int l_sign_5 = VF_SIGN(p_p5.eval(p_t));
		int l_sign_4 = VF_SIGN(p_p4.eval(p_t));
		int l_sign_3 = VF_SIGN(p_p3.eval(p_t));
		int l_sign_2 = VF_SIGN(p_p2.eval(p_t));
		int l_sign_1 = VF_SIGN(p_p1.eval(p_t));
		int l_sign_0 = VF_SIGN(p_p0.eval(p_t));
		int l_sign_start_changed = 0;
		if (l_sign_6 * l_sign_5 < 0.0)  l_sign_start_changed++;
		if (l_sign_5 * l_sign_4 < 0.0)  l_sign_start_changed++;
		if (l_sign_4 * l_sign_3 < 0.0)  l_sign_start_changed++;
		if (l_sign_3 * l_sign_2 < 0.0)  l_sign_start_changed++;
		if (l_sign_2 * l_sign_1 < 0.0)  l_sign_start_changed++;
		if (l_sign_1 * l_sign_0 < 0.0)  l_sign_start_changed++;
		//std::cout << "pt: " << p_t << " signes: " << l_sign_5 << l_sign_4 << l_sign_3 << " " << l_sign_2 << " " << l_sign_1 << " " << l_sign_0 << " " << l_sign_start_changed << std::endl;
		return l_sign_start_changed;
	}
	template<typename T>
	int sturm_root_count_6(	const	vufPolinomCoeff<T, 6>& p_p6,
							const	vufPolinomCoeff<T, 5>& p_p5,
							const	vufPolinomCoeff<T, 4>& p_p4,
							const	vufPolinomCoeff<T, 3>& p_p3,
							const	vufPolinomCoeff<T, 2>& p_p2,
							const	vufPolinomCoeff<T, 1>& p_p1,
							const	vufPolinomCoeff<T, 0>& p_p0,
		T p_start, T p_end)
	{
		int l_sign_start	= sturm_sign_changed_6<T>(p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_6<T>(p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_end);
		return std::abs(l_sign_start - l_sign_end);
	}
	template<typename T>
	bool sturm_isolate_root_6(	const	vufPolinomCoeff<T, 6>& p_p6,
								const	vufPolinomCoeff<T, 5>& p_p5,
								const	vufPolinomCoeff<T, 4>& p_p4,
								const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T& p_start, T& p_end)
	{
		//if (std::abs(p_p3.eval(p_end)) < VF_MATH_EPSILON) return p_end;
		int l_sign_start	= sturm_sign_changed_6<double>(p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_6<double>(p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_end);
		int l_root_count = std::abs(l_sign_start - l_sign_end);
		T l_temp = p_start;
		if (l_root_count == 0)	return false;
		while (l_root_count > 1)
		{
			l_temp = (p_start + p_end) * 0.5;
			//if (std::abs(p_p3.eval(l_temp)) < VF_MATH_EPSILON) return l_temp;
			//std::cout << l_temp << std::endl;
			int l_sign = sturm_sign_changed_6<double>(p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, l_temp);
			//std::cout << l_root_count << " " << l_temp <<  std::endl;
			if (std::abs(l_sign_start - l_sign) == 0)
			{
				p_start = l_temp;
				l_sign_start = l_sign;
				l_root_count = std::abs(l_sign_start - l_sign_end);
				continue;
			}
			p_end = l_temp;
			l_sign_end = l_sign;
			l_root_count = std::abs(l_sign_start - l_sign_end);
		}
		return true;
	}
	template<>
	int32_t vufPolinomCoeff<double, 6>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res, double p_precision)
	{
		if (p_res.size() < 6) p_res.resize(6);
		vufPolinomCoeff<double, 5> l_p5;
		vufPolinomCoeff<double, 4> l_p4;
		vufPolinomCoeff<double, 3> l_p3;
		vufPolinomCoeff<double, 2> l_p2;
		vufPolinomCoeff<double, 1> l_p1;
		vufPolinomCoeff<double, 0> l_p0;
		sturm_chain_6<double>(*this, l_p5, l_p4, l_p3, l_p2, l_p1, l_p0);
		int l_root_count = sturm_root_count_6<double>(*this, l_p5, l_p4, l_p3, l_p2, l_p1, l_p0, p_start, p_end);
		if (l_root_count == 0) return 0;
		double l_start = p_start;
		double l_end = p_end;
		for (int i = 0; i < l_root_count; ++i)
		{
			sturm_isolate_root_6<double>(*this, l_p5, l_p4, l_p3, l_p2, l_p1, l_p0, l_start, l_end);
			double l_s = l_start;
			double l_e = l_end;
			while ((l_e - l_s) > p_precision)
			{
				double l_middle = (l_s + l_e) * 0.5;
				if (sturm_root_count_6<double>(*this, l_p5, l_p4, l_p3, l_p2, l_p1, l_p0, l_s, l_middle) > 0)
				{
					l_e = l_middle;
					continue;
				}
				l_s = l_middle;
			}
			p_res[i] = l_s;
			l_start = l_end;
			l_end = p_end;
		}
		return l_root_count;
	}
#pragma endregion
#pragma region Sturm Chain Degree 7
	template<typename T>
	int sturm_chain_7(const	vufPolinomCoeff<T, 7>& p_p7,
							vufPolinomCoeff<T, 6>& p_p6,
							vufPolinomCoeff<T, 5>& p_p5,
							vufPolinomCoeff<T, 4>& p_p4,
							vufPolinomCoeff<T, 3>& p_p3,
							vufPolinomCoeff<T, 2>& p_p2,
							vufPolinomCoeff<T, 1>& p_p1,
							vufPolinomCoeff<T, 0>& p_p0)
	{
		vufPolinomCoeff<T, 1> l_res;
		vufPolinomCoeff<T, 1> l_p1;
		p_p6 = p_p7.get_derivative();
		p_p7.div(p_p6, l_res, p_p5); p_p5 = -p_p5; if (p_p5.get_degree() == 0) return 2;
		p_p6.div(p_p5, l_res, p_p4); p_p4 = -p_p4; if (p_p4.get_degree() == 0) return 3;
		p_p5.div(p_p4, l_res, p_p3); p_p3 = -p_p3; if (p_p3.get_degree() == 0) return 4;
		p_p4.div(p_p3, l_res, p_p2); p_p2 = -p_p2; if (p_p2.get_degree() == 0) return 5;
		p_p3.div(p_p2, l_res, p_p1); p_p1 = -p_p1; if (p_p1.get_degree() == 0) return 6;
		p_p2.div(p_p1, l_res, p_p0); p_p0 = -p_p0; if (p_p0.get_degree() == 0) return 7;
		return 8;
	}
	template<typename T>
	int sturm_sign_changed_7(	const	vufPolinomCoeff<T, 7>& p_p7,
								const	vufPolinomCoeff<T, 6>& p_p6,
								const	vufPolinomCoeff<T, 5>& p_p5,
								const	vufPolinomCoeff<T, 4>& p_p4,
								const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T p_t)
	{
		int l_root_count = 0;
		int l_sign_7 = VF_SIGN(p_p7.eval(p_t));
		int l_sign_6 = VF_SIGN(p_p6.eval(p_t));
		int l_sign_5 = VF_SIGN(p_p5.eval(p_t));
		int l_sign_4 = VF_SIGN(p_p4.eval(p_t));
		int l_sign_3 = VF_SIGN(p_p3.eval(p_t));
		int l_sign_2 = VF_SIGN(p_p2.eval(p_t));
		int l_sign_1 = VF_SIGN(p_p1.eval(p_t));
		int l_sign_0 = VF_SIGN(p_p0.eval(p_t));
		int l_sign_start_changed = 0;
		if (l_sign_7 * l_sign_6 < 0.0)  l_sign_start_changed++;
		if (l_sign_6 * l_sign_5 < 0.0)  l_sign_start_changed++;
		if (l_sign_5 * l_sign_4 < 0.0)  l_sign_start_changed++;
		if (l_sign_4 * l_sign_3 < 0.0)  l_sign_start_changed++;
		if (l_sign_3 * l_sign_2 < 0.0)  l_sign_start_changed++;
		if (l_sign_2 * l_sign_1 < 0.0)  l_sign_start_changed++;
		if (l_sign_1 * l_sign_0 < 0.0)  l_sign_start_changed++;
		//std::cout << "pt: " << p_t << " signes: " << l_sign_5 << l_sign_4 << l_sign_3 << " " << l_sign_2 << " " << l_sign_1 << " " << l_sign_0 << " " << l_sign_start_changed << std::endl;
		return l_sign_start_changed;
	}
	template<typename T>
	int sturm_root_count_7(	const	vufPolinomCoeff<T, 7>& p_p7,
							const	vufPolinomCoeff<T, 6>& p_p6,
							const	vufPolinomCoeff<T, 5>& p_p5,
							const	vufPolinomCoeff<T, 4>& p_p4,
							const	vufPolinomCoeff<T, 3>& p_p3,
							const	vufPolinomCoeff<T, 2>& p_p2,
							const	vufPolinomCoeff<T, 1>& p_p1,
							const	vufPolinomCoeff<T, 0>& p_p0,
							T p_start, T p_end)
	{
		int l_sign_start	= sturm_sign_changed_7<T>(p_p7, p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_7<T>(p_p7, p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_end);
		return std::abs(l_sign_start - l_sign_end);
	}
	template<typename T>
	bool sturm_isolate_root_7(	const	vufPolinomCoeff<T, 7>& p_p7,
								const	vufPolinomCoeff<T, 6>& p_p6,
								const	vufPolinomCoeff<T, 5>& p_p5,
								const	vufPolinomCoeff<T, 4>& p_p4,
								const	vufPolinomCoeff<T, 3>& p_p3,
								const	vufPolinomCoeff<T, 2>& p_p2,
								const	vufPolinomCoeff<T, 1>& p_p1,
								const	vufPolinomCoeff<T, 0>& p_p0,
								T& p_start, T& p_end)
	{
		//if (std::abs(p_p3.eval(p_end)) < VF_MATH_EPSILON) return p_end;
		int l_sign_start	= sturm_sign_changed_7<double>(p_p7, p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_start);
		int l_sign_end		= sturm_sign_changed_7<double>(p_p7, p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, p_end);
		int l_root_count = std::abs(l_sign_start - l_sign_end);
		T l_temp = p_start;
		if (l_root_count == 0)	return false;
		while (l_root_count > 1)
		{
			l_temp = (p_start + p_end) * 0.5;
			//if (std::abs(p_p3.eval(l_temp)) < VF_MATH_EPSILON) return l_temp;
			//std::cout << l_temp << std::endl;
			int l_sign = sturm_sign_changed_7<double>(p_p7, p_p6, p_p5, p_p4, p_p3, p_p2, p_p1, p_p0, l_temp);
			//std::cout << l_root_count << " " << l_temp <<  std::endl;
			if (std::abs(l_sign_start - l_sign) == 0)
			{
				p_start = l_temp;
				l_sign_start = l_sign;
				l_root_count = std::abs(l_sign_start - l_sign_end);
				continue;
			}
			p_end = l_temp;
			l_sign_end = l_sign;
			l_root_count = std::abs(l_sign_start - l_sign_end);
		}
		return true;
	}
	template<>
	int32_t vufPolinomCoeff<double, 7>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res, double p_precision)
	{
		if (p_res.size() < 6) p_res.resize(6);
		vufPolinomCoeff<double, 6> l_p6;
		vufPolinomCoeff<double, 5> l_p5;
		vufPolinomCoeff<double, 4> l_p4;
		vufPolinomCoeff<double, 3> l_p3;
		vufPolinomCoeff<double, 2> l_p2;
		vufPolinomCoeff<double, 1> l_p1;
		vufPolinomCoeff<double, 0> l_p0;
		sturm_chain_7<double>(*this, l_p6, l_p5, l_p4, l_p3, l_p2, l_p1, l_p0);
		int l_root_count = sturm_root_count_7<double>(*this, l_p6, l_p5, l_p4, l_p3, l_p2, l_p1, l_p0, p_start, p_end);
		if (l_root_count == 0) return 0;
		double l_start = p_start;
		double l_end = p_end;
		for (int i = 0; i < l_root_count; ++i)
		{
			sturm_isolate_root_7<double>(*this, l_p6, l_p5, l_p4, l_p3, l_p2, l_p1, l_p0, l_start, l_end);
			double l_s = l_start;
			double l_e = l_end;
			while ((l_e - l_s) > p_precision)
			{
				double l_middle = (l_s + l_e) * 0.5;
				if (sturm_root_count_7<double>(*this, l_p6, l_p5, l_p4, l_p3, l_p2, l_p1, l_p0, l_s, l_middle) > 0)
				{
					l_e = l_middle;
					continue;
				}
				l_s = l_middle;
			}
			p_res[i] = l_s;
			l_start = l_end;
			l_end = p_end;
		}
		return l_root_count;
	}
#pragma endregion
#pragma region Sturm Chain Degree 8

#pragma endregion
#pragma region Sturm Chain Degree 9

#pragma endregion

}
#endif //!VF_MATH_POLINOM_H