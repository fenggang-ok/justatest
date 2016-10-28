#include "acqpath.h"
#include "acqexception.h"
#include <Shlwapi.h>

ACQ_NAMESPACE_START

path_t::path_t(): string_t()
{}

path_t::path_t(const char* p_c, buf_attr_t attr): string_t(p_c, attr)
{}

path_t::path_t(const wchar* p_wc): string_t(p_wc)
{}

path_t::path_t(const string_t& other): string_t(other)
{}

path_t::path_t(const path_t& other): string_t(other)
{}

path_t::~path_t()
{}

string_t path_t::base_name() const
{
	if (_attr & CODING_UNICODE)
	{
		size_type pos = rfind(WPATH_SEPARATOR);
		if (pos == nopos)
		{
			return *this;
		}
		return sub(pos + sizeof(wchar));
	}
	else
	{
		size_type pos = rfind(PATH_SEPARATOR);
		if (pos == nopos)
		{
			return *this;
		}
		return sub(pos + sizeof(char));
	}
}

string_t& path_t::base_name(string_t& str) const
{
	size_type pos;
	if (_attr & CODING_UNICODE)
	{
		pos = rfind(WPATH_SEPARATOR);
		if (pos == nopos)
		{
			str = *this;
			return str;
		}
		pos += sizeof(wchar);
	}
	else
	{
		pos = rfind(PATH_SEPARATOR);
		if (pos == nopos)
		{
			str = *this;
			return str;
		}
		pos += sizeof(char);
	}
	str.set_data(data() + pos, _used - pos);
	return str;
}

string_t path_t::dir_name() const
{
	size_type pos;
	if (_attr & CODING_UNICODE)
	{
		pos = rfind(WPATH_SEPARATOR);
	}
	else
	{
		pos = rfind(PATH_SEPARATOR);
	}
	if (pos == nopos)
	{
		string_t s;
		return s;
	}
	return sub(0, pos);
}

string_t& path_t::dir_name(string_t& str) const
{
	size_type pos;
	if (_attr & CODING_UNICODE)
	{
		pos = rfind(WPATH_SEPARATOR);
	}
	else
	{
		pos = rfind(PATH_SEPARATOR);
	}
	if (pos == nopos)
	{
		str.clean_out();
	}
	else
	{
		str.set_data(0, pos);
	}
	return str;
}

bool path_t::is_abs() const
{
	bool res = false;
	if (_attr & CODING_UNICODE)
	{
		res = !PathIsRelativeW(c_wstr());
	}
	else if (_attr & CODING_ANSI)
	{
		res = !PathIsRelativeA(c_str());
	}
	else if (_attr & CODING_UTF8)
	{
		path_t copy(*this);
		copy.encode(CODING_UNICODE);
		res = !PathIsRelativeW(copy.c_wstr());
	}
	return res;
}

string_t path_t::driver_name() const
{
	if (is_abs())
	{
		return _attr & CODING_UNICODE? sub(0, sizeof(wchar)): sub(0, sizeof(char));
	}
	return string_t();
}

path_t& path_t::join(const string_t& right)
{
	string_t right_copy(right);
	if (has_attr(CODING_UNICODE))
	{
		string_t clothes(WPATH_SEPARATOR);
		clothes.append(WSTR_CLOTHES);
		strip_right(clothes);
		right_copy.strip_left(clothes);
		append(WPATH_SEPARATOR);
	}
	else
	{
		string_t clothes(PATH_SEPARATOR);
		clothes.append(STR_CLOTHES);
		strip_right(clothes);
		right_copy.strip_left(clothes);
		append(PATH_SEPARATOR);
	}
	append(right_copy);
	return *this;
}

path_t& path_t::join(const char* p_c, uint32 coding)
{
	return join(string_t(p_c, coding));
}

path_t& path_t::join(const wchar* p_wc)
{
	return join(string_t(p_wc));
}

bool path_t::exists() const
{
	bool res = false;
	if (_attr & CODING_UNICODE)
	{
		res = PathFileExistsW(c_wstr()) > 0;
	}
	else if (_attr & CODING_ANSI)
	{
		res = PathFileExistsA(c_str()) > 0;
	}
	else if (_attr & CODING_UTF8)
	{
		path_t copy(*this);
		copy.encode(CODING_UNICODE);
		res = PathFileExistsW(copy.c_wstr()) > 0;
	}
	return res;
}


bool path_t::is_file() const
{
	if (exists())
	{
		return !is_dir();
	}
	return 0;
}

bool path_t::is_dir() const
{
	bool res = false;
	if (_attr & CODING_UNICODE)
	{
		res = PathIsDirectoryW(c_wstr()) > 0;
	}
	else if (_attr & CODING_ANSI)
	{
		res = PathIsDirectoryA(c_str()) > 0;
	}
	else if (_attr & CODING_UTF8)
	{
		path_t copy(*this);
		copy.encode(CODING_UNICODE);
		res = PathIsDirectoryW(copy.c_wstr()) > 0;
	}
	return res;
}

bool path_t::is_url() const
{
	bool res = false;
	if (_attr & CODING_UNICODE)
	{
		res = PathIsURLW(c_wstr()) > 0;
	}
	else if (_attr & CODING_ANSI)
	{
		res = PathIsURLA(c_str()) > 0;
	}
	else if (_attr & CODING_UTF8)
	{
		path_t copy(*this);
		copy.encode(CODING_UNICODE);
		res = PathIsURLW(copy.c_wstr()) > 0;
	}
	return res;
}

