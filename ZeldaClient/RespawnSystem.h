#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class RespawnTrigger;
	class PzObject;

	class RespawnSystem final : public PurahEngine::Component
	{
	public:
		enum class Mode
		{
			Nearest,	// 가장 가까운 Respawn Point에서 리스폰
			MaxLevel,	// 도달한 level중 가장 높은 리스폰 포인트에서 리스폰
			LastLevel,	// 가장 최근 도달했던 level의 리스폰 포인트에서 리스폰
		};

	public:
		void Awake() override;

		void OnDeath();
		void OnLevel(int level);

	private:
		PzObject* FindPZObject(PurahEngine::GameObject* obj);

	private:
		Mode mode;
		PurahEngine::GameObject* targetObject;
		bool keepRotation;
		std::vector<PurahEngine::Transform*> respawnPoints;
		std::vector<RespawnTrigger*> levelTriggers;
		std::vector<RespawnTrigger*> deathTriggers;

	private:
		int lastLevel;
		int maxLevel;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	};
}