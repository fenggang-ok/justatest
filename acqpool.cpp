#include "acqpool.h"
#include "acqexception.h"

ACQ_NAMESPACE_START

fixed_length_pool_t::fixed_length_pool_t(l_size_type block_size,
										 l_size_type block_num):
    _block_size(0),
	_block_num(0),
	_last_allocated(0),
	_available_block_num(0),
	_p_block(NULL)
{
	if (block_size * block_num)
	{
		init(block_size, block_num);
	}
}

fixed_length_pool_t::~fixed_length_pool_t()
{
	destroy();
}

int fixed_length_pool_t::init(l_size_type block_size,
							  l_size_type block_num)
{
	if (!_p_block)
	{
		destroy();
	}
	if (block_size * block_num)
	{
		_block_size = round_up(block_size + sizeof(l_size_type), sizeof(l_size_type));
		_block_num  = block_num;
		_available_block_num = _block_num;
		_last_allocated = 0;
		_p_block = new uint8[_block_size * _block_num];
		l_size_type i = 0;
		for (uint8* p = _p_block; i != _block_num; p += _block_size)
		{
			*(l_size_type*)p = ++i;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

int fixed_length_pool_t::destroy()
{
	if (_p_block)
	{
		delete[] _p_block;
		_p_block = NULL;
	}
    _block_size = 0;
	_block_num = 0;
	_last_allocated = 0;
	_available_block_num = 0;
	return 0;
}

ACQ_NAMESPACE_END