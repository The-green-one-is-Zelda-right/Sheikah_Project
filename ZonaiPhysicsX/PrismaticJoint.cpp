#include "PrismaticJoint.h"

namespace ZonaiPhysics
{
	PrismaticJoint::PrismaticJoint(
		physx::PxPhysics*& _factory, 
		RigidBody* _object0, const ZnTransform& _transform0,
		RigidBody* _object1, const ZnTransform& _transform1) noexcept
	{

	}

	PrismaticJoint::~PrismaticJoint() noexcept
	{
		joint->release();
	}

	void PrismaticJoint::SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept
	{
	}

	Eigen::Vector3f PrismaticJoint::GetLocalPosition(eOBJECT) const noexcept
	{
	}

	void PrismaticJoint::SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept
	{
	}

	Eigen::Quaternionf PrismaticJoint::GetLocalQuaternion(eOBJECT) const noexcept
	{
	}

	Eigen::Vector3f PrismaticJoint::GetRelativeLinearVelocity() const noexcept
	{
	}

	Eigen::Vector3f PrismaticJoint::GetRelativeAngularVelocity() const noexcept
	{
	}

	void PrismaticJoint::SetBreakForce(float _force, float _torque) noexcept
	{
	}

	void PrismaticJoint::GetBreakForce(float& _force, float& _torque) const noexcept
	{
	}
}
