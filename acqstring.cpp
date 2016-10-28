#include "acqstring.h"
#include "acqexception.h"

ACQ_NAMESPACE_START

const char* hex_string_array[] = 
{
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29",	"2A", "2B", "2C", "2D", "2E", "2F",
	"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
	"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
	"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
	"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
	"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
	"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
	"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
	"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
	"B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
	"C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
	"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
	"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
	"F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
};

string_t::string_t(buf_attr_t attr, size_type capacity)
{
	_attr = attr;
	if (capacity)
	{
		reserve(capacity);
	}
}

string_t::string_t(char c, size_type repeat, buf_attr_t attr):
	buffer_t(ALIGN_CAPACITY(repeat), attr)
{
	memset(_p_data, c, repeat);
	_used = repeat;
	*(char*)(_p_data + _used) = 0;
	if (!get_coding())
	{
		set_coding(CODING_GBK);
	}
}

string_t::string_t(wchar wc, size_type repeat, buf_attr_t attr):
	buffer_t(WALIGN_CAPACITY(repeat), attr)
{
	wmemset((wchar*)_p_data, wc, repeat);
	_used = repeat * sizeof(wchar);
	*(wchar*)(_p_data + _used) = 0;
	if (!get_coding())
	{
		set_coding(CODING_UNICODE);
	}
}

string_t::string_t(const char* p_c)
{
	size_type str_len = len(p_c);
	assign(p_c, str_len, CODING_GBK, ALIGN_CAPACITY(str_len));
}

string_t::string_t(const char* p_c, buf_attr_t attr, size_type take_over_capacity)
{
	size_type str_len = len(p_c);
	_attr = attr;
	if (attr & TAKE_OVER)
	{
		if (take_over_capacity == 0)
		{
			take_over_capacity = str_len + sizeof(char);
		}
		take_over((uint8*)p_c, take_over_capacity, str_len, attr);
	}
	else
	{
		assign(p_c, str_len, attr, ALIGN_CAPACITY(str_len));
	}
}

string_t::string_t(const char* p_c, size_type length)
{
	assign(p_c, length, CODING_GBK, ALIGN_CAPACITY(length));
}

string_t::string_t(const char* p_c, size_type length, buf_attr_t attr, size_type take_over_capacity)
{
	if (attr & TAKE_OVER)
	{
		if (take_over_capacity == 0)
		{
			take_over_capacity = length + sizeof(char);
		}
		take_over((uint8*)p_c, take_over_capacity, length, attr);
	}
	else
	{
		assign(p_c, length, attr, ALIGN_CAPACITY(length));
	}
}

string_t::string_t(const wchar* p_wc)
{
	size_type str_len = len(p_wc);
	assign(p_wc, str_len, CODING_UNICODE, WALIGN_CAPACITY(str_len));
}

string_t::string_t(const wchar* p_wc, buf_attr_t attr, size_type take_over_capacity)
{
	size_type str_len = len(p_wc);
	if (attr & TAKE_OVER)
	{
		if (take_over_capacity == 0)
		{
			take_over_capacity = str_len + sizeof(wchar);
		}
		take_over((uint8*)p_wc, take_over_capacity, str_len, attr);
	}
	else
	{
		assign(p_wc, str_len, attr, WALIGN_CAPACITY(str_len));
	}
}

string_t::string_t(const wchar* p_wc, size_type length)
{
	assign(p_wc, length, CODING_UNICODE, WALIGN_CAPACITY(length));
}

string_t::string_t(const wchar* p_wc, size_type length, buf_attr_t attr, size_type take_over_capacity)
{
	if (attr & TAKE_OVER)
	{
		if (take_over_capacity == 0)
		{
			take_over_capacity = length + sizeof(wchar);
		}
		take_over((uint8*)p_wc, take_over_capacity, length, attr);
	}
	else
	{
		assign(p_wc, length, attr, WALIGN_CAPACITY(length));
	}
}

string_t::string_t(const string_t& other): buffer_t(other)
{}

string_t::string_t(iterator i0, iterator i1, buf_attr_t attr)
{
	if (i0 > i1)
	{
		return;
	}
	_attr = attr;
	_used = i1 - i0;
	_capacity = ALIGN_CAPACITY(_used);
	_p_data = new uint8[_capacity];
	memmove(_p_data, i0, _used);
	*(char*)(_p_data + _used) = 0;
	if (!get_coding())
	{
		set_coding(CODING_GBK);
	}
}

string_t::string_t(witerator wi0, witerator wi1, buf_attr_t attr)
{
	if (wi0 > wi1)
	{
		return;
	}
	_attr = attr;
	_used = (wi1 - wi0) * 2;
	_capacity = WALIGN_CAPACITY(_used);
	wmemmove((witerator)_p_data, wi0, wi1 - wi0);
	*(wchar*)(_p_data + _used) = 0;
	set_coding(CODING_UNICODE);
}

string_t::string_t(const _CAP_t& capacity, buf_attr_t attr)
{
	_attr = attr;
	_p_data = NULL;
	_used = 0;
	_capacity = 0;
	if (!get_coding())
	{
		set_coding(CODING_GBK);
	}
	reserve(capacity._capacity);
}

string_t::~string_t()
{}

string_t& string_t::no_release()
{
	_attr |= NO_RELEASE;
	return *this;
}

string_t& string_t::take_over(uint8* p_mem, size_type mem_length, size_type mem_used, buf_attr_t attr)
{
	release();
	_attr = attr;
	_p_data = p_mem;
	_capacity = mem_length;
	_used = mem_used;
	return *this;
}

void string_t::set_data(const uint8* p_data, size_type length, size_type mem_grow)
{
	release();
	if (length + sizeof(wchar) > capacity())
	{
		reserve(length + sizeof(wchar) + mem_grow, 0);
	}
	memmove(_p_data, p_data, length);
	_used = length;
	*(wchar*)(_p_data + length) = 0;
}

void string_t::set_used_size(size_type new_used_size)
{
	if (new_used_size < capacity())
	{
		_used = new_used_size;
	}
	if (_attr & CODING_UNICODE)
	{
		*(wchar*)(_p_data + _used) = 0;
	}
	else
	{
		*(char*)(_p_data + _used) = 0;
	}
	throw str_excep_t(str_excep_t::OVERFLOW);
}

string_t& string_t::assign(const char* p_c, size_type length, buf_attr_t attr, size_type real_capacity)
{
	release();
	_attr = attr;
	_used = length;
	_capacity = real_capacity;
	_p_data = new uint8[_capacity];
	memmove(_p_data, p_c, length);
	*(char*)(_p_data + length) = 0;
	if (!get_coding())
	{
		set_coding(CODING_GBK);
	}
	return *this;
}

