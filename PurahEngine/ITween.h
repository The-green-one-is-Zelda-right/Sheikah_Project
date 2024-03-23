#pragma once
#include "EasingFunc.h"

namespace PurahEngine
{
	class ITween
	{
	public:
		virtual ~ITween() = default;

	public:
		// 		virtual ITween* From(float value) = 0;				// Set the starting value
		// 		virtual ITween* To(float value) = 0;				// Set the target value
		// 		virtual ITween* Easing(eEasing easingFunc) = 0;		// Set the easing function
		// 		virtual ITween* Duration(float durationSec) = 0;	// Set the duration
		// 		virtual ITween* Play() = 0;	// Set the duration
		// 		virtual ITween* Pause() = 0;	// Set the duration
		// 
		// 		// Callbacks
		// 		virtual ITween* OnPlay(std::function<void()> callback) = 0;		// Callback when the tween starts playing
		// 		virtual ITween* OnPause(std::function<void()> callback) = 0;	// Callback when the tween is paused
		// 		virtual ITween* OnComplete(std::function<void()> callback) = 0;	// Callback when the tween completes
	};
}