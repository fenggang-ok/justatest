#include "../def.h"
#include "acqmatch.h"
#include "acqexception.h"

#include <iostream>

ACQ_NAMESPACE_START

bm_tables_t::bm_tables_t(): _p_bad_char_table(NULL), _p_good_suffix_table(NULL)
{}

bm_tables_t::bm_tables_t(const uint8* p_pattern, size_type pat_len): _p_bad_char_table(NULL), _p_good_suffix_table(NULL)
{
	prepare(p_pattern, pat_len);
}

bm_tables_t::~bm_tables_t()
{
	if (_p_bad_char_table)
	{
		delete[] _p_bad_char_table;
	}
	if (_p_good_suffix_table)
	{
		delete[] _p_good_suffix_table;
	}
}

int bm_tables_t::prepare(const uint8* p_pattern, size_type pat_len)
{
	return prepare_bad_char_table(p_pattern, pat_len) + prepare_good_suffix_table(p_pattern, pat_len);
}

int bm_tables_t::prepare_bad_char_table(const uint8* p_pattern, size_type pat_len)
{
	if (!p_pattern || !pat_len)
	{
		return -1;
	}
	_p_bad_char_table = new size_type[256];
	return __prepare_bad_char_shift_table(p_pattern, pat_len, _p_bad_char_table);
}

int bm_tables_t::prepare_good_suffix_table(const uint8* p_pattern, size_type pat_len)
{
	if (!p_pattern || !pat_len)
	{
		return -1;
	}
	_p_good_suffix_table = new size_type[pat_len];
	return __prepare_good_suffixes_shift_table(p_pattern, pat_len, _p_good_suffix_table);
}

size_type match_char(uint8 uchar, const uint8* p_text, size_type text_len) // 需要优化
{
	for (size_type i = 0; i < text_len; i++)
	{
		if (uchar == p_text[i])
		{
			return i;
		}
	}
	return match_nopos;
}

// 速度比strstr函数慢一倍...
size_type naive_match(const uint8* p_pattern, size_type pat_len, const uint8* p_text, size_type text_len)
{
	const uint8* p0 = p_text;
	const uint8* p1 = p_text;
	const uint8* p2 = p_pattern;
	for (; p0 - p_text != text_len; p0++, p1 = p0, p2 = p_pattern)
	{
		while (*p1++ == *p2++)
		{
			if (p2 - p_pattern == pat_len)
			{
				return p0 - p_text;
			}
		}
	}
	return match_nopos;
}

int __is_prefix(const uint8* word, size_type word_len, size_type pos)
{
    size_type suffix_len = word_len - pos;
    for (size_type i = 0; i < suffix_len; i++)
	{
        if (word[i] != word[pos + i])
		{
            return 0;
        }
    }
    return 1;
}

size_type __suffix_length(const uint8* word, size_type word_len, size_type pos)
{
    for (size_type i = 0; word[pos-i] == word[word_len - 1 - i] && i < pos; i++);
    return i;
}

int __prepare_good_suffixes_shift_table(const uint8* p_pattern, size_type pat_len, size_type* p_table)
{
    size_type last_prefix_index = pat_len - 1;
    for (size_type p = pat_len - 1; ; p--)
	{
        if (__is_prefix(p_pattern, pat_len, p+1))
		{
            last_prefix_index = p + 1;
        }
        p_table[p] = last_prefix_index + pat_len - 1 - p;
		if (!p)
		{
			break;
		}
    }
    for (p = 0; p < pat_len - 1; p++)
	{
        size_type slen = __suffix_length(p_pattern, pat_len, p);
        if (p_pattern[p - slen] != p_pattern[pat_len -1 - slen])
		{
            p_table[pat_len - 1 - slen] = pat_len-1 - p + slen;
        }
    }
	return 0;
}

int __prepare_bad_char_shift_table(const uint8* p_pattern, size_type pat_len, size_type* p_table)
{
	if (!p_pattern || pat_len < 1 || !p_table)
	{
		return -1;
	}
	for (size_type i = 0; i < 256; i++)
	{
		p_table[i] = pat_len;
	}
	for (i = 0; i < pat_len - 1; i++)
	{
		p_table[p_pattern[i]] = pat_len - i - 1;
	}
	return 0;
}

size_type bm_match(const uint8* p_pattern, size_type pat_len, const uint8* p_text, size_type text_len, bm_tables_t* p_tables)
{
	size_type i = pat_len - 1;
	while (i < text_len)
	{
		size_type j = pat_len - 1;
		while (p_text[i] == p_pattern[j] && j != maxsize)
		{
			i--;
			j--;
		}
		if (j == maxsize)
		{
			return i + 1;
		}
		i += get_max(p_tables->_p_bad_char_table[p_text[i]], p_tables->_p_good_suffix_table[j]);
	}
	return match_nopos;
}

ACQ_NAMESPACE_END