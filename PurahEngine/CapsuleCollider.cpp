#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "CapsuleCollider.h"

namespace PurahEngine
{
	CapsuleCollider::~CapsuleCollider()
	{
		auto& physicsSystem = PhysicsSystem::GetInstance();

		if (znCollider)
		{
			physicsSystem.FreeObject(znCollider, this->GetGameObject());
			znCollider = nullptr;
		}
	}

	void CapsuleCollider::PostInitialize()
	{
		Collider::PostInitialize();

		const auto scale = transform->GetWorldScale();
		const float maxScale = max(scale.x(), scale.z());

		auto& physics = PhysicsSystem::GetInstance();
		this->znCollider = PhysicsSystem::GetInstance().CreateCapsuleCollider(
			this->GetGameObject(),
			radius * maxScale, height * scale.y(),
			physicsMaterial
		);

		Collider::SetUserData();

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetRotationOffset(rotationOffset);
		SetPositionOffset(positionOffset);

		znCollider->SetQuaternion(transform->GetWorldRotation());
		znCollider->SetPosition(transform->GetWorldPosition());
	}

	void CapsuleCollider::SetSize(float _radius, float _halfHeight)
	{
		if (awake)
		{
			OutputDebugStringW(L"실행 도중에 Collider size 변경은 불가능 합니다.");
			return;
		}

		radius = _radius;
		height = _halfHeight;
	}

	void CapsuleCollider::PreSerialize(json& jsonData) const
	{
	}

	void CapsuleCollider::PreDeserialize(const json& jsonData)
	{
		Collider::PreDeserialize(jsonData);

		PREDESERIALIZE_VALUE(radius);
		PREDESERIALIZE_VALUE(height);
	}

	void CapsuleCollider::PostSerialize(json& jsonData) const
	{
	}

	void CapsuleCollider::PostDeserialize(const json& jsonData)
	{
	}
}