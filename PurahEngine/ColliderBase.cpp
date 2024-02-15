#include "ZnCollider.h"
#include "ColliderBase.h"

void PurahEngine::ColliderBase::SetOffsetPosition(const Eigen::Vector3f& _position)
{
	if (awake)
	{
		znCollider->SetLocalPosition(_position);
	}
	else
	{
		this->positionOffset = _position;
	}
}

Eigen::Vector3f PurahEngine::ColliderBase::GetOffsetPosition() const
{
	if (awake)
	{
		return this->znCollider->GetLocalPosition();
	}
	else
	{
		return this->positionOffset;
	}
}

void PurahEngine::ColliderBase::SetOffsetQuaternion(const Eigen::Quaternionf& _quaternion)
{
	if (awake)
	{
		this->znCollider->SetLocalQuaternion(_quaternion);
	}
	else
	{
		this->rotationOffset = _quaternion;
	}
}

Eigen::Quaternionf PurahEngine::ColliderBase::GetOffsetQuaternion() const
{
	if (awake)
	{
		return this->znCollider->GetLocalQuaternion();
	}
	else
	{
		return this->rotationOffset;
	}
}
