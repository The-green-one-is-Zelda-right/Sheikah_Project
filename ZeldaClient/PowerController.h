#pragma once

#include "ElectricalDevice.h"

#include <vector>

namespace Phyzzle
{
	class PowerController final : public ElectricalDevice
	{
	public:
		enum class Type : int
		{
			Normal = 0,
			OnlyPowerOn = 1,
			OnlyPowerOff = 2,
			ToggleCounter = 3,
			PowerOnBuffer = 4,	// PowerOn이 된 후, PowerOff가 되는 시간 지연
			PowerOffBuffer = 5,	// PowerOff가 된 후, PowerOn이 되는 시간 지연
			PowerOnFilter = 6	// PowerOn 상태가 유지되어도 일정시간만 PowerOn 상태를 유지한다.
		};

	public:
		void Awake() override;
		void Update() override;

		void PowerOn() override;
		void PowerOff() override;

	private:
		void TargetPowerOn();
		void TargetPowerOff();

	private:
		Type type;
		int numOfToggles; // 토글(On/Off 변경) 가능한 횟수
		float delay;
		std::vector<ElectricalDevice*> targetDeviceList;

	private:
		int toggleCount;
		int powerCounter;
		bool isPowerOn;
		float elapsedTime;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}