bool path_t::is_unc() const
{
	bool res = false;
	if (attr() & CODING_UNICODE)
	{
		res = PathIsUNCW(c_wstr()) > 0;
	}
	else if (attr() & CODING_ANSI)
	{
		res = PathIsUNCA(c_str()) > 0;
	}
	else if (_attr & CODING_UTF8)
	{
		path_t copy(*this);
		copy.encode(CODING_UNICODE);
		res = PathIsUNCW(copy.c_wstr()) > 0;
	}
	return res;
}

DWORD path_t::get_attr(DWORD flag) const
{
	DWORD attribute;
	if (_attr & CODING_UNICODE)
	{
		attribute = GetFileAttributesW(c_wstr());
	}
	else if (_attr & CODING_ANSI)
	{
		attribute = GetFileAttributesA(c_str());
	}
	else if (_attr & CODING_UTF8)
	{
		path_t copy(*this);
		copy.encode(CODING_UNICODE);
		attribute = GetFileAttributesW(copy.c_wstr());
	}

	if (attribute != INVALID_FILE_ATTRIBUTES)
	{
		if (flag)
		{
			return attribute & flag;
		}
		return attribute;
	}

	return 0;
}

bool path_t::is_hidden() const
{
	return get_attr(FILE_ATTRIBUTE_HIDDEN) > 0;
}

bool path_t::is_read_only() const
{
	return get_attr(FILE_ATTRIBUTE_READONLY) > 0; 
}

bool path_t::has_wildcard() const
{
	if (_attr & CODING_UNICODE)
	{
		return rfind_char_in_set(L"*?") != nopos;
	}
	else
	{
		return rfind_char_in_set("*?") != nopos;
	}
}

int path_t::level() const
{
	if (is_url())
	{
		return 0;
	}
	else if (is_unc())
	{
		return 0;
	}
	else
	{
		if (_attr & CODING_UNICODE)
		{
			size_type sep_num = sub_num(PATH_SEPARATOR);
			return ends_with(PATH_SEPARATOR)? sep_num: sep_num + 1;
		}
		else
		{
			size_type sep_num = sub_num(WPATH_SEPARATOR);
			return ends_with(WPATH_SEPARATOR)? sep_num: sep_num + 1;
		}
	}
	return -1;
}

path_t path_t::level(int idx) const
{
	path_t copy(_CAP(length() + 2));
	copy.append(c_str());
	if (!copy.ends_with(PATH_SEPARATOR))
	{
		copy.append(PATH_SEPARATOR);
	}
	size_type pos = nopos;
	if (idx >= 0)
	{
		pos = copy.find_n(PATH_SEPARATOR, idx);
	}
	else
	{
		pos = copy.rfind_n(PATH_SEPARATOR, 0-idx);
	}
	if (pos != nopos)
	{
		return copy.erase(copy.begin() + pos, copy.end());
	}
	return path_t("");
}

#if 0
size_type path_t::level_pos(int idx) const
{
	size_type pos = nopos;
	if (!ends_with(PATH_SEPARATOR))
	{
		set_end(PATH_SEPARATOR);
		pos = idx >= 0? find_n(PATH_SEPARATOR, idx): rfind_n(PATH_SEPARATOR, 0-idx);
		recover_end();
	}
	else
	{
		pos = idx >= 0? find_n(PATH_SEPARATOR, idx): rfind_n(PATH_SEPARATOR, 0-idx);
	}
	return pos;
}

size_type path_t::existed_pos() const
{
	size_type pos = nopos;
	int flag = 0;
	if (!ends_with(PATH_SEPARATOR))
	{
		set_end(PATH_SEPARATOR);
		flag = 1;
	}
	for (pos = _used; pos != 0; pos--)
	{
		if (at(pos) != PATH_SEPARATOR)
		{
			continue;
		}
		uint32 v = truncate(pos);
		if (PathFileExists((const char*)_p_data))
		{
			recover_truncate(pos, v);
			break;
		}
		recover_truncate(pos, v);
	}
	if (flag)
	{
		recover_end();
	}

	return pos;
}

path_t path_t::existed_path() const
{
	size_type pos = existed_pos();
	if (pos != nopos)
	{
		return sub(0, pos);
	}
	return path_t("");
}

bool path_t::make_dir() const
{
	if (exists())
	{
		return false;
	}

	size_type pos = existed_pos();
	if (pos == nopos)
	{
		return false; // ÅÌ·û¶¼²»´æÔÚ
	}

	int flag = 0;
	if (!ends_with(PATH_SEPARATOR))
	{
		set_end(PATH_SEPARATOR);
		flag = 1;
	}
	while (1)
	{
		pos = find(PATH_SEPARATOR, pos + 1);
		if (pos == nopos)
		{
			break;
		}
		uint32 v = truncate(pos);
		if (!CreateDirectory((const char*)_p_data, NULL))
		{
			recover_truncate(pos, v);
			break;
		}
		recover_truncate(pos, v);
	}
	if (flag)
	{
		recover_end();
	}

	return exists();
}

bool path_t::remove(int all)
{
	if (!exists())
	{
		return true;
	}
	if (is_file())
	{
		return DeleteFile(c_str()) > 0;
	}
	else if (is_dir())
	{
		return RemoveDirectory(c_str()) > 0;
	}
	return false;
}

path_t path_t::change_to() const
{
	return *this;
}

path_t path_t::get_curdir()
{
	DWORD path_len = GetCurrentDirectory(0, NULL);
	path_t path;
	path.reserve(path_len);
	if (!GetCurrentDirectory(path_len, path))
	{
		// throw ...
	}
	path.adapt();
	return path;
}

path_t path_t::set_workingdir(const char* p_c)
{
	path_t old_workingdir = get_curdir();
	if (!SetCurrentDirectory(p_c))
	{
		//throw ...
	}
	return old_workingdir;
}

path_t path_t::set_workingdir(const path_t& path)
{
	return set_workingdir((const char*)path);
}
#endif
ACQ_NAMESPACE_END