string_t& string_t::assign(const wchar* p_wc, size_type length, buf_attr_t attr, size_type real_capacity)
{
	assign((const char*)p_wc, length, attr, real_capacity);
	set_coding(CODING_UNICODE);
	return *this;
}

string_t& string_t::set_char(size_type pos, char c)
{
	if (!_used || pos > _used - sizeof(char))
	{
		throw str_excep_t(str_excep_t::OVERFLOW);
	}
	*(char*)(_p_data + pos) = c;
	return *this;
}

string_t& string_t::set_wchar(size_type pos, wchar wc)
{
	if (!_used || pos > _used - sizeof(wchar))
	{
		throw str_excep_t(str_excep_t::OVERFLOW);
	}
	if (pos % 2)
	{
		pos--;
	}
	*(wchar*)(_p_data + pos) = wc;
	return *this;
}

string_t string_t::sub(iterator i0, iterator i1) const
{
	if (i1 < i0 || i0 < begin() || i1 > end())
	{
		throw str_excep_t(str_excep_t::BAD_ITERATOR);
	}
	return string_t(i0, i1, _attr);
}

string_t string_t::sub(witerator wi0, witerator wi1) const
{
	if (wi1 < wi0 || wi0 < wbegin() || wi1 > wend())
	{
		throw str_excep_t(str_excep_t::BAD_ITERATOR);
	}
	return string_t(wi0, wi1, _attr);
}

string_t string_t::sub(size_type pos, size_type length) const
{
	if (_attr & CODING_UNICODE)
	{
		if (pos % 2)
		{
			pos--;
		}
		if (!_used || pos > _used - sizeof(wchar))
		{
			throw str_excep_t(str_excep_t::OVERFLOW);
		}
	}
	else
	{
		if (!_used || pos > _used - sizeof(char))
		{
			throw str_excep_t(str_excep_t::OVERFLOW);
		}
	}
	if (length == nopos || pos +  length > _used)
	{
		length = size() - pos;
	}
	return string_t((iterator)(_p_data + pos), (iterator)(_p_data + pos + length), _attr);
}

size_type string_t::sub_num(char c) const
{
	size_type num = 0;
	for (const_iterator i = begin(); i != end(); i++)
	{
		if (*i == c)
		{
			num++;
		}
	}
	return num;
}

size_type string_t::sub_num(wchar wc) const
{
	size_type num = 0;
	for (const_witerator wi = wbegin(); wi != wend(); wi++)
	{
		if (*wi == wc)
		{
			num++;
		}
	}
	return num;
}

size_type string_t::sub_num(const char* p_c, size_type clen) const
{
	clen = clen? clen: len(p_c);
	size_type start_pos = find(p_c, 0);
	size_type sum = 0;
	while (start_pos != nopos)
	{
		sum++;
		start_pos += clen;
		start_pos = find(p_c, start_pos);
	}
	return sum;
}

size_type string_t::sub_num(const wchar* p_wc, size_type clen) const
{
	clen = clen? clen: len(p_wc);
	size_type start_pos = find(p_wc, 0);
	size_type sum = 0;
	while (start_pos != nopos)
	{
		sum++;
		start_pos += clen;
		start_pos = find(p_wc, start_pos);
	}
	return sum;
}

string_t& string_t::encode_unicode()
{
	if (_attr & CODING_UNICODE)
	{
		return *this;
	}
	UINT code_page;
	if (_attr & CODING_GBK)
	{
		code_page = CP_ACP;
	}
	else if (_attr & CODING_UTF8)
	{
		code_page = CP_UTF8;
	}
	else if (_attr & CODING_UTF7)
	{
		code_page = CP_UTF7;
	}
	size_type new_size = MultiByteToWideChar(code_page, NULL, c_str(), -1, NULL, 0);
	if (!new_size)
	{
		throw str_excep_t(str_excep_t::ENCODE_ERROR).with_sysinfo();
	}
	size_type new_capacity = WALIGN_CAPACITY((new_size + 1) * sizeof(wchar));
	uint8* p_new = new uint8[new_capacity];
	if (MultiByteToWideChar(code_page, NULL, begin(), -1, (wchar*)p_new, new_size))
	{
		release();
		_p_data = p_new;
		_capacity = new_capacity;
		_used = new_size * sizeof(wchar);
		*(wchar*)(_p_data + _used) = 0;
		set_coding(CODING_UNICODE);
	}
	else
	{
		throw str_excep_t(str_excep_t::ENCODE_ERROR).with_sysinfo();
	}
	return *this;
}

string_t& string_t::encode(buf_attr_t code_name, size_type mem_grow)
{
	if (_attr & code_name)
	{
		return *this;
	}
	encode_unicode();
	if (code_name == CODING_UNICODE)
	{
		return *this;
	}
	UINT code_page;
	if (code_name & CODING_GBK)
	{
		code_page = CP_ACP;
	}
	else if (code_name & CODING_UTF8)
	{
		code_page = CP_UTF8;
	}
	else if (code_name & CODING_UTF7)
	{
		code_page = CP_UTF7;
	}
	else
	{
		throw str_excep_t(str_excep_t::ENCODE_ERROR);
	}
	size_type new_size = WideCharToMultiByte(code_page, NULL, wbegin(), -1, NULL, 0, NULL, NULL);
	if (!new_size)
	{
		throw str_excep_t(str_excep_t::ENCODE_ERROR).with_sysinfo();
	}
	size_type new_capacity = ALIGN_CAPACITY(new_size + mem_grow); // new_size 包含结尾的0;
	uint8* p_new = new uint8[new_capacity];
	if (WideCharToMultiByte(code_page, NULL, wbegin(), -1, (char*)p_new, new_size, NULL, NULL))
	{
		release();
		_p_data = p_new;
		_capacity = new_capacity;
		_used = new_size - 1;
		*(char*)(_p_data + _used) = 0;
		set_coding(code_name);
	}
	else
	{
		throw str_excep_t(str_excep_t::ENCODE_ERROR).with_sysinfo();
	}
	return *this;
}

size_type string_t::find(char c, size_type start_pos) const
{
	for (const char* p = begin(); p != end(); p++)
	{
		if (*p == c)
		{
			return p - begin();
		}
	}
	return nopos;
}

