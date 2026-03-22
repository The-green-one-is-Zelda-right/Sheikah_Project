#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "FixedJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	FixedJoint::FixedJoint(physx::PxFixedJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		assert(_pxJoint != nullptr);
		joint = _pxJoint;
		joint->userData = this;

		rigidbody0 = _znBody0;
		rigidbody1 = _znBody1;
	}

	FixedJoint::~FixedJoint()
	{
		rigidbody0 = nullptr;
		rigidbody1 = nullptr;
		JointHelper::Release(joint);
		joint = nullptr;
	}
}
