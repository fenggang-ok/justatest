#pragma once

#include "../def.h"

ACQ_NAMESPACE_START

static const size_type match_nopos = ~(size_type)0;

ACQ_DLL_FUN_DECLEAR(size_type) match_char(uint8, const uint8*, size_type);

/**
 *  朴素匹配，从头开始
 */
ACQ_DLL_FUN_DECLEAR(size_type) naive_match(const uint8*, size_type, const uint8*, size_type);

ACQ_DLL_FUN_DECLEAR(int) __prepare_good_suffixes_shift_table(const uint8*, size_type, size_type*);
ACQ_DLL_FUN_DECLEAR(int) __prepare_bad_char_shift_table(const uint8*, size_type, size_type*);

ACQ_DLL_CLASS_DECLEAR(bm_tables_t)
{
public:
	size_type* _p_bad_char_table;
	size_type* _p_good_suffix_table;

public:
	bm_tables_t();
	bm_tables_t(const uint8*, size_type);
	~bm_tables_t();

	int prepare(const uint8*, size_type);
	int prepare_bad_char_table(const uint8*, size_type);
	int prepare_good_suffix_table(const uint8*, size_type);
};

ACQ_DLL_FUN_DECLEAR(size_type) bm_match(const uint8*, size_type, const uint8*, size_type, bm_tables_t*);

ACQ_NAMESPACE_END