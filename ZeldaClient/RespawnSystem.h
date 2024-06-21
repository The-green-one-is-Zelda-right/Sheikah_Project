#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class RespawnTrigger;

	class RespawnSystem final : public PurahEngine::Component
	{
	public:
		enum class Mode
		{
			Nearest,	// ���� ����� Respawn Point���� ������
			MaxLevel,	// ������ level�� ���� ���� ������ ����Ʈ���� ������
			LastLevel,	// ���� �ֱ� �����ߴ� level�� ������ ����Ʈ���� ������
		};

	public:
		void Awake() override;

		void OnDeath();
		void OnLevel(int level);

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