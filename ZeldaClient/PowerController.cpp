#include "PowerController.h"

namespace Phyzzle
{
	void PowerController::Awake()
	{
		toggleCount = 0;
		powerCounter = 0;
		isPowerOn = false;
	}

	void PowerController::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (type == Type::PowerOnBuffer && isPowerOn && powerCounter == 0)
		{
			elapsedTime += deltaTime;

			if (delay <= elapsedTime)
			{
				TargetPowerOff();
			}
		}

		if (type == Type::PowerOffBuffer && !isPowerOn && powerCounter > 0)
		{
			elapsedTime += deltaTime;

			if (delay <= elapsedTime)
			{
				TargetPowerOn();
			}
		}

		if (type == Type::PowerOnFilter && isPowerOn)
		{
			elapsedTime += deltaTime;

			if (delay <= elapsedTime)
			{
				TargetPowerOff();
			}
		}
	}

	void PowerController::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= 1)
		{
			switch (type)
			{
				case Phyzzle::PowerController::Type::Normal:
				case Phyzzle::PowerController::Type::OnlyPowerOn:
				case Phyzzle::PowerController::Type::ToggleCounter:
				case Phyzzle::PowerController::Type::PowerOnBuffer:
				{
					TargetPowerOn();
					break;
				}
				case Phyzzle::PowerController::Type::PowerOnFilter:
				{
					if (powerCounter == 1)
					{
						TargetPowerOn();
					}
					break;
				}
				case Phyzzle::PowerController::Type::PowerOffBuffer:
				{
					elapsedTime = 0.0f;
					break;
				}
				case Phyzzle::PowerController::Type::OnlyPowerOff:
				{
					// Do Nothing
					break;
				}
				default:
				{
					assert(0);
					break;
				}
			}
		}
	}
	void PowerController::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < 1)
		{
			switch (type)
			{
				case Phyzzle::PowerController::Type::Normal:
				case Phyzzle::PowerController::Type::OnlyPowerOff:
				case Phyzzle::PowerController::Type::ToggleCounter:
				case Phyzzle::PowerController::Type::PowerOffBuffer:
				case Phyzzle::PowerController::Type::PowerOnFilter:
				{
					TargetPowerOff();
					break;
				}
				case Phyzzle::PowerController::Type::PowerOnBuffer:
				{
					elapsedTime = 0.0f;
					break;
				}
				case Phyzzle::PowerController::Type::OnlyPowerOn:
				{
					// Do Nothing
					break;
				}
				default:
				{
					assert(0);
					break;
				}
			}
		}
	}

	void PowerController::TargetPowerOn()
	{
		// 켜져있을 때 눌러도 타이머가 갱신되는 방식
		if (type == Type::PowerOnBuffer)
		{
			elapsedTime = 0.0f;
		}

		if (isPowerOn)
		{
			return;
		}

		isPowerOn = true;

		// 꺼져있을 때 누른것만 타이머가 갱신
		if (type == Type::PowerOnFilter)
		{
			elapsedTime = 0.0f;
		}

		if (type == PowerController::Type::ToggleCounter)
		{
			if (toggleCount < numOfToggles)
			{
				toggleCount += 1;
			}
			else
			{
				return;
			}
		}

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOn();
		}
	}

	void PowerController::TargetPowerOff()
	{
		if (type == Type::PowerOffBuffer)
		{
			elapsedTime = 0.0f;
		}

		if (!isPowerOn)
		{
			return;
		}

		isPowerOn = false;

		if (type == PowerController::Type::ToggleCounter)
		{
			if (toggleCount < numOfToggles)
			{
				toggleCount += 1;
			}
			else
			{
				return;
			}
		}

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOff();
		}
	}

	void PowerController::PreSerialize(json& jsonData) const
	{
	}
	void PowerController::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(type);
		PREDESERIALIZE_VALUE(numOfToggles);
		PREDESERIALIZE_VALUE(delay);
	}
	void PowerController::PostSerialize(json& jsonData) const
	{
	}
	void PowerController::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}