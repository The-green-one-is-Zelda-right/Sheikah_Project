#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "HingeJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	HingeJoint::HingeJoint(physx::PxRevoluteJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		assert(_pxJoint != nullptr);
		joint = _pxJoint;
		joint->userData = this;

		rigidbody0 = _znBody0;
		rigidbody1 = _znBody1;
	}

	HingeJoint::~HingeJoint()
	{
		rigidbody0 = nullptr;
		rigidbody1 = nullptr;
		JointHelper::Release(joint);
		joint = nullptr;
	}

	float HingeJoint::GetAngle() const
	{
		assert(joint != nullptr);

		return joint->getAngle();
	}

	float HingeJoint::GetVelocity() const
	{
		assert(joint != nullptr);

		return joint->getVelocity();
	}

	void HingeJoint::SetLimit(float _lower, float _upper)
	{
		assert(joint != nullptr);

		joint->setLimit({_lower, _upper});
	}

	void HingeJoint::GetLimit(float& _upper, float& _lower) const
	{
		const auto limit = joint->getLimit();

		_upper = limit.upper;
		_lower = limit.lower;
	}

	void HingeJoint::SetRestitution(float _restitution) const
	{
		auto limit = joint->getLimit();

		limit.restitution = _restitution;

		joint->setLimit(limit);
	}

	float HingeJoint::GetRestitution() const
	{
		const auto limit = joint->getLimit();

		return limit.restitution;
	}

	void HingeJoint::SetBounceThreshold(float _bounceThreshold) const
	{
		auto limit = joint->getLimit();

		limit.bounceThreshold = _bounceThreshold;

		joint->setLimit(limit);
	}

	float HingeJoint::GetBounceThreshold() const
	{
		const auto limit = joint->getLimit();

		return limit.bounceThreshold;
	}

	void HingeJoint::SetSpringArg(float _stiffness, float _damping)
	{
		auto limit = joint->getLimit();

		limit.stiffness = _stiffness;
		limit.damping = _damping;

		joint->setLimit(limit);
	}

	void HingeJoint::GetSpringArg(float& _stiffness, float& _damping)
	{
		auto limit = joint->getLimit();

		_stiffness = limit.stiffness;
		_damping = limit.damping;
	}

	void HingeJoint::SetDriveVelocity(float _velocity)
	{
		assert(joint != nullptr);

		joint->setDriveVelocity(_velocity);
	}

	float HingeJoint::GetDriveVelocity() const
	{
		assert(joint != nullptr);

		return joint->getDriveVelocity();
	}

	void HingeJoint::SetDriveForceLimit(float _limit)
	{
		assert(joint != nullptr);

		joint->setDriveForceLimit(_limit);
	}

	float HingeJoint::GetDriveForceLimit() const
	{
		assert(joint != nullptr);

		return joint->getDriveForceLimit();
	}

	void HingeJoint::SetDriveGearRatio(float _ratio)
	{
		assert(joint != nullptr);

		_ratio = std::clamp(_ratio, 0.f, 1.f);

		joint->setDriveGearRatio(_ratio);
	}

	float HingeJoint::GetDriveGearRatio() const
	{
		assert(joint != nullptr);

		return joint->getDriveGearRatio();
	}

	void HingeJoint::SetLimitEnable(bool _value)
	{
		assert(joint != nullptr);

		joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, _value);
	}

	void HingeJoint::SetDriveEnable(bool _value)
	{
		assert(joint != nullptr);

		joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, _value);
	}

	void HingeJoint::SetDriveFreespin(bool _value)
	{
		assert(joint != nullptr);

		joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_FREESPIN, _value);
	}
}
