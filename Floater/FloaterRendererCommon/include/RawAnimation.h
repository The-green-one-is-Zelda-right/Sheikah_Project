#pragma once
#include "Transform.h"
#include <string>
#include <vector>

namespace flt
{
	struct RawAnimation
	{
#pragma region Nasted
		struct KeyPosition
		{
			KeyPosition() : KeyPosition(0.0f, { 0.0f, 0.0f, 0.0f, 0.0f }) {}
			KeyPosition(float time, const Vector4f& pos) : time(time), position(pos) {}

			float time;
			Vector4f position;
		};

		struct KeyRotation
		{
			KeyRotation() : KeyRotation(0.0f, { 0.0f, 0.0f, 0.0f, 1.0f }) {}
			KeyRotation(float time, const Quaternion& rot) : time(time), rotation(rot) {}

			float time;
			Quaternion rotation;
		};

		struct KeyScale
		{
			KeyScale() : KeyScale(0.0f, { 1.0f, 1.0f, 1.0f, 0.0f }) {}
			KeyScale(float time, const Vector4f& scl) : time(time), scale(scl) {}

			float time;
			Vector4f scale;
		};
#pragma endregion

		RawAnimation() : RawAnimation(L"", 0.0f, 0.0f) {}
		RawAnimation(const std::wstring& name, float startTime, float endTime) : name(name), startTime(startTime), endTime(endTime), keyPosition(), keyRotation(), keyScale() {}

		std::wstring name;
		float startTime;
		float endTime;

		std::vector<KeyPosition> keyPosition;
		std::vector<KeyRotation> keyRotation;
		std::vector<KeyScale> keyScale;
	};
}
