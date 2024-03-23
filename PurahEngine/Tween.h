#pragma once
#include <functional>
#include "EasingFunc.h"
#include "TweenHandler.h"

namespace PurahEngine
{
	class ITween;

	template<typename T>
	concept Addable = requires(T a, T b) {
		{ a + b } -> std::same_as<T>;
	};

	template<typename T>
	concept Subtractable = requires(T a, T b) {
		{ a - b } -> std::same_as<T>;
	};

	template<typename T>
	concept Multiplicative = requires(T a, float b) {
		{ a * b } -> std::same_as<T>;
		{ a / b } -> std::same_as<T>;
	};

	template<typename T>
	concept Arithmetic = Addable<T> && Subtractable<T> && Multiplicative<T>;

	class Tween
	{
	public:
		template <Arithmetic Type>
		static ITween* DoTween(
			Type&					_start,
			eCycleMode				_cycleMode,
			const Type&				_end,
			eEasing					_easing,
			float					_durationSec,
			float					_delay = 0.f,
			std::function<void()>	_callback = nullptr
		)
		{
			std::function<void(TweenHandler<Type>*)> stepFunc = TweenHandler<Type>::cycleFunc[_cycleMode];
			ITween* handler = Create<Type>(_start, _end, _easing, _durationSec, _callback);

			if (_delay > 0)
			{
				Timer::Delay(
					_delay,
					false,
					[stepFunc, handler]()
					{
						stepFunc(dynamic_cast<TweenHandler<Type>*>(handler));
					}
				);
			}
			else
			{
				stepFunc(dynamic_cast<TweenHandler<Type>*>(handler));
			}
			return handler;
		}

	public:
		template <Arithmetic Type>
		static ITween* Create(
			Type& _start, const Type& _end,					// 시작점, 끝점
			eEasing _easing,								// 그래프
			float _durationSec,								// 지속시간
			std::function<void()> _callback = nullptr		// 콜백
		)
		{
			ITween* result = new TweenHandler<Type>(
				_start, _end,
				easingFunc[_easing],
				_durationSec,
				_callback
			);

			return result;
		}
		
	public:
		static void		Release(ITween* _tween)
		{
			assert(_tween != nullptr);

			delete _tween;
		}

	private:
		static std::function<float(float)> easingFunc[31];
	};
}

