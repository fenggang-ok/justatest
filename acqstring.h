#pragma once

#include "../def.h"
#include "acqbuffer.h"
#include "acqmatch.h"

#include <vector>

ACQ_NAMESPACE_START

class exception_t;

static const char* STR_CLOTHES       = " \t\r\n\f\b";
static const char* LINE_SEPARATOR    = "\r\n";

static const wchar* WSTR_CLOTHES     = L" \t\r\n\f\b";
static const wchar* WLINE_SEPARATOR  = L"\r\n";

static const char* MAX_UINT8_STR     = "255";
static const char* MAX_UINT16_STR    = "65535";
static const char* MAX_UINT32_STR    = "4294967295";
static const char* MIN_INT64_STR     = "-9223372036854775808";
static const char* MAX_INT64_STR     = "9223372036854775807";
static const char* MAX_UINT64_STR    = "18446744073709551615";

static const wchar* WMAX_UINT8_STR   = L"255";
static const wchar* WMAX_UINT16_STR  = L"65535";
static const wchar* WMAX_UINT32_STR  = L"4294967295";
static const wchar* WMIN_INT64_STR   = L"-9223372036854775808";
static const wchar* WMAX_INT64_STR   = L"9223372036854775807";
static const wchar* WMAX_UINT64_STR  = L"18446744073709551615";

