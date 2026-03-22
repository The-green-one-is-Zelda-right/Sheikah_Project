#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class SceneLoadSupport final : public PurahEngine::Component
	{
	public:
		void Awake() override;
		void Start() override;
		void Update() override;

		virtual void OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;
		virtual void OnTriggerEnter(const PurahEngine::Collider* other) override;

	private:

	private:
		bool loadWithStart;
		bool loadWithTrigger;
		bool loadWithCollision;
		std::vector<std::wstring> targetTags;
		std::wstring targetScene;
		float delay;

	private:
		bool waitLoad;
		float elapsedTime;

	public:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}