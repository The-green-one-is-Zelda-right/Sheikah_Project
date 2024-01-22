#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "FixedJoint.h"

namespace ZonaiPhysics
{
	FixedJoint::FixedJoint(
		physx::PxPhysics*& _factory,
		RigidBody* _object0, const ZnTransform& _transform0,
		RigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		rigidbody[0] = _object0;
		rigidbody[1] = _object1;

		using namespace physx;

		PxTransform t0{}, t1{};

		t0.p = EigenToPhysx(_transform0.position);
		t0.q = EigenToPhysx(_transform0.quaternion);
		t1.p = EigenToPhysx(_transform1.position);
		t1.q = EigenToPhysx(_transform1.quaternion);

		PxRigidDynamic* rigid0 = nullptr;
		PxRigidDynamic* rigid1 = nullptr;

		if (_object0)
		{
			rigid0 = _object0->pxBody;
		}
		if (_object1)
		{
			rigid1 = _object1->pxBody;
		}

		joint = PxFixedJointCreate(
			*_factory,
			rigid0, t0,
			rigid1, t1
		);

		assert(joint != nullptr, "ZonaiPhysicsX :: Fixed Joint Initialize Error");

		if (joint)
		{
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		}
	}

	FixedJoint::~FixedJoint() noexcept
	{
		rigidbody[0] = nullptr;
		rigidbody[1] = nullptr;
		joint->release();
	}
}
