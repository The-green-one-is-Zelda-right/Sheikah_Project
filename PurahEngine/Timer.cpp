#include <cassert>

#include "PreTimerHandler.h"
#include "TimerHandler.h"

#include "Timer.h"

namespace PurahEngine
{
	std::queue<ITimer*> Timer::addTimerBuffer;
	std::queue<ITimer*> Timer::removeTimerBuffer;
	std::vector<ITimer*> Timer::timerList;

	std::queue<ITimer*> Timer::addPreTimerBuffer;
	std::vector<ITimer*> Timer::preTimerList;

	void Timer::PreUpdate()
	{
		// 새 타이머 추가
		while (!addPreTimerBuffer.empty())
		{
			preTimerList.push_back(addPreTimerBuffer.front());
			addPreTimerBuffer.pop();
		}

		// 타이머 업데이트
		for (const auto timer : preTimerList)
		{
			timer->Update();
		}
	}

	void Timer::Update()
	{
		// 새 타이머 추가
		while (!addTimerBuffer.empty())
		{
			timerList.push_back(addTimerBuffer.front());
			addTimerBuffer.pop();
		}

		// 타이머 제거
		while (!removeTimerBuffer.empty())
		{
			auto timer = removeTimerBuffer.front();

			const auto iter = std::ranges::find(timerList, timer);
			if (iter != timerList.end())
			{
				timerList.erase(iter);
				Release(timer);
			}
			removeTimerBuffer.pop();
		}

		// 타이머 업데이트
		for (const auto timer : timerList)
		{
			timer->Update();
		}
	}

	ITimer* Timer::Delay(float _delayTime, bool _loop, const std::function<void()>& _callback)
	{
		ITimer* timer = new TimerHandler(_delayTime, _loop, _callback);
		addTimerBuffer.push(timer);
		return timer;
	}

	ITimer* Timer::PreDelay(float _delayTime, bool _loop, const std::function<void()>& _callback)
	{
		ITimer* timer = new PreTimerHandler(_delayTime, _loop, _callback);
		addPreTimerBuffer.push(timer);
		return timer;
	}

	void Timer::Cancel(ITimer* _timer)
	{
		assert(_timer != nullptr);

		removeTimerBuffer.push(_timer);
	}

	void Timer::Release(ITimer* _timer)
	{
		assert(_timer != nullptr);

		delete _timer;
	}

	void Timer::BufferClear()
	{
		while (!addPreTimerBuffer.empty())
		{
			auto timer = addPreTimerBuffer.front();
			Release(timer);
			addPreTimerBuffer.pop();
		}

		for (const auto timer : preTimerList)
		{
			Release(timer);
		}

		preTimerList.clear();

		while (!addTimerBuffer.empty())
		{
			auto timer = addTimerBuffer.front();
			Release(timer);
			addTimerBuffer.pop();
		}

		while (!removeTimerBuffer.empty())
		{
			auto timer = removeTimerBuffer.front();
			Release(timer);
			removeTimerBuffer.pop();
		}

		for (const auto timer : timerList)
		{
			Release(timer);
		}

		timerList.clear();
	}
}
