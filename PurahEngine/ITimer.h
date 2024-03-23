#pragma once
#include <functional>

namespace PurahEngine
{
	constexpr float timerEpsilon = 0.001f;

	class ITimer
	{
	public:
		friend class Timer;
	
	public:
		ITimer() = delete;
		ITimer(float _delayTime, bool _loop, const std::function<void()>& _callback)
			: accumulation(), delay(_delayTime), loop(_loop), callback(_callback)
		{}
		virtual ~ITimer() = default;

	protected:
		virtual void Update() = 0;

	protected:
		float accumulation;
		float delay;
		bool loop;
		std::function<void()> callback;
	};

}