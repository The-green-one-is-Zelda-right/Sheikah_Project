#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "MeshCollider.h"

namespace PurahEngine
{
	MeshCollider::~MeshCollider()
	{
		auto& physicsSystem = PhysicsSystem::GetInstance();

		if (znCollider)
		{
			physicsSystem.FreeObject(znCollider, this->GetGameObject());
			znCollider = nullptr;
		}
	}

	void MeshCollider::PostInitialize()
	{
		Collider::PostInitialize();

		const auto rot = transform->GetWorldRotation();
		const auto scale = transform->GetWorldScale();

		auto& physics = PhysicsSystem::GetInstance();
		this->znCollider = physics.CreateMeshCollider(
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

		znCollider->SetQuaternion(transform->GetWorldRotation());
		znCollider->SetPosition(transform->GetWorldPosition());
	}

	void MeshCollider::PreDeserialize(const json& jsonData)
	{
		CustomCollider::PreDeserialize(jsonData);
	}
}