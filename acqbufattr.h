#pragma once

#include "../def.h"

ACQ_NAMESPACE_START

ACQ_DLL_CLASS_DECLEAR(buf_attr_t)
{
public:
	uint32 _value;
	long   _l_value;

public:
	buf_attr_t(uint32 value): _value(value), _l_value(0){}

	bool operator == (const buf_attr_t& attr) const
	{
		return _value == attr._value;
	}
	bool operator != (const buf_attr_t& attr) const
	{
		return _value != attr._value;
	}
	buf_attr_t& operator |= (uint32 value)
	{
		_value |= value;
		return *this;
	}
	buf_attr_t& operator &= (uint32 value)
	{
		_value &= value;
		return *this;
	}
	operator uint32() const
	{
		return _value;
	}
	buf_attr_t operator ~() const
	{
		return buf_attr_t(~_value);
	}
};

inline buf_attr_t operator  | (const buf_attr_t& attr0, const buf_attr_t& attr1)
{
    return buf_attr_t(attr0._value | attr1._value);
}

inline buf_attr_t operator & (const buf_attr_t& attr0, const buf_attr_t& attr1)
{
	return buf_attr_t(attr0._value & attr1._value);
}

// 缓冲区属性的值为32位整数
// 第一字节表示内存分配方式及数据类别
static const buf_attr_t EMPTY           (0);
static const buf_attr_t TAKE_OVER       (1<<1);    // must use "multithreaded dll"
static const buf_attr_t NO_RELEASE      (1<<2);
static const buf_attr_t CONST_REF       (1<<3 | 1<<1 | 1<<2);

// 第二字节表示编码方式
static const buf_attr_t CODING_GB2312   (1<<8);
static const buf_attr_t CODING_GBK      (1<<9);
static const buf_attr_t CODING_ANSI     (1<<9);
static const buf_attr_t CODING_UTF7     (1<<10);
static const buf_attr_t CODING_UTF8     (1<<11);
static const buf_attr_t CODING_UTF16    (1<<12);
static const buf_attr_t CODING_UNICODE  (1<<12);
static const buf_attr_t CODING_UTF32    (1<<13);
static const buf_attr_t MASK_CODING     (0xFF00);

static const buf_attr_t LONG_VALUE      (1<<33);

ACQ_NAMESPACE_END