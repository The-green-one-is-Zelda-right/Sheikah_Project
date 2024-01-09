#pragma once
#include "ZnHingeJoint.h"
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

	class HingeJoint : public ZnHingeJoint
	{
	public:
		HingeJoint() noexcept;
		HingeJoint(
			physx::PxPhysics*& _factory,
			RigidBody* _object0, const ZnTransform& _transform0,
			RigidBody* _object1, const ZnTransform& _transform1) noexcept;
		~HingeJoint() noexcept override;

	public:
		void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept override;
		Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept override;

		void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept override;
		Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept override;

		Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept override;
		Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept override;

		void		SetBreakForce(float _force, float _torque) noexcept override;
		void		GetBreakForce(float& _force, float& _torque) const noexcept override;

	public:
		float GetAngle() const;
		float GetVelocity() const;
		void SetLimit(float _upper, float _lower);
		void SetLimit(float _upper, float _lower, float _stiffness, float _damping);

	private:
		RigidBody* object[2];
		physx::PxRevoluteJoint* joint;
	};
} // namespace ZonaiPhysics

