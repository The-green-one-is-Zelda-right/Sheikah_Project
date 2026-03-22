#pragma once

#include "ElectricalDevice.h"

#include <vector>

namespace Phyzzle
{
	class ObjectController final : public ElectricalDevice
	{
	public:
		void Awake() override;

		void PowerOn() override;
		void PowerOff() override;

	private:
		void TargetOn();
		void TargetOff();

	private:
		bool inverse;
		std::vector<PurahEngine::GameObject*> targetObjectList;

	private:
		int powerCounter;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}