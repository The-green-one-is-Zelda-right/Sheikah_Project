#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "DistanceJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	//DistanceJoint::DistanceJoint(
	//	physx::PxPhysics*& _factory,
	//	RigidBody* _object0, const ZnTransform& _transform0,
	//	RigidBody* _object1, const ZnTransform& _transform1)
	//{
	//	rigidbody0 = _object0;
	//	rigidbody1 = _object1;
	//
	//	using namespace physx;
	//
	//	PxTransform t0{}, t1{};
	//
	//	t0.p = EigenToPhysx(_transform0.position);
	//	t0.q = EigenToPhysx(_transform0.quaternion);
	//	t1.p = EigenToPhysx(_transform1.position);
	//	t1.q = EigenToPhysx(_transform1.quaternion);
	//
	//	PxRigidDynamic* rigid0 = nullptr;
	//	PxRigidDynamic* rigid1 = nullptr;
	//
	//	if (_object0)
	//	{
	//		rigid0 = _object0->pxBody;
	//	}
	//	if (_object1)
	//	{
	//		rigid1 = _object1->pxBody;
	//	}
	//
	//	joint = PxDistanceJointCreate(
	//		*_factory,
	//		rigid0, t0,
	//		rigid1, t1
	//	);
	//
	//	assert(joint != nullptr, "ZonaiPhysicsX :: Distance Joint Initialize Error");
	//
	//	if (joint)
	//	{
	//		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	//	}
	//}

	DistanceJoint::DistanceJoint(physx::PxDistanceJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		joint = _pxJoint;
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
