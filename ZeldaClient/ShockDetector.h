#pragma once

#include "PurahEngine.h"

#include <vector>
#include <list>

namespace Phyzzle
{
	class ElectricalDevice;

	class ShockDetector final : public PurahEngine::Component
	{
	private:
		static std::list<ShockDetector*> objectList;

	public:
		ShockDetector();
		virtual ~ShockDetector();

	public:
		void Awake() override;
		void Update() override;
		void LateUpdate() override;

		// 충돌체가 충돌했을 때 호출
		virtual void OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;

	private:
		void TargetPowerOn();
		void TargetPowerOff();

	private:
		bool onlyPowerOn;
		float activationShock;
		std::vector<ElectricalDevice*> targetDeviceList;

	private:
		bool isPowerOn;
		float shock;

		bool isMainObject = false;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}