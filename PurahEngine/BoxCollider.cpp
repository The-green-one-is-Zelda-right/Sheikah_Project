#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "BoxCollider.h"

namespace PurahEngine
{
	BoxCollider::~BoxCollider()
	{
		auto& physicsSystem = PhysicsSystem::GetInstance();

		if (znCollider)
		{
			physicsSystem.FreeObject(znCollider, this->GetGameObject());
			znCollider = nullptr;
		}
	}

	void BoxCollider::PostInitialize()
	{
		Collider::PostInitialize();

		const auto scale = transform->GetWorldScale();

		auto& physics = PhysicsSystem::GetInstance();
		this->znCollider = physics.CreateBoxCollider(
			this->GetGameObject(),
			size.x() * 0.5f * scale.x(), size.y() * 0.5f * scale.y(), size.z() * 0.5f * scale.z(),
			physicsMaterial
		);

		Collider::SetUserData();

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetRotationOffset(rotationOffset);
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

	void BoxCollider::PreDeserialize(const json& jsonData)
	{
		Collider::PreDeserialize(jsonData);

		PREDESERIALIZE_VECTOR3F(size);
	}
}
