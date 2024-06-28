#pragma once
#include "ZnHingeJoint.h"
#include "TemplateJoint.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#include "PxPhysicsAPI.h"
#pragma warning (pop)

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
		~HingeJoint() override;

	public:
		float GetAngle() const override;
		float GetVelocity() const override;

		void SetLimit(float _upper, float _lower) override;
		void GetLimit(float& _upper, float& _lower) const override;

		void SetRestitution(float _restitution) const override;
		float GetRestitution() const override;

		void SetBounceThreshold(float _bounceThreshold) const override;
		float GetBounceThreshold() const override;

		void SetSpringArg(float _stiffness, float _damping) override;
		void GetSpringArg(float& _stiffness, float& _damping) override;

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
