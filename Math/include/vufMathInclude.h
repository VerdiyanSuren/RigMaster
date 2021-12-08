#ifndef VF_MATH_ICNLD_H
#define VF_MATH_ICNLD_H

#include <cstring>
#include <sstream>
//--------------------------------------------------------------------------------
//  CONSTATNTS
//--------------------------------------------------------------------------------
#define VF_MATH_VERSION 1
#define VF_MATH_PI 3.141592653589793238462643383279
#define VF_MATH_EPSILON	1e-12
//--------------------------------------------------------------------------------
// METHODS
//--------------------------------------------------------------------------------
#define VF_RESIZE_ARRAY_IF_DIFFERENT_SIZE(ARRAY, SZ) if (ARRAY.size() != SZ) ARRAY.resize(SZ);
#define VF_CLAMP( l_min, l_max, a)	a > l_max ? l_max : (a < l_min ? l_min : a)
#define VF_MAX(   a, b )		a > b ? a : b
#define VF_MIN(   a, b)			a < b ? a : b
#define VF_SIGN(  a )			a > 0 ? 1 : (a < 0 ? -1: 0)
#define VF_BIT(a)				(1 << a)
#define VF_ABS(a)				a < 0 ? -a:a
#define VF_FLOOR(x)				x > 0 ? (int)x:(int)x -1
#define VF_RAND(Length)			Length *(double)(rand() - RAND_MAX/2) / (double)(RAND_MAX/2)
//--------------------------------------------------------------------------------
// SERIALIZATION HELPERS
//--------------------------------------------------------------------------------
#define VF_FIND_FIRST_SYMBOL_POS(STR, OFFSET, SYMBOL)	\
	while (STR[OFFSET] != SYMBOL) 						\
	{													\
		++OFFSET;										\
	}

#define VF_STR_SKIP_WHITESPACES(STR,OFFSET)							\
	while (	OFFSET <  p_str.length() &&								\
	std::isspace(static_cast<unsigned char>(STR[OFFSET])) )			\
	{																\
		++OFFSET;													\
	}

//--------------------------------------------------------------------------------
#pragma region SERIALIZE_ROUTINE
#define VF_SAFE_CHECK_SIZE_AND_RETURN_IF_FAILED(BUFF, OFFSET, SIZE )	\
	if (BUFF.size() < OFFSET + SIZE) return 0;

#define VF_SAFE_READ_AND_RETURN_IF_FAILED(BUFF, OFFSET, DATA, SIZE )	\
	if (BUFF.size() < OFFSET + SIZE) return 0;							\
	std::memcpy(&DATA, &p_buff[p_offset], SIZE);						\
	OFFSET += SIZE;

#define VF_SAFE_WRITE_TO_BUFF(BUFF, OFFSET, DATA, SIZE )				\
	if (BUFF.size() < OFFSET + SIZE) BUFF.resize(OFFSET + SIZE);		\
	std::memcpy(&p_buff[p_offset], &DATA, SIZE);						\
	OFFSET += SIZE;

// encode for base
#define VF_ENCODE_FOR_BASE()										\
	uint64_t l_size = get_binary_size();							\
	std::vector<char> l_buff(l_size);								\
	to_binary(l_buff);												\
	vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);		\
	p_offset = l_serializer.encode_to_buff(p_buff, p_offset);		\
	return p_offset;

// decode for base
#define VF_DECODE_FOR_BASE()										\
	std::vector<char> l_buff;										\
	vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);		\
	p_offset = l_serializer.decode_from_buff(p_buff, p_offset);		\
	from_binary(l_buff);											\
	return p_offset;

#pragma endregion SERIALIZE_ROUTINE
//--------------------------------------------------------------------------------
#pragma region SERIALIZE_NUMERIC_ARRAY
// Set precission
#define VF_SET_PRECISION(SS,PRECISION)				\
	if (PRECISION > 0)								\
	{												\
		if (PRECISION > 64)							\
		{											\
			SS.precision(64);						\
		}											\
		else										\
		{											\
			SS.precision(PRECISION);				\
		}											\
	}

// Tab count
#define VF_GENERATE_TAB_COUNT(STR,COUNT,SYMBOL)		\
	STR = std::string(COUNT, SYMBOL);

