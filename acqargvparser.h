#pragma once
#pragma warning(disable : 4786)

#include "../def.h"

#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

ACQ_NAMESPACE_START

static const int on  = 1;
static const int off = 0;
static const int unknown = -1;

#ifdef DLLFLAG
typedef std::vector<std::string>            string_vector_t;
typedef std::map<std::string, int>          switch_map_t;
struct _param_t
{
	int _ignore_case;
	std::string _value;

	_param_t(int ignore_case = 1, std::string value = ""):
	         _ignore_case(ignore_case), _value(value)
	{
		 if (ignore_case)
		 {
			to_lower();
		 }
	}
	void to_lower()
	{
		std::transform(_value.begin(), _value.end(), _value.begin(), tolower);
	}
	const char* c_str() const
	{
		return _value.c_str();
	}
};
typedef std::map<std::string, _param_t>  param_map_t;
struct _argv_parser_inner_data_t
{
	std::string      _arg0;
	string_vector_t  _v_arg;
	param_map_t      _m_param;
	switch_map_t     _m_switch;
	string_vector_t  _v_anonymous_param;
};
#endif

ACQ_DLL_CLASS_DECLEAR(argv_parser_t)
{
private:
	void* _p_data;

public:
	argv_parser_t(int argc, char* argv[]);
	virtual ~argv_parser_t();

	const char* arg0() const;

	void set_switch(const char*, int status = off);
	void set_param(const char*, const char* p_value = "", int ignore_case = 1);

	int is_switch(const char*);
	int is_param(const char*);
	int get_switch_status(const char*);

	int get_param_value(const char*, const char**);
	int get_anonymous_params(const char***); // 输出参数需要free

	int parse();
};

ACQ_NAMESPACE_END