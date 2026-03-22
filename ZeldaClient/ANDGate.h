#pragma once

#include "ElectricalDevice.h"

#include <vector>

namespace Phyzzle
{
	class ANDGate : public ElectricalDevice
	{
	public:
		void Awake() override;

		void PowerOn() override;
		void PowerOff() override;

	private:
		void TargetPowerOn();
		void TargetPowerOff();

	private:
		int powerRequired;
		std::vector<ElectricalDevice*> targetDeviceList;

	private:
		int powerCounter;
		bool isPowerOn;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}