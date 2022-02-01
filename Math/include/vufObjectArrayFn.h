#ifndef VF_MATH_OBJ_ARRAY_FN_H
#define VF_MATH_OBJ_ARRAY_FN_H

#include <cctype>
#include <vufMathInclude.h>
#include <memory>

namespace vufMath
{
#pragma region VUF_OBJECT_ARRAY_FN
	//---------------------------------------------------------------------------------------------------------
	//									VF OBJECT ARRAY FN (BASE CLASS)
	//---------------------------------------------------------------------------------------------------------

	class vufObjectArrayFn
	{
	public:
		virtual	std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0, bool p_multiline = false)	const	= 0;
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0)					= 0;

		virtual uint64_t	get_binary_size()														const	= 0;
		virtual uint64_t	to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)				const	= 0;
		virtual uint64_t	from_binary(const std::vector<char>&p_buff, uint64_t p_offset = 0)				= 0;
		virtual uint64_t	encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)		const	= 0;
		virtual uint64_t	decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)				= 0;
	};
	//---------------------------------------------------------------------------------------------------------
	//											VF ARRAY FN
	//---------------------------------------------------------------------------------------------------------
	/*
	* Examples
	* std::vector<int> l_int_array(10);
	* for (int i = 0; i < l_int_array.size(); ++i) l_int_array[i] = i;
	* vufMath::vufArrayFn<int> l_int_fn(l_int_array);
	* std::vector<int> l_int_readed_array;
	*/
	template<typename T>
	//typename		std::enable_if<std::is_arithmetic<T>::value,T>::type
	class vufArrayFn:public vufObjectArrayFn
	{
		template< bool cond, typename U >
		using resolvedType = typename std::enable_if_t<cond, U >;
		static constexpr bool p_state = std::is_arithmetic_v<T> == true;
		vufArrayFn() = delete;
	public:
		vufArrayFn(std::vector<T>& p_vec) :m_array_v(p_vec) {};
		vufArrayFn(const vufObjectArrayFn& p_other) :m_array_v(p_other.m_array_v) {}
		// to string 
		virtual	std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0, bool p_multiline = false)	const override
		{
			return to_string_i(p_precision, p_tab_count, p_multiline);
			//VF_OBJECTS_ARRAY_TO_STRNG_AND_RETURN_IT(m_obj_array_v);
		}
		// if failed return the same offset (p_offset)
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) override
		{
			return from_string_i(p_str, p_offset);
			//VF_OBJECTS_ARRAY_FROM_STRING_AND_RETURN_OFFSET(p_str, p_offset, m_obj_array_v);
		}
		
		virtual uint64_t	get_binary_size() const override
		{
			return get_binary_size_i();
		}
		virtual uint64_t	to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)			const override
		{
			return to_binary_i(p_buff, p_offset);
			//VF_OBJECTS_ARRAY_TO_BINARY_AND_RETURN_SIZE(p_buff, m_obj_array_v);
		}
		virtual uint64_t	from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0) override
		{
			return from_binary_i(p_buff, p_offset);
			//VF_OBJECTS_ARRAY_FROM_BINARY_AND_RETURN_OFFSET(p_buff, p_offset, m_obj_array_v);
		}
		virtual uint64_t	encode_to_buff(std::vector< char>& p_buff, uint64_t p_offset = 0)	const override
		{
			uint64_t l_size = get_binary_size();
			std::vector<char> l_buff(l_size);
			uint64_t l_offset = to_binary(l_buff);
			if (l_offset == 0)
			{
				return p_offset;
			}
			vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);
			l_offset = l_serializer.encode_to_buff(p_buff, p_offset);
			return l_offset;
		}
		virtual uint64_t	decode_from_buff(std::vector< char>& p_buff, uint64_t p_offset = 0) override
		{			
			std::vector<char> l_buff;
			vuf::txtStdVectorSerializerFn<char> l_serializer(l_buff);
			p_offset = l_serializer.decode_from_buff(p_buff, p_offset);
			from_binary(l_buff);
			return p_offset;
		}
	private:
		//---------------------------------------------------------------
		// arithmetic
		template <typename  U = T >
		std::string to_string_i(int p_precision = -1, uint32_t p_tab_count = 0, bool p_multiline = false, resolvedType<p_state,U>* = nullptr)	const
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_SET_PRECISION(l_ss, p_precision);
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '\t');
			uint64_t l_sz = m_array_v.size();
			l_ss << l_str_offset << "[[" << l_sz << ']';
			if (l_sz > 0)
			{
				l_ss << m_array_v[0];
			}
			for (uint64_t i = 1; i < l_sz; ++i)
			{
				l_ss << "," << m_array_v[i];
			}
			l_ss << ']';
			return l_ss.str();
		}
		template <typename  U = T >
		uint64_t	from_string_i(const std::string& p_str, uint64_t p_offset = 0, resolvedType<p_state, U>* = nullptr)
		{
			//declare temp vars
			uint64_t l_str_pos = p_offset;
			uint64_t l_vec_sz = 0;
			
			// skip first two [[
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != '[') return p_offset;
			l_str_pos++;
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != '[') return p_offset;
			l_str_pos++;

			// try to read size
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			std::stringstream l_ss;
			while (l_str_pos <  p_str.length() && 
								std::isdigit( p_str[l_str_pos]) )
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> l_vec_sz;
			if (l_ss.fail() == true) return p_offset;
			m_array_v.resize(l_vec_sz);

			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != ']') return p_offset;
			++l_str_pos;

			if (l_vec_sz == 0)
			{
				VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
				if (p_str[l_str_pos] != ']') return p_offset;
				return l_str_pos + 1;
			}
			// read array elements	
			for (uint64_t i = 0; i < l_vec_sz -1; ++i)	/* read array*/	
			{
				VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
				l_ss.clear();
				l_ss.str("");
				while	(l_str_pos < p_str.length() && p_str[l_str_pos] != ',' )
				{
					l_ss << p_str[l_str_pos++];
				}
				//std::cout << "_______" << l_ss.str() << std::endl;
				l_ss >> m_array_v[i];
				if (l_ss.fail() == true) return p_offset;
				VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
				if (p_str[l_str_pos] != ',') return p_offset;
				++l_str_pos;
			}
			// read last element in array
			l_ss.clear();
			l_ss.str("");
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			while (l_str_pos < p_str.length() && p_str[l_str_pos] != ']')
			{
				l_ss << p_str[l_str_pos++];
			}
			//std::cout << "_______" << l_ss.str() << std::endl;
			l_ss >> m_array_v.back();
			if (l_ss.fail() == true) return p_offset;

			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != ']')  return p_offset;
			return l_str_pos + 1;
		}
		template <typename  U = T >
		uint64_t	get_binary_size_i(resolvedType<p_state, U>* = nullptr) const
		{
			uint64_t l_sz = sizeof(uint64_t);
			l_sz += m_array_v.size() * sizeof(T);
			return l_sz;
		}
		template <typename  U = T >
		uint64_t	to_binary_i(std::vector<char>& p_buff, uint64_t p_offset = 0, resolvedType<p_state, U>* = nullptr)	const
		{
			uint64_t l_sz = m_array_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_sz, sizeof(l_sz));
			if (l_sz > 0)
			{
				VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, m_array_v[0], l_sz * sizeof(T));
			}
			return p_offset;
		}
		template <typename  U = T >
		uint64_t	from_binary_i(const std::vector<char>& p_buff, uint64_t p_offset = 0,  resolvedType<p_state, U>* = nullptr)
		{
			uint64_t l_sz = 0;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_sz, sizeof(l_sz));
			m_array_v.resize(l_sz);
			if (l_sz > 0)
			{
				VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, m_array_v[0], l_sz *sizeof(T));
			}
			return p_offset;
		}
		//---------------------------------------------------------------
		// object
		template <typename  U = T >
		std::string to_string_i(int p_precision = -1, uint32_t p_tab_count = 0, bool p_multiline = false, resolvedType<!p_state, U>* = nullptr)	const
		{
			std::stringstream l_ss;
			std::string l_str_offset;
			VF_GENERATE_TAB_COUNT(l_str_offset, p_tab_count, '\t');
			uint64_t l_sz = m_array_v.size();
			l_ss << l_str_offset << "[[" << l_sz << ']';
			if (p_multiline == true)
			{
				l_ss << std::endl;
			}
			else
			{
				p_tab_count = 0;
			}
			if (l_sz == 0) 
			{
				l_ss << ']';
				return l_ss.str();
			}
			for (uint64_t i = 0; i < l_sz -1; ++i)	
			{
				l_ss << m_array_v[i].to_string(p_precision, p_tab_count, p_multiline);
				l_ss << ',';
				if (p_multiline == true) l_ss << std::endl;
			}
			l_ss << m_array_v.back().to_string(p_precision, p_tab_count, p_multiline);
			if (p_multiline == true) l_ss << std::endl << l_str_offset;
			l_ss << ']';
			return l_ss.str();
		}
		template <typename  U = T >
		uint64_t	from_string_i(const std::string& p_str, uint64_t p_offset = 0, resolvedType<!p_state, U>* = nullptr)
		{
			//declare temp vars
			uint64_t l_str_pos = p_offset;
			uint64_t l_vec_sz = 0;

			// skip first two [[
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != '[') return p_offset;
			l_str_pos++;
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != '[') return p_offset;
			l_str_pos++;

			// try to read size
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			std::stringstream l_ss;
			while (l_str_pos < p_str.length() &&
				std::isdigit(p_str[l_str_pos]))
			{
				l_ss << p_str[l_str_pos++];
			}
			l_ss >> l_vec_sz;
			if (l_ss.fail() == true) return p_offset;
			m_array_v.resize(l_vec_sz);

			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != ']') return p_offset;
			++l_str_pos;

			if (l_vec_sz == 0)
			{
				VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
				if (p_str[l_str_pos] != ']') return p_offset;
				return l_str_pos + 1;
			}

			// read array elements	
			for (uint64_t i = 0; i < l_vec_sz - 1; ++i)	/* read array*/
			{
				VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
				l_str_pos = m_array_v[i].from_string(p_str, l_str_pos);
				VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
				if (p_str[l_str_pos] != ',') return p_offset;
				++l_str_pos;
			}
			// read last element in array
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			l_str_pos = m_array_v.back().from_string(p_str, l_str_pos);
			VF_STR_SKIP_WHITESPACES(p_str, l_str_pos);
			if (p_str[l_str_pos] != ']')  return p_offset;
			return l_str_pos + 1;
			return 0;
		}
		template <typename  U = T >
		uint64_t	get_binary_size_i(resolvedType<!p_state, U>* = nullptr) const
		{
			uint64_t l_sz = sizeof(uint64_t);
			for (uint64_t i = 0; i < m_array_v.size(); i++)
			{
				l_sz += m_array_v[i].get_binary_size();
			}
			return l_sz;
		}
		template <typename  U = T >
		uint64_t	to_binary_i(std::vector<char>& p_buff, uint64_t p_offset = 0, resolvedType<!p_state, U>* = nullptr)	const
		{
			uint64_t l_sz = m_array_v.size();
			VF_SAFE_WRITE_TO_BUFF(p_buff, p_offset, l_sz, sizeof(l_sz));
			for (uint64_t i = 0; i < m_array_v.size(); i++)
			{
				p_offset = m_array_v[i].to_binary(p_buff, p_offset);
			}
			return p_offset;
		}
		template <typename  U = T >
		uint64_t	from_binary_i(const std::vector<char>& p_buff, uint64_t p_offset = 0,  resolvedType<!p_state, U>* = nullptr)
		{
			uint64_t l_sz = 0;
			VF_SAFE_READ_AND_RETURN_IF_FAILED(p_buff, p_offset, l_sz, sizeof(l_sz));
			m_array_v.resize(l_sz);
			for (uint64_t i = 0; i < l_sz; i++)
			{
				p_offset = m_array_v[i].from_binary(p_buff, p_offset);
			}
			return p_offset;
		}
	private:
		std::vector<T>& m_array_v;
	};
	//---------------------------------------------------------------------------------------------------------
	//										VF OBJECT ARRAY
	//---------------------------------------------------------------------------------------------------------
	template<typename T, 
			template<typename> class V = vufArrayFn>
	class vufObjectArray
	{
	public:
		vufObjectArray()	{}
		vufObjectArray(const vufObjectArray& p_other)
		{
			m_array_v = p_other.m_array_v;
		}
		~vufObjectArray()	{}
		static std::shared_ptr <vufObjectArray<T,V> > create()
		{
			return std::shared_ptr< vufObjectArray<T,V> >(new vufObjectArray<T, V>());
		}
		std::shared_ptr<vufObjectArray> get_copy() const
		{
			auto l_ptr = vufObjectArray<T,V>::create();
			l_ptr->m_array_v = m_array_v;
			return l_ptr;
		}

		std::string	to_string(int p_precision = -1, uint32_t p_tab_count = 0, bool p_multiline = false)	const
		{
			V<T> l_fn(m_array_v);
			return l_fn.to_string(p_precision, p_tab_count, p_multiline);
		}
		uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0)
		{
			V<T> l_fn(m_array_v);
			return l_fn.from_string(p_str, p_offset);
		}
		uint64_t	get_binary_size()														const
		{
			V<T> l_fn(m_array_v);
			return l_fn.get_binary_size();
		}
		// TO Do 
		// this method has to be const. fix it later
		uint64_t	to_binary(std::vector<char>& p_buff, uint64_t p_offset = 0)	//			const
		{
			V<T> l_fn(m_array_v);
			return l_fn.to_binary(p_buff, p_offset);
		}
		uint64_t	from_binary(const std::vector<char>& p_buff, uint64_t p_offset = 0)
		{
			V<T> l_fn(m_array_v);
			return l_fn.from_binary(p_buff, p_offset);
		}
		// TO Do 
		// this method has to be const. fix it later
		uint64_t	encode_to_buff(std::vector<char>& p_buff, uint64_t p_offset = 0)//		const
		{
			V<T> l_fn(m_array_v);
			return l_fn.encode_to_buff(p_buff, p_offset);
		}
		uint64_t	decode_from_buff(std::vector<char>& p_buff, uint64_t p_offset = 0)
		{
			V<T> l_fn(m_array_v);
			return l_fn.decode_from_buff(p_buff, p_offset);
		}
	//private:
		std::vector<T> m_array_v;
	};
#pragma endregion
}




#endif //!VF_MATH_OBJ_ARRAY_FN_H