size_type string_t::find(wchar wc, size_type start_pos) const
{
	for (const wchar* wp = wbegin(); wp != wend(); wp++)
	{
		if (*wp == wc)
		{
			return (wp - wbegin()) * 2;
		}
	}
	return nopos;
}

size_type string_t::rfind(char c, size_type start_pos) const
{
	if (empty())
	{
		return nopos;
	}
	if (start_pos > _used)
	{
		start_pos = _used - 2;
	}
	for (const char* p = begin() + start_pos; p != begin(); p--)
	{
		if (*p == c)
		{
			return p - begin();
		}
	}
	if (*begin() == c)
	{
		return 0;
	}
	return nopos;
}

size_type string_t::rfind(wchar wc, size_type start_pos) const
{
	if (empty())
	{
		return nopos;
	}
	if (start_pos > _used)
	{
		start_pos = _used - 2;
	}
	for (const wchar* wp = (wchar*)(begin() + start_pos); wp != wbegin(); wp--)
	{
		if (*wp == wc)
		{
			return (wp - wbegin()) * 2;
		}
	}
	if (*wbegin() == wc)
	{
		return 0;
	}
	return nopos;
}

size_type string_t::find(const char* p_c, size_type start_pos) const
{
	if (start_pos > _used)
	{
		return nopos;
	}
	const char* p_start = begin() + start_pos;
	const char* p0 = p_start;
	const char* p1 = p_start;
	const char* p2 = p_c;
	for (; p0 != end(); p0++, p1 = p0, p2 = p_c)
	{
		while (*p1 == *p2)
		{
			p1++;
			p2++;
			if (*p2 == 0)
			{
				return p0 - begin();
			}
		}
	}
	return nopos;
}

size_type string_t::find(const wchar* p_wc, size_type start_pos) const
{
	if (start_pos != nopos && start_pos % 2 != 0)
	{
		start_pos++;
	}
	if (start_pos > _used)
	{
		return nopos;
	}
	const wchar* p_start = (const wchar*)(begin() + start_pos);
	const wchar* p0 = p_start;
	const wchar* p1 = p_start;
	const wchar* p2 = p_wc;
	for (; p0 < wend(); p0++, p1 = p0, p2 = p_wc)
	{
		while (*p1 == *p2)
		{
			p1++;
			p2++;
			if (*p2 == 0)
			{
				return (const char*)p0 - begin();
			}
		}
	}
	return nopos;
}

