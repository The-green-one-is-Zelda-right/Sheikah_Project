#pragma once
#include "Timer.h"
#include "ITween.h"

namespace PurahEngine
{
	template <typename Type>
	class TweenHandler : public ITween
	{
	public:
		friend class Tween;

	public:
		TweenHandler(
			Type& _start, const Type& _end,
			std::function<float(float)> _easing,
			float _duration,
			std::function<void(void)> _callback = nullptr
		) :
			origin(_start),
			start(_start), end(_end),
			difference(_end - start),
			easing(_easing),
			duration(_duration), accumulation(),
			callback(_callback),
			timer()
		{}
		~TweenHandler() override = default;

	private:
		Type origin;
		Type& start;
		const Type difference;
		const Type end;
		std::function<float(float)> easing;
		float duration;
		float accumulation;
		std::function<void()> callback;
		ITimer* timer;

	public:
		static void Once(TweenHandler<Type>* _tween)
		{
			_tween->timer =
				Timer::Delay(
					0.f,		// delay 시간
					false,		// 반복
					[_tween]()	// 콜백
					{
						_tween->start =
							_tween->end -
							_tween->difference *
							_tween->easing(
								_tween->accumulation / _tween->duration
							);

						// _tween->accumulation += Time::deltaTime;

						if (_tween->accumulation >= _tween->duration)
						{
							_tween->start = _tween->origin;

							if (_tween->callback)
								_tween->callback();

							Timer::Cancel(_tween->timer);
							Tween::Release(_tween);
						}
					}
			);
		}
		
		static void Loop(TweenHandler<Type>* _tween)
		{
			_tween->timer =
				Timer::Delay(
					0.f,
					true,
					[_tween]()
					{
						_tween->start =
							_tween->end -
							_tween->difference *
							_tween->easing(
								_tween->accumulation / _tween->duration
							);

						// _tween->accumulation += (float)Time::deltaTime;

						if (_tween->accumulation >= _tween->duration)
						{
							_tween->start = _tween->origin;

							if (_tween->callback)
								_tween->callback();

							_tween->accumulation = 0;
						}
					}
			);
		}

		static void Cancel(TweenHandler<Type>* _tween)
		{
			Timer::Cancel(_tween->timer);
			Tween::Release(_tween);
		}

	private:
		static std::function<void(TweenHandler<Type>*)> cycleFunc[6];
	};

	template <typename Type>
	std::function<void(TweenHandler<Type>*)> TweenHandler<Type>::cycleFunc[6] =
	{
		Once,
		Loop,
	};
}

