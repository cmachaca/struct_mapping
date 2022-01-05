#pragma once

#include "utility.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <functional>
#include <string>
#include <vector>

namespace struct_mapping::detail
{

template<
	typename T,
	template<typename, bool, bool> typename ObjectType>
class Functions
{
public:
	using CheckNotEmpty = void (T&, const std::string&);
	using Init = void (T&);
	using IterateOver = void (T&, const std::string&);
	using Release = bool (T&);
	using SetBool = void (T&, const std::string&, const json&);
	using SetDefault = void (T&, Index);
	using SetFloatingPoint = void (T&, const std::string&, const json&);
	using SetIntegral = void (T&, const std::string&, const json&);
	using SetString = void (T&, const std::string&, const json&);
	using SetStruct = bool (T&, const std::string&, const json&);
	using Use = void (T&, const std::string&);

public:
	template<typename V>
	Index add(MemberPtr<T, V> ptr)
	{
		check_not_empty.emplace_back(
			[ptr] (T& o, const std::string& name_)
			{
				ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::check_not_empty(o.*ptr, name_);
			});

		init.emplace_back(
			[ptr] (T& o)
			{
				ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::init(o.*ptr);
			});

		iterate_over.emplace_back(
			[ptr] (T& o, const std::string& name_)
			{
				ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::iterate_over(o.*ptr, name_);
			});

		release.emplace_back(
			[ptr] (T& o)
			{
				return ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::release(o.*ptr);
			});

		set_bool.emplace_back(
			[ptr] (T& o, const std::string& name_, const json& value_)
			{
				ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::set_bool(o.*ptr, name_, value_);
			});

		set_default.emplace_back(
			[ptr] (T& o, Index index_)
			{
				o.*ptr = ObjectType<T, is_array_like_v<T>, is_map_like_v<T>>::template members_default<V>[index_];
			});

		set_floating_point.emplace_back(
			[ptr] (T& o, const std::string& name_, const json& value_)
			{
				ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::set_floating_point(o.*ptr, name_, value_);
			});

		set_integral.emplace_back(
			[ptr] (T& o, const std::string& name_, const json& value_)
			{
				ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::set_integral(o.*ptr, name_, value_);
			});

		set_string.emplace_back(
			[ptr] (T& o, const std::string& name_, const json& value_)
			{
				ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::set_string(o.*ptr, name_, value_);
			});

		use.emplace_back(
			[ptr] (T& o, const std::string& name_)
			{
				ObjectType<V, is_array_like_v<V>, is_map_like_v<V>>::use(o.*ptr, name_);
			});

		return static_cast<Index>(use.size()) - 1;
	}

public:
	std::vector<std::function<CheckNotEmpty>> check_not_empty;
	std::vector<std::function<Init>> init;
	std::vector<std::function<IterateOver>> iterate_over;
	std::vector<std::function<Release>> release;
	std::vector<std::function<SetBool>> set_bool;
	std::vector<std::function<SetDefault>> set_default;
	std::vector<std::function<SetFloatingPoint>> set_floating_point;
	std::vector<std::function<SetIntegral>> set_integral;
	std::vector<std::function<SetString>> set_string;
	std::vector<std::function<SetStruct>> set_struct;
	std::vector<std::function<Use>> use;
};

} // struct_mapping::detail