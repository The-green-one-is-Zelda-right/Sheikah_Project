#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "JointHelper.h"

#include "SpringFlexJoint.h"

using namespace physx;

namespace ZonaiPhysics
{
	SpringFlexJoint::SpringFlexJoint(D1SpringFlexJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		assert(_pxJoint != nullptr);
		joint = _pxJoint;
		joint->userData = this;

		rigidbody0 = _znBody0;
		rigidbody1 = _znBody1;
	}

	SpringFlexJoint::~SpringFlexJoint()
	{
		rigidbody0 = nullptr;
		rigidbody1 = nullptr;
		JointHelper::Release(joint);
		joint = nullptr;
	}

	void SpringFlexJoint::SetSpringStiffness(float stiffness)
	{
		joint->setSpringStiffness(stiffness);
	}

	float SpringFlexJoint::GetSpringStiffness() const
	{
		return joint->getSpringStiffness();
	}

	void SpringFlexJoint::SetSpringDamping(float damping)
	{
		joint->setSpringDamping(damping);
	}

	float SpringFlexJoint::GetSpringDamping() const
	{
		return joint->getSpringDamping();
	}

	void SpringFlexJoint::SetLinearLimit(float limit)
	{
		joint->setLinearLimit(limit);
	}

	float SpringFlexJoint::GetLinearLimit() const
	{
		return 	joint->getLinearLimit();
	}
}