#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "DistanceJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	DistanceJoint::DistanceJoint(physx::PxDistanceJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		assert(_pxJoint != nullptr);
		joint = _pxJoint;
		joint->userData = this;

		rigidbody0 = _znBody0;
		rigidbody1 = _znBody1;
	}

	DistanceJoint::~DistanceJoint()
	{
		rigidbody0 = nullptr;
		rigidbody1 = nullptr;
		JointHelper::Release(joint);
		joint = nullptr;
	}

	float DistanceJoint::GetDistance() const
	{
		assert(joint != nullptr);

		return joint->getDistance();
	}

	void DistanceJoint::SetMinDistance(float _distance)
	{
		assert(joint != nullptr);

		joint->setMinDistance(_distance);
	}

	float DistanceJoint::GetMinDistance() const
	{
		assert(joint != nullptr);

		return joint->getMinDistance();
	}

	void DistanceJoint::SetMaxDistance(float _distance)
	{
		assert(joint != nullptr);

		joint->setMaxDistance(_distance);
	}

	float DistanceJoint::GetMaxDistance() const
	{
		assert(joint != nullptr);

		return joint->getMaxDistance();
	}

	void DistanceJoint::SetTolerance(float _tolerance)
	{
		assert(joint != nullptr);

		joint->setTolerance(_tolerance);
	}

	float DistanceJoint::GetTolerance() const
	{
		assert(joint != nullptr);

		return joint->getTolerance();
	}

	void DistanceJoint::SetStiffness(float _stiffness)
	{
		assert(joint != nullptr);

		joint->setStiffness(_stiffness);
	}

	float DistanceJoint::GetStiffness() const
	{
		assert(joint != nullptr);

		return joint->getStiffness();
	}

	void DistanceJoint::SetDamping(float _damping)
	{
		assert(joint != nullptr);

		joint->setDamping(_damping);
	}

	float DistanceJoint::GetDamping() const
	{
		assert(joint != nullptr);

		return joint->getDamping();
	}

	void DistanceJoint::SetSpringArg(float _stiffness, float _damping)
	{
		assert(joint != nullptr);

		joint->setStiffness(_stiffness);
		joint->setDamping(_damping);
	}

	void DistanceJoint::SetMinDistanceEnable(bool _value)
	{
		assert(joint != nullptr);

		using namespace physx;
		joint->setDistanceJointFlag(PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, _value);
	}

	void DistanceJoint::SetMaxDistanceEnable(bool _value)
	{
		assert(joint != nullptr);

		using namespace physx;
		joint->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, _value);
	}

	void DistanceJoint::SetSpringEnable(bool _value)
	{
		assert(joint != nullptr);

		using namespace physx;
		joint->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, _value);
	}
}
