#include "Door.h"

#include "DoorParts.h"

namespace Phyzzle
{
	void Door::Awake()
	{
		powerCounter = 0;
	}

	void Door::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= 1)
		{
			for (int i = 0; i < targetDeviceList.size(); i++)
			{
				targetDeviceList[i]->PowerOn();
			}
		}
	}

	void Door::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < 1)
		{
			for (int i = 0; i < targetDeviceList.size(); i++)
			{
				targetDeviceList[i]->PowerOff();
			}
		}
	}

	void Door::PreSerialize(json& jsonData) const
	{
	}

	void Door::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}

	void Door::PostSerialize(json& jsonData) const
	{
	}

	void Door::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}