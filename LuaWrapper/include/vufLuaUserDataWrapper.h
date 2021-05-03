#ifndef VF_LUA_USR_DATA_ERPR_H
#define VF_LUA_USR_DATA_ERPR_H


namespace vuf
{
	template<typename T> class vufQuaternion;
	template<typename T> class vufVector4;
	template<typename T> class vufMatrix4;
	// wrapper class for types like vector,quaternions
	// support data created in lua and references to data created outside of lua
	// can hold data or pointer to outside data
	template<typename T> 
	class vufLuaDataWrapper
	{
	public:
		vufLuaDataWrapper(const T& p_data, T* p_ref_ptr = nullptr) :			 
			m_ref_ptr(p_ref_ptr)
			, m_data(p_data)
		{
			m_ref_count = p_ref_ptr == nullptr ? 0 : 2;
		}
		T& get_data() 					
		{
			if (m_ref_count == 0)
			{
				return m_data;
			}
			if (m_ref_ptr != nullptr)
			{
				m_ref_count--;
				VF_LOG_WARN("Decrement ref couter");
				if (m_ref_count == 0)
				{
					VF_LOG_WARN("MAKE UNIQUE");
					m_data = *m_ref_ptr;
					return m_data;
				}
				return *m_ref_ptr;
			}
			return m_data; 
		}
		void set_data(const T& p_data)	
		{ 			
			m_data = p_data; 
		}
		uint16_t	get_ref_count() const	{ return m_ref_count; }
		T* get_ref_ptr() const				{ return p_ref_ptr; }
	private:
		uint16_t				m_ref_count		= 0;	// reference count
		T*						m_ref_ptr	= nullptr;	// reference to data
		T						m_data;					// lvalue
	};
	
	// class session storage for arrays created in lua
	// handly called deallocation after end of session
	// used in custom types by upvalues and luightuserdata
	template <typename T> 
	class vufLuaSessionDataStore
	{
	public:
		vufLuaSessionDataStore():m_last_ndx(0)
		{}
		~vufLuaSessionDataStore()
		{
			clear();
		}
		T* get_new()
		{
			T* l_new_ptr = new T();
			m_data_v.push_back(l_new_ptr);
			m_last_ndx++;
			return l_new_ptr;
		}
		void clear()
		{
			for (size_t i = 0; i < m_last_ndx; ++i)
			{
				m_data_v[i]->~T();
			}
			m_last_ndx = 0;
		}
		void reserve(size_t p_size)
		{
			m_data_v.reserve(p_size);
		}
		void log()
		{
			std::cout << "Store: " << m_last_ndx << std::endl;
		}
	private:
		std::vector<T*> m_data_v;
		size_t			m_last_ndx = 0;
	};

	template<typename T>	using vufLuaQuaternionWrapper = vufLuaDataWrapper< vufQuaternion<T>>;
	template<typename T>	using vufLuaVector4Wrapper = vufLuaDataWrapper< vufVector4<T>>;
	template<typename T>	using vufLuaMattrix4Wrapper = vufLuaDataWrapper< vufMatrix4<T>>;

}

#endif // !VF_LUA_USR_DATA_ERPR_H
