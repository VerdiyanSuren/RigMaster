#ifndef VF_MATH_MATRIX_H
#define VF_MATH_MATRIX_H

#include <random>
#include <iostream>
#include <new>
#include <math/vufVector.h>
#include <math/vufQuaternion.h>
#include <vufObject.h>
#include <vufMathInclude.h>
#include <vufObjectArrayFn.h>

#ifndef vufMatrix4_kTol
	#define vufMatrix4_kTol 1.0e-10
#endif


namespace vufMath
{
	template<typename T>	class vufVector3;
	template<typename T>	class vufVector4;

	//------------------------------------------------------------------------------------------------------------------------- 
	//	M A T R I X 2 
	//------------------------------------------------------------------------------------------------------------------------- 
	// Matrix 2x2
#pragma region VUF_MATRIX_2X2
	template<typename T>
	class vufMatrix2
	{
	public:
		T m_ptr[2][2] = { { 1., .0 },
						  { .0, 1.  } };
		vufMatrix2() {}
		vufMatrix2(const vufMatrix2& p_other)
		{
			m_ptr[0][0] = p_other.m_ptr[0][0];
			m_ptr[0][1] = p_other.m_ptr[0][1];
			m_ptr[1][0] = p_other.m_ptr[1][0];
			m_ptr[1][1] = p_other.m_ptr[1][1];
		}
		vufMatrix2(const T p_other[4][4])
		{
			m_ptr[0][0] = p_other[0][0];
			m_ptr[0][1] = p_other[0][1];
			m_ptr[1][0] = p_other[1][0];
			m_ptr[1][1] = p_other[1][1];
		}
		~vufMatrix2() {}
		vufMatrix2& operator = (const vufMatrix2& p_other)
		{
			m_ptr[0][0] = p_other.m_ptr[0][0];
			m_ptr[0][1] = p_other.m_ptr[0][1];
			m_ptr[1][0] = p_other.m_ptr[1][0];
			m_ptr[1][1] = p_other.m_ptr[1][1];
			return *this;
		}
		T			operator()(unsigned int p_row, unsigned int p_col) const
		{
			return m_ptr[p_row][p_col];
		}
		const T* operator[](unsigned int p_row) const
		{
			return m_ptr[p_row];
		}
		static vufMatrix2 random_matrix()
		{
			vufMatrix2 l_matr;
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					l_matr.m_ptr[i][j] = (T)(rand()) / (T)(RAND_MAX);
				}
			}
			return l_matr;
		}
		vufMatrix2 transpose() const
		{
			vufMatrix2 l_matr;
			l_matr.m_ptr[0][0] = m_ptr[0][0];
			l_matr.m_ptr[1][0] = m_ptr[0][1];
			l_matr.m_ptr[0][1] = m_ptr[1][0];
			l_matr.m_ptr[1][1] = m_ptr[1][1];
			return l_matr;
		}
		vufMatrix2& transpose_in_place()
		{
			T l_a;

			l_a = m_ptr[1][0];
			m_ptr[1][0] = m_ptr[0][1];
			m_ptr[0][1] = l_a;

			return *this;
		}

		std::string		to_string() const
		{
			std::stringstream l_ss;
			l_ss.precision(64);
			l_ss << "[";
			for (int i = 0; i < 2; ++i)
			{
				l_ss << "(";
				for (int j = 0; j < 2; ++j)
				{
					l_ss << m_ptr[i][j];
					if (j != 1)
					{
						l_ss << ",";
					}
				}
				l_ss << ")";

				if (i != 1)
				{
					l_ss << ",";
				}
			}
			l_ss << "]";
			return l_ss.str();
		}
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
					if (p_str[l_str_pos] == '[' || p_str[l_str_pos] == ' ' || p_str[l_str_pos] == '\t'
						|| p_str[l_str_pos] == '(' || p_str[l_str_pos] == ')')
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
					for (int i = 0; i < 2; ++i)
					{
						for (int j = 0; j < 2; ++j)
						{
							m_ptr[i][j] = l_x[i * 2 + j];
						}
					}
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
		/** read vector from binary return new offset in buffer */
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

		vufMatrix2& operator+=(const vufMatrix2& p_right)
		{
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					m_ptr[i][j] += p_right.m_ptr[i][j];
				}
			}
			return *this;
		}
		vufMatrix2  operator+ (const vufMatrix2& p_right) const
		{
			vufMatrix2 l_matr;
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] + p_right.m_ptr[i][j];
				}
			}
			return l_matr;
		}
		vufMatrix2& operator-=(const vufMatrix2& p_right)
		{
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					m_ptr[i][j] -= p_right.m_ptr[i][j];
				}
			}
			return *this;

		}
		vufMatrix2  operator- (const vufMatrix2& p_right) const
		{
			vufMatrix2 l_matr;
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] - p_right.m_ptr[i][j];
				}
			}
			return l_matr;
		}
		vufMatrix2& operator*= (const vufMatrix2& p_right)
		{
			*this = *this * p_right;
			return *this;
		}
		vufMatrix2  operator*(const vufMatrix2& p_right) const
		{
			vufMatrix2 l_m;

			l_m.m_ptr[0][0] = m_ptr[0][0] * p_right.m_ptr[0][0] +
				m_ptr[0][1] * p_right.m_ptr[1][0];

			l_m.m_ptr[0][1] = m_ptr[0][0] * p_right.m_ptr[0][1] +
				m_ptr[0][1] * p_right.m_ptr[1][1];

			l_m.m_ptr[1][0] = m_ptr[1][0] * p_right.m_ptr[0][0] +
				m_ptr[1][1] * p_right.m_ptr[1][0];

			l_m.m_ptr[1][1] = m_ptr[1][0] * p_right.m_ptr[0][1] +
				m_ptr[1][1] * p_right.m_ptr[1][1];
			return l_m;

		}
		vufMatrix2& operator*=(T p_val)
		{
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					m_ptr[i][j] *= p_val;
				}
			}
			return *this;
		}
		vufMatrix2  operator* (T p_val) const
		{
			vufMatrix2 l_matr;
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] * p_val;
				}
			}
			return l_matr;
		}
		bool		operator==(const vufMatrix2& p_other) const
		{
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					if (m_ptr[i][j] != p_other.m_ptr[i][j])
					{
						return false;
					}
				}
			}
			return true;
		}
		bool		operator!=(const vufMatrix2& p_other) const
		{
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					if (m_ptr[i][j] != p_other.m_ptr[i][j])
					{
						return true;
					}
				}
			}
			return false;
		}

		friend std::ostream& operator<<(std::ostream& p_out, const vufMatrix2& p_m)
		{
			auto l_flags = p_out.flags();
			auto l_old_precision = p_out.precision(3);
			p_out << std::fixed;;
			p_out << "[ vufMatrix 2x2" << std::endl;
			for (int i = 0; i < 2; i++)
			{
				p_out << " [";
				for (int j = 0; j < 2; j++)
				{
					p_out << " " << p_m(i, j);
				}
				p_out << " ]" << std::endl;;
			}
			p_out << "]";
			p_out.precision(l_old_precision);
			p_out.flags(l_flags);
			return p_out;
		}
	};
#pragma endregion VUF_MATRIX_2X2
	//------------------------------------------------------------------------------------------------------------------------- 
	//	M A T R I X 3
	//------------------------------------------------------------------------------------------------------------------------- 
	// Matrix 3x3
