#include "GameObject.h"
#include "PhysicsSystem.h"

#include "BoxCollider.h"

namespace PurahEngine
{
	BoxCollider::BoxCollider()
	{
	}
	BoxCollider::~BoxCollider()
	{
	}
	void BoxCollider::Awake()
	{
		collider = PhysicsSystem::GetInstance().CreateBoxCollider(
			GetGameObject()->GetName(),
			size.x(), size.y(), size.z()
		);

		awake = false;

		collider->SetLocalPosition(positionOffset);
		collider->SetLocalQuaternion(rotationOffset);
	}

	void BoxCollider::SetSize(const Eigen::Vector3f& _size)
	{
		size = _size;
	}

	void BoxCollider::SetOffsetPosition(const Eigen::Vector3f& _position)
	{
		if (awake)
		{
			positionOffset = _position;
		}
		else
		{
			collider->SetLocalPosition(_position);
		}
	}

	Eigen::Vector3f BoxCollider::GetOffsetPosition()
	{
		if (awake)
		{
			return positionOffset;
		}
		else
		{
			return collider->GetLocalPosition();
		}
	}

	void BoxCollider::SetOffsetQuaternion(const Eigen::Quaternionf& _quaternion)
	{
		if (awake)
		{
			rotationOffset = _quaternion;
		}
		else
		{
			collider->SetLocalQuaternion(_quaternion);
		}
	}

	Eigen::Quaternionf BoxCollider::GetOffsetQuaternion()
	{
		if (awake)
		{
			return rotationOffset;
		}
		else
		{
			return collider->GetLocalQuaternion();
		}
	}
}