class HEX
{
public:
	uint64 _data;
	int _size;
	int _attr;
	const char* _pre;

public:
	HEX(char d,   int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(short d,  int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(int d,    int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(long d,   int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(float d,  int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(double d, int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(int64 d,  int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(void* d,  int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(unsigned char d,   int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(unsigned short d,  int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(unsigned int d,    int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(unsigned long d,   int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){set_default_case();}
	HEX(uint64 d,  int attr = HEX_UPPER, const char* pre = NULL): _data((uint64)d), _size(sizeof(d)), _attr(attr), _pre(pre){}

private:
	HEX(){}
	HEX(const HEX&){}

	void set_default_case()
	{
		if (!(_attr & HEX_UPPER) && !(_attr & HEX_LOWER))
		{
			_attr |= HEX_UPPER;
		}
	}
};

class _CAP_t
{
public:
	size_type _capacity;
};

inline _CAP_t _CAP(size_type capacity)
{
	return *(_CAP_t*)&capacity;
}

ACQ_DLL_CLASS_DECLEAR(string_t): public buffer_t
{
public:
	typedef char* iterator;
	typedef const char* const_iterator;
	typedef wchar* witerator;
	typedef const wchar* const_witerator;

public:
	static inline size_type ALIGN_CAPACITY(size_type capacity)
	{
		return round_down(capacity + sizeof(char)  + 32, 32);
	}
	static inline size_type WALIGN_CAPACITY(size_type capacity)
	{
		return round_down(capacity + sizeof(wchar) + 64, 64);
	}

public:
	// 默认构造
	string_t(buf_attr_t = CODING_GBK, size_type capacity = 0);

	// 由字符构造
	string_t(char, size_type = 1, buf_attr_t = CODING_GBK);
	string_t(wchar, size_type = 1, buf_attr_t = CODING_UNICODE);

	// 用C类型字符串构造
	string_t(const char*);
	string_t(const char*, buf_attr_t, size_type = 0);
	string_t(const char*, size_type);
	string_t(const char*, size_type, buf_attr_t, size_type = 0);

	string_t(const wchar*);
	string_t(const wchar*, buf_attr_t, size_type = 0);
	string_t(const wchar*, size_type);
	string_t(const wchar*, size_type, buf_attr_t, size_type = 0);

	// 由迭代器构造
	string_t(iterator, iterator, buf_attr_t = CODING_GBK);
	string_t(witerator, witerator, buf_attr_t = CODING_UNICODE);

	// 指定容量构造
	string_t(const _CAP_t&, buf_attr_t = EMPTY);

	// 拷贝构造
	string_t(const string_t&);

	// 析构
	~string_t();

	string_t& no_release();
	string_t& take_over(uint8* p_mem, size_type mem_length, size_type mem_used, buf_attr_t attr);

	void reserve(size_type capacity, int retain_data = 1)
	{
		size_type new_capacity;
		if (_attr & CODING_UNICODE)
		{
			new_capacity = WALIGN_CAPACITY(capacity);
		}
		else
		{
			new_capacity = ALIGN_CAPACITY(capacity);
		}
		buffer_t::reserve(new_capacity, retain_data);
	}

	inline size_type capacity() const
	{
		size_type end_len = _attr & CODING_UNICODE? sizeof(wchar): sizeof(char);
		return _capacity < end_len ? 0: _capacity - end_len;
	}

	inline string_t& clean_out()
	{
		release();
		_p_data = NULL;
		_capacity = 0;
		_used = 0;
		_attr = 0;
		return *this;
	}

	inline iterator begin()                 {return (char*)_p_data;}
	inline iterator end()                   {return (char*)_p_data + _used;}
	inline const_iterator begin() const     {return (const char*)_p_data;}
	inline const_iterator end() const       {return (const char*)_p_data + _used;}

	inline witerator wbegin()               {return (wchar*)_p_data;}
	inline witerator wend()                 {return (wchar*)(_p_data + _used);}
	inline const_witerator wbegin() const   {return (const wchar*)_p_data;}
	inline const_witerator wend() const     {return (const wchar*)(_p_data + _used);}

	inline uint8* data() const              {return (uint8*)_p_data;}
	inline const char* c_str() const        {return (const char*)_p_data;}
	inline const wchar* c_wstr() const      {return (const wchar*)_p_data;}

	inline size_type size() const           {return _used;}
	inline size_type length() const         {return _used;}

	inline bool empty() const               {return _used == 0;}
	inline buf_attr_t get_coding() const    {return _attr & MASK_CODING;}

	inline buf_attr_t set_coding(const buf_attr_t& coding)
	{
		_attr &= ~MASK_CODING;
		return _attr |= coding;
	}

	void set_data(const uint8*, size_type, size_type = 0);
	void set_used_size(size_type new_used_size);

	string_t& encode_unicode();
	string_t& encode(buf_attr_t, size_type=0);

	string_t& assign(const char*, size_type, buf_attr_t, size_type);
	string_t& assign(const wchar*, size_type, buf_attr_t, size_type);

	bool starts_with(const char*, int = 0)  const;
	bool starts_with(const wchar*, int = 0) const;

	bool starts_with(char, int = 0)  const;
	bool starts_with(wchar, int = 0) const;

	bool ends_with(const char*, int = 0, size_type = 0) const;
	bool ends_with(const wchar*, int = 0, size_type = 0) const;

	bool ends_with(char, int = 0) const;
	bool ends_with(wchar, int = 0) const;

	bool starts_with(const string_t&, int = 0) const;
	bool ends_with(const string_t&, int = 0) const;

	string_t& set_char(size_type, char);
	string_t& set_wchar(size_type, wchar);

	string_t sub(iterator, iterator)   const;
	string_t sub(witerator, witerator) const;
	string_t sub(size_type, size_type = nopos) const;

	size_type sub_num(char) const;
	size_type sub_num(wchar) const;
	size_type sub_num(const char*, size_type = 0) const;
	size_type sub_num(const wchar*, size_type = 0) const;
	size_type sub_num(const string_t&) const;

	size_type find(char, size_type = 0) const;
	size_type find(wchar, size_type = 0) const;

	size_type rfind(char, size_type = nopos) const;
	size_type rfind(wchar, size_type = nopos) const;

	size_type find(const char*, size_type = 0) const;
	size_type find(const wchar*, size_type = 0) const;

	size_type rfind(const char*, size_type = nopos, size_type = 0) const;
	size_type rfind(const wchar*, size_type = nopos, size_type = 0) const;

	size_type find(const string_t&, size_type = 0) const;
	size_type rfind(const string_t&, size_type = nopos) const;

	size_type find_char_in_set(const char*, size_type=0, size_type=0) const;
	size_type find_char_in_set(const wchar*, size_type=0, size_type=0) const;

	size_type rfind_char_in_set(const char*, size_type=nopos, size_type=0) const;
	size_type rfind_char_in_set(const wchar*, size_type=nopos, size_type=0) const;

	size_type find_char_in_set(const string_t&, size_type=0) const;
	size_type rfind_char_in_set(const string_t&, size_type=nopos) const;

	size_type find_n(char, size_type, size_type = 0) const;
	size_type find_n(wchar, size_type, size_type = 0) const;
	size_type find_n(const char*, size_type, size_type = 0) const;
	size_type find_n(const wchar*, size_type, size_type = 0) const;
	size_type find_n(const string_t&, size_type, size_type = 0) const;

	size_type rfind_n(char, size_type, size_type = nopos) const;
	size_type rfind_n(wchar, size_type, size_type = nopos) const;
	size_type rfind_n(const char*, size_type, size_type = nopos, size_type = 0) const;
	size_type rfind_n(const wchar*, size_type, size_type = nopos, size_type = 0) const;
	size_type rfind_n(const string_t&, size_type, size_type = nopos) const;

	string_t& erase();
	string_t& erase(iterator, iterator);
	string_t& erase(witerator, witerator);

	string_t& remove(char, size_type = 1);
	string_t& remove(wchar, size_type = 1);
	string_t& remove(const char*, size_type = 1, size_type = 0);
	string_t& remove(const wchar*, size_type = 1, size_type = 0);
	string_t& remove(const string_t&, size_type = 1);

	string_t& insert(char, size_type, size_type = sizeof(char), size_type = 0);
	string_t& insert(wchar, size_type, size_type = sizeof(wchar), size_type = 0);
	string_t& insert(const char*, size_type, size_type = 0, size_type = 0);
	string_t& insert(const wchar*, size_type, size_type = 0, size_type = 0);
	string_t& insert(const string_t&, size_type, size_type = 0);

	inline char& at(long pos)
	{
		if (pos >= 0)
		{
			return *((char*)_p_data + pos);
		}
		return *(char*)(_p_data + _used + pos);
	}

	inline wchar& wat(long pos)
	{
		if (pos >= 0)
		{
			return *((wchar*)_p_data + pos);
		}
		return *((wchar*)(_p_data + _used) + pos);
	}

	inline const char& at(long pos) const
	{
		if (pos >= 0)
		{
			return *((const char*)_p_data + pos);
		}
		return *((const char*)_p_data + _used + pos);
	}

	inline const wchar& wat(long pos) const
	{
		if (pos >= 0)
		{
			return *(wchar*)(_p_data + pos);
		}
		return *(wchar*)(_p_data + _used + pos);
	}

	string_t& strip_left(const char* = STR_CLOTHES);
	string_t& strip_right(const char* = STR_CLOTHES);
	string_t& strip(const char* = STR_CLOTHES);

	string_t& strip_left(const wchar* = WSTR_CLOTHES);
	string_t& strip_right(const wchar* = WSTR_CLOTHES);
	string_t& strip(const wchar* = WSTR_CLOTHES);

	string_t& strip_left(const string_t& clothes)
	{
		if (has_attr(CODING_UNICODE))
		{
			return strip_left(clothes.c_wstr());
		}
		return strip_left(clothes.c_str());
	}
	string_t& strip_right(const string_t& clothes)
	{
		if (has_attr(CODING_UNICODE))
		{
			return strip_right(clothes.c_wstr());
		}
		return strip_right(clothes.c_str());
	}
	string_t& strip(const string_t& clothes)
	{
		if (has_attr(CODING_UNICODE))
		{
			return strip(clothes.c_wstr());
		}
		return strip(clothes.c_str());
	}

	bool is_digit(int radix = 10) const;
	bool is_negative() const;
	string_t get_abs() const;
	bool is_upper() const;
	bool is_lower() const;
	string_t& to_upper();
	string_t& to_lower();

	string_t& append(char, size_type = 1, size_type = 0);
	string_t& append(wchar, size_type = 1, size_type = 0);
	string_t& append(const char*, size_type = 0, size_type = 0);
	string_t& append(const wchar*, size_type = 0, size_type = 0);
	string_t& append(const string_t&, size_type = 0);

	int compare(const char*, int = 0, size_type = 0) const;
	int compare(const wchar*, int = 0, size_type = 0) const;
	int compare(const string_t&, int = 0) const;

	bool equal(const char*, int, size_type = 0) const;
	bool equal(const wchar*, int, size_type = 0) const;
	bool equal(const string_t&, int) const;

	char& operator [] (int idx)
	{
		return idx > 0? *(char*)(_p_data+idx): *(char*)(_p_data+_used+idx);
	}
	string_t& operator =  (const char*);
	string_t& operator =  (const wchar*);
	string_t& operator =  (const string_t&);
	string_t& operator += (const char*);
	string_t& operator += (const wchar*);
	string_t& operator += (const string_t&);
	string_t& operator -= (const char*);
	string_t& operator -= (const wchar*);
	string_t& operator -= (const string_t&);
	string_t& operator *= (int);
	bool operator ! () const;

public:
	int truncate(size_type) const;
	int wtruncate(size_type) const;
	void recover_truncate(size_type, int) const;
	void recover_wtruncate(size_type, int) const;
};

typedef string_t& string_refrence_t;
typedef const string_t& const_string_refrence_t;

ACQ_DLL_FUN_DECLEAR(string_t) operator + (const string_t& left, const char* p_c);
ACQ_DLL_FUN_DECLEAR(string_t) operator + (const char* p_c, const string_t& right);
ACQ_DLL_FUN_DECLEAR(string_t) operator + (const string_t& left, const wchar* p_wc);
ACQ_DLL_FUN_DECLEAR(string_t) operator + (const wchar* p_wc, const string_t& right);
ACQ_DLL_FUN_DECLEAR(string_t) operator + (const string_t& left, const string_t& right);

ACQ_DLL_FUN_DECLEAR(string_t) operator - (const string_t& left, const char* p_c);
ACQ_DLL_FUN_DECLEAR(string_t) operator - (const char* p_c, const string_t& right);
ACQ_DLL_FUN_DECLEAR(string_t) operator - (const string_t& left, const string_t& right);

ACQ_DLL_FUN_DECLEAR(string_t) operator * (const string_t& left, int n);
ACQ_DLL_FUN_DECLEAR(string_t) operator * (int n, const string_t& left);

ACQ_DLL_FUN_DECLEAR(bool) operator == (const string_t& left, const string_t& right);
ACQ_DLL_FUN_DECLEAR(bool) operator != (const string_t& left, const string_t& right);
ACQ_DLL_FUN_DECLEAR(bool) operator >= (const string_t& left, const string_t& right);
ACQ_DLL_FUN_DECLEAR(bool) operator <= (const string_t& left, const string_t& right);
ACQ_DLL_FUN_DECLEAR(bool) operator >  (const string_t& left, const string_t& right);
ACQ_DLL_FUN_DECLEAR(bool) operator <  (const string_t& left, const string_t& right);

ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, char);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, short);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, unsigned short);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, int);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, unsigned int);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, long);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, unsigned long);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, float);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, double);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, const char*);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, const wchar*);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, const string_t&);
ACQ_DLL_FUN_DECLEAR(string_refrence_t) operator << (string_t&, const HEX&);

