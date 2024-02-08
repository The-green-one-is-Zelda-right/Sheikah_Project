#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "PrismaticJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	//PrismaticJoint::PrismaticJoint(
	//	physx::PxPhysics*& _factory,
	//	RigidBody* _object0, const ZnTransform& _transform0,
	//	RigidBody* _object1, const ZnTransform& _transform1) : tolerances()
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
	//	joint = PxPrismaticJointCreate(
	//		*_factory,
	//		rigid0, t0,
	//		rigid1, t1
	//	);
	// 
	//	assert(joint != nullptr, "ZonaiPhysicsX :: Prismatic Joint Initialize Error");
	// 
	//	tolerances = &(_factory->getTolerancesScale());
	// 
	//	if (joint)
	//	{
	//		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	//	}
	//}

	PrismaticJoint::PrismaticJoint(physx::PxPrismaticJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1, const physx::PxTolerancesScale* _pxScale):
		tolerances(_pxScale)
	{
		joint = _pxJoint;
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

		joint->setLimit({*tolerances, _lower, _upper});
	}

	void PrismaticJoint::SetLimitWithSpring(float _lower, float _upper, float _stiffness, float _damping)
	{
		assert(joint != nullptr);

		joint->setLimit({_lower, _upper, {_stiffness, _damping}});
	}

	void PrismaticJoint::SetLimitEnable(bool _value)
	{
		assert(joint != nullptr);

		joint->setPrismaticJointFlag(physx::PxPrismaticJointFlag::eLIMIT_ENABLED, _value);
	}

	bool PrismaticJoint::IsLimitEnalbed() const
	{
		assert(joint != nullptr);

		return joint->getPrismaticJointFlags() & physx::PxPrismaticJointFlag::eLIMIT_ENABLED;
	}
}
