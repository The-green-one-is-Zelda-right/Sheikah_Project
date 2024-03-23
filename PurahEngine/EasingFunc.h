#pragma once
#include <cmath>
#include <numbers>

namespace PurahEngine
{
	template <typename T>
	constexpr T TWEEN_PI = std::numbers::pi_v<T>;

	enum eCycleMode
	{
		ONCE,
		LOOP,
	};

	enum eEasing : unsigned int
	{
		eInSine,
		eOutSine,
		eInOutSine,

		eInQuad,
		eOutQuad,
		eInOutQuad,

		eInCubic,
		eOutCubic,
		eInOutCubic,

		eInQuart,
		eOutQuart,
		eInOutQuart,

		eInQuint,
		eOutQuint,
		eInOutQuint,

		eInExpo,
		eOutExpo,
		eInOutExpo,

		eInCirc,
		eOutCirc,
		eInOutCirc,

		eInBack,
		eOutBack,
		eInOutBack,

		eInElastic,
		eOutElastic,
		eInOutElastic,

		eInBounce,
		eOutBounce,
		eInOutBounce,
	};

	float InSine(float x);
	float OutSine(float x);
	float InOutSine(float x);

	float InQuad(float x);
	float OutQuad(float x);
	float InOutQuad(float x);

	float InCubic(float x);
	float OutCubic(float x);
	float InOutCubic(float x);

	float InQuart(float x);
	float OutQuart(float x);
	float InOutQuart(float x);

	float InQuint(float x);
	float OutQuint(float x);
	float InOutQuint(float x);

	float InExpo(float x);
	float OutExpo(float x);
	float InOutExpo(float x);

	float InCirc(float x);
	float OutCirc(float x);
	float InOutCirc(float x);

	float InBack(float x);
	float OutBack(float x);
	float InOutBack(float x);

	float InElastic(float x);
	float OutElastic(float x);
	float InOutElastic(float x);

	float InBounce(float x);
	float OutBounce(float x);
	float InOutBounce(float x);
}
