#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "JointHelper.h"

#include "D6Spring.h"

namespace ZonaiPhysics
{
	D6Spring::D6Spring(physx::PxD6Joint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		assert(_pxJoint != nullptr);
		joint = _pxJoint;

		const auto constrain = joint->getConstraint();
		constrain->userData = this;

		rigidbody0 = _znBody0;
		rigidbody1 = _znBody1;
	}

	D6Spring::~D6Spring()
	{
		rigidbody0 = nullptr;
		rigidbody1 = nullptr;
		JointHelper::Release(joint);
		joint = nullptr;
	}

	void D6Spring::SetLinearLimit(float limit)
	{
		assert(joint != nullptr);

		using namespace physx;

		PxJointLinearLimit linearLimit(limit);
		joint->setLinearLimit(linearLimit);
	}

	float D6Spring::GetLinearLimit() const
	{

		assert(joint != nullptr);

		using namespace physx;

		const PxJointLinearLimit linearLimit = joint->getLinearLimit();
		return linearLimit.value;
	}

	void D6Spring::SetSpringProperties(float stiffness, float damping)
	{
		assert(joint != nullptr);

		using namespace physx;

		PxD6JointDrive drive(stiffness, damping, PX_MAX_F32, true);
		joint->setDrive(PxD6Drive::eSLERP, drive);
	}

	void D6Spring::GetSpringProperties(float& stiffness, float& damping) const
	{
		assert(joint != nullptr);

		using namespace physx;

		const PxD6JointDrive drive = joint->getDrive(PxD6Drive::eSLERP);
		stiffness = drive.stiffness;
		damping = drive.damping;
	}
}