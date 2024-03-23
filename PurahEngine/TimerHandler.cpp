#include "TimeController.h"

#include "Timer.h"
#include "TimerHandler.h"

void PurahEngine::TimerHandler::Update()
{
	accumulation += TimeController::GetInstance().GetDeltaTime("Simulate");
	// accumulation += 0.1f;

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