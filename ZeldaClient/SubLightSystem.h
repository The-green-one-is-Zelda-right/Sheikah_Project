#pragma once

#include "PurahEngine.h"

#include <vector>
#include <string>

namespace Phyzzle
{
	class LightSystem;

	class SubLightSystem final : public PurahEngine::Component
	{
	public:
		void Update() override;

		// Ʈ���Ű� �浹���� �� ȣ��
		virtual void OnTriggerEnter(const PurahEngine::Collider* other) override;
		// Ʈ���Ű� �浹�� ����� �� ȣ��
		virtual void OnTriggerExit(const PurahEngine::Collider* other) override;

	private:
		LightSystem* lightSystem = nullptr;
		std::vector<std::wstring> containTags;
		float maxDistance;
		float minDistance;
		Eigen::Vector3f maxColor;
		Eigen::Vector3f minColor;

	private:
		PurahEngine::Transform* targetTransform = nullptr;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}