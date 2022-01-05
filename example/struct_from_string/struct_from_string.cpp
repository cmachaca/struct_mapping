#include "struct_mapping/struct_mapping.h"

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>

namespace sm = struct_mapping;

struct Color
{
	int value;

	bool operator<(const Color& o) const
	{
		return value < o.value;
	}
};

static Color color_from_string(const std::string & value)
{
	if (value == "red")
	{
		return Color{1};
	}
	else if (value == "blue")
	{
		return Color{2};
	}
	
	return Color{0};
}

static Color color_from_json(const json & value)
{
	if (value.is_string())
	{
		return color_from_string(value.get<std::string>());
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

static std::string color_to_string(const Color& color)
{
	switch (color.value)
	{
	case 1: return "red";
	case 2: return "blue";
	default: return "green";
	}
}

static json color_to_json(const Color& color)
{
	return color_to_string(color);
}

struct Background
{
	Color color;
};

struct Palette
{
	Color main_color;
	Background background;
	std::list<Color> special_colors;
	std::set<Color> old_colors;
	std::map<std::string, Color> new_colors;
	std::list<Color> other_colors;
};

int main()
{
	sm::MemberNLJson<Color>::set(color_from_json, color_to_json);

	sm::reg(&Palette::main_color, "main_color", sm::Default{Color{45}});
	sm::reg(&Palette::background, "background", sm::Required{});
	sm::reg(&Palette::special_colors, "special_colors");
	sm::reg(&Palette::old_colors, "old_colors");
	sm::reg(&Palette::new_colors, "new_colors");
	sm::reg(&Palette::other_colors, "other_colors", sm::Default{R"json(["red", "blue", "green"])json"_json});

	sm::reg(&Background::color, "color");

	Palette palette;

	std::istringstream json_data(R"json(
	{
		"background": {
			"color": "blue"
		},
		"new_colors": {
			"dark": "green",
			"light": "red",
			"neutral": "blue"
		},
		"old_colors": ["red", "green", "blue"],
		"special_colors": ["red", "green", "red", "blue"]
	}
	)json");

	sm::map_json_to_struct(palette, json_data);

	std::ostringstream out_json_data;
	struct_mapping::map_struct_to_json(palette, out_json_data, "  ");

	std::cout << out_json_data.str() << std::endl;
}
