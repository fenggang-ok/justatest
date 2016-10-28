#pragma once

#include "../def.h"
#include "acqbuffer.h"

ACQ_NAMESPACE_START

ACQ_DLL_CLASS_DECLEAR(pool_pointer_t)
{
protected:
	uint8* _pointer;
	uint8* _block_pointer;

public:
	pool_pointer_t(uint8* pointer = NULL, uint8* block_pointer = NULL): 
		_pointer(pointer), _block_pointer(block_pointer)
	{}
};

ACQ_DLL_CLASS_DECLEAR(fixed_length_pool_t)
{
protected:
	uint8*      _p_block;
	l_size_type _block_size;
	l_size_type _block_num;
	l_size_type _available_block_num;
	l_size_type _last_allocated;

public:
	fixed_length_pool_t(l_size_type = 0, l_size_type = 0);
	~fixed_length_pool_t();

	inline uint8* allocate()
	{
		if (!_available_block_num)
		{
			return NULL;
		}
		uint8* p_block = _p_block + _last_allocated * _block_size;
		_last_allocated = *(l_size_type*)p_block;
		_available_block_num--;
		return p_block + sizeof(l_size_type);
	}

	inline void deallocate(uint8* pointer)
	{
		if (!pointer)
		{
			return;
		}
		uint8* p_block = pointer - sizeof(l_size_type);
		*(l_size_type*)p_block = _last_allocated;
		_last_allocated = (p_block - _p_block) / _block_size;
		_available_block_num++;
	}

	int init(l_size_type, l_size_type);
	int destroy();
};

ACQ_NAMESPACE_END