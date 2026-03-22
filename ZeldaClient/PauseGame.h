#pragma once

#include "Component.h"

namespace Phyzzle
{
	class Player;

	class PauseGame : public PurahEngine::Component
	{
	public:
		~PauseGame();

	public:
		void OnDestroy() override;
		void Awake() override;
		void Start() override;
		void Update() override;
		void LateUpdate() override;

	private:
		bool isPause;
		bool previousIsPause = false;
		Phyzzle::Player* player = nullptr;

		std::vector<std::pair<PurahEngine::GameObject*, bool>> onOffEventList;

	private:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}

