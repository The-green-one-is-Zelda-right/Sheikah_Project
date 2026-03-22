

#include "Holder.h"

namespace Phyzzle
{
	void Holder::OnTriggerEnter(const PurahEngine::Collider* collider)
	{
		rigidbody = collider->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
	}

	void Holder::OnTriggerStay(const PurahEngine::Collider* collider)
	{

	}

	void Holder::OnTriggerExit(const PurahEngine::Collider* collider)
	{
		rigidbody = nullptr;
	}

	void Holder::OnCollisionEnter(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
	}

	PurahEngine::RigidBody* Holder::GetHolderableBody()
	{
		return rigidbody;
	}

	void Holder::PreSerialize(json& jsonData) const
	{
	}

	void Holder::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}

	void Holder::PostSerialize(json& jsonData) const
	{
	}

	void Holder::PostDeserialize(const json& jsonData)
	{
	}
}
