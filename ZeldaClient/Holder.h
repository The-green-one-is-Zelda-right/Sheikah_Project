#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class Holder : public PurahEngine::Component
	{
	public:
		~Holder() override = default;

	public:
		void OnTriggerEnter(const PurahEngine::Collider*) override;
		void OnTriggerStay(const PurahEngine::Collider*) override;
		void OnTriggerExit(const PurahEngine::Collider*) override;

		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;

	protected:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		PurahEngine::RigidBody* GetHolderableBody();

	private:
		PurahEngine::RigidBody* rigidbody = nullptr;
	};
}
