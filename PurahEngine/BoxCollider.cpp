#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "BoxCollider.h"

namespace PurahEngine
{
	void BoxCollider::Initialize()
	{
		Collider::Initialize();
	}

	void BoxCollider::OnDataLoadComplete()
	{
		const auto scale = transform->GetWorldScale();

		this->znCollider = PhysicsSystem::GetInstance().CreateBoxCollider(
			this->GetGameObject(),
			size.x() * 0.5f * scale.x(), size.y() * 0.5f * scale.y(), size.z() * 0.5f * scale.z()
		);

		znCollider->SetUserData(this);

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);

		znCollider->SetPosition(transform->GetWorldPosition());
		znCollider->SetQuaternion(transform->GetWorldRotation());
	}

	void BoxCollider::SetSize(const Eigen::Vector3f& _size)
	{
		if (awake)
		{
			OutputDebugStringW(L"실행 도중에 Collider size 변경은 불가능 합니다.");
			return;
		}

		this->size = _size;
	}

	void BoxCollider::PreSerialize(json& jsonData) const
	{

	}

	void BoxCollider::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(cType);
		PREDESERIALIZE_VECTOR3F(size);
		PREDESERIALIZE_VALUE(isTrigger);
		PREDESERIALIZE_VALUE(layer);
		PREDESERIALIZE_VECTOR3F(positionOffset);
	}

	void BoxCollider::PostSerialize(json& jsonData) const
	{

	}

	void BoxCollider::PostDeserialize(const json& jsonData)
	{

	}

}
