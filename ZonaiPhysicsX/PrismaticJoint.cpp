#include "RigidBody.h"
#include "ZnTransform.h"

#include "PrismaticJoint.h"

namespace ZonaiPhysics
{
	PrismaticJoint::PrismaticJoint(
		physx::PxPhysics*& _factory, 
		RigidBody* _object0, const ZnTransform& _transform0,
		RigidBody* _object1, const ZnTransform& _transform1) noexcept : tolerances()
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

		joint = physx::PxPrismaticJointCreate(
			*_factory,
			rigid0, t0,
			rigid1, t1
		);

		assert(joint == nullptr, "ZonaiPhysicsX :: Prismatic Joint Initialize Error");

		tolerances = &(_factory->getTolerancesScale());

		if (joint)
		{
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		}
	}

	PrismaticJoint::~PrismaticJoint() noexcept
	{
		rigidbody[0] = nullptr;
		rigidbody[1] = nullptr;
		joint->release();
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

		joint->setLimit({ *tolerances, _lower, _upper });
	}

	void PrismaticJoint::SetLimit(float _lower, float _upper, float _stiffness, float _damping)
	{
		assert(joint != nullptr);

		joint->setLimit({ _lower, _upper, { _stiffness, _damping } });
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
