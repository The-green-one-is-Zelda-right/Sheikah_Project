#pragma once
#include "ZnFixedJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
	class PxFixedJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class FixedJoint : public ZnFixedJoint
	{
	public:
							FixedJoint() noexcept = delete;
							FixedJoint(
								physx::PxPhysics*& _factory, 
								RigidBody* _object0, const ZnTransform& _transform0, 
								RigidBody* _object1, const ZnTransform& _transform1) noexcept;
		virtual				~FixedJoint() noexcept;

	public:
		void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept override;
		Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept override;

		void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept override;
		Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept override;

		Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept override;
		Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept override;

		void		SetBreakForce(float _force, float _torque) noexcept override;
		void		GetBreakForce(float& _force, float& _torque) const noexcept override;

	private:
		RigidBody* object[2];
		physx::PxFixedJoint* joint;
	};
} // namespace ZonaiPhysics

