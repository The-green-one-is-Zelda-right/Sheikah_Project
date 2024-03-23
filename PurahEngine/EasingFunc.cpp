#include "EasingFunc.h"

namespace PurahEngine
{
	float InSine(float x)
	{
		return 1 - std::cos((x * TWEEN_PI<float>) / 2);
	}

	float OutSine(float x)
	{
		return std::sin((x * TWEEN_PI<float>) / 2);
	}

	float InOutSine(float x)
	{
		return -0.5f * (std::cos(TWEEN_PI<float> *x) - 1);
	}

	float InQuad(float x)
	{
		return x * x;
	}

	float OutQuad(float x)
	{
		return 1.0f - (1.0f - x) * (1.0f - x);
	}

	float InOutQuad(float x)
	{
		return (x < 0.5f) ? (2.0f * x * x) : (1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f);
	}

	float InCubic(float x)
	{
		return x * x * x;
	}

	float OutCubic(float x)
	{
		return 1.0f - std::pow(1.0f - x, 3.0f);
	}

	float InOutCubic(float x)
	{
		return (x < 0.5f) ? (4.0f * x * x * x) : (1.0f - std::pow(-2.0f * x + 2.0f, 3.0f) / 2.0f);
	}

	float InQuart(float x)
	{
		return x * x * x * x;
	}

	float OutQuart(float x)
	{
		return 1.0f - std::pow(1.0f - x, 4.0f);
	}

	float InOutQuart(float x)
	{
		return (x < 0.5f) ? (8.0f * x * x * x * x) : (1.0f - std::pow(-2.0f * x + 2.0f, 4.0f) / 2.0f);
	}

	float InQuint(float x)
	{
		return x * x * x * x * x;
	}

	float OutQuint(float x)
	{
		return 1.0f - std::pow(1.0f - x, 5.0f);
	}

	float InOutQuint(float x)
	{
		return (x < 0.5f) ? (16.0f * x * x * x * x * x) : (1.0f - std::pow(-2.0f * x + 2.0f, 5.0f) / 2.0f);
	}

	float InExpo(float x)
	{
		return (x == 0.0f) ? 0.0f : std::pow(2.0f, 10.0f * x - 10.0f);
	}

	float OutExpo(float x)
	{
		return (x == 1.0f) ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * x);
	}

	float InOutExpo(float x)
	{
		return (x == 0.0f) ? 0.0f : (x == 1.0f) ? 1.0f : (x < 0.5f) ? (0.5f * std::pow(2.0f, 20.0f * x - 10.0f)) : (1.0f - 0.5f * std::pow(2.0f, -20.0f * x + 10.0f));
	}

	float InCirc(float x)
	{
		return 1.0f - std::sqrt(1.0f - x * x);
	}

	float OutCirc(float x)
	{
		return std::sqrt(1.0f - (x - 1.0f) * (x - 1.0f));
	}

	float InOutCirc(float x)
	{
		return (x < 0.5f) ? (0.5f * (1.0f - std::sqrt(1.0f - 4.0f * x * x))) : (0.5f * (std::sqrt(-((2.0f * x) - 3.0f) * ((2.0f * x) - 1.0f)) + 1.0f));
	}

	float InBack(float x)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1.0f;

		return c3 * x * x * x - c1 * x * x;
	}

	float OutBack(float x)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1.0f;

		return 1.0f + c3 * std::pow(x - 1.0f, 3.0f) + c1 * std::pow(x - 1.0f, 2.0f);
	}

	float InOutBack(float x)
	{
		constexpr float c1 = 1.70158f * 1.525f;
		constexpr float c2 = (c1 + 1.0f) * 1.5f;

		return (x < 0.5f) ? (0.5f * (x * x * (((c2)+1.0f) * x - c2))) : (0.5f * ((2.0f * x - 2.0f) * (2.0f * x - 2.0f) * (((c2)+1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f));
	}

	float InElastic(float x)
	{
		constexpr float c4 = (2.0f * TWEEN_PI<float>) / 3.0f;

		return (x == 0.0f) ? 0.0f : (x == 1.0f) ? 1.0f : -std::pow(2.0f, 10.0f * x - 10.0f) * std::sin((x * 10.0f - 10.75f) * c4);
	}

	float OutElastic(float x)
	{
		constexpr float c4 = (2.0f * TWEEN_PI<float>) / 3.0f;

		return (x == 0.0f) ? 0.0f : (x == 1.0f) ? 1.0f : std::pow(2.0f, -10.0f * x) * std::sin((x * 10.0f - 0.75f) * c4) + 1.0f;
	}

	float InOutElastic(float x)
	{
		constexpr float c5 = (2.0f * TWEEN_PI<float>) / 4.5f;

		return (x == 0.0f) ? 0.0f : (x == 1.0f) ? 1.0f : (x < 0.5f) ? -(std::pow(2.0f, 20.0f * x - 10.0f) * std::sin((20.0f * x - 11.125f) * c5)) / 2.0f : (std::pow(2.0f, -20.0f * x + 10.0f) * std::sin((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
	}

	float InBounce(float x)
	{
		return 1.0f - OutBounce(1.0f - x);
	}

	float OutBounce(float x)
	{
		constexpr float n1 = 7.5625f;
		constexpr float d1 = 2.75f;

		if (x < 1.0f / d1)
		{
			return n1 * x * x;
		}
		else if (x < 2.0f / d1)
		{
			const float x1 = x - (1.5f / d1);
			return n1 * x1 * x1 + 0.75f;
		}
		else if (x < 2.5f / d1)
		{
			const float x2 = x - (2.25f / d1);
			return n1 * x2 * x2 + 0.9375f;
		}
		else
		{
			const float x3 = x - (2.625f / d1);
			return n1 * x3 * x3 + 0.984375f;
		}
	}

	float InOutBounce(float x)
	{
		return (x < 0.5f) ? (0.5f * InBounce(x * 2.0f)) : (0.5f * OutBounce(x * 2.0f - 1.0f) + 0.5f);
	}
}