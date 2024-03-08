#include "Timer.h"

namespace PurahEngine
{
	std::queue<ITimer*> Timer::addTimer;
	std::queue<ITimer*> Timer::removeTimer;
	std::vector<ITimer*> Timer::handleTimer;

	void Timer::Update()
	{
		// �� Ÿ�̸� �߰�
		while (!addTimer.empty())
		{
			handleTimer.push_back(addTimer.front());
			addTimer.pop();
		}

		// Ÿ�̸� ����
		while (!removeTimer.empty())
		{
			auto timer = removeTimer.front();
			auto iter = std::ranges::find(handleTimer, timer);
			if (iter != handleTimer.end())
			{
				handleTimer.erase(iter);
				delete timer; // �޸� ���� ����
			}
			removeTimer.pop();
		}

		// Ÿ�̸� ������Ʈ
		for (const auto timer : handleTimer)
			timer->Update();
	}
}