#pragma region VUF_MATRIX_3X3
	template<typename T>
	class vufMatrix3
	{
	public:
		T m_ptr[3][3] = { { 1., .0, .0 },
							{ .0, 1., .0 },
							{ .0, .0, 1. } };
		vufMatrix3() {}
		vufMatrix3(const vufMatrix3& p_other)
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					m_ptr[i][j] = p_other.m_ptr[i][j];
				}
			}
		}
		vufMatrix3(const T p_other[4][4])
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					m_ptr[i][j] = p_other[i][j];
				}
			}
		}
		~vufMatrix3() {}
		vufMatrix3& operator = (const vufMatrix3& p_other)
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					m_ptr[i][j] = p_other.m_ptr[i][j];
				}
			}
			return *this;
		}
		T			operator()(unsigned int p_row, unsigned int p_col) const
		{
			return m_ptr[p_row][p_col];
		}
		const T* operator[](unsigned int p_row) const
		{
			return m_ptr[p_row];
		}
		static vufMatrix3 random_matrix()
		{
			vufMatrix3 l_matr;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					l_matr.m_ptr[i][j] = (T)(rand()) / (T)(RAND_MAX);
				}
			}
			return l_matr;
		}

		vufMatrix3 transpose2() const
		{
			vufMatrix3 l_math(*this);
			l_math.m_ptr[1][0] = m_ptr[0][1];
			l_math.m_ptr[0][1] = m_ptr[1][0];
			return l_math;
		}
		vufMatrix3& transpose2_in_place()
		{
			T l_a;
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					l_a = m_ptr[j][i];
					m_ptr[j][i] = m_ptr[i][j];
					m_ptr[i][j] = l_a;
				}
			}
			return *this;
		};
		vufMatrix3 transpose() const
		{
			vufMatrix3 l_matr;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[j][i];
				}
			}
			return l_matr;
		}
		vufMatrix3& transpose_in_place()
		{
			T l_a;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					l_a = m_ptr[i][j];
					m_ptr[i][j] = m_ptr[j][i];
					m_ptr[j][i] = l_a;
				}
			}
			return *this;
		}
		
		std::string		to_string() const
		{
			std::stringstream l_ss;
			l_ss.precision(64);
			l_ss << "[";
			for (int i = 0; i < 3; ++i)
			{
				l_ss << "(";
				for (int j = 0; j < 3; ++j)
				{
					l_ss << m_ptr[i][j];
					if (j != 2)
					{
						l_ss << ",";
					}
				}
				l_ss << ")";

				if (i != 2)
				{
					l_ss << ",";
				}
			}
			l_ss << "]";
			return l_ss.str();
		}
		uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			try
			{
				T l_x[9];
				std::stringstream l_ss;
				uint64_t l_index = 0;
				uint64_t l_str_pos = p_offset;
				for (uint64_t i = p_offset; i < p_str.size(); ++i)
				{
					l_str_pos = i;
					if (p_str[l_str_pos] == '[' || p_str[l_str_pos] == ' ' || p_str[l_str_pos] == '\t'
						|| p_str[l_str_pos] == '(' || p_str[l_str_pos] == ')')
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
				if (l_index == 9)
				{
					for (int i = 0; i < 3; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							m_ptr[i][j] = l_x[i * 3 + j];
						}
					}
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

			p_buff.resize(l_old_buff_sz + 9 * sizeof(T));

			for (uint64_t i = l_old_buff_sz; i < p_buff.size(); ++i)
			{
				p_buff[i] = l_x[l_writen_size++];
			}
			return l_writen_size;
		}
		/** read vector from binary return size of readed */
		uint64_t		from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)
		{
			if (p_buff.size() < p_offset + 9 * sizeof(T))
			{
				return 0;
			}
			unsigned char* l_x = (unsigned char*)this;
			uint64_t l_read_size = 0;
			for (uint64_t i = p_offset; i < p_offset + 9 * sizeof(T); ++i)
			{
				l_x[l_read_size++] = p_buff[i];
			}
			return p_offset + l_read_size;
		}

		vufMatrix3& operator+=(const vufMatrix3& p_right)
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					m_ptr[i][j] += p_right.m_ptr[i][j];
				}
			}
			return *this;
		}
		vufMatrix3  operator+ (const vufMatrix3& p_right) const
		{
			vufMatrix3 l_matr;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] + p_right.m_ptr[i][j];
				}
			}
			return l_matr;
		}
		vufMatrix3& operator-=(const vufMatrix3& p_right)
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					m_ptr[i][j] -= p_right.m_ptr[i][j];
				}
			}
			return *this;
		}
		vufMatrix3  operator- (const vufMatrix3& p_right) const
		{
			vufMatrix3 l_matr;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] - p_right.m_ptr[i][j];
				}
			}
			return l_matr;
		}
		vufMatrix3& operator*= (const vufMatrix3& p_right)
		{
			*this = *this * p_right;
			return *this;
		}
		vufMatrix3  operator*(const vufMatrix3& p_right) const
		{
			vufMatrix3 l_m;

			l_m.m_ptr[0][0] = m_ptr[0][0] * p_right.m_ptr[0][0] +
				m_ptr[0][1] * p_right.m_ptr[1][0] +
				m_ptr[0][2] * p_right.m_ptr[2][0];
				
			l_m.m_ptr[0][1] = m_ptr[0][0] * p_right.m_ptr[0][1] +
				m_ptr[0][1] * p_right.m_ptr[1][1] +
				m_ptr[0][2] * p_right.m_ptr[2][1];
				
			l_m.m_ptr[0][2] = m_ptr[0][0] * p_right.m_ptr[0][2] +
				m_ptr[0][1] * p_right.m_ptr[1][2] +
				m_ptr[0][2] * p_right.m_ptr[2][2];
						
			l_m.m_ptr[1][0] = m_ptr[1][0] * p_right.m_ptr[0][0] +
				m_ptr[1][1] * p_right.m_ptr[1][0] +
				m_ptr[1][2] * p_right.m_ptr[2][0];

			l_m.m_ptr[1][1] = m_ptr[1][0] * p_right.m_ptr[0][1] +
				m_ptr[1][1] * p_right.m_ptr[1][1] +
				m_ptr[1][2] * p_right.m_ptr[2][1];

			l_m.m_ptr[1][2] = m_ptr[1][0] * p_right.m_ptr[0][2] +
				m_ptr[1][1] * p_right.m_ptr[1][2] +
				m_ptr[1][2] * p_right.m_ptr[2][2];

			l_m.m_ptr[2][0] = m_ptr[2][0] * p_right.m_ptr[0][0] +
				m_ptr[2][1] * p_right.m_ptr[1][0] +
				m_ptr[2][2] * p_right.m_ptr[2][0];

			l_m.m_ptr[2][1] = m_ptr[2][0] * p_right.m_ptr[0][1] +
				m_ptr[2][1] * p_right.m_ptr[1][1] +
				m_ptr[2][2] * p_right.m_ptr[2][1];

			l_m.m_ptr[2][2] = m_ptr[2][0] * p_right.m_ptr[0][2] +
				m_ptr[2][1] * p_right.m_ptr[1][2] +
				m_ptr[2][2] * p_right.m_ptr[2][2];

			l_m.m_ptr[2][3] = m_ptr[2][0] * p_right.m_ptr[0][3] +
				m_ptr[2][1] * p_right.m_ptr[1][3] +
				m_ptr[2][2] * p_right.m_ptr[2][3];

			return l_m;
		}
		vufMatrix3& operator*=(T p_val)
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					m_ptr[i][j] *= p_val;
				}
			}
			return *this;
		}
		vufMatrix3 operator* (T p_val) const
		{
			vufMatrix3 l_matr;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] * p_val;
				}
			}
			return l_matr;
		}
		bool		operator==(const vufMatrix3& p_other) const
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					if (m_ptr[i][j] != p_other.m_ptr[i][j])
					{
						return false;
					}
				}
			}
			return true;
		}
		bool		operator!=(const vufMatrix3& p_other) const
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					if (m_ptr[i][j] != p_other.m_ptr[i][j])
					{
						return true;
					}
				}
			}
			return false;
		}

		friend std::ostream& operator<<(std::ostream& p_out, const vufMatrix3& p_m)
		{
			auto l_flags = p_out.flags();
			auto l_old_precision = p_out.precision(3);
			p_out << std::fixed;;
			p_out << "[ vufMatrix 3x3" << std::endl;
			for (int i = 0; i < 4; i++)
			{
				p_out << " [";
				for (int j = 0; j < 4; j++)
				{
					p_out << " " << p_m(i, j);
				}
				p_out << " ]" << std::endl;;
			}
			p_out << "]";
			p_out.precision(l_old_precision);
			p_out.flags(l_flags);
			return p_out;
		}
	};
#pragma endregion VUF_MATRIX_3X3
	//------------------------------------------------------------------------------------------------------------------------- 
	//	M A T R I X 4
	//------------------------------------------------------------------------------------------------------------------------- 
	// Matrix 4x4
