#include "ANDGate.h"

namespace Phyzzle
{
	void ANDGate::Awake()
	{
		powerCounter = 0;
		isPowerOn = false;
	}

	void ANDGate::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= powerRequired)
		{
			TargetPowerOn();
		}
	}

	void ANDGate::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < powerRequired)
		{
			TargetPowerOff();
		}
	}

	void ANDGate::TargetPowerOn()
	{
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

	void ANDGate::TargetPowerOff()
	{
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

	void ANDGate::PreSerialize(json& jsonData) const
	{
	}
	void ANDGate::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(powerRequired);
	}
	void ANDGate::PostSerialize(json& jsonData) const
	{
	}
	void ANDGate::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}