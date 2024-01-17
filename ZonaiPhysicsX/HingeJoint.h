#pragma once
#include "ZnHingeJoint.h"
#include "TemplateJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class HingeJoint : public TemplateJoint<ZnHingeJoint, physx::PxRevoluteJoint>
	{
	public:
		HingeJoint() noexcept = delete;
		HingeJoint(
			physx::PxPhysics*& _factory,
			RigidBody* _object0, const ZnTransform& _transform0,
			RigidBody* _object1, const ZnTransform& _transform1) noexcept;
		~HingeJoint() noexcept override;

	public:
		float GetAngle() const override;
		float GetVelocity() const override;

		void SetLimit(float _upper, float _lower) override;
		void SetLimit(float _upper, float _lower, float _stiffness, float _damping) override;

		void SetDriveVelocity(float _velocity) override;
		float GetDriveVelocity() const override;

		void SetDriveForceLimit(float) override;
		float GetDriveForceLimit() const override;

		void SetDriveGearRatio(float) override;
		float GetDriveGearRatio() const override;

		void SetLimitEnable(bool) override;
		void SetDriveEnable(bool) override;
		void SetDriveFreespin(bool) override;
	};
} // namespace ZonaiPhysics

