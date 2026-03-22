#pragma once

#include "ElectricalDevice.h"

#include <vector>

namespace Phyzzle
{
	class ElectricWire : public ElectricalDevice
	{
	public:
		void Awake() override;

		void PowerOn() override;
		void PowerOff() override;


		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		std::vector<ElectricalDevice*> targetDeviceList;

		int powerCounter;

	};
}