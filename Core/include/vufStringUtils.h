#ifndef VF_CORE_STRING_UTILS_H
#define VF_CORE_STRING_UTILS_H

#include <vufCoreInclude.h>

namespace vuf
{
	template<typename T = VF_STRING>
	class vufStringUtils
	{
	public:
		static uint64_t		get_hash_of(const T& p_str)
		{
			std::hash <T> l_hasher;
			return l_hasher(p_str);
		}

	};
}
#endif // !VF_CORE_STRING_UTILS_H
