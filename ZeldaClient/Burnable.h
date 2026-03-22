#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class Burnable final : public PurahEngine::Component
	{
	public:
		void OnEnable() override;
		void Update() override;

		// 트리거가 충돌했을 때 호출
		void OnTriggerEnter(const PurahEngine::Collider*) override;
		// 트리거가 충돌을 벗어났을 때 호출
		void OnTriggerExit(const PurahEngine::Collider*) override;

	private:
		float burnTime; // 불타는데 걸리는 시간
		PurahEngine::GameObject* burnableObject; // 타는 오브젝트
		PurahEngine::ParticleSystem* effect; // 이펙트를 끄고 켜는데 사용할 오브젝트
		float destoryDelay; // 오브젝트 제거 지연시간

	private:
		std::vector<const PurahEngine::Collider*> fireList;
		float burnTimeElapsed;
		bool waitDestroy;
		float destroyElapsed;

	public:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}