#ifndef VF_MATH_UTILS_H
#define VF_MATH_UTILS_H

namespace vufMath
{
	class mathUtil
	{
	public:
		template<typename T>
		static int number_of_digits(T p_val)
		{
			if (p_val == 0 )
			{
				return 1;
			}
			int l_res = 1;
			while (true)
			{
				p_val = p_val / 10;
				if (p_val == 0)
				{
					return l_res;
				}
				l_res++;
			}
		}
	};
}

#endif //!VF_MATH_UTILS_H