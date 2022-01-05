#pragma once

#include "exception.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <istream>
#include <string>

namespace struct_mapping::detail
{

template<
	typename SetBool,
	typename SetFloatingPoint,
	typename SetIntegral,
	typename SetString,
	typename SetNull,
	typename SetStruct,
	typename StartStruct,
	typename EndStruct,
	typename StartArray,
	typename EndArray>
class Parser
{
public:
	using stream_type = std::basic_istream<char>;

public:
	Parser(
		SetBool set_bool_,
		SetIntegral set_integral_,
		SetFloatingPoint set_floating_point_,
		SetString set_string_,
		SetNull set_null_,
		SetStruct set_struct_,
		StartStruct start_struct_,
		EndStruct end_struct_,
		StartArray start_array_,
		EndArray end_array_)
		:	set_bool(set_bool_)
		,	set_integral(set_integral_)
		,	set_floating_point(set_floating_point_)
		,	set_string(set_string_)
		,	set_null(set_null_)
		,	set_struct(set_struct_)
		,	start_struct(start_struct_)
		,	end_struct(end_struct_)
		,	start_array(start_array_)
		,	end_array(end_array_)
	{}

	void parse(stream_type& data_)
	{
		data = nlohmann::json::parse(data_);
		if (data.is_object())
		{
			if (!set_struct("", data))
			{
				start_struct("");
				parse_struct(data);
			}
		}
		else if (data.is_array())
		{
			start_array("");
			parse_array_value(data);
		}
	}

private:

	void parse_array(json &data)
	{
		for (auto &[key, value] : data.items())
		{
			parse_array_value(value);
		}
		end_array();
	}

	void parse_array_value(json &value)
	{
		std::string name("");
		switch (value.type())
		{
		case nlohmann::detail::value_t::object:
		{
			if (!set_struct(name, value))
			{
				start_struct(name);
				parse_struct(value);
			}
			break;
		}
		case nlohmann::detail::value_t::array:
		{
			start_array(name);
			parse_array(value);
			break;
		}
		case nlohmann::detail::value_t::string:
		{
			set_string(name, value);
			break;
		}
		case nlohmann::detail::value_t::boolean:
		{
			set_bool(name, value);
			break;
		}
		case nlohmann::detail::value_t::number_integer:
		case nlohmann::detail::value_t::number_unsigned:
		case nlohmann::detail::value_t::number_float:
		{
			set_number(name, value);
			break;
		}
		case nlohmann::detail::value_t::discarded:
		case nlohmann::detail::value_t::null:
		default:
		{
			set_null(name);
			break;
		}
		}
	}


	void parse_struct(json &data)
	{
		parse_struct_value(data);
		end_struct();
	}

	void parse_struct_value(json &data)
	{
		for (auto &item : data.items())
		{
			std::string name = item.key();
			json value = item.value();
			switch (value.type())
			{
			case nlohmann::detail::value_t::object:
			{
				if (!set_struct(name, value))
				{
					start_struct(name);
					parse_struct(value);
				}
				break;
			}
			case nlohmann::detail::value_t::array:
			{
				start_array(name);
				parse_array(value);
				break;
			}
			case nlohmann::detail::value_t::string:
			{
				set_string(name, value);
				break;
			}
			case nlohmann::detail::value_t::boolean:
			{
				set_bool(name, value);
				break;
			}
			case nlohmann::detail::value_t::binary:
				break;
			case nlohmann::detail::value_t::number_integer:
			case nlohmann::detail::value_t::number_unsigned:
			case nlohmann::detail::value_t::number_float:
			{
				set_number(name, value);
				break;
			}
			case nlohmann::detail::value_t::null:
			case nlohmann::detail::value_t::discarded:
			default:
			{
				set_null(name);
				break;
			}
			}
		}
	}

	void set_number(std::string &name, json &data)
	{
		if (data.is_number_float())
		{
			set_floating_point(name, data);
		}
		else
		{
			set_integral(name, data);
		}
	}

private:
	SetBool set_bool;
	SetIntegral set_integral;
	SetFloatingPoint set_floating_point;
	SetString set_string;
	SetNull set_null;
	SetStruct set_struct;
	StartStruct start_struct;
	EndStruct end_struct;
	StartArray start_array;
	EndArray end_array;
	json data;
};

} // struct_mapping::detail