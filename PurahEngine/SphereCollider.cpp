#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "SphereCollider.h"


namespace PurahEngine
{
	SphereCollider::~SphereCollider()
	{
		auto& physicsSystem = PhysicsSystem::GetInstance();

		physicsSystem.FreeObject(znCollider, this->GetGameObject());
	}

	void SphereCollider::PostInitialize()
	{
		Collider::PostInitialize();

		const auto scale = transform->GetWorldScale();
		const float maxScale = max(max(scale.x(), scale.y()), scale.z());

		auto& physics = PhysicsSystem::GetInstance();
		this->znCollider = PhysicsSystem::GetInstance().CreateSphereCollider(
			this->GetGameObject(),
			radius * maxScale,
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

	void SphereCollider::SetSize(float _radius)
	{
		if (awake)
		{
			OutputDebugStringW(L"실행 도중에 Collider size 변경은 불가능 합니다.");
			return;
		}

		radius = _radius;
	}

	void SphereCollider::PreSerialize(json& jsonData) const
	{

	}

	void SphereCollider::PreDeserialize(const json& jsonData)
	{
		Collider::PreDeserialize(jsonData);

		PREDESERIALIZE_VALUE(radius);
	}

	void SphereCollider::PostSerialize(json& jsonData) const
	{

	}

	void SphereCollider::PostDeserialize(const json& jsonData)
	{

	}
}