#include "struct_mapping/struct_mapping.h"

#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

namespace sm = struct_mapping;

enum class Color
{
	red,
	blue,
	green,
};

static Color ColorFromString(const std::string& value)
{
	if (value == "red")
	{
		return Color::red;
	}
	else if (value == "blue")
	{
		return Color::blue;
	}
	
	return Color::green;
}

static Color color_from_json(const json & value)
{
	if (value.is_string())
	{
		return ColorFromString(value.get<std::string>());
	}
	else if (value.is_number())
	{
		return Color{value.get<int>()};
	}
	else if (value.is_object())
	{
		return color_from_json(value["name"]);
	}
	return Color{0};
}

static std::string ColorToString(Color color)
{
	switch (color)
	{
	case Color::red: return "red";
	case Color::green: return "green";
	default: return "blue";
	}
}

static json color_to_json(const Color& color)
{
	return ColorToString(color);
}

struct Palette
{
	Color main_color;
	Color background_color;
	std::list<Color> special_colors;
	std::map<std::string, Color> colors;
};

int main()
{
	sm::MemberNLJson<Color>::set(color_from_json, color_to_json);

	sm::reg(&Palette::main_color, "main_color", sm::Required{});
	sm::reg(&Palette::background_color, "background_color", sm::Default{Color::blue});
	sm::reg(&Palette::special_colors, "special_colors");
	sm::reg(&Palette::colors, "colors");

	Palette palette;

	std::istringstream json_data(R"json(
	{
		"main_color": "green",
		"special_colors": ["green", "green", "red"],
		"colors": {
			"dark": "green",
			"light": "red",
			"neutral": "blue"
		}
	}
	)json");

	sm::map_json_to_struct(palette, json_data);

	std::ostringstream out_json_data;
	struct_mapping::map_struct_to_json(palette, out_json_data, "  ");

	std::cout << out_json_data.str() << std::endl;
}
