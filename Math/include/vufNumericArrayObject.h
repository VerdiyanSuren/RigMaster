#ifndef VF_NUMERIC_ARRAY_H
#define VF_NUMERIC_ARRAY_H

#include <vufObject.h>
#include <vufObjectArrayFn.h>

#include <sstream>

namespace vufMath
{
#pragma region VUF_FLOAT_ARRAY_OBJECT
	template<typename T>
	class vufNumericArrayObject : public vufObject
	{
	public:
		vufNumericArrayObject() {}
		vufNumericArrayObject(const vufNumericArrayObject& p_other)	:vufObject(),m_numeric_array_v(p_other.m_numeric_array_v){}
		vufNumericArrayObject(const std::vector<T>& p_vec): vufObject(), 	m_numeric_array_v(p_vec) {}
	public:
		std::shared_ptr < vufNumericArrayObject<T> >	creator()
		{
			std::shared_ptr<vufNumericArrayObject<T>> l_ptr =
				std::shared_ptr< vufNumericArrayObject<T>>(new vufNumericArrayObject<T>());
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}
		virtual std::shared_ptr<vufObject>				make_copy() const override
		{
			std::shared_ptr<vufNumericArrayObject<T>> l_ptr = std::shared_ptr< vufNumericArrayObject<T>>(new vufNumericArrayObject<T>(*this));
			l_ptr->m_this = l_ptr;
			return l_ptr;
		}

		virtual uint64_t								get_detailed_count()				const override
		{
			return 0;
		}
		virtual std::shared_ptr<vufObject>				get_detailed_at(uint64_t p_index)	const override
		{
			return nullptr;
		}
		virtual bool		is_array()					const override
		{
			return true;
		}
		virtual bool		is_numeric()				const override
		{
			return true;
		}

		virtual std::string to_string()					const override
		{
			VF_NUMERIC_ARRAY_TO_STRING_AND_RETURN_IT(m_numeric_array_v);
		}
		virtual uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) override
		{			
			VF_NUMERIC_ARRAY_FROM_STRING_AND_RETURN_OFFSET(p_str, p_offset, m_numeric_array_v);
			/*
			uint64_t l_str_pos = p_offset;
			uint64_t l_vec_sz = 0;
			try
			{
				VF_FIND_FIRST_SYMBOL_POS(p_str, l_str_pos, '[');
				VF_FIND_FIRST_SYMBOL_POS(p_str, ++l_str_pos, '[');
				std::stringstream l_ss;	
				++l_str_pos;
				while (p_str[l_str_pos] != ']')	//try to read size
				{
					l_ss << p_str[l_str_pos++];
				}
				l_ss >> l_vec_sz;
				++l_str_pos;
				std::cout << "SIZE: " << l_vec_sz << std::endl;
				m_numeric_array_v.resize(l_vec_sz);
				if (l_vec_sz == 0) // nothing to do else, our vector has zero length
				{
					VF_FIND_FIRST_SYMBOL_POS(p_str, l_str_pos, ']');
					return l_str_pos + 1;
				}
				for (uint64_t i = 0; i < l_vec_sz; ++i)	// read array
				{
					l_ss.clear();
					while (p_str[l_str_pos] != ',' && p_str[l_str_pos] != ']')
					{												
						l_ss << p_str[l_str_pos++];
					}
					T l_val;
					l_ss >> l_val;
					m_numeric_array_v[i] = l_val;
					++l_str_pos;
				}													
				//VF_FIND_FIRST_SYMBOL_POS(p_str, l_str_pos, ']');
				return l_str_pos + 1;						
			}																
			catch (const std::exception& l_err)							
			{	
				std::cout << "Error: " << l_err.what() << __LINE__ << " " << __FILE__ << std::endl;
				return p_offset;
			}																					
			return p_offset;
			*/
			return p_offset;
		}
		virtual uint64_t	to_binary(std::vector<unsigned char>& p_buff)	const override
		{
			VF_NUMERIC_ARRAY_TO_BINARY_AND_RETURN_OFFSET(p_buff, m_numeric_array_v);
			/*
			uint64_t l_sz = m_numeric_array_v.size();
			uint64_t l_data_sz = sizeof(uint64_t) + l_sz * sizeof(T);
			uint64_t l_old_buff_sz = p_buff.size();
			p_buff.resize(l_old_buff_sz + l_data_sz);
			// write size
			uint64_t l_offset = l_old_buff_sz;
			std::memcpy((void*)&l_sz,(const void*)&p_buff[l_offset], sizeof(uint64_t));
			l_offset += sizeof(uint64_t);
			// write vectors
			if (l_sz != 0)
			{
				for (uint64_t i = 0; i < l_sz; ++i)
				{
					T l_val = m_numeric_array_v[i];
					unsigned char* l_ptr = (unsigned char*)&l_val;
					for (uint64_t j = 0; j < sizeof(T); ++j)
					{
						p_buff[l_offset++] = l_ptr[j];

					}
				}
			}
			return  l_offset;
			*/
		}
		virtual uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)  override
		{
			VF_NUMERIC_ARRAY_FROM_BINARY_AND_RETURN_OFFSET(p_buff, p_offset, m_numeric_array_v);
			/*
			uint64_t l_sz;															
			uint64_t l_buff_offset = p_offset;
			// Read size of vector												
			std::memcpy((void*)&l_sz, (void*)&p_buff[l_buff_offset], sizeof(uint64_t));
			l_buff_offset += sizeof(uint64_t);										
			m_numeric_array_v.resize(l_sz);
			if (l_sz == 0)															
			{																		
				return l_buff_offset;												
			}
			for (uint64_t i = 0; i < l_sz; ++i)
			{
				T l_val;
				unsigned char* l_ptr = (unsigned char*)&l_val;
				for (uint64_t j = 0; j < sizeof(T); ++j)
				{
					//std::memcpy((void*)l_x, (void*)&p_buff[l_buff_offset], l_sz * sizeof(T));
					l_ptr[j] = p_buff[l_buff_offset++];
				}
				m_numeric_array_v[i] = l_val;
			}
			return l_buff_offset;// +l_sz * sizeof(T);
			*/
		}

		virtual vufObjectType get_type()	const override
		{
			return vufObjectType::k_null_object;
		}

		virtual std::shared_ptr < vufNumericArrayObject<float> >	as_float_array_object()		const override { return nullptr; }
		virtual std::shared_ptr < vufNumericArrayObject<double> >	as_double_array_object()	const override { return nullptr; }
		virtual std::shared_ptr < vufNumericArrayObject<int> >		as_int_array_object()		const override { return nullptr; }
		virtual std::shared_ptr < vufNumericArrayObject<bool> >		as_bool_array_object()		const override { return nullptr; }


		std::vector<T> m_numeric_array_v;
	};
	// <float> specialization  
	template<>
	vufObjectType	vufNumericArrayObject<float>::get_type()	const
	{
		return vufObjectType::k_float_array_object;
	}
	template<>
	std::shared_ptr< vufNumericArrayObject<float> >		vufNumericArrayObject<float>::as_float_array_object()	const
	{
		return std::static_pointer_cast<vufNumericArrayObject<float>>(vufObject::m_this.lock());
	}
	// < double> specialization  
	template<>
	vufObjectType	vufNumericArrayObject<double>::get_type()	const
	{
		return vufObjectType::k_double_array_object;
	}
	template<>
	std::shared_ptr< vufNumericArrayObject<double> >		vufNumericArrayObject<double>::as_double_array_object()	const
	{
		return std::static_pointer_cast<vufNumericArrayObject<double>>(vufObject::m_this.lock());
	}
	// <int> specialization  
	template<>
	vufObjectType	vufNumericArrayObject<int>::get_type()	const
	{
		return vufObjectType::k_int_array_object;
	}
	template<>
	std::shared_ptr< vufNumericArrayObject<int> >		vufNumericArrayObject<int>::as_int_array_object()	const
	{
		return std::static_pointer_cast<vufNumericArrayObject<int>>(vufObject::m_this.lock());
	}
	// <bool> specialization  
	template<>
	vufObjectType	vufNumericArrayObject<bool>::get_type()	const
	{
		return vufObjectType::k_bool_array_object;
	}
	template<>
	std::shared_ptr< vufNumericArrayObject<bool> >		vufNumericArrayObject<bool>::as_bool_array_object()	const
	{
		return std::static_pointer_cast<vufNumericArrayObject<bool>>(vufObject::m_this.lock());
	}

