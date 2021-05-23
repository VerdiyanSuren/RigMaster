#ifndef VF_MATH_CRV_RBLD_FN_H
#define VF_MATH_CRV_RBLD_FN_H

#include "math/curves/vufCurve.h"
#include "math/vufConsoleInclude.h"
#include <memory>
#include <vector>
#include <cmath>

namespace vufMath
{
	template <class T, template<typename> class V>
	class vufRebuildUniformCurveFn;

#pragma region VF_CURVE_FN_BASE
	// functions set to work with specific part 
	template <class T, template<typename> class V>
	class vufRebuildCurveFn
	{
	public:
		enum
		{
			k_null_set = 0,
			k_rebuild_uniform			
		};
		vufRebuildCurveFn() {}
		virtual ~vufRebuildCurveFn() {}
		virtual uint8_t get_type() const = 0 { return k_null_set; }
		virtual void	log_me(int p_tab_count = 0) const = 0;
		virtual void	rebuild(std::shared_ptr < vufCurve<T, V>>) = 0;
		virtual T		get_curve_val_by_rebuilded(T p_val) const = 0;
		virtual T		get_rebuilded_val_by_curve(T p_val) const = 0;

		virtual std::shared_ptr< vufRebuildCurveFn<T, V>> make_copy() const = 0;
	};

#pragma endregion

#pragma region VF_CUrVE_REBUILD_FN
	template <class T, template<typename> class V>
	class vufRebuildUniformCurveFn :public vufRebuildCurveFn<T,V>
	{
	public:
		vufRebuildUniformCurveFn():vufRebuildCurveFn<T,V>(){}
		static std::shared_ptr<vufRebuildUniformCurveFn<T,V>> create()
		{
			return std::shared_ptr< vufRebuildUniformCurveFn>(new vufRebuildUniformCurveFn<T,V>);
		}
		virtual uint8_t get_type() const override
		{ 
			return vufRebuildCurveFn<T,V>::k_rebuild_uniform;
		}
		virtual void	log_me(int p_tab_count = 0) const override
		{
			std::string l_str_offset(p_tab_count * 4, '.');
			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_YELLOW, VF_CONSOLE_COLOR_BLACK);
			std::cout << std::fixed;
			std::cout << l_str_offset << "[ Rebuild Fn ]: " << std::endl;
			std::cout << l_str_offset << "division per segment: "	<< m_div_per_segment << std::endl;
			std::cout << l_str_offset << "m_total_division: "		<< m_total_div << std::endl;
			std::cout << l_str_offset << "curve length: "			<< m_curve_length << std::endl;
			std::cout << l_str_offset << "uniform to curve array: " << std::endl;
			std::cout << l_str_offset << "[ "; 
			for (auto i : m_uniform_to_curve_val_v)
			{
				std::cout << i << " ";
			}
			std::cout << "]" << std::endl;
			std::cout << l_str_offset << "curve to uniform array: " << std::endl;
			std::cout << l_str_offset << "[ ";
			for (auto i : m_curve_to_uniform_val_v)
			{
				std::cout << i << " ";
			}
			std::cout << "]" << std::endl;
			/*
			*/
			VF_CONSOLE_RESET_COLOR();

		}
		virtual void	rebuild(std::shared_ptr < vufCurve<T, V>> p_crv_ptr) override
		{
			if (p_crv_ptr == nullptr || p_crv_ptr->is_valid() == false)
			{
				return;
			}
			
			uint32_t l_sp_count = p_crv_ptr->get_span_count();
			m_curve_length		= 0;
			m_total_div			= l_sp_count * m_div_per_segment + 1;

			if (m_uniform_to_curve_val_v.size() != m_total_div)
			{
				m_uniform_to_curve_val_v.resize(m_total_div);
				m_curve_to_uniform_val_v.resize(m_total_div);
				m_curve_length_to_val_v.resize(m_total_div);
			}
			;
			V<T> l_vec_prev = p_crv_ptr->get_pos_at(0);
			m_uniform_to_curve_val_v[0]	= 0;
			m_curve_to_uniform_val_v[0]	= 0;
			m_curve_length_to_val_v[0]	= 0;
			// Compute curve length until sample point
			for (uint64_t i = 1; i < m_total_div; ++i)
			{
				T l_crv_val = ((T)i) / (T(m_total_div - 1));
				V<T> l_vec = p_crv_ptr->get_pos_at(l_crv_val);
				m_curve_length_to_val_v[i] = m_curve_length_to_val_v[i-1] + l_vec_prev.distance_to(l_vec);
				l_vec_prev = l_vec;
			}
			m_curve_length = m_curve_length_to_val_v.back();
			T l_u_1 = 0 , l_u_2, l_t_1 = 0, l_t_2;
			uint64_t l_ndx = 0;
			T l_step = 1. / (T(m_total_div - 1));
			T l_dnx = .0, l_tetta = .0;
			for (uint64_t i = 1; i < m_total_div; ++i)
			{
				l_u_2 = m_curve_length_to_val_v[i] / m_curve_length;
				m_curve_to_uniform_val_v[i] = l_u_2;

				l_t_2 = (T(i)) / (T(m_total_div-1));
				
				while (l_tetta <= l_u_2)
				{
					m_uniform_to_curve_val_v[l_ndx] = (l_t_1 * (l_u_2 - l_tetta) + l_t_2 * (l_tetta - l_u_1)) / (l_u_2 - l_u_1);
					l_ndx++;
					l_dnx++;
					l_tetta = l_dnx * l_step;
				}
				l_u_1 = l_u_2;
				l_t_1 = l_t_2;
			}
			/*
			*/
		}
		virtual T		get_curve_val_by_rebuilded(T p_val) const override
		{
			if (p_val < .0)
			{				
				return p_val;
			}
			if (p_val > 1.0)
			{
				return p_val;
			}
			int64_t l_sz = m_uniform_to_curve_val_v.size();
			p_val *= (T)(l_sz - 1);
			uint32_t l_ndx_1 = (int)(p_val);
			uint32_t l_ndx_2 = l_ndx_1 + 1;
			p_val -= std::floor(p_val);
			if (l_ndx_1 < 0)
			{				
				return 0;
			}
			if (l_ndx_2 > l_sz - 1)
			{
				return 1;
			}
			return  m_uniform_to_curve_val_v[l_ndx_1] * (1. - p_val) + m_uniform_to_curve_val_v[l_ndx_2] * p_val;

		}
		virtual T		get_rebuilded_val_by_curve(T p_val) const override
		{
			if (p_val < .0)
			{
				return p_val;
			}
			if (p_val > 1.0)
			{
				return p_val;
			}
			int64_t l_sz = m_curve_to_uniform_val_v.size();
			p_val *= (T)(l_sz - 1);
			uint32_t l_ndx_1 = (int)(p_val);
			uint32_t l_ndx_2 = l_ndx_1 + 1;
			p_val -= std::floor(p_val);
			if (l_ndx_1 < 0)
			{
				return 0;
			}
			if (l_ndx_2 > l_sz - 1)
			{
				return 1;
			}
			return  m_curve_to_uniform_val_v[l_ndx_1] * (1. - p_val) + m_curve_to_uniform_val_v[l_ndx_2] * p_val;

			return 0;
		}

		virtual std::shared_ptr< vufRebuildCurveFn<T, V>> make_copy() const 
		{
			auto l_ptr = create();
			l_ptr->m_div_per_segment	= m_div_per_segment;
			l_ptr->m_total_div			= m_total_div;
			l_ptr->m_curve_length		= m_curve_length;

			l_ptr->m_uniform_to_curve_val_v = m_uniform_to_curve_val_v;
			l_ptr->m_curve_to_uniform_val_v = m_curve_to_uniform_val_v;
			l_ptr->m_curve_length_to_val_v	= m_curve_length_to_val_v;
			return nullptr;
		}
		uint32_t m_div_per_segment	= 5;
		uint64_t m_total_div		= 0;
		T		 m_curve_length		= 0;

		std::vector<T>		m_uniform_to_curve_val_v;
		std::vector<T>		m_curve_to_uniform_val_v;
		std::vector<T>		m_curve_length_to_val_v;
	};
#pragma endregion
}

#endif // !VF_MATH_CRV_FN_H
