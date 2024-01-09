#include "HingeJoint.h"


namespace ZonaiPhysics
{
	HingeJoint::HingeJoint() noexcept
	{

	}

	HingeJoint::HingeJoint(
		physx::PxPhysics*& _factory, 
		RigidBody* _object0, const ZnTransform& _transform0,
		RigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		joint->release();
	}

	HingeJoint::~HingeJoint() noexcept
	{
	}

	float HingeJoint::GetAngle() const
	{
	}

	float HingeJoint::GetVelocity() const
	{
	}

	void HingeJoint::SetLimit(float _upper, float _lower)
	{
	}

	void HingeJoint::SetLimit(float _upper, float _lower, float _stiffness, float _damping)
	{
	}
}
