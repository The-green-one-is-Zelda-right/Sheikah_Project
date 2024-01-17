#include "RigidBody.h"
#include "ZnTransform.h"

#include "DistanceJoint.h"

namespace ZonaiPhysics
{
	DistanceJoint::DistanceJoint(
		physx::PxPhysics*& _factory, 
		RigidBody* _object0, const ZnTransform& _transform0, 
		RigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		rigidbody[0] = _object0;
		rigidbody[1] = _object1;

		using namespace physx;

		physx::PxTransform t0{}, t1{};

		t0.p.x = _transform0.position.x();
		t0.p.y = _transform0.position.y();
		t0.p.z = _transform0.position.z();

		t0.q.x = _transform0.quaternion.x();
		t0.q.y = _transform0.quaternion.y();
		t0.q.z = _transform0.quaternion.z();
		t0.q.w = _transform0.quaternion.w();

		t1.p.x = _transform1.position.x();
		t1.p.y = _transform1.position.y();
		t1.p.z = _transform1.position.z();

		t1.q.x = _transform1.quaternion.x();
		t1.q.y = _transform1.quaternion.y();
		t1.q.z = _transform1.quaternion.z();
		t1.q.w = _transform1.quaternion.w();

		PxRigidDynamic* rigid0 = nullptr;
		PxRigidDynamic* rigid1 = nullptr;

		if (_object0)
		{
			rigid0 = _object0->getRigidDynamic();
		}
		if (_object1)
		{
			rigid1 = _object1->getRigidDynamic();
		}

		joint = physx::PxDistanceJointCreate(
			*_factory,
			rigid0, t0,
			rigid1, t1
		);

		assert(joint != nullptr, "ZonaiPhysicsX :: Distance Joint Initialize Error");

		if (joint)
		{
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		}
	}

	DistanceJoint::~DistanceJoint() noexcept
	{
		rigidbody[0] = nullptr;
		rigidbody[1] = nullptr;
		joint->release();
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