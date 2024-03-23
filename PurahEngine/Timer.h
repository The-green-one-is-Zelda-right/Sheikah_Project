#pragma once
#include <functional>
#include <queue>

namespace PurahEngine
{
	class ITimer;

	class Timer
	{
	public:
		static void		PreUpdate();
		static void		Update();

	public:
		static ITimer*	Delay(float _delayTime, bool _loop, const std::function<void()>& _callback);
		static ITimer*	PreDelay(float _delayTime, bool _loop, const std::function<void()>& _callback);

		static void		Cancel(ITimer* _timer);
		static void		Release(ITimer* _timer);
		static void		BufferClear();

	private:
		static std::queue<ITimer*> addTimerBuffer;
		static std::queue<ITimer*> removeTimerBuffer;
		static std::vector<ITimer*> timerList;

	private:
		static std::queue<ITimer*> addPreTimerBuffer;
		static std::vector<ITimer*> preTimerList;
	};
}

