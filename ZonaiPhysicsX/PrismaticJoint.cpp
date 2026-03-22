#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "PrismaticJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	PrismaticJoint::PrismaticJoint(physx::PxPrismaticJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1, const physx::PxTolerancesScale* _pxScale):
		tolerances(_pxScale)
	{
		assert(_pxJoint != nullptr);
		joint = _pxJoint;
		joint->userData = this;

		rigidbody0 = _znBody0;
		rigidbody1 = _znBody1;
	}

	PrismaticJoint::~PrismaticJoint()
	{
		rigidbody0 = nullptr;
		rigidbody1 = nullptr;
		JointHelper::Release(joint);
		joint = nullptr;
	}

	float PrismaticJoint::GetPosition() const
	{
		assert(joint != nullptr);

		return joint->getPosition();
	}

	float PrismaticJoint::GetVelocity() const
	{
		assert(joint != nullptr);

		return joint->getVelocity();
	}

	void PrismaticJoint::SetLimit(float _lower, float _upper)
	{
		assert(joint != nullptr);

		auto limit = joint->getLimit();

		limit.lower = _lower;
		limit.upper = _upper;

		joint->setLimit(limit);
	}

	void PrismaticJoint::SetMinDistance(float _lower)
	{
		assert(joint != nullptr);

		auto limit = joint->getLimit();

		limit.lower = _lower;

		joint->setLimit(limit);
	}

	void PrismaticJoint::SetMaxDistance(float _upper)
	{
		assert(joint != nullptr);

		auto limit = joint->getLimit();

		limit.upper = _upper;

		joint->setLimit(limit);
	}

	void PrismaticJoint::SetSpringArg(float _stiffness, float _damping)
	{
		assert(joint != nullptr);

		auto limit = joint->getLimit();

		limit.stiffness = _stiffness;
		limit.damping = _damping;

		joint->setLimit(limit);
	}

	void PrismaticJoint::SetRestitution(float _restitution) const
	{
		assert(joint != nullptr);

		auto limit = joint->getLimit();

		limit.restitution = _restitution;

		joint->setLimit(limit);
	}

	float PrismaticJoint::GetRestitution() const
	{
		assert(joint != nullptr);

		const auto limit = joint->getLimit();

		return limit.restitution;
	}

	void PrismaticJoint::SetLimitEnable(bool _value)
	{
		assert(joint != nullptr);

		joint->setPrismaticJointFlag(physx::PxPrismaticJointFlag::eLIMIT_ENABLED, _value);
	}

	bool PrismaticJoint::IsLimitEnabled() const
	{
		assert(joint != nullptr);

		return joint->getPrismaticJointFlags() & physx::PxPrismaticJointFlag::eLIMIT_ENABLED;
	}
}
