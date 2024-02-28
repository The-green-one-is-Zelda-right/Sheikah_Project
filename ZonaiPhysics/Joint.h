#pragma once
#include "ZnJoint.h"

namespace ZonaiPhysics
{
	class Joint : ZnJoint
	{
	public:
		void SetLocalPosition(eOBJECT, const Eigen::Vector3f&) override;
		Eigen::Vector3f GetLocalPosition(eOBJECT) const override;

		void SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) override;
		Eigen::Quaternionf GetLocalQuaternion(eOBJECT) const override;

		Eigen::Vector3f GetRelativeLinearVelocity() const override;
		Eigen::Vector3f GetRelativeAngularVelocity() const override;

		void SetBreakForce(float _force, float _torque) override;
		void GetBreakForce(float& _force, float& _torque) const override;
	};
}
