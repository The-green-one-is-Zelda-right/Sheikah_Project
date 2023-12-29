#pragma once
#include "ZnJoint.h"

namespace ZonaiPhysics
{
	class Joint : ZnJoint
	{
	public:
		void SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept override;
		Eigen::Vector3f GetLocalPosition(eOBJECT) const noexcept override;

		void SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept override;
		Eigen::Quaternionf GetLocalQuaternion(eOBJECT) const noexcept override;

		Eigen::Vector3f GetRelativeLinearVelocity() const noexcept override;
		Eigen::Vector3f GetRelativeAngularVelocity() const noexcept override;

		void SetBreakForce(float _force, float _torque) noexcept override;
		void GetBreakForce(float& _force, float& _torque) const noexcept override;
	};
}
