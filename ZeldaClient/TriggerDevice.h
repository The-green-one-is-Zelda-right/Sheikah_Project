#pragma once

#include "ElectricalDevice.h"

#include <vector>
#include <set>

namespace Phyzzle
{
	class TriggerDevice : public PurahEngine::Component
	{
	public:
		void Awake() override;

		// Ʈ���Ű� �浹���� �� ȣ��
		void OnTriggerEnter(const PurahEngine::Collider*) override;
		// Ʈ���Ű� �浹�� ����� �� ȣ��
		void OnTriggerExit(const PurahEngine::Collider*) override;

		void TargetPowerOn();
		void TargetPowerOff();

	private:
		bool onlyPowerOn;
		std::vector<std::wstring> containTags;
		std::vector<ElectricalDevice*> targetDeviceList;

	private:
		bool isPowerOn;
		std::set<const PurahEngine::Collider*> triggerColliders;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}