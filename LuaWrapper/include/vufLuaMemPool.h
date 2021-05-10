#ifndef VF_LUA_MEM_POOL_H
#define VF_LUA_MEM_POOL_H

#include <stdint.h>
#include <functional>
#include <iostream>
namespace vuf
{
	template<typename T>
	class vufLuaMemPoolChunk
	{
	public:
		vufLuaMemPoolChunk(vufLuaMemPoolChunk* p_prev_chunk = nullptr, vufLuaMemPoolChunk* p_root_chunk = nullptr):
			  m_prev_chunk(p_prev_chunk)
			, m_root_chunk(p_root_chunk)
			, m_next_chunk(nullptr)
			, m_active_chunk(nullptr)
			, m_last_index(0)
		{
			//std::cout << "Chunk constructor" << std::endl;
			if (p_root_chunk == nullptr)
			{
				m_root_chunk	= this;
				m_active_chunk	= this;
			}
		}
		~vufLuaMemPoolChunk()
		{
			reset_this_chunk_only();
			if (m_next_chunk != nullptr)
			{
				delete m_next_chunk;
			}
		}
		void set_root()
		{
			m_root_chunk = this;
		}
		bool is_root() const
		{
			return m_root_chunk == this;
		}
		T* new_pointer()
		{
			if (m_root_chunk == nullptr)
			{
				return nullptr;
			}
			return m_root_chunk->m_active_chunk->new_chunk_pointer();
		}
		void reset()
		{
			reset_chunk();
			m_active_chunk = m_root_chunk;
		}
		void delete_chunks()
		{
			if (m_next_chunk != nullptr)
			{
				delete m_next_chunk;
				m_next_chunk = nullptr;
			}
			reset_this_chunk_only();
		}
		size_t get_chunk_count() const
		{
			size_t l_count = 1;
			vufLuaMemPoolChunk* l_chunk = m_next_chunk;
			while (l_chunk != nullptr)
			{
				l_chunk = l_chunk->m_next_chunk;
				++l_count;
			}
			return l_count;
		}
	private:
		T* new_chunk_pointer()
		{
			size_t l_index_step = sizeof(T);
			// if can allocate data
			if (m_last_index + l_index_step < m_chunk_size)
			{
				char* l_res_ptr = &m_memory_arena[m_last_index];
				m_last_index += l_index_step;
				// if we have initialize function then initialize data
				if (m_init_function != nullptr)
				{
					m_init_function(l_res_ptr);
				}
				return (T*)l_res_ptr;
			}
			// if we cant allocate data then create new chunk and get allocation space from it.
			// update active chunk info in root chunk
			if (m_next_chunk == nullptr)
			{
				m_next_chunk = new vufLuaMemPoolChunk(this, m_root_chunk);
			}
			m_root_chunk->m_active_chunk = m_next_chunk;
			return m_next_chunk->new_chunk_pointer();
		}
		void reset_chunk()
		{
			if (m_next_chunk != nullptr)
			{
				m_next_chunk->reset_chunk();
			}
			reset_this_chunk_only();			
		}
		void reset_this_chunk_only()
		{
			size_t l_index_step = sizeof(T);
			if (m_release_function != nullptr)
			{
				for (size_t i = 0; i < m_last_index; i += l_index_step)
				{
					m_release_function(&m_memory_arena[i]);
				}
			}
			m_last_index = 0;
		}
	private:
		//
		static constexpr uint64_t			m_chunk_size = 64;
		static std::function<void(void*)>	m_init_function;
		static std::function<void(void*)>	m_release_function;

		vufLuaMemPoolChunk*	m_prev_chunk	= nullptr;
		vufLuaMemPoolChunk*	m_next_chunk	= nullptr;
		vufLuaMemPoolChunk*	m_root_chunk	= nullptr;

		vufLuaMemPoolChunk*	m_active_chunk	= nullptr;	// has sence only for root chunk
		size_t				m_last_index	= 0;

		char			m_memory_arena[m_chunk_size];
	};

	template<typename T>
	class vufLuaMemePool
	{
	public:
		vufLuaMemePool() {}
		~vufLuaMemePool() {}
		T* new_global__pointer()
		{
			return m_global_pool.new_pointer();
		}
		T* new_local_pointer()
		{
			return m_local_pool.new_pointer();
		}
		void reset_global()
		{
			m_global_pool.reset();
		}
		void reset_local()
		{
			m_global_pool.reset();
		}
		void delete_global()
		{
			m_global_pool.delete_chunks();
		}
		void delete_local()
		{
			m_local_pool.delete_chunks();
		}
	private:
		vufLuaMemPoolChunk<T> m_global_pool;
		vufLuaMemPoolChunk<T> m_local_pool;// session pool
	};
}

#endif // !VF_LUA_MEM_POOL_H
