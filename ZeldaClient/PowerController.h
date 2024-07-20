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
			PowerOnBuffer = 4,	// PowerOn�� �� ��, PowerOff�� �Ǵ� �ð� ����
			PowerOffBuffer = 5,	// PowerOff�� �� ��, PowerOn�� �Ǵ� �ð� ����
			PowerOnFilter = 6	// PowerOn ���°� �����Ǿ �����ð��� PowerOn ���¸� �����Ѵ�.
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
		int numOfToggles; // ���(On/Off ����) ������ Ƚ��
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