// Serialize numeric
// Write numeric to string
#define VF_NUMERIC_ARRAY_TO_STRING(SS,ARRAY)	\
{												\
	uint64_t l_sz = ARRAY.size();				\
	SS << "[[" << l_sz << ']';					\
	if (l_sz > 0)								\
	{											\
		l_ss << ARRAY[0];						\
	}											\
	for (uint64_t i = 1; i < l_sz; ++i)			\
	{											\
		l_ss << "," << ARRAY[i];				\
	}											\
	l_ss << ']';								\
}

// Read numeric from string 
#define VF_NUMERIC_ARRAY_FROM_STRING_AND_RETURN_OFFSET( STR, OFFSET, ARRAY)								\
	uint64_t l_str_pos = OFFSET;																		\
	uint64_t l_vec_sz = 0;																				\
	try																									\
	{																									\
			VF_FIND_FIRST_SYMBOL_POS(STR, l_str_pos, '[');												\
			VF_FIND_FIRST_SYMBOL_POS(STR, ++l_str_pos, '[');											\
			std::stringstream l_ss;																		\
			++l_str_pos;																				\
			while (STR[l_str_pos] != ']')	/*try to read size*/										\
			{																							\
				l_ss << STR[l_str_pos++];																\
			}																							\
			l_ss >> l_vec_sz;																			\
			++l_str_pos;																				\
			ARRAY.resize(l_vec_sz);																		\
			if ( l_vec_sz == 0) /* nothing to do else, our vector has zero length*/						\
			{																							\
				VF_FIND_FIRST_SYMBOL_POS(STR, l_str_pos, ']');											\
				return l_str_pos + 1;																	\
			}																							\
			for (uint64_t i = 0; i < l_vec_sz; ++i)	/* read array*/										\
			{																							\
				l_ss.clear();																			\
				while (STR[l_str_pos] !=',' && STR[l_str_pos] != ']')									\
				{																						\
					l_ss << STR[l_str_pos++];															\
				}																						\
				T l_val;																				\
				l_ss >> l_val;																			\
				ARRAY[i] = l_val;																		\
				++l_str_pos;																			\
			}																							\
			--l_str_pos;																				\
			VF_FIND_FIRST_SYMBOL_POS(STR,l_str_pos,']');												\
			return l_str_pos + 1;																		\
	}																									\
	catch (const std::exception& l_err)																	\
	{																									\
		std::cout << "Error: " << l_err.what() << __LINE__ << " " << __FILE__ << std::endl;				\
		return OFFSET;																					\
	}																									\
	return OFFSET;

// Write Numeric to Binary
#define VF_NUMERIC_ARRAY_TO_BINARY_AND_RETURN_OFFSET(BUFF, ARRAY)				\
	uint64_t l_sz = ARRAY.size();												\
	uint64_t l_data_sz = sizeof(uint64_t) + l_sz * sizeof(T);					\
	uint64_t l_old_buff_sz = BUFF.size();										\
	BUFF.resize(l_old_buff_sz + l_data_sz);										\
	/*write size*/																\
	uint64_t l_offset = l_old_buff_sz;											\
	std::memcpy( (void*)&BUFF[l_offset],(const void*)&l_sz, sizeof(uint64_t));	\
	l_offset += sizeof(uint64_t);												\
	/*write vectors*/															\
	if (l_sz != 0)																\
	{																			\
		for (uint64_t i = 0; i < l_sz; ++i)										\
		{																		\
			T l_val = ARRAY[i];													\
			unsigned char* l_ptr = (unsigned char*)&l_val;						\
			for (uint64_t j = 0; j < sizeof(T); ++j)							\
			{																	\
				p_buff[l_offset++] = l_ptr[j];									\
			}																	\
		}																		\
	}																			\
	return sizeof(uint64_t) + l_sz*sizeof(T);

// Read Numeric From Binary
#define VF_NUMERIC_ARRAY_FROM_BINARY_AND_RETURN_OFFSET(BUFF, OFFSET, ARRAY)		\
	uint64_t l_sz;																\
	uint64_t l_buff_offset = OFFSET;											\
	/* Read size of vector */													\
	std::memcpy( (void*)&l_sz, (const void*)&BUFF[l_buff_offset], sizeof(uint64_t));	\
	l_buff_offset += 	sizeof(uint64_t);										\
	ARRAY.resize(l_sz);															\
	if (l_sz == 0)																\
	{																			\
		return l_buff_offset;													\
	}																			\
	for (uint64_t i = 0; i < l_sz; ++i)											\
	{																			\
		T l_val;																\
		unsigned char* l_ptr = (unsigned char*)&l_val;							\
		for (uint64_t j = 0; j < sizeof(T); ++j)								\
		{																		\
			l_ptr[j] = BUFF[l_buff_offset++];									\
		}																		\
		ARRAY[i] = l_val;														\
	}																			\
	return l_buff_offset;

