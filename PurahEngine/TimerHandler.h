#pragma once
#include "ITimer.h"

namespace PurahEngine
{
	class TimerHandler : public ITimer
	{
	public:
		TimerHandler(float _delayTime, bool _loop, const std::function<void()>& _callback)
			: ITimer(_delayTime, _loop, _callback)
		{}
		virtual ~TimerHandler() = default;

	private:
		void Update() override;
	};
}