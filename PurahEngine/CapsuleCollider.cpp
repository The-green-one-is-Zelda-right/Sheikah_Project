#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "CapsuleCollider.h"

namespace PurahEngine
{
	CapsuleCollider::~CapsuleCollider()
	{
		auto& physicsSystem = PhysicsSystem::GetInstance();

		physicsSystem.FreeObject(znCollider, this->GetGameObject());
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

		if (!znCollider)
		{
			GetGameObject()->DeleteComponent(this);
			return;
		}

		Collider::SetUserData();

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);

		znCollider->SetPosition(transform->GetWorldPosition());
		znCollider->SetQuaternion(transform->GetWorldRotation());
	}

	void CapsuleCollider::SetSize(float _radius, float _halfHeight)
	{
		if (awake)
		{
			OutputDebugStringW(L"���� ���߿� Collider size ������ �Ұ��� �մϴ�.");
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