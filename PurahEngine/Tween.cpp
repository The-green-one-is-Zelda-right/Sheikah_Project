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

	//ITween* Tween::Create(float _start, float _end, eEasing _easing, float _durationSec, std::function<void()> _callback)
	//{
	//	ITween* result = new TweenHandler(
	//		_start, _end,
	//		easingFunc[_easing],
	//		_durationSec,
	//		_callback
	//	);

	//	return result;
	//}

	//void Tween::Release(ITween* _tween)
	//{
	//	assert(_tween != nullptr);

	//	delete _tween;
	//}
}