#include "acqexception.h"

ACQ_NAMESPACE_START


exception_t::exception_t(DWORD code): _p_message(NULL),
			                          _code(code)
{}

exception_t::exception_t(const char* p_message): _p_message(NULL),
											     _code(0)
{
	if (!p_message)
	{
		return;
	}
	int msg_len = len(p_message);
	_p_message = new char[msg_len + sizeof(int)];
	memset(_p_message, 0, msg_len + sizeof(int));
	memmove(_p_message, p_message, msg_len);
}

exception_t::exception_t(const exception_t& e)
{
	if (e._p_message)
	{
		int msg_len = len(e._p_message);
		_p_message = new char[msg_len + sizeof(int)];
		memset(_p_message, 0, msg_len + sizeof(int));
		memmove(_p_message, e._p_message, msg_len);
	}
}

exception_t::exception_t(const string_t& buf)
{
	if (buf.has_attr(NO_RELEASE))
	{
		_p_message = (char*)buf.data();
	}
	else
	{
		int msg_len = len(buf.c_str());
		_p_message = new char[msg_len + sizeof(int)];
		memset(_p_message, 0, msg_len + sizeof(int));
		memmove(_p_message, buf.c_str(), msg_len);
	}
}

exception_t::~exception_t()
{
	if (_p_message)
	{
		delete[] _p_message;
	}
}

DWORD exception_t::code() const
{
	return _code;
}

const char* exception_t::info() const
{
	if (_p_message)
	{
		return _p_message;
	}
	return "";
}

exception_t& exception_t::with_sysinfo(const char* p_c)
{
	LPCTSTR p_sys_message = NULL;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&p_sys_message,
        64,
        NULL);
	size_t sep_len = len(p_c);
	size_t acu_msg_len = lstrlen((LPCTSTR)_p_message);
	size_t sys_msg_len = lstrlen((LPCTSTR)p_sys_message);
	size_t merge_len = 0;
	if (!(merge_len  = sys_msg_len + acu_msg_len))
	{
		return *this;
	}

	char* p_merge = new char[merge_len + sep_len + 4];
	memmove(p_merge, _p_message, acu_msg_len);

	if (p_sys_message)
	{
		memmove(p_merge + acu_msg_len, p_c, sep_len);
		memmove(p_merge + acu_msg_len + sep_len, p_sys_message, sys_msg_len);
		char* p_end = p_merge + acu_msg_len + sep_len + sys_msg_len;
		if (*(p_end - 1) == '\n') *(p_end - 1) = 0;
		if (*(p_end - 2) == '\r') *(p_end - 2) = 0;
		*(int*)(p_end) = 0;
	}
	delete[] _p_message;
	_p_message = p_merge;
	return *this;
}

const char* str_excep_t::messages[] = {"正常", "越界", "迭代器错误", "编码错误", "进制错误"};

str_excep_t::str_excep_t(DWORD code): exception_t(code)
{}

str_excep_t::~str_excep_t()
{}

const char* str_excep_t::info() const
{
	if (_p_message)
	{
		return _p_message;
	}
	else if (_code < 4)
	{
		return messages[_code];
	}
	return "";
}

str_excep_t& str_excep_t::with_sysinfo(const char* p_c)
{
	if (_code < 4)
	{
		int msg_len = len(messages[_code]);
		_p_message = new char[msg_len + sizeof(int)];
		memset(_p_message, 0, msg_len + sizeof(int));
		memmove(_p_message, messages[_code], msg_len);
	}
	exception_t::with_sysinfo(p_c);
	return *this;
}

ACQ_NAMESPACE_END