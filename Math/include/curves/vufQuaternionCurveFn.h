#ifndef VF_MATH_CRV_QTRN_RBLD_FN_H
#define VF_MATH_CRV_QTRN_RBLD_FN_H

#include <curves/vufCurve.h>
#include <vufQuaternion.h>

namespace vufMath
{
	template <class T, template<typename> class V>
	class vufQuaternionCloseCurveFn;

#pragma region VF_CURVE_FN_BASE
	// functions set to work with specific part 
	template <class T, template<typename> class V>
	class vufQuaternionCurveFn
	{
	public:
		enum
		{
			k_null_set = 0,
			k_quaterernion_close_fn,
			k_quarernion_curve_param_fn
		};
		vufQuaternionCurveFn() {}
		virtual ~vufQuaternionCurveFn() {}
		virtual uint8_t get_type() const = 0 { return k_null_set; }
		virtual void	log_me(int p_tab_count = 0) const = 0;
		virtual void	set_item_count(uint32_t p_count) = 0;
		virtual void	set_item_at(uint32_t p_index, const V<T> p_pos, const vufQuaternion<T>& p_quat) = 0;
		virtual void	compute(std::shared_ptr < vufCurve<T, V>>) = 0;

		virtual vufQuaternion<T> get_quaternion_at(T p_val) const = 0;

		virtual std::shared_ptr< vufQuaternionCurveFn<T, V>> make_copy() const = 0;
	};
#pragma endregion
#pragma region VF_QUATERNION_CLOSE_FN
	template <class T, template<typename> class V>
	class vufQuaternionCloseCurveFn : public vufQuaternionCurveFn<T, V>
	{
	public:
		vufQuaternionCloseCurveFn() :vufQuaternionCurveFn<T, V>() {}
		static std::shared_ptr<vufQuaternionCloseCurveFn<T, V>> create()
		{
			return std::shared_ptr< vufQuaternionCloseCurveFn>(new vufQuaternionCloseCurveFn<T, V>());
		}
		void set_pin_to_start(bool p_val)
		{
			m_pin_to_start = p_val;
		}
		bool get_pin_to_start() const
		{
			return m_pin_to_start;
		}
		void set_pin_to_end(bool p_val)
		{
			m_pin_to_end = p_val;
		}
		bool get_pin_to_end() const
		{
			return m_pin_to_end;
		}

		virtual uint8_t get_type() const override{ return vufQuaternionCurveFn<T,V>::k_quaterernion_close_fn; }
		virtual void	log_me(int p_tab_count = 0) const override
		{
			std::string l_str_offset(p_tab_count * 4, '.');
			VF_CONSOLE_SET_COLOR(VF_CONSOLE_COLOR_GREEN, VF_CONSOLE_COLOR_BLACK);
			std::cout << std::fixed;
			std::cout << l_str_offset << "[ Quaternion CLose Fn ]: " << std::endl;

			VF_CONSOLE_RESET_COLOR();
		}
		virtual void	set_item_count(uint32_t p_count) override
		{
			if (m_positon_v.size() != p_count)
			{
				m_positon_v.resize(p_count);
				m_quaternion_v.resize(p_count);
				m_quat_parem_v.resize(p_count);
			}
		}
		virtual void	set_item_at(uint32_t p_index, const V<T> p_pos, const vufQuaternion<T>& p_quat) override
		{
			m_positon_v[p_index]	= p_pos;
			m_quaternion_v[p_index]	= p_quat;
		}
		virtual void	compute(std::shared_ptr < vufCurve<T, V>>) override
		{
			uint64_t l_start = 0, l_end = m_quat_parem_v.size();
			if (m_pin_to_start == true)
			{
				m_quat_parem_v[0] = 0;
				++l_start;
			}
			if (m_pin_to_end == true)
			{
				m_quat_parem_v.back() = 1.;
				--l_end;
			}
			//--------------------------------
			for (uint64_t i = l_start; i < l_end; ++i)
			{

			}
		}
		
		virtual vufQuaternion<T> get_quaternion_at(T p_val) const override
		{
			return vufQuaternion<T>();
		}

		virtual std::shared_ptr< vufQuaternionCurveFn<T, V>> make_copy() const override
		{
			return nullptr;
		}

	private:
		bool	m_pin_to_start	= false;
		bool	m_pin_to_end	= false;
		std::vector< V<T>>				m_positon_v;
		std::vector< vufQuaternion<T>>	m_quaternion_v;
		std::vector<T>					m_quat_parem_v;
	};

#pragma endregion
}
#endif // !VF_MATH_CRV_QTRN_RBLD_FN_H
