#include "DisconnectedWire.h"

namespace Phyzzle
{
	void DisconnectedWire::Awake()
	{
		connectCounter = 0;
		powerCounter = 0;

		isPowerOn = false;

		frameNumber = 0;
	}

	void DisconnectedWire::Update()
	{
		frameNumber++;
	}

	void DisconnectedWire::OnTriggerEnter(const PurahEngine::Collider* collider)
	{
		PurahEngine::GameObject* otherObject = collider->GetGameObject();

		if (otherObject->tag.IsContain(L"Conductor Object"))
		{
			connectCounter += 1;

			if (connectCounter >= 1)
			{
				TargetPowerOn();
			}
		}
	}

	void DisconnectedWire::OnTriggerExit(const PurahEngine::Collider* collider)
	{
		PurahEngine::GameObject* otherObject = collider->GetGameObject();

		if (otherObject->tag.IsContain(L"Conductor Object"))
		{
			connectCounter -= 1;

			if (connectCounter < 1)
			{
				TargetPowerOff();
			}
		}
	}

	void DisconnectedWire::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= 1)
		{
			TargetPowerOn();
		}
	}

	void DisconnectedWire::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < 1)
		{
			TargetPowerOff();
		}
	}

	void DisconnectedWire::TargetPowerOn()
	{
		if (!(connectCounter >= 1 && powerCounter >= 1))
		{
			return;
		}

		if (isPowerOn == true)
		{
			return;
		}

		isPowerOn = true;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOn();
		}
	}

	void DisconnectedWire::TargetPowerOff()
	{
		if ((connectCounter >= 1 && powerCounter >= 1))
		{
			return;
		}

		if (isPowerOn == false)
		{
			return;
		}

		isPowerOn = false;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOff();
		}
	}

	void DisconnectedWire::PreSerialize(json& jsonData) const
	{
	}

	void DisconnectedWire::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}

	void DisconnectedWire::PostSerialize(json& jsonData) const
	{
	}

	void DisconnectedWire::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}