size_type string_t::rfind(const char* p_c, size_type start_pos, size_type clen) const
{
	clen = clen? clen: len(p_c);
	start_pos = start_pos > _used - 1? _used - 1: start_pos;
	if (_used == 0 || !clen)
	{
		return nopos;
	}
	const char* p_start = begin() + start_pos;     
	const char* p_end   = begin();
	const char* p0 = p_start;
	const char* p1 = p_start;
	const char* p2 = p_c + clen - 1;
	for (; p1 >= p_end; p0--, p1 = p0, p2 = p_c + clen - 1)
	{
		while (*p1 == *p2)
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

size_type string_t::rfind(const wchar* p_wc, size_type start_pos, size_type clen) const
{
	if (start_pos != nopos && start_pos % 2 != 0)
	{
		start_pos--;
	}
	clen = clen? clen: len(p_wc);
	start_pos = start_pos > _used? _used - 2: start_pos;
	if (_used == 0 || !clen)
	{
		return nopos;
	}
	const wchar* p_start = (wchar*)(begin() + start_pos);
	const wchar* p_end   = wbegin();
	const wchar* p0 = p_start;
	const wchar* p1 = p_start;
	const wchar* p2 = (wchar*)((const char*)p_wc + clen - 2);

	for (; p1 >= p_end; p0--, p1 = p0, p2 = (wchar*)((const char*)p_wc + clen - 2))
	{
		while (*p1 == *p2)
		{
			if (p2 == p_wc)
			{
				return (p1 - wbegin()) * 2;
			}
			p1--;
			p2--;
		}
	}
	return nopos;
}

size_type string_t::find(const string_t& sub, size_type start_pos) const
{
	return buffer_t::find(sub.data(), sub.size(), start_pos);
}

size_type string_t::rfind(const string_t& sub, size_type start_pos) const
{
	return buffer_t::rfind(sub.data(), sub.size(), start_pos);
}

size_type string_t::find_n(char c, size_type n, size_type start_pos) const
{
	size_type pos = start_pos;
	while (1)
	{
		pos = find(c, pos);
		if (!n || pos == nopos)
		{
			break;
		}
		n--;
		pos++;
	}
	return pos;
}

size_type string_t::find_n(wchar wc, size_type n, size_type start_pos) const
{
	size_type pos = start_pos;
	while (1)
	{
		pos = find(wc, pos);
		if (!n || pos == nopos)
		{
			break;
		}
		n--;
		pos+=sizeof(wchar);
	}
	return pos;
}

size_type string_t::find_n(const char* p_c, size_type n, size_type start_pos) const
{
	size_type pos = start_pos;
	size_type sub_len = len(p_c);
	while (1)
	{
		pos = find(p_c, pos);
		if (!n || pos == nopos)
		{
			break;
		}
		n--;
		pos+=sub_len;
	}
	return pos;
}

size_type string_t::find_n(const wchar* p_wc, size_type n, size_type start_pos) const
{
	size_type pos = start_pos;
	size_type sub_len = len(p_wc);
	while (1)
	{
		pos = find(p_wc, pos);
		if (!n || pos == nopos)
		{
			break;
		}
		n--;
		pos+=sub_len;
	}
	return pos;
}

size_type string_t::rfind_n(char c, size_type n, size_type start_pos) const
{
	size_type pos = start_pos;
	while (1)
	{
		pos = rfind(c, pos);
		if (pos == 0 && n)
		{
			return nopos;
		}
		if (!n || pos == nopos)
		{
			break;
		}
		n--;
		pos--;
	}
	return pos;
}

size_type string_t::rfind_n(wchar wc, size_type n, size_type start_pos) const
{
	size_type pos = start_pos;
	while (1)
	{
		pos = rfind(wc, pos);
		if (pos == 0 && n)
		{
			return nopos;
		}
		if (!n || pos == nopos)
		{
			break;
		}
		n--;
		pos--;
	}
	return pos;
}

size_type string_t::rfind_n(const char* p_c, size_type n, size_type start_pos, size_type clen) const
{
	clen = clen? clen: len(p_c);
	size_type pos = start_pos;
	while (1)
	{
		pos = rfind(p_c, pos, clen);
		if (pos == 0 && n)
		{
			return nopos;
		}
		if (!n || pos == nopos)
		{
			break;
		}
		n--;
		pos--;
	}
	return pos;
}

size_type string_t::rfind_n(const wchar* p_wc, size_type n, size_type start_pos, size_type clen) const
{
	clen = clen? clen: len(p_wc);
	size_type pos = start_pos;
	while (1)
	{
		pos = rfind(p_wc, pos, clen);
		if (!n || pos == nopos)
		{
			break;
		}
		n--;
		pos-=sizeof(wchar);
	}
	return pos;
}

size_type string_t::rfind_n(const string_t& buf, size_type n, size_type start_pos) const
{
	if (_attr & CODING_UNICODE)
	{
		return rfind_n(buf.c_wstr(), n, start_pos, buf.size());
	}
	else
	{
		return rfind_n(buf.c_str(), n, start_pos, buf.size());
	}
}

size_type string_t::find_n(const string_t& buf, size_type n, size_type start_pos) const
{
	if (_attr & CODING_UNICODE)
	{
		return find_n(buf.c_wstr(), n, start_pos);
	}
	else
	{
		return find_n(buf.c_str(), n, start_pos);
	}
}

size_type string_t::find_char_in_set(const char* p_c, size_type start_pos, size_type clen) const
{
	clen = clen? clen: len(p_c);
	if (start_pos > size())
	{
		start_pos = size();
	}
	string_t set(p_c);
	for (size_type pos = start_pos; pos < size(); pos++)
	{
		if (set.find(at(pos)) != nopos)
		{
			return pos;
		}
	}
	return nopos;
}

size_type string_t::find_char_in_set(const wchar* p_wc, size_type start_pos, size_type clen) const
{
	clen = clen? clen: len(p_wc);
	if (start_pos > size())
	{
		start_pos = size();
	}
	string_t set(p_wc);
	for (size_type pos = start_pos; pos < size(); pos+=sizeof(wchar))
	{
		if (set.find(wat(pos)) != nopos)
		{
			return pos;
		}
	}
	return nopos;
}

size_type string_t::find_char_in_set(const string_t& buf, size_type start_pos) const
{
	return find_char_in_set(buf.c_str(), start_pos, buf.size());
}

size_type string_t::rfind_char_in_set(const char* p_c, size_type start_pos, size_type clen) const
{
	clen = clen? clen: len(p_c);
	if (!size())
	{
		return nopos;
	}
	if (start_pos > size())
	{
		start_pos = size() - 1;
	}
	size_type pos = nopos;
	string_t set(p_c);
	for (pos = start_pos; ; pos--)
	{
		if (set.find(at(pos)) != nopos)
		{
			return pos;
		}
		if (pos == 0)
		{
			return nopos;
		}
	}
}

size_type string_t::rfind_char_in_set(const wchar* p_wc, size_type start_pos, size_type clen) const
{
	clen = clen? clen: len(p_wc);
	if (!size())
	{
		return nopos;
	}
	if (start_pos > size())
	{
		start_pos = size() - sizeof(wchar);
	}
	size_type pos = nopos;
	string_t set(p_wc);
	for (pos = start_pos; ; pos-=sizeof(wchar))
	{
		if (set.find(wat(pos)) != nopos)
		{
			return pos;
		}
		if (pos == 0)
		{
			return nopos;
		}
	}
}

size_type string_t::rfind_char_in_set(const string_t& buf, size_type start_pos) const
{
	return (_attr & CODING_UNICODE)? rfind_char_in_set(buf.c_wstr(), start_pos, buf.size()):
	                                 rfind_char_in_set(buf.c_str(), start_pos, buf.size());
}

size_type string_t::sub_num(const string_t& buf) const
{
	return  (_attr & CODING_UNICODE)? sub_num(buf.c_wstr(), buf.size()):
	                                  sub_num(buf.c_str(), buf.size());
}

bool string_t::starts_with(char c, int ignore_case) const
{
	if (_used < 1)
	{
		return false;
	}
	char c_start = *begin();
	return c_start == c? true: tolower(c_start) == tolower(c);
}

bool string_t::starts_with(wchar wc, int ignore_case) const
{
	if (_used < 2)
	{
		return false;
	}
	wchar wc_start = *wbegin();
	return wc_start == wc? true: towlower(wc_start) == towlower(wc);
}

bool string_t::starts_with(const char* p_c, int ignore_case) const
{
	const char* p0 = begin();
	const char* p1 = p_c;
	for (; *p1 != 0 && p0 != end(); p0++, p1++)
	{
		if (*p0 != *p1)
		{
			if (ignore_case && tolower(*p0) == tolower(*p1))
			{
				continue;
			}
			return 0;
		}
	}
	return 1;
}

bool string_t::starts_with(const wchar* p_wc, int ignore_case) const
{
	const wchar* p0 = wbegin();
	const wchar* p1 = p_wc;
	for (; *p1 != 0 && p0 != wend(); p0++, p1++)
	{
		if (*p0 != *p1)
		{
			if (ignore_case && towlower(*p0) == towlower(*p1))
			{
				continue;
			}
			return 0;
		}
	}
	return 1;
}

bool string_t::ends_with(char c, int ignore_case) const
{
	if (_used < 1)
	{
		return false;
	}
	char c_end = *(char*)(begin() + _used - 1);
	return c_end == c? true: tolower(c_end) == tolower(c);
}

bool string_t::ends_with(wchar wc, int ignore_case) const
{
	if (_used < 2)
	{
		return false;
	}
	wchar wc_end = *(wchar*)(begin() + _used - sizeof(wchar));
	return wc_end == wc? true: towlower(wc_end) == towlower(wc);
}

bool string_t::ends_with(const char* p_c, int ignore_case, size_type clen) const
{
	clen = clen? clen: len(p_c);
	if (!clen)
	{
		return 0;
	}
	const char* p0 = end() - 1;
	const char* p1 = p_c + clen - 1;
	while (1)
	{
		if (*p0 != *p1)
		{
			if (ignore_case && tolower(*p0) == tolower(*p1))
			{
				continue;
			}
			return 0;
		}
		if (p0 == begin() || p1 == p_c)
		{
			break;
		}
		p0--;
		p1--;
	}
	return 1;
}

bool string_t::ends_with(const wchar* p_wc, int ignore_case, size_type clen) const
{
	clen = clen? clen: len(p_wc);
	if (!clen)
	{
		return 0;
	}
	const wchar* p0 = wend() - 1;
	const wchar* p1 = (const wchar*)((const char*)p_wc + clen - sizeof(wchar));
	while (1)
	{
		if (*p0 != *p1)
		{
			if (ignore_case && towlower(*p0) == towlower(*p1))
			{
				continue;
			}
			return 0;
		}
		if (p0 == wbegin() || p1 == p_wc)
		{
			break;
		}
		p0--;
		p1--;
	}
	return 1;
}

bool string_t::starts_with(const string_t& head, int ignore_case) const
{
	if (_attr & CODING_UNICODE)
	{
		return starts_with(head.c_wstr(), ignore_case);
	}
	return starts_with(head.c_str(), ignore_case);
}

bool string_t::ends_with(const string_t& end, int ignore_case) const
{
	if (_attr & CODING_UNICODE)
	{
		return ends_with(end.c_wstr(), ignore_case, end.length());
	}
	return ends_with(end.c_str(), ignore_case, end.length());
}

string_t& string_t::erase()
{
	return erase(begin(), end());
}

string_t& string_t::erase(string_t::iterator i0, string_t::iterator i1)
{
	if (i1 < i0 || i0 < begin() || i1 > end())
	{
		throw str_excep_t(str_excep_t::BAD_ITERATOR);
	}
	if (i0 != i1)
	{
		memmove(i0, i1, end() - i1);
		_used -= i1 - i0;
		*(char*)(_p_data + _used) = 0;
	}
	return *this;
}

string_t& string_t::erase(string_t::witerator wi0, string_t::witerator wi1)
{
	if (wi1 < wi0 || wi0 < wbegin() || wi1 > wend())
	{
		throw str_excep_t(str_excep_t::BAD_ITERATOR);
	}
	if (wi0 != wi1)
	{
		memmove(wi0, wi1, (wend() - wi1) * sizeof(wchar));
		_used -= (wi1 - wi0) * sizeof(wchar);
		*(wchar*)(_p_data + _used) = 0;
	}
	return *this;
}

string_t& string_t::remove(char c, size_type max)
{
	size_type start_pos = 0;
	while (max)
	{
		size_type pos = find(c, start_pos);
		if (pos == nopos)
		{
			break;
		}
		max--;
		erase(begin() + pos, begin() + pos + sizeof(char));
		start_pos = pos;
	}
	return *this;
}

string_t& string_t::remove(wchar wc, size_type max)
{
	size_type start_pos = 0;
	while (max)
	{
		size_type pos = find(wc, start_pos);
		if (pos == nopos)
		{
			break;
		}
		max--;
		erase(begin() + pos, begin() + pos + sizeof(wchar));
		start_pos = pos;
	}
	return *this;
}

string_t& string_t::remove(const char* p_c, size_type max, size_type clen)
{
	clen = clen? clen: len(p_c);
	if (!clen)
	{
		return *this;
	}
	size_type start_pos = 0;
	while (max)
	{
		size_type pos = find(p_c, start_pos);
		if (pos == nopos)
		{
			break;
		}
		max--;
		erase(begin()+pos, begin()+pos+clen);
		start_pos = pos;
	}
	return *this;
}

string_t& string_t::remove(const wchar* p_wc, size_type max, size_type clen)
{
	clen = clen? clen: len(p_wc);
	if (!clen)
	{
		return *this;
	}
	size_type start_pos = 0;
	while (max)
	{
		size_type pos = find(p_wc, start_pos);
		if (pos == nopos)
		{
			break;
		}
		max--;
		erase(begin() + pos, begin() + pos + clen);
		start_pos = pos;
	}
	return *this;
}

string_t& string_t::remove(const string_t& buf, size_type max)
{
	return remove(buf.c_str(), max, buf.length());
}

string_t& string_t::insert(char c, size_type pos, size_type repeat, size_type mem_grow)
{
	if (!repeat)
	{
		return *this;
	}
	pos = pos > _used? _used: pos;
	if (_used + repeat > capacity())
	{
		reserve(_used + repeat + mem_grow);
	}
	char* p_insert = begin() + pos;
	char* p_insert_end = begin() + pos + repeat;
	memmove(p_insert_end, p_insert, end() - p_insert);
	for (size_type i = 0; i < repeat; i++)
	{
		*(char*)(p_insert + i) = c;
	}
	_used += repeat;
	return *this;
}

string_t& string_t::insert(wchar wc, size_type pos, size_type repeat, size_type mem_grow)
{
	if (!repeat)
	{
		return *this;
	}
	pos = pos > _used? _used: pos;
	if (pos % 2 != 0)
	{
		pos--;
	}
	if (_used + repeat * sizeof(wchar) > capacity())
	{
		reserve(_used + repeat * sizeof(wchar) + mem_grow);
	}
	char* p_insert = (char*)_p_data + pos;
	char* p_insert_end = (char*)_p_data + pos + repeat * sizeof(wchar);
	memmove(p_insert_end, p_insert, end() - p_insert);
	for (size_type i = 0; i < repeat; i++)
	{
		*(wchar*)(p_insert + i * sizeof(wchar)) = wc;
	}
	_used += repeat * sizeof(wchar);
	return *this;
}

string_t& string_t::insert(const char* p_c, size_type pos, size_type mem_grow, size_type clen)
{
	pos = pos > _used? _used: pos;
	clen = clen? clen: len(p_c);
	if (!clen)
	{
		return *this;
	}
	if (_used + clen > capacity())
	{
		reserve(length() + clen + mem_grow);
	}
	char* p_insert = (char*)_p_data + pos;
	char* p_insert_end = (char*)_p_data + pos + clen;
	memmove(p_insert_end, p_insert, end() - p_insert);
	memmove(p_insert, p_c, clen);
	_used += clen;
	*(char*)(_p_data + _used) = 0;
	return *this;
}

string_t& string_t::insert(const wchar* p_wc, size_type pos, size_type mem_grow, size_type clen)
{
	pos = pos > _used? _used: pos;
	if (pos % 2 != 0)
	{
		pos--;
	}
	clen = clen? clen: len(p_wc);
	if (!clen)
	{
		return *this;
	}
	if (_used + clen > capacity())
	{
		reserve(length() + clen + mem_grow);
	}
	char* p_insert = (char*)_p_data + pos;
	char* p_insert_end = (char*)_p_data + pos + clen;
	memmove(p_insert_end, p_insert, end() - p_insert);
	memmove(p_insert, p_wc, clen);
	_used += clen;
	*(wchar*)(_p_data + _used) = 0;
	return *this;
}

string_t& string_t::insert(const string_t& buf, size_type pos, size_type mem_grow)
{
	return insert(buf.c_str(), pos, buf.length(), mem_grow);
}

string_t& string_t::strip_left(const char* p_c)
{
	if (empty())
	{
		return *this;
	}
	string_t clothes(p_c, CONST_REF);
	for (char* p = begin(); p != end(); p++)
	{
		if (clothes.find(*p) == nopos)
		{
			break;
		}
	}
	return erase(begin(), p);
}

string_t& string_t::strip_left(const wchar* p_wc)
{
	if (empty())
	{
		return *this;
	}
	string_t clothes(p_wc, CONST_REF);
	for (wchar* wp = wbegin(); wp != wend(); wp++)
	{
		if (clothes.find(*wp) == nopos)
		{
			break;
		}
	}
	return erase(wbegin(), wp);
}

string_t& string_t::strip_right(const char* p_c)
{
	if (empty())
	{
		return *this;
	}
	string_t clothes(p_c, CONST_REF);
	for (char* p = end()-1; p != begin(); p--)
	{
		if (clothes.find(*p) == nopos)
		{
			break;
		}
	}
	char* erase_start = clothes.find(*p) == nopos? p + 1: p;
	return erase(erase_start, end());
}

string_t& string_t::strip_right(const wchar* p_wc)
{
	if (empty())
	{
		return *this;
	}
	string_t clothes(p_wc, CONST_REF);
	for (wchar* wp = wend()-1; wp != wbegin(); wp--)
	{
		if (clothes.find(*wp) == nopos)
		{
			break;
		}
	}
	wchar* erase_start = clothes.find(*wp) == nopos? wp + 1: wp;
	return erase(erase_start, wend());
}

string_t& string_t::strip(const char* p_c)
{
	strip_left(p_c);
	strip_right(p_c);
	return *this;
}

string_t& string_t::strip(const wchar* p_wc)
{
	strip_left(p_wc);
	strip_right(p_wc);
	return *this;
}

bool string_t::is_digit(int radix) const
{
	if (radix > 36 || radix <= 0)
	{
		throw str_excep_t(str_excep_t::BAD_RADIX);
	}	
	string_t copy(*this);
	copy.to_upper();
	if (_attr & CODING_UNICODE)
	{
		string_t all_base(L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", CONST_REF);
		string_t base = all_base.sub(0, radix * sizeof(wchar));
		copy.strip_left(L"-");
		if (radix == 16)
		{
			copy.strip_left(L"0x");
		}
		for (const_witerator cwi = copy.wbegin(); cwi != copy.wend(); cwi++)
		{
			if (base.find(*cwi) == nopos)
			{
				return 0;
			}
		}
	}
	else
	{
		string_t all_base("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", CONST_REF);
		string_t base = all_base.sub(0, radix);
		copy.strip_left("-");
		if (radix == 16)
		{
			copy.strip_left("0x");
		}
		for (const_iterator ci = copy.begin(); ci != copy.end(); ci++)
		{
			if (base.find(*ci) == nopos)
			{
				return 0;
			}
		}
	}
	return 1;
}

bool string_t::is_upper() const
{
	if (_attr & CODING_UNICODE)
	{
		for (const_witerator wi = wbegin(); wi != wend(); wi++)
		{
			if (*wi >= 97 && *wi <= 122)
			{
				return 0;
			}
		}
	}
	else
	{
		for (const_iterator i = begin(); i != end(); i++)
		{
			if (*i >= 97 && *i <= 122)
			{
				return 0;
			}
		}
	}
	return 1;
}

bool string_t::is_lower() const
{
	return !is_upper();
}

string_t& string_t::to_upper()
{
	if (_attr & CODING_UNICODE)
	{
		for (witerator wi = wbegin(); wi != wend(); wi++)
		{
			if (*wi >= 97 && *wi <= 122)
			{
				*wi -= 32;
			}
		}
	}
	else
	{
		for (iterator i = begin(); i != end(); i++)
		{
			if (*i >= 97 && *i <= 122)
			{
				*i -= 32;
			}
		}
	}
	return *this;
}

string_t& string_t::to_lower()
{
	if (_attr & CODING_UNICODE)
	{
		for (witerator wi = wbegin(); wi != wend(); wi++)
		{
			if (*wi >= 65 && *wi <= 90)
			{
				*wi += 32;
			}
		}
	}
	else
	{
		for (iterator i = begin(); i != end(); i++)
		{
			if (*i >= 65 && *i <= 90)
			{
				*i += 32;
			}
		}
	}
	return *this;
}

string_t& string_t::append(char c, size_type repeat, size_type mem_grow)
{
	return insert(c, nopos, repeat, mem_grow);
}

string_t& string_t::append(wchar wc, size_type repeat, size_type mem_grow)
{
	return insert(wc, nopos, repeat, mem_grow);
}

string_t& string_t::append(const char* p_c, size_type mem_grow, size_type clen)
{
	clen = clen? clen: len(p_c);
	return insert(p_c, length(), mem_grow, clen);
}

string_t& string_t::append(const wchar* p_wc, size_type mem_grow, size_type clen)
{
	clen = clen? clen: len(p_wc);
	return insert(p_wc, length(), mem_grow, clen);
}

string_t& string_t::append(const string_t& buf, size_type mem_grow)
{
	return append(buf.c_str(), mem_grow, buf.length());
}

int string_t::compare(const char* p_c, int ignore_case, size_type clen) const
{
	clen = clen? clen: len(p_c);
	size_type compare_len = min(clen, size());
	if (!ignore_case)
	{
		for (size_type i = 0; i < compare_len; i++)
		{
			if (at(i) > p_c[i])
			{
				return 1;
			}
			else if (at(i) < p_c[i])
			{
				return -1;
			}
		}
		if (clen < size())
		{
			return 1; 
		}
		else if (clen > size())
		{
			return -1;
		}
		return 0;
	}
	else
	{
		string_t s(p_c);
		s.to_lower();
		string_t copy(*this);
		copy.to_lower();
		return copy.compare(s.c_str(), 0, clen);
	}
}

int string_t::compare(const wchar* p_wc, int ignore_case, size_type clen) const
{
	clen = clen? clen: len(p_wc);
	size_type compare_len = min(clen, size());
	if (!ignore_case)
	{
		for (size_type i = 0; i < compare_len; i+=sizeof(wchar))
		{
			if (wat(i) > p_wc[i])
			{
				return 1;
			}
			else if (wat(i) < p_wc[i])
			{
				return -1;
			}
		}
		if (clen < size())
		{
			return 1; 
		}
		else if (clen > size())
		{
			return -1;
		}
		return 0;
	}
	else
	{
		string_t s(p_wc);
		s.to_lower();
		string_t copy(*this);
		copy.to_lower();
		return copy.compare(s.c_wstr(), 0, clen);
	}
}

int string_t::compare(const string_t& buf, int ignore_case) const
{
	return compare(buf.c_str(), ignore_case, buf.size());
}

bool string_t::is_negative() const
{
	if (_attr & CODING_UNICODE)
	{
		if (!empty())
			return wat(0) == '-';
	}
	else
	{
		if (!empty())
			return at(0) == '-';
	}
	return 0;
}

string_t string_t::get_abs() const
{
	if (!is_negative())
	{
		return *this;
	}
	return _attr & CODING_UNICODE? sub(sizeof(wchar)): sub(sizeof(char));
}

bool string_t::equal(const char* p_c, int ignore_case, size_type clen) const
{
	clen = clen? clen: len(p_c);
	return compare(p_c, ignore_case, clen) == 0;
}

bool string_t::equal(const wchar* p_wc, int ignore_case, size_type clen) const
{
	clen = clen? clen: len(p_wc);
	return compare(p_wc, ignore_case, clen) == 0;
}

bool string_t::equal(const string_t& buf, int ignore_case) const
{
	if (_attr & CODING_UNICODE)
	{
		return equal(buf.c_wstr(), ignore_case, buf.size());
	}
	else
	{
		return equal(buf.c_str(), ignore_case, buf.size());
	}
}

string_t& string_t::operator = (const char* p_c)
{
	set_data((const uint8*)p_c, len(p_c));
	if (!get_coding())
	{
		set_coding(CODING_GBK);
	}
	return *this;
}

string_t& string_t::operator = (const wchar* p_wc)
{
	set_data((const uint8*)p_wc, len(p_wc));
	set_coding(CODING_UNICODE);
	return *this;
}

string_t& string_t::operator = (const string_t& buf)
{
	set_data(buf.data(), buf.length());
	_attr = buf.attr();
	return *this;
}

string_t& string_t::operator += (const char* p_c)
{
	return append(p_c, 0, 0);
}

string_t& string_t::operator += (const wchar* p_wc)
{
	return append(p_wc, 0, 0);
}

string_t& string_t::operator += (const string_t& buf)
{
	return append(buf, 0);
}

string_t& string_t::operator -= (const char* p_c)
{
	return remove(p_c, nopos, 0);
}

string_t& string_t::operator -= (const wchar* p_wc)
{
	return remove(p_wc, nopos, 0);
}

string_t& string_t::operator -= (const string_t& buf)
{
	return remove(buf, nopos);
}

string_t& string_t::operator *= (int n)
{
	if (n <= 0)
	{
		set_data((const uint8*)"", 0);
		return *this;
	}
	string_t back_up(*this);
	reserve(length() * n, 1);
	while (n != 1)
	{
		append(back_up);
		n--;
	}
	return *this;
}

bool string_t::operator ! () const
{
	return _p_data == NULL || _used == 0;
}

int string_t::truncate(size_type pos) const
{
	if (pos > _used)
	{
		throw str_excep_t(str_excep_t::OVERFLOW);
	}
	char old_char = *((char*)(_p_data + pos));
	*((char*)(_p_data + pos)) = 0;
	return old_char;
}

int string_t::wtruncate(size_type pos) const
{
	if (pos > _used)
	{
		throw str_excep_t(str_excep_t::OVERFLOW);
	}
	wchar old_wchar = *((wchar*)(_p_data + pos));
	*((wchar*)(_p_data + pos)) = 0;
	return old_wchar;
}

void string_t::recover_truncate(size_type pos, int v) const
{
	if (pos > _used)
	{
		throw str_excep_t(str_excep_t::OVERFLOW);
	}
	*((char*)(_p_data + pos)) = v;
}

void string_t::recover_wtruncate(size_type pos, int v) const
{
	if (pos > _used)
	{
		throw str_excep_t(str_excep_t::OVERFLOW);
	}
	*((wchar*)(_p_data + pos)) = v;
}

string_t operator + (const string_t& left, const char* p_c)
{
	size_type clen = len(p_c);
	char* p_buf = new char[left.length() + clen + sizeof(int)];
	memmove(p_buf, left.data(), left.length());
	memmove(p_buf + left.length(), p_c, clen);
	*(int*)(p_buf + left.length() + clen) = 0;
	return string_t(p_buf, left.attr());
}

string_t operator + (const char* p_c, const string_t& right)
{
	return operator + (right, p_c);
}

string_t operator + (const string_t& left, const wchar* p_wc)
{
	size_type clen = len(p_wc);
	char* p_buf = new char[left.length() + clen + sizeof(int)];
	memmove(p_buf, left.data(), left.length());
	memmove(p_buf + left.length(), p_wc, clen);
	*(int*)(p_buf + left.length() + clen) = 0;
	return string_t((wchar*)p_buf, left.attr());
}

string_t operator + (const wchar* p_wc, const string_t& right)
{
	return operator + (right, p_wc);
}

string_t operator + (const string_t& left, const string_t& right)
{
	char* p_buf = new char[left.length() + right.length() + sizeof(int)];
	memmove(p_buf, left.data(), left.length());
	memmove(p_buf + left.length(), right.data(), right.length());
	*(int*)(p_buf + left.length() + right.length()) = 0;
	return string_t(p_buf, left.attr());
}

string_t operator - (const string_t& left, const char* p_c)
{
	string_t s(left);
	return s.remove(p_c, nopos);
}

string_t operator - (const char* p_c, const string_t& right)
{
	string_t s(p_c);
	return s.remove(right, nopos);
}

string_t operator - (const string_t& left, const string_t& right)
{
	string_t s(left);
	return s.remove(right, nopos);
}

string_t operator * (const string_t& left, int n)
{
	if (n <= 0)
	{
		return string_t("");
	}
	string_t s(_CAP(left.length() * n));
	s << left;
	while (n != 1)
	{
		s.append(left);
		n--;
	}
	return s;
}

string_t operator * (int n, const string_t& left)
{
	return operator * (left, n);
}

bool operator == (const string_t& left, const string_t& right)
{
	return left.compare(right, 0) == 0;
}

bool operator != (const string_t& left, const string_t& right)
{
	return left.compare(right, 0) != 0;
}

bool operator >= (const string_t& left, const string_t& right)
{
	return left.compare(right, 0) >= 0;
}

bool operator <= (const string_t& left, const string_t& right)
{
	return left.compare(right, 0) <= 0;
}

bool operator > (const string_t& left, const string_t& right)
{
	return left.compare(right, 0) > 0;
}

bool operator < (const string_t& left, const string_t& right)
{
	return left.compare(right, 0) < 0;
}

string_t& operator << (string_t& buf, char c)
{
	return buf.append(c);
}

string_t& operator << (string_t& buf, short n)
{
	return operator << (buf, (long)n);
}

string_t& operator << (string_t& buf, unsigned short n)
{
	return operator << (buf, (unsigned long)n);
}

string_t& operator << (string_t& buf, int n)
{
	return operator << (buf, (long)n);
}

string_t& operator << (string_t& buf, unsigned int n)
{
	return operator << (buf, (unsigned long)n);
}

string_t& operator << (string_t& buf, long n)
{
	char a[32] = {0};
	if (!ltoa(n, a, 10))
	{
		return buf;
	}
	return buf.append(a, len(a));
}

string_t& operator << (string_t& buf, unsigned long n)
{
	char a[32] = {0};
	if (!ultoa(n, a, 10))
	{
		return buf;
	}
	return buf.append(a, len(a));
}

string_t& operator << (string_t& buf, float f)
{
	char a[32] = {0};
	char d[32] = {0};
	int ipart = int(f);
	int dpart = 1000000 * (f - int(f));

	if (!itoa(ipart, a, 10))
	{
		return buf;
	}
	if (!itoa(dpart, d, 10))
	{
		return buf;
	}
	strcat(a, ".");
	strcat(a, d);
	return buf.append(a, len(a));
}

string_t& operator << (string_t& buf, double d)
{
	return operator << (buf, (float)d);
}

string_t& operator << (string_t& buf, const char* p_c)
{
	if (!p_c)
	{
		return buf;
	}
	return buf.append(p_c, len(p_c));
}

string_t& operator << (string_t& buf, const wchar* p_wc)
{
	if (!p_wc)
	{
		return buf;
	}
	return buf.append(p_wc, len(p_wc));
}

string_t& operator << (string_t& buf, const string_t& right)
{
	return buf.append(right);
}

char* hex_output(uint8 i, char* p_out, int zero_fill)
{
	p_out[2] = 0;
	*(uint16*)p_out = *(uint16*)hex_string_array[i  & 0xFF];
	if (!zero_fill)
	{
		if ((*(uint16*)p_out & 0xFF) == 0x30) *(uint16*)p_out >>= 8;
	}
	return p_out;
}

// 小尾序
char* LE_hex_output(uint16 i, char* p_out, int zero_fill)
{
	p_out[4] = 0;
	*((uint16*)p_out)   = *(uint16*)hex_string_array[(i & 0xFF00) >> 8];
	*((uint16*)p_out+1) = *(uint16*)hex_string_array[(i & 0xFF)];
	if (!zero_fill)
	{
		uint32 out = *(uint32*)p_out;
		if ((out & 0xFF) == 0x30) out >>= 8;
		if ((out & 0xFF) == 0x30) out >>= 8;
		if ((out & 0xFF) == 0x30) out >>= 8;
		*(uint32*)p_out = out;
	}
	return p_out;
}

// 小尾序
char* LE_hex_output(uint32 i, char* p_out, int zero_fill)
{
	p_out[8] = 0;
	*(uint16*)p_out     = *(uint16*)hex_string_array[(i & 0xFF000000) >> 24];
	*((uint16*)p_out+1) = *(uint16*)hex_string_array[(i & 0xFF0000) >> 16];
	*((uint16*)p_out+2) = *(uint16*)hex_string_array[(i & 0xFF00) >> 8];
	*((uint16*)p_out+3) = *(uint16*)hex_string_array[(i & 0xFF)];
	if (!zero_fill)
	{
		uint64 out = *(uint64*)p_out;
		if ((out & 0xFF) == 0x30) out >>= 8;
		if ((out & 0xFF) == 0x30) out >>= 8;
		if ((out & 0xFF) == 0x30) out >>= 8;
		if ((out & 0xFF) == 0x30) out >>= 8;
		if ((out & 0xFF) == 0x30) out >>= 8;
		if ((out & 0xFF) == 0x30) out >>= 8;
		if ((out & 0xFF) == 0x30) out >>= 8;
		*(uint64*)p_out = out;
	}
	return p_out;
}

// 小尾序
char* LE_hex_output(uint64 i, char* p_out)
{
	// 待实现。。。
	return p_out;
}

string_t& operator << (string_t& buf, const HEX& h)
{
	char a[32] = {0};
	if (byte_order::little())
	{
		for (int i = h._size - 1; i >= 0; i--)
		{
			strcat(a, hex_string_array[*((uint8*)&h._data + i)]);
		}
	}
	else
	{
		for (int i = 0; i < h._size; i++)
		{
			strcat(a, hex_string_array[*((uint8*)&h._data + i)]);
		}
	}
	if (h._pre)
	{
		buf.append(h._pre, len(h._pre));
	}
	char* p;
	if (h._attr & HEX_LOWER)
	{
		for (p = a; *p != 0; *p = tolower(*p), p++);
	}

	if (buf.get_coding() == CODING_UNICODE)
	{
		string_t tmp(a);
		return buf.append(tmp.encode(CODING_UNICODE));
	}
	else
	{
		for (p = a; *p == '0' && !(h._attr & HEX_ZERO_FILL); p++);
		return buf.append(p, len(p));
	}
}

#if 0
size_type len(const char* p_str)
{
    const char* p_c;
    const ulong* p_longword;
    register ulong longword;
	register ulong magic_bits = 0x7efefeffL;
    if (sizeof(ulong) == 8)
	{
		magic_bits = ((0x7efefefeL << 16) << 16) | 0xfefefeffL;
	}
    for (p_c = p_str; ((ulong)p_c & (sizeof(ulong)-1)) != 0; p_c++)
	{
		if (*p_c == '\0')
		{
            return p_c - p_str;
		}
    }
    p_longword = (ulong*)p_c;
    while (1)
	{
		longword = *p_longword++;
		if (__contain_0_byte(longword, magic_bits))
		{
			const char *cp = (const char*)(p_longword - 1);
			if (cp[0] == 0)
			{
				return cp - p_str;
			}
			else if (cp[1] == 0)
			{
				return cp - p_str + 1;
			}
			else if (cp[2] == 0)
			{
				return cp - p_str + 2;
			}
			if (cp[3] == 0)
			{
				return cp - p_str + 3;
			}
		}
     }
}
#endif

ACQ_NAMESPACE_END