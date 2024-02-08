#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "HingeJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	//HingeJoint::HingeJoint(
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
	//	joint = PxRevoluteJointCreate(
	//		*_factory,
	//		rigid0, t0,
	//		rigid1, t1
	//	);
	// 
	//	assert(joint != nullptr, "ZonaiPhysicsX :: Hinge Joint Initialize Error");
//
	//	if (joint)
	//	{
	//		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	//	}
	//}

	HingeJoint::HingeJoint(physx::PxRevoluteJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		joint = _pxJoint;
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

	void HingeJoint::SetLimitWithSpring(float _lower, float _upper, float _stiffness, float _damping)
	{
		assert(joint != nullptr);

		joint->setLimit({_lower, _upper, {_stiffness, _damping}});
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
