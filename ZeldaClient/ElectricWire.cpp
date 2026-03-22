#include "ElectricWire.h"

namespace Phyzzle
{
	void ElectricWire::Awake()
	{
		powerCounter = 0;
	}

	void ElectricWire::PowerOn()
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

	void ElectricWire::PowerOff()
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

	void ElectricWire::PreSerialize(json& jsonData) const
	{

	}

	void ElectricWire::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}

	void ElectricWire::PostSerialize(json& jsonData) const
	{
	}

	void ElectricWire::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}