#pragma endregion SERIALIZE_NUMERIC_ARRAY
//--------------------------------------------------------------------------------
#pragma region SERIALIZE OBJECT_ARRAY
// Serialize Object
// Write Object to string
#define VF_OBJECTS_ARRAY_TO_STRNG_AND_RETURN_IT(ARRAY)	\
	uint64_t l_sz = ARRAY.size();						\
	std::stringstream l_ss;								\
	l_ss.precision(64);									\
	l_ss << "[[" << l_sz << ']';						\
	for (uint64_t i = 0; i < l_sz; ++i)					\
	{													\
		l_ss << ARRAY[i].to_string();					\
	}													\
	l_ss << ']';										\
	return l_ss.str()

// Read Object from string
#define VF_OBJECTS_ARRAY_FROM_STRING_AND_RETURN_OFFSET( STR, OFFSET, ARRAY)								\
	uint64_t l_str_pos = OFFSET;																		\
	uint64_t l_vec_sz = 0;																				\
	try																									\
	{																									\
			VF_FIND_FIRST_SYMBOL_POS(STR, l_str_pos, '[');												\
			VF_FIND_FIRST_SYMBOL_POS(STR, ++l_str_pos, '[');											\
			std::stringstream l_ss;																		\
			++l_str_pos;																				\
			while (STR[l_str_pos] != ']')	/*try to read size*/										\
			{																							\
				l_ss << STR[l_str_pos++];																\
			}																							\
			l_ss >> l_vec_sz;																			\
			++l_str_pos;																				\
			ARRAY.resize(l_vec_sz);																		\
			if ( l_vec_sz == 0) /* nothing to do else, our vector has zero length*/						\
			{																							\
				return l_str_pos + 1;																	\
			}																							\
			for (uint64_t i = 0; i < l_vec_sz; ++i)	/* read array*/										\
			{																							\
				l_str_pos = ARRAY[i].from_string(STR, l_str_pos);										\
			}																							\
			VF_FIND_FIRST_SYMBOL_POS(STR,l_str_pos,']');												\
			return l_str_pos + 1;																		\
	}																									\
	catch (const std::exception& l_err)																	\
	{																									\
		std::cout << "Error: " << l_err.what() << __LINE__ << " " << __FILE__ << std::endl;				\
		return OFFSET;																					\
	}																									\
	return OFFSET;

#define VF_OBJECTS_ARRAY_TO_BINARY_AND_RETURN_SIZE(BUFF, ARRAY)						\
	uint64_t l_sz = ARRAY.size();													\
	uint64_t l_data_sz = sizeof(uint64_t);											\
	uint64_t l_old_buff_sz = BUFF.size();											\
	/* Write size */																\
	BUFF.resize(l_old_buff_sz + sizeof(uint64_t));									\
	std::memcpy((void*)&BUFF[l_old_buff_sz], (const void*)&l_sz, sizeof(uint64_t));	\
	/* wrrite vectors */															\
	for (uint64_t i = 0; i < l_sz; ++i)												\
	{																				\
		ARRAY[i].to_binary(p_buff);													\
	}																				\
	return p_buff.size() - l_old_buff_sz;

#define VF_OBJECTS_ARRAY_FROM_BINARY_AND_RETURN_OFFSET(BUFF, OFFSET, ARRAY)			\
	uint64_t l_sz;																	\
	uint64_t l_buff_offset = OFFSET;												\
	/* read size of vector */														\
	unsigned char* l_uint = (unsigned char*)&l_sz;									\
	std::memcpy((void*)&l_sz, (const void*)&BUFF[l_buff_offset], sizeof(uint64_t));	\
	ARRAY.resize(l_sz);																\
	l_buff_offset += sizeof(uint64_t);												\
	/* read vector */																\
	for (uint64_t i = 0; i < l_sz; ++i)												\
	{																				\
		l_buff_offset = ARRAY[i].from_binary(p_buff, l_buff_offset);				\
	}																				\
	return l_buff_offset;

#pragma endregion OBJECT_ARRAY

//--------------------------------------------------------------------------------
// CONSOLE HELPER
//--------------------------------------------------------------------------------

#pragma endregion
#endif // !VF_MATH_ICNLD_H
