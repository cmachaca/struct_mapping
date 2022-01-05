#pragma once

#include "exception.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <functional>
#include <string>

namespace struct_mapping
{
template<
	typename T,
	bool exist = false>
struct IsMemberNLJsonExist
{
	static inline bool value = exist;
};

template<typename T>
class MemberNLJson
{
public:
	using FromJson = T (const json&);
	using ToJson = json (T);

public:
	template<
		typename From,
		typename To>
	static void set(From function_from_json_, To function_to_json_)
	{
		IsMemberNLJsonExist<T>::value = true;
		function_from_json = std::function<FromJson>(function_from_json_);
		function_to_json = std::function<ToJson>(function_to_json_);
	}

	static auto& from_json(const std::string& name = "")
	{
		if (IsMemberNLJsonExist<T>::value)
		{
			return function_from_json;
		}
		throw StructMappingException("MemberNLJson not set for member: " + name);
	}

	static auto& to_json(const std::string& name = "")
	{
		if (IsMemberNLJsonExist<T>::value)
		{
			return function_to_json;
		}
		throw StructMappingException("MemberNLJson not set for member: " + name);
	}

private:
	static inline std::function<FromJson> function_from_json;
	static inline std::function<ToJson> function_to_json;
};
} // struct_mapping