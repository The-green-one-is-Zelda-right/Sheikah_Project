#include "TimeController.h"

#include "Timer.h"
#include "PreTimerHandler.h"

void PurahEngine::PreTimerHandler::Update()
{
	accumulation += TimeController::GetInstance().GetDeltaTime("Simulate");

	if (delay <= accumulation)
	{
		callback();

		if (loop)
		{
			accumulation = 0.f;
		}
		else
		{
			Timer::Cancel(this);
		}
	}
}
