#pragma once
#include "ZnHingeJoint.h"
#include "TemplateJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
	class PxRevoluteJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class HingeJoint : public TemplateJoint<ZnHingeJoint, physx::PxRevoluteJoint>
	{
	public:
		HingeJoint() = delete;
		HingeJoint(physx::PxRevoluteJoint*, RigidBody*, RigidBody*);
		//HingeJoint(
		//	physx::PxPhysics*& _factory,
		//	RigidBody* _object0, const ZnTransform& _transform0,
		//	RigidBody* _object1, const ZnTransform& _transform1);
		~HingeJoint() override;

	public:
		float GetAngle() const override;
		float GetVelocity() const override;

		void SetLimit(float _upper, float _lower) override;
		void SetLimitWithSpring(float _upper, float _lower, float _stiffness, float _damping) override;

		void SetDriveVelocity(float _velocity) override;
		float GetDriveVelocity() const override;

		void SetDriveForceLimit(float _limit) override;
		float GetDriveForceLimit() const override;

		void SetDriveGearRatio(float _ratio) override;
		float GetDriveGearRatio() const override;

		void SetLimitEnable(bool) override;
		void SetDriveEnable(bool) override;
		void SetDriveFreespin(bool) override;
	};
} // namespace ZonaiPhysics