#define EB acq::string_t(_CAP(32)).no_release()

inline __contain_0_byte(ulong longword, ulong magic_bits)
{
	return (((longword + magic_bits) ^ ~longword) & ~magic_bits) != 0;
}

inline size_type len(const char* p_c)
{
	return strlen(p_c);
}

inline size_type len(const wchar* p_wc)
{
	return wcslen(p_wc);
}

inline size_type len(const string_t& str_buf)
{
	return str_buf.length();
}

inline size_type find_char(char c, const char* p_text)
{
	char* p = strchr(p_text, c);
	return p == NULL? nopos: p - p_text;
}

inline size_type find_wchar(wchar wc, const wchar* p_wtext)
{
	wchar* p = wcschr(p_wtext, wc);
	return p == NULL? nopos: (p - p_wtext)*2;
}

inline char* find_sub(const char* p_text, const char* p_sub)
{
	return strstr(p_text, p_sub);
}

inline wchar* find_sub(const wchar* p_wtext, const wchar* p_wsub)
{
	return wcsstr(p_wtext, p_wsub);
}

#define CHECK_NULL(p) \
	if (!(p)) {acq::string_t s(_CAP(64), NO_RELEASE); s << "指针不应为空，文件" << __FILE__ << "第" << __LINE__ << "行"; throw acq::exception_t(s);}

typedef char* char_pointer_t;
ACQ_DLL_FUN_DECLEAR(char_pointer_t) LE_hex_output(uint8  i, char* p_out, int zero_fill = 1);
ACQ_DLL_FUN_DECLEAR(char_pointer_t) LE_hex_output(uint16 i, char* p_out, int zero_fill = 1);
ACQ_DLL_FUN_DECLEAR(char_pointer_t) LE_hex_output(uint32 i, char* p_out, int zero_fill = 1);
ACQ_DLL_FUN_DECLEAR(char_pointer_t) LE_hex_output(uint64 i, char* p_out, int zero_fill = 1);

inline bool is_blank(char c)
{
	string_t s(STR_CLOTHES, len(STR_CLOTHES), CONST_REF);
	return s.find(c) != nopos;
}


ACQ_NAMESPACE_END