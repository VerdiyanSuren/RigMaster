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
			for (uint32_t i = MAX_POLYNOM_DEGREE; i > 0 + 1; --i)
			{
				if (a[i] != 0)
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
		bool div(const vufPolinomCoeff<T, OTHER_DEGREE>& p_divider,
			vufPolinomCoeff<T, MAX_POLYNOM_DEGREE - OTHER_DEGREE>& p_res,
			vufPolinomCoeff<T, OTHER_DEGREE - 1>& p_reminder)
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
			p_res.a[l_mult_degree] = a[l_main_degree] / p_divider.a[l_divider_degree];
			l_temp = sub(p_res.mult(p_divider));
			//std::cout << "1:l_mult_degree " << l_mult_degree << std::endl;
			//std::cout << "1:RES   " << p_res.to_string() << std::endl;
			//std::cout << "1:REM   " << l_temp.to_string() << std::endl;
			//std::cout << "1:CHECK " << (p_divider.mult(p_res).add(l_temp)).to_string() << std::endl << std::endl;

			while (true)
			{
				l_main_degree = l_temp.get_degree();

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

		int32_t find_root_on_interval(T p_start, T p_end, std::vector<T>& p_res)
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
			T l_sum = 0;
			for (int i = 0; i < MAX_POLYNOM_DEGREE + 1; ++i)
			{
				l_sum += a[i] * pow(t, i);
			}
			return l_sum;
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
	int32_t vufPolinomCoeff<double, 1>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res)
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
	int32_t vufPolinomCoeff<double, 2>::find_root_on_interval(double p_start, double p_end, std::vector<double>& p_res)
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

}
#endif //!VF_MATH_POLINOM_H