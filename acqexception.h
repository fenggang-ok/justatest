#pragma once

#include "../def.h"
#include "acqstring.h"

#include <windows.h>
#include <stdio.h>

ACQ_NAMESPACE_START

ACQ_DLL_CLASS_DECLEAR(exception_t)
{
public:
	char*  _p_message;
	DWORD  _code;

public:
	exception_t(DWORD code = 0);
	exception_t(const char*);
	exception_t(const string_t&);
	exception_t(const exception_t&);
    virtual ~exception_t();

	DWORD code() const;
    NEW_VIRTUAL const char* info() const;
	exception_t& with_sysinfo(const char* p_c = ": ");
};

ACQ_DLL_CLASS_DECLEAR(str_excep_t): public exception_t
{
public:
	enum
	{
		OVERFLOW = 1,
		BAD_ITERATOR,
		ENCODE_ERROR,
		BAD_RADIX
	};

	static const char* messages[];

public:
	str_excep_t(DWORD code = 0);
	virtual ~str_excep_t();
	OVERWRITE const char* info() const;
	str_excep_t& with_sysinfo(const char* p_c = ": ");
};

inline void output_dbg_f(const char* p_fmt, ...)
{
    char buf[256] = {0};
    va_list args;
    va_start(args, p_fmt);
    vsprintf(buf, p_fmt, args);
    va_end(args);
    OutputDebugString(buf);
}

ACQ_NAMESPACE_END