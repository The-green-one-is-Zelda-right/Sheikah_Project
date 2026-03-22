#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class RespawnSystem;

	class RespawnTrigger final : public PurahEngine::Component
	{
	public:
		enum class Mode
		{
			levelTrigger,
			deathTrigger
		};

		void RegisterTargetSystem(RespawnSystem* targetSystem, Mode mode, int level, PurahEngine::GameObject* targetObejct);

		void OnTriggerEnter(const PurahEngine::Collider* other) override;

	private:
		std::vector<std::tuple<RespawnSystem*, Mode, int, PurahEngine::GameObject*>> targetSystems;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}