#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "CapsuleCollider.h"

namespace PurahEngine
{
	void CapsuleCollider::Initialize()
	{
		Collider::Initialize();
	}

	void CapsuleCollider::OnDataLoadComplete()
	{
		const auto scale = transform->GetWorldScale();

		const float maxScale = max(scale.x(), scale.z());

		this->znCollider = PhysicsSystem::GetInstance().CreateCapsuleCollider(
			this->GetGameObject(),
			radius * maxScale, height * scale.y()
		);

		znCollider->SetUserData(this);

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
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VALUE(cType);
		PREDESERIALIZE_VALUE(radius);
		PREDESERIALIZE_VALUE(height);
		PREDESERIALIZE_VALUE(isTrigger);
		PREDESERIALIZE_VALUE(layer);
		PREDESERIALIZE_VECTOR3F(positionOffset);
	}

	void CapsuleCollider::PostSerialize(json& jsonData) const
	{
	}

	void CapsuleCollider::PostDeserialize(const json& jsonData)
	{
	}
}