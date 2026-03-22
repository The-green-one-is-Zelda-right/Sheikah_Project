#pragma once

struct Color
{
	union
	{
		struct
		{
			float r, g, b, a;
		};

		struct
		{
			float x, y, z, w;
		};
	};

	bool operator==(const Color& right) const
	{
		return r == right.r && g == right.g && b == right.b && a == right.a;
	}
};

namespace std
{
	template <>
	struct hash<Color>
	{
		size_t operator()(const Color& obj) const
		{
			return hash<float>{}(obj.r) ^ hash<float>{}(obj.g) ^ hash<float>{}(obj.b) ^ hash<float>{}(obj.a);
		};
	};
}