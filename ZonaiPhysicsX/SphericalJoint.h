#pragma once
#include "ZnSphericalJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
	class PxSphericalJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class SphericalJoint : public ZnSphericalJoint
	{
	public:
							SphericalJoint() noexcept = delete;
							SphericalJoint(
								physx::PxPhysics*& _factory,
								RigidBody* _object0, const ZnTransform& _transform0,
								RigidBody* _object1, const ZnTransform& _transform1) noexcept;
							~SphericalJoint() noexcept override;

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
		void GetLimitAngle(float* _outY, float* _outZ) override;
		void LimitEnable(bool) override;
		void SetLimitCone(float _yAngle, float _zAngle) override;
		void SetLimitCone(float _yAngle, float _zAngle, float _stiffness, float _damping) override;

	private:
		RigidBody* object[2];
		physx::PxSphericalJoint* joint;
	};
}

