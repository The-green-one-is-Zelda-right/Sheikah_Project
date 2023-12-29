#include "FixedJoint.h"

#include "ZnFixedJoint.h"

namespace PurahEngine
{
	void FixedJoint::SetLocalPosition(int _index, const Eigen::Vector3f& _position) noexcept
	{
		assert(_index == 1 || _index == 0);

		 joint->SetLocalPosition(static_cast<ZonaiPhysics::ZnJoint::eOBJECT>(_index), _position);
	}

	Eigen::Vector3f FixedJoint::GetLocalPosition(int _index) const noexcept
	{
		assert(_index == 1 || _index == 0);

		return joint->GetLocalPosition(static_cast<ZonaiPhysics::ZnJoint::eOBJECT>(_index));
	}

	void FixedJoint::SetLocalQuaternion(int _index, const Eigen::Quaternionf& _quaternion) noexcept
	{
		assert(_index == 1 || _index == 0);

		joint->SetLocalQuaternion(static_cast<ZonaiPhysics::ZnJoint::eOBJECT>(_index), _quaternion);
	}

	Eigen::Quaternionf FixedJoint::GetLocalQuaternion(int _index) const noexcept
	{
		assert(_index == 1 || _index == 0);

		return joint->GetLocalQuaternion(static_cast<ZonaiPhysics::ZnJoint::eOBJECT>(_index));
	}

	Eigen::Vector3f FixedJoint::GetRelativeLinearVelocity() const noexcept
	{
		return joint->GetRelativeLinearVelocity();
	}

	Eigen::Vector3f FixedJoint::GetRelativeAngularVelocity() const noexcept
	{
		return joint->GetRelativeAngularVelocity();
	}

	void FixedJoint::SetBreakForce(float _force, float _torque) noexcept
	{
		joint->SetBreakForce(_force, _torque);
	}

	void FixedJoint::GetBreakForce(float& _force, float& _torque) const noexcept
	{
		joint->GetBreakForce(_force, _torque);
	}
}
