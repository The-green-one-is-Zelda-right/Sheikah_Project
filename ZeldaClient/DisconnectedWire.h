#pragma once

#include "ElectricalDevice.h"

#include <vector>

namespace Phyzzle
{
	class DisconnectedWire : public ElectricalDevice
	{
	public:
		void Awake() override;

		void Update() override;

		void OnTriggerEnter(const PurahEngine::Collider* collider) override;
		void OnTriggerExit(const PurahEngine::Collider* collider) override;

		void PowerOn() override;
		void PowerOff() override;

	private:
		void TargetPowerOn();
		void TargetPowerOff();

	private:
		std::vector<ElectricalDevice*> targetDeviceList;


		int connectCounter;
		int powerCounter;
		bool isPowerOn;




		int frameNumber;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}