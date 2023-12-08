#pragma once
#include "Transform.h"
#include <string>
#include <vector>

namespace flt
{
	struct RawAnimation
	{
		struct KeyData
		{
			KeyData() : time(0.0f), transform() {}
			KeyData(float time, const Transform& transform) : time(time), transform(transform) {}

			float time;
			Transform transform;
		};

		RawAnimation() : RawAnimation(L"", 0.0f, 0.0f) {}
		RawAnimation(const std::wstring& name, float startTime, float endTime) : name(name), startTime(startTime), endTime(endTime), keyDatas() {}

		std::wstring name;
		float startTime;
		float endTime;

		std::vector<KeyData> keyDatas;
	};
}
