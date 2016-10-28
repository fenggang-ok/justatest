#include "acqargvparser.h"

ACQ_NAMESPACE_START

argv_parser_t::argv_parser_t(int argc, char* argv[]): _p_data(NULL)
{
	if (argc < 1 || !argv)
	{
		return;
	}
	_p_data = new _argv_parser_inner_data_t;
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;
	p_data->_arg0 = argv[0];
	for (int i = 1; i < argc; i++)
	{
		p_data->_v_arg.push_back(std::string(argv[i]));
	}
}

argv_parser_t::~argv_parser_t()
{
	if (_p_data)
	{
		delete _p_data;
	}
}

const char* argv_parser_t::arg0() const
{
	if (!_p_data)
	{
		return NULL;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;
	return p_data->_arg0.c_str();
}

void argv_parser_t::set_switch(const char* p_name, int status)
{
	if (!_p_data)
	{
		return;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;
	switch_map_t::iterator it = p_data->_m_switch.find(p_name);
	if (it != p_data->_m_switch.end())
	{
		it->second = status;
	}
	else
	{
		p_data->_m_switch.insert(switch_map_t::value_type(p_name, status));
	}
}

void argv_parser_t::set_param(const char* p_name, const char* p_value, int ignore_case)
{
	if (!_p_data)
	{
		return;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;
	std::string name = p_name;
	std::transform(name.begin(), name.end(), name.begin(), tolower); 
	param_map_t::iterator it = p_data->_m_param.find(name);
	if (it != p_data->_m_param.end())
	{
		it->second._value = p_value;
		if (it->second._ignore_case)
		{
			it->second.to_lower();
		}
	}
	else
	{
		_param_t param(ignore_case, p_value);
		p_data->_m_param.insert(param_map_t::value_type(name, param));
	}
}

int argv_parser_t::is_switch(const char* p_name)
{
	if (!_p_data)
	{
		return 0;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;
	switch_map_t::iterator it = p_data->_m_switch.find(p_name);
	return it != p_data->_m_switch.end();
}

int argv_parser_t::is_param(const char* p_name)
{
	if (!_p_data)
	{
		return 0;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;
	param_map_t::iterator it = p_data->_m_param.find(p_name);
	return it != p_data->_m_param.end();
}

int argv_parser_t::parse()
{
	if (!_p_data)
	{
		return 0;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;

	string_vector_t::iterator it;
	for (it = p_data->_v_arg.begin(); it != p_data->_v_arg.end(); it++)
	{
		std::string::size_type pos = (*it).find('=');
		if (pos == std::string::npos)
		{
			std::string temp = *it;
			std::transform(temp.begin(), temp.end(), temp.begin(), tolower);
			if (is_switch(temp.c_str()))
			{
				set_switch(temp.c_str(), on);
			}
			else if (is_param(temp.c_str()))
			{
				std::cerr << "参数" << *it << "的值缺失，请注意等号（不要加入空格）" << std::endl;
				return 0;
			}
			else
			{
				p_data->_v_anonymous_param.push_back((*it).c_str());
			}
		}
		else
		{
			std::string name  = (*it).substr(0, pos);
			std::string value = (*it).substr(pos + 1);

			std::string temp = name;
			std::transform(temp.begin(), temp.end(), temp.begin(), tolower);

			if (is_switch(temp.c_str()))
			{
				std::transform(value.begin(), value.end(), value.begin(), tolower);
				if (value=="on")
				{
					set_switch(temp.c_str(), on);
				}
				else if (value=="off")
				{
					set_switch(temp.c_str(), off);
				}
				else
				{
					std::cerr << "开关" << name << "只能为on或off" << std::endl;
					return 0;
				}
			}
			else if (is_param(temp.c_str()))
			{
				set_param(temp.c_str(), value.c_str());
			}
			else
			{
				p_data->_v_anonymous_param.push_back(*it);
			}
		}
	}

	return 1;
}

int argv_parser_t::get_switch_status(const char* p_name)
{
	if (!_p_data)
	{
		return unknown;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;

	if (is_switch(p_name))
	{
		return p_data->_m_switch[p_name];
	}
	return unknown;
}

int argv_parser_t::get_param_value(const char* p_name, const char** pp_value)
{
	if (!_p_data)
	{
		return 0;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;

	if (is_param(p_name))
	{
		*pp_value = p_data->_m_param[p_name].c_str();
		return 1;
	}
	return 0;
}

int argv_parser_t::get_anonymous_params(const char*** ppp_params)
{
	if (!_p_data)
	{
		return 0;
	}
	_argv_parser_inner_data_t* p_data = (_argv_parser_inner_data_t*)_p_data;

	int num = p_data->_v_anonymous_param.size();
	if (num)
	{
		(*ppp_params) = new const char*[num];
		for (int i = 0; i < num; i++)
		{
			(*ppp_params)[i] = p_data->_v_anonymous_param[i].c_str();
		}
	}

	return num;
}

ACQ_NAMESPACE_END