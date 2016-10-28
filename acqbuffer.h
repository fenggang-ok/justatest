#pragma once

#include "../def.h"
#include "acqbufattr.h"
#include <string>

ACQ_NAMESPACE_START

ACQ_DLL_CLASS_DECLEAR(buffer_t)
{
public:
	typedef uint8* iterator;
	typedef const uint8* const_iterator;

protected:
	uint8*      _p_data;
	size_type   _capacity;
	size_type   _used;
	buf_attr_t  _attr;

	void clear_data();

public:
	// 指定容量构造
	buffer_t(size_type = 0, buf_attr_t attr = EMPTY);

	// 从其它内存段复制
	buffer_t(uint8*, size_type length, size_type capacity = autosize, buf_attr_t attr = EMPTY);

	// 拷贝构造
	buffer_t(const buffer_t&);

	// 析构
	~buffer_t();

	inline size_type size() const         {return _used;}
	inline size_type capacity() const     {return _capacity;}
	inline iterator begin()               {return _p_data;}
	inline iterator end()                 {return _p_data + _used;}
	inline const_iterator begin() const   {return _p_data;}
	inline const_iterator end() const     {return _p_data + _used;}
	inline uint8* data() const            {return _p_data;}
	inline uint8* unused_data() const     {return _p_data + _used;}

	void set_data(const uint8*, size_type, size_type = autosize);

	inline uint8& at(size_type pos)             {return _p_data[pos];}
	inline const uint8& at(size_type pos) const {return _p_data[pos];}

	void reserve(size_type, int = 1);
	void release(int = 0);
	
	inline bool has_attr(buf_attr_t attr) const // !!!此方法还没有运用到代码中
	{
		return (_attr._value & attr._value) != 0;
	}   

	inline bool valid() const             {return _p_data != NULL;}
	inline buf_attr_t attr() const        {return _attr;}
	inline size_type unused_size() const  {return _capacity - _used;}
	inline size_type used_size() const    {return _used;}

	buffer_t& append(const buffer_t&, size_type mem_grow = autosize);
	buffer_t& append(const uint8*, size_type, size_type mem_grow = autosize);

	size_type find(const uint8* p_c, size_type length, size_type start_pos) const;
	size_type rfind(const uint8* p_c, size_type length, size_type start_pos) const;
	size_type find(const buffer_t& buf, size_type start_pos) const;
	size_type rfind(const buffer_t& buf, size_type start_pos) const;

	buf_attr_t set_attr(buf_attr_t);
};

typedef buffer_t& buffer_refrence_t;
typedef const buffer_t& const_buffer_refrence_t;

ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, char);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, unsigned char);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, short);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, unsigned short);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, int);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, unsigned int);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, long);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, unsigned long);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, float);
ACQ_DLL_FUN_DECLEAR(buffer_refrence_t) operator << (buffer_t&, double);

inline std::ostream& operator << (std::ostream& os, const buffer_t& buf)
{
	if (buf.data())
	{
		return os << (char*)buf.data();
	}
	return os;
}

inline std::wostream& operator << (std::wostream& wos, const buffer_t& buf)
{
	if (buf.data())
	{
		return wos << (wchar_t*)buf.data();
	}
	return wos;
}

inline std::istream& operator >> (std::istream& is, buffer_t& buf)
{
	return is;
}

inline std::wistream& operator >> (std::wistream& wis, buffer_t& buf)
{
	return wis;
}

ACQ_NAMESPACE_END