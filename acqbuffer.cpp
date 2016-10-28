#include "acqbuffer.h"
#include <algorithm>

ACQ_NAMESPACE_START

buffer_t::buffer_t(size_type capacity, buf_attr_t attr): _capacity(capacity),
                                                         _attr(attr),
                                                         _used(0),
                                                         _p_data(NULL)
{
	if (capacity)
	{
		_p_data = new uint8[_capacity];
		clear_data();
	}
}

buffer_t::buffer_t(uint8* p_data, size_type length, size_type capacity, buf_attr_t attr): _p_data(NULL),
                                                                                          _attr(attr),
                                                                                          _used(0),
																	                      _capacity(0)
{
	if (!length || !p_data)
	{
		return;
	}
	if (capacity == autosize || capacity < length)
	{
		_capacity = round_down(length + 32, 32);
	}
	else
	{
		_capacity = capacity;
	}
	_p_data = new uint8[_capacity];
	_used = length;
	memmove(_p_data, p_data, _used);
}

buffer_t::buffer_t(const buffer_t& other): _p_data(NULL),
                                           _capacity(other.capacity()),
                                           _attr(other.attr()),
                                           _used(other.used_size())
{
	if (_used)
	{
		_p_data = new uint8[_capacity];
		memmove(_p_data, other.data(), _capacity);
	}
	else
	{
		_p_data = NULL;
		_used = 0;
		_attr = 0;
		_capacity = 0;
	}
}

buffer_t::~buffer_t()
{
	if (_attr & NO_RELEASE)
	{
		return;
	}
	release();
}

buf_attr_t buffer_t::set_attr(buf_attr_t attr)
{
	buf_attr_t old_attr = _attr;
	_attr = attr;
	return old_attr;
}

void buffer_t::set_data(const uint8* p_data, size_type length, size_type mem_grow)
{
	if (!_p_data || _capacity < length)
	{
		size_type new_capacity;
		new_capacity = mem_grow == autosize?
			round_down(length + 32, 32): length + mem_grow;
		release();
		_p_data = new uint8[new_capacity];
		_capacity = new_capacity;
	}
	clear_data();
	memmove(_p_data, p_data, length);
	_used = length;
}

void buffer_t::reserve(size_type new_capacity, int retain_data)
{
	if (retain_data)
	{
		uint8* p_new_data = new uint8[new_capacity];
		memset(p_new_data, 0, new_capacity);
		size_type move_size = min(_capacity, new_capacity);
		memmove(p_new_data, _p_data, move_size);
		size_type tmp = _used;
		release();
		_p_data = p_new_data;
		_used = min(new_capacity, tmp);
	}
	else
	{
		release();
		_p_data = new uint8[new_capacity];
		clear_data();
	}
	_capacity = new_capacity;
}

void buffer_t::release(int force)
{
	if (_attr & NO_RELEASE && !force)
	{
		return;
	}
	if (_p_data)
	{
		delete[] _p_data;
		_p_data = NULL;
	}
	_capacity = 0;
	_used = 0;
}

void buffer_t::clear_data()
{
	memset(_p_data, 0, _capacity);
	_used = 0;
}

buffer_t& buffer_t::append(const buffer_t& other_buf, size_type mem_grow)
{
	return append(other_buf.data(), other_buf.used_size(), mem_grow);
}

buffer_t& buffer_t::append(const uint8* p_data, size_type length, size_type mem_grow)
{
	if (_used + length > _capacity)
	{
		size_type new_capacity;
		new_capacity = mem_grow == autosize?
			round_down(_used + length + 32, 32): _used + length + mem_grow;
		reserve(new_capacity);
	}
	memmove(unused_data(), p_data, length);
	_used += length;
	return *this;
}

size_type buffer_t::find(const uint8* p_c, size_type length, size_type start_pos) const
{
	if (start_pos > _used)
	{
		return nopos;
	}
	const uint8* p_start = begin() + start_pos;
	const uint8* p0 = p_start;
	const uint8* p1 = p_start;
	const uint8* p2 = p_c;
	for (; p0 != end(); p0++, p1 = p0, p2 = p_c)
	{
		while (*p1 == *p2)
		{
			p1++;
			p2++;
			if (p2 - p_c == length)
			{
				return p0 - begin();
			}
		}
	}
	return nopos;
}

size_type buffer_t::rfind(const uint8* p_c, size_type length, size_type start_pos) const
{
	start_pos = start_pos > _used - 1? _used - 1: start_pos;
	if (_used == 0 || !length)
	{
		return nopos;
	}
	const uint8* p_start = begin() + start_pos;     
	const uint8* p_end   = begin();
	const uint8* p0 = p_start;
	const uint8* p1 = p_start;
	const uint8* p2 = p_c + length - 1;
	for (; p1 != p_end; p0--, p1 = p0, p2 = p_c + length - 1)
	{
		while (*p1 == *p2 && p1 != p_end)
		{
			if (p2 == p_c)
			{
				return p1 - begin();
			}
			p1--;
			p2--;
		}
	}
	return nopos;
}

size_type buffer_t::find(const buffer_t& buf, size_type start_pos) const
{
	return find(buf.data(), buf.used_size(), start_pos);
}

size_type buffer_t::rfind(const buffer_t& buf, size_type start_pos) const
{
	return rfind(buf.data(), buf.used_size(), start_pos);
}

buffer_t& operator << (buffer_t& buf, char c)
{
	buf.append((uint8*)&c, 1);
	return buf;
}

buffer_t& operator << (buffer_t& buf, unsigned char uc)
{
	buf.append((uint8*)&uc, 1);
	return buf;
}

buffer_t& operator << (buffer_t& buf, short n)
{
	return operator << (buf, (long)n);
}

buffer_t& operator << (buffer_t& buf, unsigned short n)
{
	return operator << (buf, (unsigned long)n);
}

buffer_t& operator << (buffer_t& buf, int n)
{
	return operator << (buf, (long)n);
}

buffer_t& operator << (buffer_t& buf, unsigned int n)
{
	return operator << (buf, (unsigned long)n);
}

buffer_t& operator << (buffer_t& buf, long n)
{
	return buf.append((uint8*)&n, sizeof(n));
}

buffer_t& operator << (buffer_t& buf, unsigned long n)
{
	return buf.append((uint8*)&n, sizeof(n));
}

buffer_t& operator << (buffer_t& buf, float f)
{
	return buf.append((uint8*)&f, sizeof(f));
}

buffer_t& operator << (buffer_t& buf, double d)
{
	return operator << (buf, (float)d);
}

ACQ_NAMESPACE_END