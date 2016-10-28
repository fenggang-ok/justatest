#pragma once

#include "../def.h"
#include "acqstring.h"

#include <windows.h>

#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFF

ACQ_NAMESPACE_START

static const char  PATH_SEPARATOR  = '\\';
static const wchar WPATH_SEPARATOR = L'\\';

ACQ_DLL_CLASS_DECLEAR(path_t): public string_t
{
public:
	static bool change_to(const char*);
	static bool change_to(const path_t&);
	static path_t get_curdir();
	static path_t set_workingdir(const char*);
	static path_t set_workingdir(const path_t&);

public:
	path_t();
	path_t(const char*, buf_attr_t = CODING_GBK);
	path_t(const wchar*);
	path_t(const string_t&);
	path_t(const path_t&);

	~path_t();

	string_t base_name() const;
	string_t& base_name(string_t&) const;

	string_t dir_name() const;
	string_t& dir_name(string_t&) const;
	string_t driver_name() const;

	path_t& join(const string_t&);
	path_t& join(const char*, uint32 = CODING_GBK);
	path_t& join(const wchar*);

	path_t change_to() const;

	int level() const;
	path_t level(int) const;
	size_type level_pos(int) const;
	size_type existed_pos() const;
	path_t existed_path() const;

	bool exists() const;
	bool is_dir() const;
	bool is_file() const;
	bool is_hidden() const;
	bool is_read_only() const;
	bool has_wildcard() const;

	bool is_abs() const;
	bool is_root() const;
	bool is_unc() const;
	bool is_url() const;

	bool is_parent(const path_t&) const;
	bool is_sub(const path_t&) const;

	DWORD get_attr(DWORD = 0) const;

	bool make_dir() const;
	bool remove(int = 0);

	size_type get_sub_dir(std::vector<path_t>&) const;  // 跨dll时，内存应该遵循哪里申请哪里释放的原则。
};

ACQ_NAMESPACE_END