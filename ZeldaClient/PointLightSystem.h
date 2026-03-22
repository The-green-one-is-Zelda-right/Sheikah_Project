#pragma once

#include "LightSystem.h"

namespace Phyzzle
{
	class PointLightSystem final : public LightSystem
	{
	public:
		void Awake() override;

		void LateUpdate() override;

		void UpdateLight(Eigen::Vector3f lightColor, float distance) override;

	private:
		PurahEngine::PointLight* pointLight = nullptr;
		Eigen::Vector3f originColor;
		Eigen::Vector3f targetColor;
		int countUpdateLight;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}