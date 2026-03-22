#include <cassert>

#include "TweenHandler.h"
#include "Tween.h"

namespace PurahEngine
{
	std::function<float(float)> Tween::easingFunc[31] = 
	{
			InSine,
			OutSine,
			InOutSine,

			InQuad,
			OutQuad,
			InOutQuad,

			InCubic,
			OutCubic,
			InOutCubic,

			InQuart,
			OutQuart,
			InOutQuart,

			InQuint,
			OutQuint,
			InOutQuint,

			InExpo,
			OutExpo,
			InOutExpo,

			InCirc,
			OutCirc,
			InOutCirc,

			InBack,
			OutBack,
			InOutBack,

			InElastic,
			OutElastic,
			InOutElastic,

			InBounce,
			OutBounce,
			InOutBounce
	};
}