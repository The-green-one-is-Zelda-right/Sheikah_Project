#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "ConvexCollider.h"

namespace PurahEngine
{
	ConvexCollider::~ConvexCollider()
	{
		auto& physicsSystem = PhysicsSystem::GetInstance();

		if (znCollider)
		{
			physicsSystem.FreeObject(znCollider, this->GetGameObject());
			znCollider = nullptr;
		}
	}

	void ConvexCollider::PostInitialize()
	{
		Collider::PostInitialize();

		const auto scale = transform->GetWorldScale();
		const auto rot = transform->GetWorldRotation();

		auto& physics = PhysicsSystem::GetInstance();
		this->znCollider = physics.CreateConvexCollider(
			this->GetGameObject(),
			modelName,
			{ scale.x() * scaleOffset.x(), scale.y() * scaleOffset.y(), scale.z() * scaleOffset.z() },
			physicsMaterial
		);

		if (!znCollider)
		{
			GetGameObject()->DeleteComponent(this);
			return;
		}

		Collider::SetUserData();

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetRotationOffset(rotationOffset);
		SetPositionOffset(positionOffset);

		znCollider->SetPosition(transform->GetWorldPosition());
		znCollider->SetQuaternion(transform->GetWorldRotation());
	}

	void ConvexCollider::PreDeserialize(const json& jsonData)
	{
		CustomCollider::PreDeserialize(jsonData);
	}
}