#pragma region VUF_MATRIX_4X4
	template<typename T>
	class vufMatrix4
	{
	public:
		T m_ptr[4][4] = { { 1., .0, .0, .0 },
							{ .0, 1., .0, .0 },
							{ .0, .0, 1., .0 },
							{ .0, .0, .0, 1. } };
		vufMatrix4() {}
		vufMatrix4(const vufMatrix4& p_other)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_ptr[i][j] = p_other.m_ptr[i][j];
				}
			}
		}
		vufMatrix4(const T p_other[4][4])
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_ptr[i][j] = p_other[i][j];
				}
			}
		}
		vufMatrix4(const T p_other[16])
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_ptr[i][j] = p_other[i*4 + j];
				}
			}
		}

		~vufMatrix4() {}
		vufMatrix4& operator = (const vufMatrix4& p_other)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_ptr[i][j] = p_other.m_ptr[i][j];
				}
			}
			return *this;
		}
		T			operator()(unsigned int p_row, unsigned int p_col) const
		{
			return m_ptr[p_row][p_col];
		}
		const T*	operator[](unsigned int p_row) const
		{
			return m_ptr[p_row];
		}
		T* operator[](unsigned int p_row)
		{
			return m_ptr[p_row];
		}
		vufVector4<T>& get_row(unsigned int p_row)
		{ 
			return *( (vufVector4<T>*)m_ptr[p_row] ); 
		}
		void set_row(unsigned int p_row, const vufVector4<T>& p_vec)
		{
			*((vufVector4<T>*)m_ptr[p_row]) = p_vec;
		}
		static vufMatrix4 random_matrix()
		{
			vufMatrix4 l_matr; 
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					l_matr.m_ptr[i][j] = (T)(rand()) / (T)(RAND_MAX);
				}
			}
			return l_matr;
		}
		static vufMatrix4 numerate_matrix(T p_start_val = 0, T p_step = 1)
		{
			vufMatrix4 l_matr;
			T l_count = (T)p_start_val;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					l_matr.m_ptr[i][j] = l_count;
					l_count += p_step;
				}
			}
			return l_matr;
		}

		vufMatrix4		get_transposed_3() const
		{
			vufMatrix4 l_math(*this);
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					l_math.m_ptr[j][i] = m_ptr[i][j];
				}
			}
			return l_math;
		}
		vufMatrix4&		transpose_3_in_place()
		{
			T l_a;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = i; j < 3; ++j)
				{
					l_a = m_ptr[j][i];
					m_ptr[j][i] = m_ptr[i][j];
					m_ptr[i][j] = l_a;
				}
			}
			return *this;
		}
		vufMatrix4		get_transposed_4() const
		{
			vufMatrix4 l_matr;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[j][i];
				}
			}
			return l_matr;
		}
		vufMatrix4&		transpose_4_in_place()
		{
			T l_a;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = i; j < 4; ++j)
				{
					l_a = m_ptr[i][j];
					m_ptr[i][j] = m_ptr[j][i];
					m_ptr[j][i] = l_a;
				}
			}
			return *this;
		}
		vufMatrix4&		set_to_identity_in_place()
		{
			new (this) vufMatrix4<T>();
			return *this;
		}
		T				determinant3() const
		{
			return 
				m_ptr[0][0] * (	m_ptr[1][1] * m_ptr[2][2] -	m_ptr[1][2] * m_ptr[2][1] ) -	
				m_ptr[0][1] *  (m_ptr[1][0] * m_ptr[2][2] - m_ptr[1][2] * m_ptr[2][0] ) +
				m_ptr[0][2] * ( m_ptr[1][0] * m_ptr[2][1] - m_ptr[1][1] * m_ptr[2][0]);
		}
		T				determinant() const
		{
			return
				m_ptr[0][0] * (	m_ptr[1][1] * m_ptr[2][2] * m_ptr[3][3] +	//123 123
								m_ptr[2][1] * m_ptr[3][2] * m_ptr[1][3] +	//231 123
								m_ptr[3][1] * m_ptr[1][2] * m_ptr[2][3] -	//312 123
								
								m_ptr[3][1] * m_ptr[2][2] * m_ptr[1][3] -	//321 123
								m_ptr[2][1] * m_ptr[1][2] * m_ptr[3][3] -	//213 123
								m_ptr[1][1] * m_ptr[3][2] * m_ptr[2][3] ) -	//132 123

				m_ptr[0][1] * ( m_ptr[1][0] * m_ptr[2][2] * m_ptr[3][3] +	//123 023 
								m_ptr[2][0] * m_ptr[3][2] * m_ptr[1][3] +	//231 023 
								m_ptr[3][0] * m_ptr[1][2] * m_ptr[2][3] -	//312 023

								m_ptr[3][0] * m_ptr[2][2] * m_ptr[1][3] -	//321 023
								m_ptr[2][0] * m_ptr[1][2] * m_ptr[3][3] -	//213 023 
								m_ptr[1][0] * m_ptr[3][2] * m_ptr[2][3]) +	//132 023

				m_ptr[0][2] * ( m_ptr[1][0] * m_ptr[2][1] * m_ptr[3][3] +	//123 013
								m_ptr[2][0] * m_ptr[3][1] * m_ptr[1][3] +	//231 013
								m_ptr[3][0] * m_ptr[1][1] * m_ptr[2][3] -	//312 013

								m_ptr[3][0] * m_ptr[2][1] * m_ptr[1][3] -	//321 013
								m_ptr[2][0] * m_ptr[1][1] * m_ptr[3][3] -	//213 013
								m_ptr[1][0] * m_ptr[3][1] * m_ptr[2][3]) -	//132 013

				m_ptr[0][3] * (	m_ptr[1][0] * m_ptr[2][1] * m_ptr[3][2] +	//123 012
								m_ptr[2][0] * m_ptr[3][1] * m_ptr[1][2] +	//231 012
								m_ptr[3][0] * m_ptr[1][1] * m_ptr[2][2] -	//312 012

								m_ptr[3][0] * m_ptr[2][1] * m_ptr[1][2] -	//321 012
								m_ptr[2][0] * m_ptr[1][1] * m_ptr[3][2] -	//213 012
								m_ptr[1][0] * m_ptr[3][1] * m_ptr[2][2]);	//132 012

		}
		bool			is_singular() const
		{
			if (abs(determinant()) > VF_MATH_EPSILON)
			{
				return false;
			}
			return true;
		}
		vufMatrix4		get_orthogonalized_3() const
		{
			vufMatrix4 l_matr(*this);
			l_matr.orthogonalize_3_in_place();
			return l_matr;
		}
		vufMatrix4&		orthogonalize_3_in_place()
		{
			T l_length = sqrt( m_ptr[0][0] * m_ptr[0][0] + m_ptr[0][1] * m_ptr[0][1] + m_ptr[0][2] * m_ptr[0][2] );
			if (l_length == 0)
			{
				return *this;
			}
			//x axis
			m_ptr[0][0] /= l_length;
			m_ptr[0][1] /= l_length;
			m_ptr[0][2] /= l_length;
			
			//y axis by x axis
			T l_dot_xy = m_ptr[0][0] * m_ptr[1][0] + m_ptr[0][1] * m_ptr[1][1] + m_ptr[0][2] * m_ptr[1][2];
			
			m_ptr[1][0] -= l_dot_xy * m_ptr[0][0];
			m_ptr[1][1] -= l_dot_xy * m_ptr[0][1];
			m_ptr[1][2] -= l_dot_xy * m_ptr[0][2];
			l_length = sqrt(m_ptr[1][0] * m_ptr[1][0] + m_ptr[1][1] * m_ptr[1][1] + m_ptr[1][2] * m_ptr[1][2]);
			if (l_length == 0)
			{
				return *this;
			}
			m_ptr[1][0] /= l_length;
			m_ptr[1][1] /= l_length;
			m_ptr[1][2] /= l_length;
			
			//z axis by y axis
			T l_dot_yz = m_ptr[2][0] * m_ptr[1][0] + m_ptr[2][1] * m_ptr[1][1] + m_ptr[2][2] * m_ptr[1][2];
			T l_dot_xz = m_ptr[2][0] * m_ptr[0][0] + m_ptr[2][1] * m_ptr[0][1] + m_ptr[2][2] * m_ptr[0][2];
			m_ptr[2][0] -= l_dot_yz * m_ptr[1][0];
			m_ptr[2][1] -= l_dot_yz * m_ptr[1][1];
			m_ptr[2][2] -= l_dot_yz * m_ptr[1][2];
			// z axis by x axis			
			m_ptr[2][0] -= l_dot_xz * m_ptr[0][0];
			m_ptr[2][1] -= l_dot_xz * m_ptr[0][1];
			m_ptr[2][2] -= l_dot_xz * m_ptr[0][2];
			

			l_length = sqrt(m_ptr[2][0] * m_ptr[2][0] + m_ptr[2][1] * m_ptr[2][1] + m_ptr[2][2] * m_ptr[2][2]);
			if (l_length == 0)
			{
				return *this;
			}
			m_ptr[2][0] /= l_length;
			m_ptr[2][1] /= l_length;
			m_ptr[2][2] /= l_length;

			return *this;
		}
		vufMatrix4		get_orthogonalized_4() const
		{
			vufMatrix4 l_matr(*this);
			l_matr.orthogonalize_4_in_place();
			return l_matr;
		}
		vufMatrix4&		orthogonalize_4_in_place()
		{
			T l_length = sqrt(m_ptr[0][0] * m_ptr[0][0] + m_ptr[0][1] * m_ptr[0][1] + m_ptr[0][2] * m_ptr[0][2] + m_ptr[0][3] * m_ptr[0][3]);
			if (l_length == 0)
			{
				return *this;
			}
			//x axis
			m_ptr[0][0] /= l_length;
			m_ptr[0][1] /= l_length;
			m_ptr[0][2] /= l_length;
			m_ptr[0][3] /= l_length;

			//y axis by x axis
			T l_dot_xy = m_ptr[0][0] * m_ptr[1][0] + m_ptr[0][1] * m_ptr[1][1] + m_ptr[0][2] * m_ptr[1][2] + m_ptr[0][3] * m_ptr[1][3];
			m_ptr[1][0] -= l_dot_xy * m_ptr[0][0];
			m_ptr[1][1] -= l_dot_xy * m_ptr[0][1];
			m_ptr[1][2] -= l_dot_xy * m_ptr[0][2];
			m_ptr[1][3] -= l_dot_xy * m_ptr[0][3];
			l_length = sqrt(m_ptr[1][0] * m_ptr[1][0] + m_ptr[1][1] * m_ptr[1][1] + m_ptr[1][2] * m_ptr[1][2] + m_ptr[1][3] * m_ptr[1][3]);
			if (l_length == 0)
			{
				return *this;
			}
			m_ptr[1][0] /= l_length;
			m_ptr[1][1] /= l_length;
			m_ptr[1][2] /= l_length;
			m_ptr[1][3] /= l_length;




			T l_dot_xz = m_ptr[2][0] * m_ptr[0][0] + m_ptr[2][1] * m_ptr[0][1] + m_ptr[2][2] * m_ptr[0][2] + m_ptr[2][3] * m_ptr[0][3];
			T l_dot_yz = m_ptr[2][0] * m_ptr[1][0] + m_ptr[2][1] * m_ptr[1][1] + m_ptr[2][2] * m_ptr[1][2] + m_ptr[2][3] * m_ptr[1][3];
			//z axis by y
			m_ptr[2][0] -= l_dot_yz * m_ptr[1][0];
			m_ptr[2][1] -= l_dot_yz * m_ptr[1][1];
			m_ptr[2][2] -= l_dot_yz * m_ptr[1][2];
			m_ptr[2][3] -= l_dot_yz * m_ptr[1][3];
			// z axis by x
			m_ptr[2][0] -= l_dot_xz * m_ptr[0][0];
			m_ptr[2][1] -= l_dot_xz * m_ptr[0][1];
			m_ptr[2][2] -= l_dot_xz * m_ptr[0][2];
			m_ptr[2][3] -= l_dot_xz * m_ptr[0][3];

			l_length = sqrt(m_ptr[2][0] * m_ptr[2][0] + m_ptr[2][1] * m_ptr[2][1] + m_ptr[2][2] * m_ptr[2][2] + m_ptr[2][3] * m_ptr[2][3]);
			if (l_length == 0)
			{
				return *this;
			}
			m_ptr[2][0] /= l_length;
			m_ptr[2][1] /= l_length;
			m_ptr[2][2] /= l_length;
			m_ptr[2][3] /= l_length;




			T l_dot_xw = m_ptr[0][0] * m_ptr[3][0] + m_ptr[0][1] * m_ptr[3][1] + m_ptr[0][2] * m_ptr[3][2] + m_ptr[0][3] * m_ptr[3][3];
			T l_dot_yw = m_ptr[1][0] * m_ptr[3][0] + m_ptr[1][1] * m_ptr[3][1] + m_ptr[1][2] * m_ptr[3][2] + m_ptr[1][3] * m_ptr[3][3];
			T l_dot_zw = m_ptr[2][0] * m_ptr[3][0] + m_ptr[2][1] * m_ptr[3][1] + m_ptr[2][2] * m_ptr[3][2] + m_ptr[2][3] * m_ptr[3][3];
			
			// w axis by z axis
			m_ptr[3][0] -= l_dot_zw * m_ptr[2][0];
			m_ptr[3][1] -= l_dot_zw * m_ptr[2][1];
			m_ptr[3][2] -= l_dot_zw * m_ptr[2][2];
			m_ptr[3][3] -= l_dot_zw * m_ptr[2][3];
			// w axis by y axis
			m_ptr[3][0] -= l_dot_yw * m_ptr[1][0];
			m_ptr[3][1] -= l_dot_yw * m_ptr[1][1];
			m_ptr[3][2] -= l_dot_yw * m_ptr[1][2];
			m_ptr[3][3] -= l_dot_yw * m_ptr[1][3];
			
			// w axis by x asis
			//std::cout << "l_dot_xw " << l_dot_xw << std::endl;
			m_ptr[3][0] -= l_dot_xw * m_ptr[0][0];
			m_ptr[3][1] -= l_dot_xw * m_ptr[0][1];
			m_ptr[3][2] -= l_dot_xw * m_ptr[0][2];
			m_ptr[3][3] -= l_dot_xw * m_ptr[0][3];
			

			l_length = sqrt(m_ptr[3][0] * m_ptr[3][0] + m_ptr[3][1] * m_ptr[3][1] + m_ptr[3][2] * m_ptr[3][2] + m_ptr[3][3] * m_ptr[3][3]);
			if (l_length == 0)
			{
				return *this;
			}
			m_ptr[3][0] /= l_length;
			m_ptr[3][1] /= l_length;
			m_ptr[3][2] /= l_length;
			m_ptr[3][3] /= l_length;

			return *this;
		}
		vufMatrix4		get_reseted_rotate_and_scale() const
		{
			vufMatrix4 l_matr(*this);
			return l_matr.reset_rotate_and_scale_in_place();
		}
		vufMatrix4&		reset_rotate_and_scale_in_place()
		{
			m_ptr[0][0] = 1.0; m_ptr[0][1] = 0.0; m_ptr[0][2] = 0.0;
			m_ptr[1][0] = 0.0; m_ptr[1][1] = 1.0; m_ptr[1][2] = 0.0;
			m_ptr[2][0] = 0.0; m_ptr[2][1] = 0.0; m_ptr[2][2] = 1.0;
			return *this;
		}
		vufVector3<T>	get_translation_3() const
		{
			return vufVector3<T>(m_ptr[3][0], m_ptr[3][1], m_ptr[3][2]);
		}
		vufVector4<T>	get_translation_4() const
		{
			return vufVector4<T>(m_ptr[3][0], m_ptr[3][1], m_ptr[3][2],1.0);
		}
		vufVector3<T>	get_axis_x_3() const
		{
			return vufVector3<T>(m_ptr[0][0], m_ptr[0][1], m_ptr[0][2]);
		}
		vufVector4<T>	get_axis_x_4() const
		{
			return vufVector4<T>(m_ptr[0][0], m_ptr[0][1], m_ptr[0][2],1.0);
		}
		vufVector3<T>	get_axis_y_3() const
		{
			return vufVector3<T>(m_ptr[1][0], m_ptr[1][1], m_ptr[1][2]);
		}
		vufVector4<T>	get_axis_y_4() const
		{
			return vufVector4<T>(m_ptr[1][0], m_ptr[1][1], m_ptr[1][2],1.0);
		}
		vufVector3<T>	get_axis_z_3() const
		{
			return vufVector3<T>(m_ptr[2][0], m_ptr[2][1], m_ptr[2][2]);
		}
		vufVector4<T>	get_axis_z_4() const
		{
			return vufVector4<T>(m_ptr[3][0], m_ptr[3][1], m_ptr[3][2],1.0);
		}
		T				get_scale_x() const
		{
			return sqrt(m_ptr[0][0] * m_ptr[0][0] + m_ptr[0][1] * m_ptr[0][1] + m_ptr[0][2] * m_ptr[0][2]);
		}
		T				get_scale_y() const
		{
			return sqrt(m_ptr[1][0] * m_ptr[1][0] + m_ptr[1][1] * m_ptr[1][1] + m_ptr[1][2] * m_ptr[1][2]);
		}
		T				get_scale_z() const
		{
			auto sgn = (get_axis_x_3().cross_in_place(get_axis_y_3())).dot(get_axis_z_3()) < 0.0;
			return sgn = true ?  sqrt( m_ptr[2][0] * m_ptr[2][0] + m_ptr[2][1] * m_ptr[2][1] + m_ptr[2][2] * m_ptr[2][2] ) :
								-sqrt( m_ptr[2][0] * m_ptr[2][0] + m_ptr[2][1] * m_ptr[2][1] + m_ptr[2][2] * m_ptr[2][2] );
		}
		vufQuaternion<T> get_quaternion() const
		{
			//has problem with negative scale
			vufQuaternion<T> l_q;
			vufVector3<T> l_axis_x = get_axis_x_3();
			vufVector3<T> l_axis_y = get_axis_y_3();

			l_axis_x.normalize_in_place();
			l_axis_y.make_ortho_to_in_place(l_axis_x);
			l_axis_y.normalize_in_place();

			vufVector3<T> l_axis_z = l_axis_x.get_cross(l_axis_y);
			//zAxis.makeOrthoToInPlace(xAxis);
			//zAxis.makeOrthoToInPlace(yAxis);
			l_axis_z.normalize_in_place();
			double l_s;
			double  l_chk = l_axis_x.x + l_axis_y.y + l_axis_z.z;
			if (l_chk > .0)
			{
				l_s = sqrt(1. + l_chk) * 2.;
				l_q.w = l_s * 0.25;
				l_q.x = (l_axis_z.y - l_axis_y.z) / l_s;
				l_q.y = (l_axis_x.z - l_axis_z.x) / l_s;
				l_q.z = (l_axis_y.x - l_axis_x.y) / l_s;
				return l_q;
			}
			if ((l_axis_x.x > l_axis_y.y) && (l_axis_x.x > l_axis_z.z))
			{
				l_s = sqrt(1. + l_axis_x.x - l_axis_y.y - l_axis_z.z) * 2.;
				l_q.w = (l_axis_z.y - l_axis_y.z) / l_s;
				l_q.x = l_s * 0.25;
				l_q.y = (l_axis_x.y + l_axis_y.x) / l_s;
				l_q.z = (l_axis_x.z + l_axis_z.x) / l_s;
				//std::cout << "--------------------HERE- 2 ----------------\n";
				return l_q;
			}
			if (l_axis_y.y > l_axis_z.z)
			{
				l_s = sqrt(1. + l_axis_y.y - l_axis_x.x - l_axis_z.z) * 2.;
				l_q.w = (l_axis_x.z - l_axis_z.x) / l_s;
				l_q.x = (l_axis_x.y + l_axis_y.x) / l_s;
				l_q.y = l_s * 0.25;
				l_q.z = (l_axis_y.z + l_axis_z.y) / l_s;
				//std::cout << "--------------------HERE- 3 ----------------\n";
				return l_q;
			}
			l_s = sqrt(1. + l_axis_z.z - l_axis_x.x - l_axis_y.y) * 2.;
			l_q.w = (l_axis_y.x - l_axis_x.y) / l_s;
			l_q.x = (l_axis_x.z + l_axis_z.x) / l_s;
			l_q.y = (l_axis_y.z + l_axis_z.y) / l_s;
			l_q.z = 0.25 * l_s;
			//std::cout << "--------------------HERE- 4 ----------------\n";
			return l_q;
		}
		vufVector3<T>	get_euler_xyz() const
		{
			vufMatrix4 l_matr(*this);
			l_matr.set_scale_x(1.0);
			l_matr.set_scale_y(1.0);
			l_matr.set_scale_z(1.0);
			
			
			T l_x, l_y, l_z;
			if (l_matr[2][0] <= -1.)
			{
				l_z = 0;
				l_y = VF_MATH_PI * 0.5;
				l_x = atan(-l_matr[0][1]);
				return vufVector3<T>(l_x, l_y, l_z);
			}
			if (l_matr[2][0] >= 1.)
			{
				l_z = 0;
				l_y = -VF_MATH_PI * 0.5;
				l_x = asin(VF_CLAMP(-1, 1, -l_matr[0][1]));
				return vufVector3<T>(l_x, l_y, l_z);
			}
			T l_sin_y	= VF_CLAMP(-1.,1.,-l_matr[2][0]);
			l_y			= -asin(l_sin_y);
			T l_cos_y	= cos(l_y);
			l_x = atan(l_matr[2][1] / l_cos_y);
			l_z = atan(l_matr[1][0] / l_cos_y);

			return vufVector3<T>(l_x, l_y, l_z);
		}

		void			set_translation(const vufVector3<T>& p_v)
		{
			m_ptr[3][0] = p_v.x;
			m_ptr[3][1] = p_v.y;
			m_ptr[3][2] = p_v.z;
			m_ptr[3][3] = 1.;
		}		
		void			set_translation(const vufVector4<T>& p_v)
		{
			m_ptr[3][0] = p_v.x;
			m_ptr[3][1] = p_v.y;
			m_ptr[3][2] = p_v.z;
			m_ptr[3][3] = 1.;
		}
		void			set_axis_x(const vufVector3<T>& p_v)
		{
			m_ptr[0][0] = p_v.x;
			m_ptr[0][1] = p_v.y;
			m_ptr[0][2] = p_v.z;
			m_ptr[0][3] = .0;
		}
		void			set_axis_x(const vufVector4<T>& p_v)
		{
			m_ptr[0][0] = p_v.x;
			m_ptr[0][1] = p_v.y;
			m_ptr[0][2] = p_v.z;
			m_ptr[0][3] = .0;
		}
		void			set_axis_y(const vufVector3<T>& p_v)
		{
			m_ptr[1][0] = p_v.x;
			m_ptr[1][1] = p_v.y;
			m_ptr[1][2] = p_v.z;
			m_ptr[1][3] = .0;
		}
		void			set_axis_y(const vufVector4<T>& p_v)
		{
			m_ptr[1][0] = p_v.x;
			m_ptr[1][1] = p_v.y;
			m_ptr[1][2] = p_v.z;
			m_ptr[1][3] = .0;
		}
		void			set_axis_z(const vufVector3<T>& p_v)
		{
			m_ptr[2][0] = p_v.x;
			m_ptr[2][1] = p_v.y;
			m_ptr[2][2] = p_v.z;
			m_ptr[2][3] = .0;
		}
		void			set_axis_z(const vufVector4<T>& p_v)
		{
			m_ptr[2][0] = p_v.x;
			m_ptr[2][1] = p_v.y;
			m_ptr[2][2] = p_v.z;
			m_ptr[2][3] = .0;
		}
		void			set_scale_x(T p_s)
		{
			T l_length = sqrt(	m_ptr[0][0] * m_ptr[0][0] +
								m_ptr[0][1] * m_ptr[0][1] +
								m_ptr[0][2] * m_ptr[0][2]);
			
			if (l_length == 0)
			{
				return;
			}
			p_s /= l_length;
			m_ptr[0][0] *= p_s;
			m_ptr[0][1] *= p_s;
			m_ptr[0][2] *= p_s;
		}
		void			set_scale_y(double p_s)
		{
			T l_length = sqrt(	m_ptr[1][0] * m_ptr[1][0] +
								m_ptr[1][1] * m_ptr[1][1] +
								m_ptr[1][2] * m_ptr[1][2]);

			if (l_length == 0)
			{
				return;
			}
			p_s /= l_length;
			m_ptr[1][0] *= p_s;
			m_ptr[1][1] *= p_s;
			m_ptr[1][2] *= p_s;
		}
		void			set_scale_z(double p_s)
		{
			T l_length = sqrt(	m_ptr[2][0] * m_ptr[2][0] +
								m_ptr[2][1] * m_ptr[2][1] +
								m_ptr[2][2] * m_ptr[2][2]);

			if (l_length == 0)
			{
				return;
			}
			p_s /= l_length;
			m_ptr[2][0] *= p_s;
			m_ptr[2][1] *= p_s;
			m_ptr[2][2] *= p_s;
		}
		void			set_quaternion(const vufQuaternion<T>& p_q)
		{
			vufVector3<T> l_axis_x( 1. - 2. * p_q.y * p_q.y - 2. * p_q.z * p_q.z,
										 2. * p_q.x * p_q.y - 2. * p_q.z * p_q.w,
										 2. * p_q.x * p_q.z + 2. * p_q.y * p_q.w);
			vufVector3<T> l_axis_y(		 2. * p_q.x * p_q.y + 2. * p_q.z * p_q.w,
									1. - 2. * p_q.x * p_q.x - 2. * p_q.z * p_q.z,
										 2. * p_q.y * p_q.z - 2. * p_q.x * p_q.w);
			vufVector3<T> l_axis_z(		 2. * p_q.x * p_q.z - 2. * p_q.y * p_q.w,
										 2. * p_q.y * p_q.z + 2. * p_q.x * p_q.w,
									1. - 2. * p_q.x * p_q.x - 2. * p_q.y * p_q.y);

			double l_scale_x = get_scale_x();
			double l_scale_y = get_scale_y();
			double l_scale_z = get_scale_z();

			l_axis_x.normalize_in_place();
			l_axis_x *= l_scale_x;
			l_axis_y.normalize_in_place();
			l_axis_y *= l_scale_y;
			l_axis_z.normalize_in_place();
			l_axis_z *= l_scale_z;
			
			set_axis_x(l_axis_x);
			set_axis_y(l_axis_y);
			set_axis_z(l_axis_z);
		}
		void			set_euler_xyz(T p_x, T p_y, T p_z)
		{
			double l_scale_x = get_scale_x();
			double l_scale_y = get_scale_y();
			double l_scale_z = get_scale_z();

			vufMatrix4 l_rx, l_ry, l_rz;
			l_rx[1][1] = cos(p_x);		l_rx[1][2] = -sin(p_x);
			l_rx[2][1] = -l_rx[1][2];	l_rx[2][2] = l_rx[1][1];

			l_ry[0][0] = cos(p_y);		l_ry[0][2] = sin(p_y);
			l_ry[2][0] = -l_ry[0][2];	l_ry[2][2] = l_ry[0][0];

			l_rz[0][0] = cos(p_z);		l_rz[0][1] = -sin(p_z);
			l_rz[1][0] = -l_rz[0][1];	l_rz[1][1] = l_rz[0][0];

			l_rx = l_rz* l_ry* l_rx;
			l_rx.set_scale_x(l_scale_x);
			l_rx.set_scale_y(l_scale_y);
			l_rx.set_scale_z(l_scale_z);

			m_ptr[0][0] = l_rx[0][0];	m_ptr[0][1] = l_rx[0][1];	m_ptr[0][2] = l_rx[0][2];
			m_ptr[1][0] = l_rx[1][0];	m_ptr[1][1] = l_rx[1][1];	m_ptr[1][2] = l_rx[1][2];
			m_ptr[2][0] = l_rx[2][0];	m_ptr[2][1] = l_rx[2][1];	m_ptr[2][2] = l_rx[2][2];

		}

		inline bool		is_equivalent(const vufMatrix4& p_other, T p_tolerance = vufMatrix4_kTol)
		{
			return	abs(m_ptr[0][0] - p_other.m_ptr[0][0]) < p_tolerance &&
					abs(m_ptr[0][1] - p_other.m_ptr[0][1]) < p_tolerance &&
					abs(m_ptr[0][2] - p_other.m_ptr[0][2]) < p_tolerance &&
					abs(m_ptr[0][3] - p_other.m_ptr[0][3]) < p_tolerance &&

					abs(m_ptr[1][0] - p_other.m_ptr[1][0]) < p_tolerance &&
					abs(m_ptr[1][1] - p_other.m_ptr[1][1]) < p_tolerance &&
					abs(m_ptr[1][2] - p_other.m_ptr[1][2]) < p_tolerance &&
					abs(m_ptr[1][3] - p_other.m_ptr[1][3]) < p_tolerance &&

					abs(m_ptr[2][0] - p_other.m_ptr[2][0]) < p_tolerance &&
					abs(m_ptr[2][1] - p_other.m_ptr[2][1]) < p_tolerance &&
					abs(m_ptr[2][2] - p_other.m_ptr[2][2]) < p_tolerance &&
					abs(m_ptr[2][3] - p_other.m_ptr[2][3]) < p_tolerance &&

					abs(m_ptr[3][0] - p_other.m_ptr[3][0]) < p_tolerance &&
					abs(m_ptr[3][1] - p_other.m_ptr[3][1]) < p_tolerance &&
					abs(m_ptr[3][2] - p_other.m_ptr[3][2]) < p_tolerance &&
					abs(m_ptr[3][3] - p_other.m_ptr[3][3]) < p_tolerance;
		}

		vufMatrix4& operator+=(const vufMatrix4& p_right)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_ptr[i][j] += p_right.m_ptr[i][j];
				}
			}
			return *this;
		}
		vufMatrix4  operator+ (const vufMatrix4& p_right) const
		{
			vufMatrix4 l_matr;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] + p_right.m_ptr[i][j];
				}
			}
			return l_matr;
		}
		vufMatrix4& operator-=(const vufMatrix4& p_right)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_ptr[i][j] -= p_right.m_ptr[i][j];
				}
			}
			return *this;

		}
		vufMatrix4  operator- (const vufMatrix4& p_right) const
		{
			vufMatrix4 l_matr;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] - p_right.m_ptr[i][j];
				}
			}
			return l_matr;
		}
		vufMatrix4& operator*= (const vufMatrix4& p_right)
		{
			*this = *this * p_right;
			return *this;
		}
		vufMatrix4  operator*(const vufMatrix4& p_right) const
		{
			vufMatrix4 l_m;

			l_m.m_ptr[0][0] =	m_ptr[0][0] * p_right.m_ptr[0][0] + 
								m_ptr[0][1] * p_right.m_ptr[1][0] + 
								m_ptr[0][2] * p_right.m_ptr[2][0] + 
								m_ptr[0][3] * p_right.m_ptr[3][0];

			l_m.m_ptr[0][1] =	m_ptr[0][0] * p_right.m_ptr[0][1] + 
								m_ptr[0][1] * p_right.m_ptr[1][1] + 
								m_ptr[0][2] * p_right.m_ptr[2][1] + 
								m_ptr[0][3] * p_right.m_ptr[3][1];

			l_m.m_ptr[0][2] =	m_ptr[0][0] * p_right.m_ptr[0][2] +
								m_ptr[0][1] * p_right.m_ptr[1][2] +
								m_ptr[0][2] * p_right.m_ptr[2][2] +
								m_ptr[0][3] * p_right.m_ptr[3][2];

			l_m.m_ptr[0][3] =	m_ptr[0][0] * p_right.m_ptr[0][3] +
								m_ptr[0][1] * p_right.m_ptr[1][3] +
								m_ptr[0][2] * p_right.m_ptr[2][3] +
								m_ptr[0][3] * p_right.m_ptr[3][3];



			l_m.m_ptr[1][0] =	m_ptr[1][0] * p_right.m_ptr[0][0] +
								m_ptr[1][1] * p_right.m_ptr[1][0] +
								m_ptr[1][2] * p_right.m_ptr[2][0] +
								m_ptr[1][3] * p_right.m_ptr[3][0];

			l_m.m_ptr[1][1] =	m_ptr[1][0] * p_right.m_ptr[0][1] +
								m_ptr[1][1] * p_right.m_ptr[1][1] +
								m_ptr[1][2] * p_right.m_ptr[2][1] +
								m_ptr[1][3] * p_right.m_ptr[3][1];

			l_m.m_ptr[1][2] =	m_ptr[1][0] * p_right.m_ptr[0][2] +
								m_ptr[1][1] * p_right.m_ptr[1][2] +
								m_ptr[1][2] * p_right.m_ptr[2][2] +
								m_ptr[1][3] * p_right.m_ptr[3][2];

			l_m.m_ptr[1][3] =	m_ptr[1][0] * p_right.m_ptr[0][3] +
								m_ptr[1][1] * p_right.m_ptr[1][3] +
								m_ptr[1][2] * p_right.m_ptr[2][3] +
								m_ptr[1][3] * p_right.m_ptr[3][3];



			l_m.m_ptr[2][0] =	m_ptr[2][0] * p_right.m_ptr[0][0] +
								m_ptr[2][1] * p_right.m_ptr[1][0] +
								m_ptr[2][2] * p_right.m_ptr[2][0] +
								m_ptr[2][3] * p_right.m_ptr[3][0];

			l_m.m_ptr[2][1] =	m_ptr[2][0] * p_right.m_ptr[0][1] +
								m_ptr[2][1] * p_right.m_ptr[1][1] +
								m_ptr[2][2] * p_right.m_ptr[2][1] +
								m_ptr[2][3] * p_right.m_ptr[3][1];

			l_m.m_ptr[2][2] =	m_ptr[2][0] * p_right.m_ptr[0][2] +
								m_ptr[2][1] * p_right.m_ptr[1][2] +
								m_ptr[2][2] * p_right.m_ptr[2][2] +
								m_ptr[2][3] * p_right.m_ptr[3][2];

			l_m.m_ptr[2][3] =	m_ptr[2][0] * p_right.m_ptr[0][3] +
								m_ptr[2][1] * p_right.m_ptr[1][3] +
								m_ptr[2][2] * p_right.m_ptr[2][3] +
								m_ptr[2][3] * p_right.m_ptr[3][3];



			l_m.m_ptr[3][0] =	m_ptr[3][0] * p_right.m_ptr[0][0] +
								m_ptr[3][1] * p_right.m_ptr[1][0] +
								m_ptr[3][2] * p_right.m_ptr[2][0] +
								m_ptr[3][3] * p_right.m_ptr[3][0];

			l_m.m_ptr[3][1] =	m_ptr[3][0] * p_right.m_ptr[0][1] +
								m_ptr[3][1] * p_right.m_ptr[1][1] +
								m_ptr[3][2] * p_right.m_ptr[2][1] +
								m_ptr[3][3] * p_right.m_ptr[3][1];

			l_m.m_ptr[3][2] =	m_ptr[3][0] * p_right.m_ptr[0][2] +
								m_ptr[3][1] * p_right.m_ptr[1][2] +
								m_ptr[3][2] * p_right.m_ptr[2][2] +
								m_ptr[3][3] * p_right.m_ptr[3][2];

			l_m.m_ptr[3][3] =	m_ptr[3][0] * p_right.m_ptr[0][3] +
								m_ptr[3][1] * p_right.m_ptr[1][3] +
								m_ptr[3][2] * p_right.m_ptr[2][3] +
								m_ptr[3][3] * p_right.m_ptr[3][3];

			return l_m;

		}
		vufMatrix4& operator*=(T p_val)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_ptr[i][j] *= p_val;
				}
			}
			return *this;
		}
		vufMatrix4  operator* (T p_val) const
		{
			vufMatrix4 l_matr;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					l_matr.m_ptr[i][j] = m_ptr[i][j] * p_val;
				}
			}
			return l_matr;
		}
		vufMatrix4<T> operator-() const
		{
			vufMatrix4<T> l_res;
			for (int i = 0; i <4; i++ )
			{
				for (int j = 0; j < 4; j++)
				{
					l_res.m_ptr[i][j] = -m_ptr[i][j];
				}
			}
			return l_res;
		}
		bool		operator==(const vufMatrix4& p_other) const
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (m_ptr[i][j] != p_other.m_ptr[i][j])
					{
						return false;
					}
				}
			}
			return true;
		}
		bool		operator!=(const vufMatrix4& p_other) const
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (m_ptr[i][j] != p_other.m_ptr[i][j])
					{
						return true;
					}
				}
			}
			return false;
		}

		bool		close_to(const vufMatrix4& p_other, double p_prcn = VF_MATH_EPSILON)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (abs(m_ptr[i][j] - p_other.m_ptr[i][j]) > p_prcn)
					{
						return false;
					}
				}
			}
			return true;

		}
		vufMatrix4  inverse() const
		{
			vufMatrix4 l_m(*this);
			double l_cof[4][4], l_det;
			//---------------------------------------------------------
			l_cof[0][0] =	  m_ptr[1][1] * m_ptr[2][2] * m_ptr[3][3]	//123 123
							+ m_ptr[2][1] * m_ptr[3][2] * m_ptr[1][3]	//231 123
							+ m_ptr[3][1] * m_ptr[1][2] * m_ptr[2][3]	//312 123
							- m_ptr[3][1] * m_ptr[2][2] * m_ptr[1][3]	//321 123
							- m_ptr[2][1] * m_ptr[1][2] * m_ptr[3][3]	//213 123
							- m_ptr[1][1] * m_ptr[3][2] * m_ptr[2][3];	//132 123

			l_cof[0][1] =	- m_ptr[1][0] * m_ptr[2][2] * m_ptr[3][3]	//123 023
							- m_ptr[2][0] * m_ptr[3][2] * m_ptr[1][3]	//231 023
							- m_ptr[3][0] * m_ptr[1][2] * m_ptr[2][3]	//312 023
							+ m_ptr[3][0] * m_ptr[2][2] * m_ptr[1][3]	//321 023
							+ m_ptr[2][0] * m_ptr[1][2] * m_ptr[3][3]	//213 023
							+ m_ptr[1][0] * m_ptr[3][2] * m_ptr[2][3];	//132 023

			l_cof[0][2] =	  m_ptr[1][0] * m_ptr[2][1] * m_ptr[3][3]	//123 013
							+ m_ptr[2][0] * m_ptr[3][1] * m_ptr[1][3]	//231 013
							+ m_ptr[3][0] * m_ptr[1][1] * m_ptr[2][3]	//312 013
							- m_ptr[3][0] * m_ptr[2][1] * m_ptr[1][3]	//321 013
							- m_ptr[1][0] * m_ptr[3][1] * m_ptr[2][3]	//132 013
							- m_ptr[2][0] * m_ptr[1][1] * m_ptr[3][3];	//213 013

			l_cof[0][3] =	- m_ptr[1][0] * m_ptr[2][1] * m_ptr[3][2]	//123 012
							- m_ptr[2][0] * m_ptr[3][1] * m_ptr[1][2]	//231 012
							- m_ptr[3][0] * m_ptr[1][1] * m_ptr[2][2]	//312 012
							+ m_ptr[3][0] * m_ptr[2][1] * m_ptr[1][2]	//321 012
							+ m_ptr[2][0] * m_ptr[1][1] * m_ptr[3][2]	//213 012
							+ m_ptr[1][0] * m_ptr[3][1] * m_ptr[2][2];	//132 012
			//---------------------------------------------------------
			l_cof[1][0] =	- m_ptr[0][1] * m_ptr[2][2] * m_ptr[3][3]	//023 123
							- m_ptr[2][1] * m_ptr[3][2] * m_ptr[0][3]	//230 123
							- m_ptr[3][1] * m_ptr[0][2] * m_ptr[2][3]	//302 123
							+ m_ptr[3][1] * m_ptr[2][2] * m_ptr[0][3]	//320 123
							+ m_ptr[2][1] * m_ptr[0][2] * m_ptr[3][3]	//203 123
							+ m_ptr[0][1] * m_ptr[3][2] * m_ptr[2][3];	//032 123

			l_cof[1][1] =	  m_ptr[0][0] * m_ptr[2][2] * m_ptr[3][3]	//023 023
							+ m_ptr[2][0] * m_ptr[3][2] * m_ptr[0][3]	//230 023
							+ m_ptr[3][0] * m_ptr[0][2] * m_ptr[2][3]	//302 023
							- m_ptr[3][0] * m_ptr[2][2] * m_ptr[0][3]	//320 023
							- m_ptr[2][0] * m_ptr[0][2] * m_ptr[3][3]	//203 023
							- m_ptr[0][0] * m_ptr[3][2] * m_ptr[2][3];	//032 023

			l_cof[1][2] =	- m_ptr[0][0] * m_ptr[2][1] * m_ptr[3][3]	//023 013
							- m_ptr[2][0] * m_ptr[3][1] * m_ptr[0][3]	//230 013
							- m_ptr[3][0] * m_ptr[0][1] * m_ptr[2][3]	//302 013
							+ m_ptr[3][0] * m_ptr[2][1] * m_ptr[0][3]	//320 013
							+ m_ptr[2][0] * m_ptr[0][1] * m_ptr[3][3]	//203 013
							+ m_ptr[0][0] * m_ptr[3][1] * m_ptr[2][3];	//032 013

			l_cof[1][3] =	  m_ptr[0][0] * m_ptr[2][1] * m_ptr[3][2]	//023 012
							+ m_ptr[2][0] * m_ptr[3][1] * m_ptr[0][2]	//230 012
							+ m_ptr[3][0] * m_ptr[0][1] * m_ptr[2][2]	//302 012
							- m_ptr[3][0] * m_ptr[2][1] * m_ptr[0][2]	//320 012
							- m_ptr[2][0] * m_ptr[0][1] * m_ptr[3][2]	//203 012
							- m_ptr[0][0] * m_ptr[3][1] * m_ptr[2][2];	//032 012
			//---------------------------------------------------------
			l_cof[2][0] =	  m_ptr[0][1] * m_ptr[1][2] * m_ptr[3][3]	//013 123
							+ m_ptr[1][1] * m_ptr[3][2] * m_ptr[0][3]	//130 123
							+ m_ptr[3][1] * m_ptr[0][2] * m_ptr[1][3]	//301 123
							- m_ptr[3][1] * m_ptr[1][2] * m_ptr[0][3]	//310 123
							- m_ptr[1][1] * m_ptr[0][2] * m_ptr[3][3]	//103 123
							- m_ptr[0][1] * m_ptr[3][2] * m_ptr[1][3];	//031 123

			l_cof[2][1] =	- m_ptr[0][0] * m_ptr[1][2] * m_ptr[3][3]	//013 023
							- m_ptr[1][0] * m_ptr[3][2] * m_ptr[0][3]	//130 023
							- m_ptr[3][0] * m_ptr[0][2] * m_ptr[1][3]	//301 023
							+ m_ptr[3][0] * m_ptr[1][2] * m_ptr[0][3]	//310 023
							+ m_ptr[1][0] * m_ptr[0][2] * m_ptr[3][3]	//103 023
							+ m_ptr[0][0] * m_ptr[3][2] * m_ptr[1][3];	//031 023

			l_cof[2][2] =	  m_ptr[0][0] * m_ptr[1][1] * m_ptr[3][3]	//013 013
							+ m_ptr[1][0] * m_ptr[3][1] * m_ptr[0][3]	//130 013
							+ m_ptr[3][0] * m_ptr[0][1] * m_ptr[1][3]	//301 013
							- m_ptr[3][0] * m_ptr[1][1] * m_ptr[0][3]	//310 013
							- m_ptr[1][0] * m_ptr[0][1] * m_ptr[3][3]	//103 013
							- m_ptr[0][0] * m_ptr[3][1] * m_ptr[1][3];	//031 013

			l_cof[2][3] =	- m_ptr[0][0] * m_ptr[1][1] * m_ptr[3][2]	//013 012
							- m_ptr[1][0] * m_ptr[3][1] * m_ptr[0][2]	//130 012
							- m_ptr[3][0] * m_ptr[0][1] * m_ptr[1][2]	//301 012
							+ m_ptr[3][0] * m_ptr[1][1] * m_ptr[0][2]	//310 012
							+ m_ptr[1][0] * m_ptr[0][1] * m_ptr[3][2]	//103 012
							+ m_ptr[0][0] * m_ptr[3][1] * m_ptr[1][2];	//031 012
			//---------------------------------------------------------
			l_cof[3][0] =	- m_ptr[0][1] * m_ptr[1][2] * m_ptr[2][3]	//012 123
							- m_ptr[1][1] * m_ptr[2][2] * m_ptr[0][3]	//120 123
							- m_ptr[2][1] * m_ptr[0][2] * m_ptr[1][3]	//201 123
							+ m_ptr[2][1] * m_ptr[1][2] * m_ptr[0][3]	//210 123
							+ m_ptr[1][1] * m_ptr[0][2] * m_ptr[2][3]	//102 123
							+ m_ptr[0][1] * m_ptr[2][2] * m_ptr[1][3];	//021 123

			l_cof[3][1] =	  m_ptr[0][0] * m_ptr[1][2] * m_ptr[2][3]	//012 023
							+ m_ptr[1][0] * m_ptr[2][2] * m_ptr[0][3]	//120 023
							+ m_ptr[2][0] * m_ptr[0][2] * m_ptr[1][3]	//201 023
							- m_ptr[2][0] * m_ptr[1][2] * m_ptr[0][3]	//210 023
							- m_ptr[1][0] * m_ptr[0][2] * m_ptr[2][3]	//102 023
							- m_ptr[0][0] * m_ptr[2][2] * m_ptr[1][3];	//021 023

			l_cof[3][2] =	- m_ptr[0][0] * m_ptr[1][1] * m_ptr[2][3]	//012 013
							- m_ptr[1][0] * m_ptr[2][1] * m_ptr[0][3]	//120 013
							- m_ptr[2][0] * m_ptr[0][1] * m_ptr[1][3]	//201 013	
							+ m_ptr[2][0] * m_ptr[1][1] * m_ptr[0][3]	//210 013
							+ m_ptr[1][0] * m_ptr[0][1] * m_ptr[2][3]	//102 013
							+ m_ptr[0][0] * m_ptr[2][1] * m_ptr[1][3];	//021 013

			l_cof[3][3] =	  m_ptr[0][0] * m_ptr[1][1] * m_ptr[2][2]	//012 012
							+ m_ptr[1][0] * m_ptr[2][1] * m_ptr[0][2]	//120 012
							+ m_ptr[2][0] * m_ptr[0][1] * m_ptr[1][2]	//201 012
							- m_ptr[2][0] * m_ptr[1][1] * m_ptr[0][2]	//210 012
							- m_ptr[1][0] * m_ptr[0][1] * m_ptr[2][2]	//102 012
							- m_ptr[0][0] * m_ptr[2][1] * m_ptr[1][2];	//021 012

			l_det = m_ptr[0][0] * l_cof[0][0] +
					m_ptr[0][1] * l_cof[0][1] +
					m_ptr[0][2] * l_cof[0][2] +
					m_ptr[0][3] * l_cof[0][3];

			if (l_det == .0)
			{
				return l_m;
			}

			l_det = 1.0 / l_det;

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					l_m.m_ptr[i][j] = l_cof[j][i] * l_det;
				}
			}
			return l_m;
		}
		vufMatrix4& inverse_in_place()
		{
			double l_cof[4][4], l_det;
			//---------------------------------------------------------
			l_cof[0][0] = m_ptr[1][1] * m_ptr[2][2] * m_ptr[3][3]	//123 123
				+ m_ptr[2][1] * m_ptr[3][2] * m_ptr[1][3]	//231 123
				+ m_ptr[3][1] * m_ptr[1][2] * m_ptr[2][3]	//312 123
				- m_ptr[3][1] * m_ptr[2][2] * m_ptr[1][3]	//321 123
				- m_ptr[2][1] * m_ptr[1][2] * m_ptr[3][3]	//213 123
				- m_ptr[1][1] * m_ptr[3][2] * m_ptr[2][3];	//132 123

			l_cof[0][1] = -m_ptr[1][0] * m_ptr[2][2] * m_ptr[3][3]	//123 023
				- m_ptr[2][0] * m_ptr[3][2] * m_ptr[1][3]	//231 023
				- m_ptr[3][0] * m_ptr[1][2] * m_ptr[2][3]	//312 023
				+ m_ptr[3][0] * m_ptr[2][2] * m_ptr[1][3]	//321 023
				+ m_ptr[2][0] * m_ptr[1][2] * m_ptr[3][3]	//213 023
				+ m_ptr[1][0] * m_ptr[3][2] * m_ptr[2][3];	//132 023

			l_cof[0][2] = m_ptr[1][0] * m_ptr[2][1] * m_ptr[3][3]	//123 013
				+ m_ptr[2][0] * m_ptr[3][1] * m_ptr[1][3]	//231 013
				+ m_ptr[3][0] * m_ptr[1][1] * m_ptr[2][3]	//312 013
				- m_ptr[3][0] * m_ptr[2][1] * m_ptr[1][3]	//321 013
				- m_ptr[1][0] * m_ptr[3][1] * m_ptr[2][3]	//132 013
				- m_ptr[2][0] * m_ptr[1][1] * m_ptr[3][3];	//213 013

			l_cof[0][3] = -m_ptr[1][0] * m_ptr[2][1] * m_ptr[3][2]	//123 012
				- m_ptr[2][0] * m_ptr[3][1] * m_ptr[1][2]	//231 012
				- m_ptr[3][0] * m_ptr[1][1] * m_ptr[2][2]	//312 012
				+ m_ptr[3][0] * m_ptr[2][1] * m_ptr[1][2]	//321 012
				+ m_ptr[2][0] * m_ptr[1][1] * m_ptr[3][2]	//213 012
				+ m_ptr[1][0] * m_ptr[3][1] * m_ptr[2][2];	//132 012
//---------------------------------------------------------
			l_cof[1][0] = -m_ptr[0][1] * m_ptr[2][2] * m_ptr[3][3]	//023 123
				- m_ptr[2][1] * m_ptr[3][2] * m_ptr[0][3]	//230 123
				- m_ptr[3][1] * m_ptr[0][2] * m_ptr[2][3]	//302 123
				+ m_ptr[3][1] * m_ptr[2][2] * m_ptr[0][3]	//320 123
				+ m_ptr[2][1] * m_ptr[0][2] * m_ptr[3][3]	//203 123
				+ m_ptr[0][1] * m_ptr[3][2] * m_ptr[2][3];	//032 123

			l_cof[1][1] = m_ptr[0][0] * m_ptr[2][2] * m_ptr[3][3]	//023 023
				+ m_ptr[2][0] * m_ptr[3][2] * m_ptr[0][3]	//230 023
				+ m_ptr[3][0] * m_ptr[0][2] * m_ptr[2][3]	//302 023
				- m_ptr[3][0] * m_ptr[2][2] * m_ptr[0][3]	//320 023
				- m_ptr[2][0] * m_ptr[0][2] * m_ptr[3][3]	//203 023
				- m_ptr[0][0] * m_ptr[3][2] * m_ptr[2][3];	//032 023

			l_cof[1][2] = -m_ptr[0][0] * m_ptr[2][1] * m_ptr[3][3]	//023 013
				- m_ptr[2][0] * m_ptr[3][1] * m_ptr[0][3]	//230 013
				- m_ptr[3][0] * m_ptr[0][1] * m_ptr[2][3]	//302 013
				+ m_ptr[3][0] * m_ptr[2][1] * m_ptr[0][3]	//320 013
				+ m_ptr[2][0] * m_ptr[0][1] * m_ptr[3][3]	//203 013
				+ m_ptr[0][0] * m_ptr[3][1] * m_ptr[2][3];	//032 013

			l_cof[1][3] = m_ptr[0][0] * m_ptr[2][1] * m_ptr[3][2]	//023 012
				+ m_ptr[2][0] * m_ptr[3][1] * m_ptr[0][2]	//230 012
				+ m_ptr[3][0] * m_ptr[0][1] * m_ptr[2][2]	//302 012
				- m_ptr[3][0] * m_ptr[2][1] * m_ptr[0][2]	//320 012
				- m_ptr[2][0] * m_ptr[0][1] * m_ptr[3][2]	//203 012
				- m_ptr[0][0] * m_ptr[3][1] * m_ptr[2][2];	//032 012
//---------------------------------------------------------
			l_cof[2][0] = m_ptr[0][1] * m_ptr[1][2] * m_ptr[3][3]	//013 123
				+ m_ptr[1][1] * m_ptr[3][2] * m_ptr[0][3]	//130 123
				+ m_ptr[3][1] * m_ptr[0][2] * m_ptr[1][3]	//301 123
				- m_ptr[3][1] * m_ptr[1][2] * m_ptr[0][3]	//310 123
				- m_ptr[1][1] * m_ptr[0][2] * m_ptr[3][3]	//103 123
				- m_ptr[0][1] * m_ptr[3][2] * m_ptr[1][3];	//031 123

			l_cof[2][1] = -m_ptr[0][0] * m_ptr[1][2] * m_ptr[3][3]	//013 023
				- m_ptr[1][0] * m_ptr[3][2] * m_ptr[0][3]	//130 023
				- m_ptr[3][0] * m_ptr[0][2] * m_ptr[1][3]	//301 023
				+ m_ptr[3][0] * m_ptr[1][2] * m_ptr[0][3]	//310 023
				+ m_ptr[1][0] * m_ptr[0][2] * m_ptr[3][3]	//103 023
				+ m_ptr[0][0] * m_ptr[3][2] * m_ptr[1][3];	//031 023

			l_cof[2][2] = m_ptr[0][0] * m_ptr[1][1] * m_ptr[3][3]	//013 013
				+ m_ptr[1][0] * m_ptr[3][1] * m_ptr[0][3]	//130 013
				+ m_ptr[3][0] * m_ptr[0][1] * m_ptr[1][3]	//301 013
				- m_ptr[3][0] * m_ptr[1][1] * m_ptr[0][3]	//310 013
				- m_ptr[1][0] * m_ptr[0][1] * m_ptr[3][3]	//103 013
				- m_ptr[0][0] * m_ptr[3][1] * m_ptr[1][3];	//031 013

			l_cof[2][3] = -m_ptr[0][0] * m_ptr[1][1] * m_ptr[3][2]	//013 012
				- m_ptr[1][0] * m_ptr[3][1] * m_ptr[0][2]	//130 012
				- m_ptr[3][0] * m_ptr[0][1] * m_ptr[1][2]	//301 012
				+ m_ptr[3][0] * m_ptr[1][1] * m_ptr[0][2]	//310 012
				+ m_ptr[1][0] * m_ptr[0][1] * m_ptr[3][2]	//103 012
				+ m_ptr[0][0] * m_ptr[3][1] * m_ptr[1][2];	//031 012
//---------------------------------------------------------
			l_cof[3][0] = -m_ptr[0][1] * m_ptr[1][2] * m_ptr[2][3]	//012 123
				- m_ptr[1][1] * m_ptr[2][2] * m_ptr[0][3]	//120 123
				- m_ptr[2][1] * m_ptr[0][2] * m_ptr[1][3]	//201 123
				+ m_ptr[2][1] * m_ptr[1][2] * m_ptr[0][3]	//210 123
				+ m_ptr[1][1] * m_ptr[0][2] * m_ptr[2][3]	//102 123
				+ m_ptr[0][1] * m_ptr[2][2] * m_ptr[1][3];	//021 123

			l_cof[3][1] = m_ptr[0][0] * m_ptr[1][2] * m_ptr[2][3]	//012 023
				+ m_ptr[1][0] * m_ptr[2][2] * m_ptr[0][3]	//120 023
				+ m_ptr[2][0] * m_ptr[0][2] * m_ptr[1][3]	//201 023
				- m_ptr[2][0] * m_ptr[1][2] * m_ptr[0][3]	//210 023
				- m_ptr[1][0] * m_ptr[0][2] * m_ptr[2][3]	//102 023
				- m_ptr[0][0] * m_ptr[2][2] * m_ptr[1][3];	//021 023

			l_cof[3][2] = -m_ptr[0][0] * m_ptr[1][1] * m_ptr[2][3]	//012 013
				- m_ptr[1][0] * m_ptr[2][1] * m_ptr[0][3]	//120 013
				- m_ptr[2][0] * m_ptr[0][1] * m_ptr[1][3]	//201 013	
				+ m_ptr[2][0] * m_ptr[1][1] * m_ptr[0][3]	//210 013
				+ m_ptr[1][0] * m_ptr[0][1] * m_ptr[2][3]	//102 013
				+ m_ptr[0][0] * m_ptr[2][1] * m_ptr[1][3];	//021 013

			l_cof[3][3] = m_ptr[0][0] * m_ptr[1][1] * m_ptr[2][2]	//012 012
				+ m_ptr[1][0] * m_ptr[2][1] * m_ptr[0][2]	//120 012
				+ m_ptr[2][0] * m_ptr[0][1] * m_ptr[1][2]	//201 012
				- m_ptr[2][0] * m_ptr[1][1] * m_ptr[0][2]	//210 012
				- m_ptr[1][0] * m_ptr[0][1] * m_ptr[2][2]	//102 012
				- m_ptr[0][0] * m_ptr[2][1] * m_ptr[1][2];	//021 012

			l_det = m_ptr[0][0] * l_cof[0][0] +
				m_ptr[0][1] * l_cof[0][1] +
				m_ptr[0][2] * l_cof[0][2] +
				m_ptr[0][3] * l_cof[0][3];

			if (l_det == .0)
			{
				return *this;
			}

			l_det = 1.0 / l_det;

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_ptr[i][j] = l_cof[j][i] * l_det;
				}
			}
			return *this;

		}
		
		// serialization
		std::string		to_string(int p_precision = -1) const
		{
			std::stringstream l_ss;
			if (p_precision > 0)
			{
				if (p_precision > 64)
				{
					l_ss.precision(64);
				}
				else
				{
					l_ss.precision(p_precision);
				}
			}
			l_ss << "[";
			for (int i = 0; i < 4; ++i)
			{
				l_ss << "(";
				for (int j = 0; j < 4; ++j)
				{
					l_ss << m_ptr[i][j];
					if ( j != 3 )
					{
						l_ss << ",";
					}
				}
				l_ss << ")";
				
				if (i != 3)
				{
					l_ss << ",";
				}				
			}
			l_ss << "]";
			return l_ss.str();
		}
		/** patse string for offest return readed chars count in string*/
		uint64_t		from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			try
			{
				T l_x[16];
				std::stringstream l_ss;
				uint64_t l_index = 0;
				uint64_t l_str_pos = p_offset;
				for (uint64_t i = p_offset; i < p_str.size(); ++i)
				{
					l_str_pos = i;
					if (p_str[l_str_pos] == '[' || p_str[l_str_pos] == ' ' || p_str[l_str_pos] == '\t'
						|| p_str[l_str_pos] == '(' || p_str[l_str_pos] == ')')
					{
						continue;
					}
					if (p_str[l_str_pos] == ',' || p_str[l_str_pos] == ']' )
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
				if (l_index == 16)
				{
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							m_ptr[i][j] = l_x[i * 4 + j];
						}
					}
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
		uint64_t		to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)	const
		{
			if (p_buff.size() < p_offset + sizeof(T) * 16)
			{
				p_buff.resize(p_offset + sizeof(T) * 16);
			}
			char* l_data = (char*)m_ptr;
			std::memcpy(&p_buff[p_offset], l_data, sizeof(T) * 16);			
			return p_offset + sizeof(T) * 16;
		}
		/** read vector from binary return new offset readed */
		uint64_t		from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)
		{
			if (p_buff.size() < p_offset + 16 * sizeof(T))
			{
				return 0;
			}
			char* l_data = (char*)m_ptr;
			std::memcpy( l_data, &p_buff[p_offset], sizeof(T) * 16);
			return p_offset + sizeof(T) * 16;
		}

		uint64_t		encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			return vuf::txtSerializer::encode_to_buff(m_ptr, 16 * sizeof(T), p_buff, p_offset);
		}
		uint64_t		decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)
		{
			return vuf::txtSerializer::decode_from_buff(m_ptr, 16 * sizeof(T), p_buff, p_offset);
		}

		friend  vufMatrix4<T> operator *(T p_val, const vufMatrix4<T>& p_matr)
		{
			return p_matr * p_val;
		}
		friend std::ostream& operator<<(std::ostream& p_out, const vufMatrix4& p_m)
		{
			auto l_flags = p_out.flags();
			auto l_old_precision = p_out.precision(3);
			p_out << std::fixed;;
			p_out << "[ vufMatrix 4x4" << std::endl;
			for (int i = 0; i < 4; i++)
			{
				p_out << " [";
				for (int j = 0; j < 4; j++)
				{
					p_out << " " << p_m(i, j);
				}
				p_out << " ]" << std::endl;;
			}
			p_out << "]";
			p_out.precision(l_old_precision);
			p_out.flags(l_flags);
			return p_out;
		}
	};
