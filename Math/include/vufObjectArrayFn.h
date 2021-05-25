#ifndef VF_MATH_OBJ_ARRAY_FN_H
#define VF_MATH_OBJ_ARRAY_FN_H

#include <vufMathInclude.h>

namespace vufMath
{
#pragma region VUF_OBJECT_ARRAY_FN
	template<typename T, template <typename> class OBJ >
	class vufObjectArrayFn
	{
		vufObjectArrayFn() = delete;
	public:
		vufObjectArrayFn(std::vector<OBJ<T>>& p_vec) :m_obj_array_v(p_vec) {};
		vufObjectArrayFn(const vufObjectArrayFn& p_other) :m_obj_array_v(p_other.m_obj_array_v) {}

		std::string to_string()					const
		{
			VF_OBJECTS_ARRAY_TO_STRNG_AND_RETURN_IT(m_obj_array_v);
		}
		uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			VF_OBJECTS_ARRAY_FROM_STRING_AND_RETURN_OFFSET(p_str, p_offset, m_obj_array_v);
		}
		uint64_t	to_binary(std::vector<unsigned char>& p_buff)	const
		{
			VF_OBJECTS_ARRAY_TO_BINARY_AND_RETURN_SIZE(p_buff, m_obj_array_v);
		}
		uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)
		{
			VF_OBJECTS_ARRAY_FROM_BINARY_AND_RETURN_OFFSET(p_buff, p_offset, m_obj_array_v);
		}
		std::vector<OBJ<T>>& m_obj_array_v;
	};
#pragma endregion
}




#endif //!VF_MATH_OBJ_ARRAY_FN_H