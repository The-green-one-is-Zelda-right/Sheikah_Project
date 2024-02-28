#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "BoxCollider.h"

namespace PurahEngine
{
	void BoxCollider::Awake()
	{
		Collider::Awake();

		this->znCollider = PhysicsSystem::GetInstance().CreateBoxCollider(
			this->GetGameObject(),
			size.x(), size.y(), size.z()
		);

		znCollider->SetUserData(this);

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);
		SetRotationOffset(rotationOffset);
	}

	void BoxCollider::SetSize(const Eigen::Vector3f& _size)
	{
		this->size = _size;
	}

	void BoxCollider::PreSerialize(json& jsonData) const
	{

	}

	void BoxCollider::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VECTOR3F(size);
	}

	void BoxCollider::PostSerialize(json& jsonData) const
	{

	}

	void BoxCollider::PostDeserialize(const json& jsonData)
	{

	}

}
