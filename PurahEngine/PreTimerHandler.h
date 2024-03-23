#pragma once
#include "ITimer.h"

namespace PurahEngine
{
	class PreTimerHandler : public ITimer
	{
	public:
		PreTimerHandler(float _delayTime, bool _loop, const std::function<void()>& _callback)
			: ITimer(_delayTime, _loop, _callback)
		{}
		virtual ~PreTimerHandler() = default;

	private:
		void Update() override;
	};
}