#pragma endregion VUF_MATRIX_4X4
	//-------------------------------------------------------------------------------------------------------------------------
	//  M A T R I X   O B J E C T
	//-------------------------------------------------------------------------------------------------------------------------
/*
#pragma region VUF_MATRIX_OBJECT
	template<typename T, template <typename> class MATRIX >
	class vufMatrixObject :public vufObject
	{
		//private:
	public:
		vufMatrixObject() {}
		vufMatrixObject(const vufMatrixObject& p_other) : vufObject(), m_matrix(p_other.m_matrix) {}
		vufMatrixObject(const MATRIX<T>& p_matr) : vufObject(), m_matrix(p_matr) {}
	public:
		static std::shared_ptr < vufMatrixObject<T, MATRIX> >	creator()
		{
			std::shared_ptr<vufMatrixObject<T, MATRIX>> l_ptr = std::shared_ptr< vufMatrixObject<T,MATRIX>>(new vufMatrixObject<T, MATRIX>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual std::shared_ptr<vufObject>		make_copy() const override
		{
			std::shared_ptr<vufMatrixObject<T, MATRIX>> l_ptr = std::shared_ptr< vufMatrixObject<T, MATRIX>>(new vufMatrixObject<T, MATRIX>(*this));
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
			return m_matrix.to_string();
		}
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) override
		{
			return m_matrix.from_string(p_str, p_offset);
		}
		virtual uint64_t	to_binary(std::vector<unsigned char>& p_buff)		const override
		{
			return m_matrix.to_binary(p_buff);
		}
		virtual uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)  override
		{
			return m_matrix.from_binary(p_buff, p_offset);
		}
		virtual vufObjectType get_type()	const override
		{
			return vufObjectType::k_matrix_2d_object;
		}
		
		virtual std::shared_ptr<vufMatrixObject<double, vufMatrix2> >		as_matrix_obj_2d()	const override { return nullptr; }
		virtual std::shared_ptr<vufMatrixObject<double, vufMatrix3> >		as_matrix_obj_3d()	const override { return nullptr; }
		virtual std::shared_ptr<vufMatrixObject<double, vufMatrix4> >		as_matrix_obj_4d()	const override { return nullptr; }
		virtual std::shared_ptr<vufMatrixObject<float,  vufMatrix2 >>		as_matrix_obj_2f()	const override { return nullptr; }
		virtual std::shared_ptr<vufMatrixObject<float,  vufMatrix3 >>		as_matrix_obj_3f()	const override { return nullptr; }
		virtual std::shared_ptr<vufMatrixObject<float,  vufMatrix4 >>		as_matrix_obj_4f()	const override { return nullptr; }

		MATRIX<T> m_matrix;
	};
	
	// < float,matrix 2> specialization  
	template<>
	vufObjectType	vufMatrixObject<float, vufMatrix2>::get_type()	const
	{
		return vufObjectType::k_matrix_2f_object;
	}
	template<>
	std::shared_ptr< vufMatrixObject<float, vufMatrix2> >		vufMatrixObject<float, vufMatrix2>::as_matrix_obj_2f()	const
	{
		return std::static_pointer_cast<vufMatrixObject<float, vufMatrix2>>(vufObject::m_this.lock());
	}
	
	// < float,matrix 3> specialization  
	template<>
	vufObjectType	vufMatrixObject<float, vufMatrix3>::get_type()	const
	{
		return vufObjectType::k_matrix_3f_object;
	}
	template<>
	std::shared_ptr< vufMatrixObject<float, vufMatrix3> >		vufMatrixObject<float, vufMatrix3>::as_matrix_obj_3f()	const
	{
		return std::static_pointer_cast<vufMatrixObject<float, vufMatrix3>>(vufObject::m_this.lock());
	}
	
	// < float,matrix 4> specialization  
	template<>
	vufObjectType	vufMatrixObject<float, vufMatrix4>::get_type()	const
	{
		return vufObjectType::k_matrix_4f_object;
	}
	template<>
	std::shared_ptr< vufMatrixObject<float, vufMatrix4> >		vufMatrixObject<float, vufMatrix4>::as_matrix_obj_4f()	const
	{
		return std::static_pointer_cast<vufMatrixObject<float, vufMatrix4>>(vufObject::m_this.lock());
	}

	// < double,matrix 2> specialization  
	template<>
	vufObjectType	vufMatrixObject<double, vufMatrix2>::get_type()	const 
	{
		return vufObjectType::k_matrix_2d_object;
	}
	template<>
	std::shared_ptr< vufMatrixObject<double, vufMatrix2> >		vufMatrixObject<double, vufMatrix2>::as_matrix_obj_2d()	const 
	{
		return std::static_pointer_cast< vufMatrixObject<double,vufMatrix2> >(vufObject::m_this.lock());
	}
	
	// < double,matrix 3> specialization  
	template<>
	vufObjectType	vufMatrixObject<double, vufMatrix3>::get_type()	const
	{
		return vufObjectType::k_matrix_3d_object;
	}
	template<>
	std::shared_ptr< vufMatrixObject<double, vufMatrix3> >		vufMatrixObject<double, vufMatrix3>::as_matrix_obj_3d()	const
	{
		return std::static_pointer_cast<vufMatrixObject<double, vufMatrix3>>(vufObject::m_this.lock());
	}

	// < double,matrix 4> specialization  
	template<>
	vufObjectType	vufMatrixObject<double, vufMatrix4>::get_type()	const
	{
		return vufObjectType::k_matrix_4d_object;
	}
	template<>
	std::shared_ptr< vufMatrixObject<double, vufMatrix4> >		vufMatrixObject<double, vufMatrix4>::as_matrix_obj_4d()	const
	{
		return std::static_pointer_cast<vufMatrixObject<double, vufMatrix4>>(vufObject::m_this.lock());
	}
#pragma endregion VUF_MATRIX_OBJECT

	//-------------------------------------------------------------------------------------------------------------------------
	//  M A T R I X   O B J E C T   A R R A Y
	//-------------------------------------------------------------------------------------------------------------------------
#pragma region VUF_MATRIX_OBJECT_ARRAY
	template<typename T, template <typename> class MATRIX >
	class vufMatrixArrayObject :public vufObject
	{
	public:
		vufMatrixArrayObject() {}
		vufMatrixArrayObject(const vufMatrixArrayObject& p_other) : vufObject(), m_matrix_array_v(p_other.m_matrix_array_v) {}
		vufMatrixArrayObject(const std::vector<MATRIX<T>>& p_matr) : vufObject(), m_matrix_array_v(p_matr) {}
	public:
		static std::shared_ptr < vufMatrixArrayObject<T,MATRIX> >	creator()
		{
			std::shared_ptr<vufMatrixArrayObject<T, MATRIX>> l_ptr =
				std::shared_ptr< vufMatrixArrayObject<T, MATRIX>>(new vufMatrixArrayObject<T, MATRIX>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual std::shared_ptr<vufObject>							make_copy() const override
		{
			std::shared_ptr<vufMatrixArrayObject<T, MATRIX>> l_ptr = std::shared_ptr< vufMatrixArrayObject<T, MATRIX>>(new vufMatrixArrayObject<T, MATRIX>(*this));
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}

		virtual uint64_t									get_detailed_count()				const override
		{
			return 0;
		}
		virtual std::shared_ptr<vufObject>					get_detailed_at(uint64_t p_index)	const override
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
			VF_OBJECTS_ARRAY_TO_STRNG_AND_RETURN_IT(m_matrix_array_v);			
		}
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) override
		{
			VF_OBJECTS_ARRAY_FROM_STRING_AND_RETURN_OFFSET(p_str, p_offset, m_matrix_array_v);			
		}
		virtual uint64_t	to_binary(std::vector<unsigned char>& p_buff)	const override
		{
			VF_OBJECTS_ARRAY_TO_BINARY_AND_RETURN_SIZE(p_buff, m_matrix_array_v);			
		}
		virtual uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)  override
		{
			VF_OBJECTS_ARRAY_FROM_BINARY_AND_RETURN_OFFSET(p_buff, p_offset, m_matrix_array_v)			
		}
		
		virtual vufObjectType get_type()	const override
		{
			return vufObjectType::k_null_object;
		}

		virtual std::shared_ptr < vufMatrixArrayObject<double, vufMatrix2> >	as_matrix_array_obj_2d()	const override { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<double, vufMatrix3> >	as_matrix_array_obj_3d()	const override { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<double, vufMatrix4> >	as_matrix_array_obj_4d()	const override { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<float,  vufMatrix2> >	as_matrix_array_obj_2f()	const override { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<float,  vufMatrix3> >	as_matrix_array_obj_3f()	const override { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<float,  vufMatrix4> >	as_matrix_array_obj_4f()	const override { return nullptr; }
				
		std::vector< MATRIX<T> > m_matrix_array_v;
	};

	// < double,matrix 2> specialization  
	template<>
	vufObjectType	vufMatrixArrayObject<double, vufMatrix2>::get_type()	const
	{
		return vufObjectType::k_matrix_2d_array_object;
	}
	template<>
	std::shared_ptr< vufMatrixArrayObject<double, vufMatrix2> >		vufMatrixArrayObject<double, vufMatrix2>::as_matrix_array_obj_2d()	const
	{
		return std::static_pointer_cast<vufMatrixArrayObject<double, vufMatrix2>>(vufObject::m_this.lock());
	}

	// < double,matrix 3> specialization  
	template<>
	vufObjectType	vufMatrixArrayObject<double, vufMatrix3>::get_type()	const
	{
		return vufObjectType::k_matrix_3d_array_object;
	}
	template<>
	std::shared_ptr< vufMatrixArrayObject<double, vufMatrix3> >		vufMatrixArrayObject<double, vufMatrix3>::as_matrix_array_obj_3d()	const
	{
		return std::static_pointer_cast<vufMatrixArrayObject<double, vufMatrix3>>(vufObject::m_this.lock());
	}

	// < double,matrix 4> specialization  
	template<>
	vufObjectType	vufMatrixArrayObject<double, vufMatrix4>::get_type()	const
	{
		return vufObjectType::k_matrix_3d_array_object;
	}
	template<>
	std::shared_ptr< vufMatrixArrayObject<double, vufMatrix4> >		vufMatrixArrayObject<double, vufMatrix4>::as_matrix_array_obj_4d()	const
	{
		return std::static_pointer_cast<vufMatrixArrayObject<double, vufMatrix4>>(vufObject::m_this.lock());
	}

	// < float,matrix 2> specialization  
	template<>
	vufObjectType	vufMatrixArrayObject<float, vufMatrix2>::get_type()	const
	{
		return vufObjectType::k_matrix_2f_array_object;
	}
	template<>
	std::shared_ptr< vufMatrixArrayObject<float, vufMatrix2> >		vufMatrixArrayObject<float, vufMatrix2>::as_matrix_array_obj_2f()	const
	{
		return std::static_pointer_cast<vufMatrixArrayObject<float, vufMatrix2>>(vufObject::m_this.lock());
	}

	// < float,matrix 3> specialization  
	template<>
	vufObjectType	vufMatrixArrayObject<float, vufMatrix3>::get_type()	const
	{
		return vufObjectType::k_matrix_3f_array_object;
	}
	template<>
	std::shared_ptr< vufMatrixArrayObject<float, vufMatrix3> >		vufMatrixArrayObject<float, vufMatrix3>::as_matrix_array_obj_3f()	const
	{
		return std::static_pointer_cast<vufMatrixArrayObject<float, vufMatrix3>>(vufObject::m_this.lock());
	}

	// < float,matrix 4> specialization  
	template<>
	vufObjectType	vufMatrixArrayObject<float, vufMatrix4>::get_type()	const
	{
		return vufObjectType::k_matrix_4f_array_object;
	}
	template<>
	std::shared_ptr< vufMatrixArrayObject<float, vufMatrix4> >		vufMatrixArrayObject<float, vufMatrix4>::as_matrix_array_obj_4f()	const
	{
		return std::static_pointer_cast<vufMatrixArrayObject<float, vufMatrix4>>(vufObject::m_this.lock());
	}
#pragma endregion	VUF_MATRIX_OBJECT_ARRAY
	//-------------------------------------------------------------------------------------------------------------------------
	// USING NAMES
	//-------------------------------------------------------------------------------------------------------------------------
*/
#pragma region USING_MANES_MATRICES
	
	using vufMatrix_2f = vufMatrix2<float>;
	using vufMatrix_3f = vufMatrix3<float>;
	using vufMatrix_4f = vufMatrix4<float>;
	using vufMatrix_2d = vufMatrix2<double>;
	using vufMatrix_3d = vufMatrix3<double>;
	using vufMatrix_4d = vufMatrix4<double>;
	
	//using vufMatrixObject_2f = vufMatrixObject<float,  vufMatrix2>;
	//using vufMatrixObject_3f = vufMatrixObject<float,  vufMatrix3>;
	//using vufMatrixObject_4f = vufMatrixObject<float,  vufMatrix4>;
	//using vufMatrixObject_2d = vufMatrixObject<double, vufMatrix2>;
	//using vufMatrixObject_3d = vufMatrixObject<double, vufMatrix3>;
	//using vufMatrixObject_4d = vufMatrixObject<double, vufMatrix4>;
	
	//using vufMatrixArrayObject_2f = vufMatrixArrayObject<float,		vufMatrix2>;
	//using vufMatrixArrayObject_3f = vufMatrixArrayObject<float,		vufMatrix3>;
	//using vufMatrixArrayObject_4f = vufMatrixArrayObject<float,		vufMatrix4>;
	//using vufMatrixArrayObject_2d = vufMatrixArrayObject<double,	vufMatrix2>;
	//using vufMatrixArrayObject_3d = vufMatrixArrayObject<double,	vufMatrix3>;
	//using vufMatrixArrayObject_4d = vufMatrixArrayObject<double,	vufMatrix4>;

	//using vufMatrixArrayFn_2f = vufObjectArrayFn<float,  vufMatrix2>;
	//using vufMatrixArrayFn_3f = vufObjectArrayFn<float,  vufMatrix3>;
	//using vufMatrixArrayFn_4f = vufObjectArrayFn<float,  vufMatrix4>;
	//using vufMatrixArrayFn_2d = vufObjectArrayFn<double, vufMatrix2>;
	//using vufMatrixArrayFn_3d = vufObjectArrayFn<double, vufMatrix3>;
	//using vufMatrixArrayFn_4d = vufObjectArrayFn<double, vufMatrix4>;

#pragma endregion USING_MANES_MATRICES

}
#endif // !VF_MATH_MATRIX_H