#pragma endregion VUF_FLOAT_ARRAY_OBJECT
#pragma region VF_NUMERIC_ARRAY_FN
	template<typename T>
	class vufNumericArrayFn
	{
		vufNumericArrayFn() = delete;
	public:
		vufNumericArrayFn(std::vector<T>& p_vec) :m_numeric_array_v(p_vec) {};
		vufNumericArrayFn(const vufNumericArrayFn& p_other) :m_numeric_array_v(p_other.m_numeric_array_v) {}

		std::string to_string()					const
		{
			VF_NUMERIC_ARRAY_TO_STRING_AND_RETURN_IT(m_numeric_array_v);
		}
		uint64_t	from_string(const std::string& p_str, uint64_t p_offset = 0) 
		{
			VF_NUMERIC_ARRAY_FROM_STRING_AND_RETURN_OFFSET(p_str, p_offset, m_numeric_array_v);
		}
		uint64_t	to_binary(std::vector<unsigned char>& p_buff)	const 
		{
			VF_NUMERIC_ARRAY_TO_BINARY_AND_RETURN_OFFSET(p_buff, m_numeric_array_v);
		}
		uint64_t	from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0)  
		{
			VF_NUMERIC_ARRAY_FROM_BINARY_AND_RETURN_OFFSET(p_buff, p_offset, m_numeric_array_v);
		}

		std::vector<T>& m_numeric_array_v;
	};
#pragma endregion
#pragma region USING_MANES_NUMERIC_ARRAY
	using vufFloatArrayObject	= vufNumericArrayObject<float>;
	using vufDoubleArrayObject	= vufNumericArrayObject<double>;
	using vufIntArrayObject		= vufNumericArrayObject<int>;
	using vufBoolArrayObject	= vufNumericArrayObject<bool>;

	using vufFloatArrayFn		= vufNumericArrayFn<float>;
	using vufDoubleArrayFn		= vufNumericArrayFn<double>;
	using vufIntArrayFn			= vufNumericArrayFn<int>;
	using vufBoolArrayFn		= vufNumericArrayFn<bool>;

#pragma endregion USING_MANES_NUMERIC_ARRAY

}

#endif // !VF_NUMERIC_ARRAY_H
