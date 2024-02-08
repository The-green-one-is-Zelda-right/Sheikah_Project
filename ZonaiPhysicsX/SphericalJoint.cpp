#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "SphericalJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	//SphericalJoint::SphericalJoint(
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
	//	joint = PxSphericalJointCreate(
	//		*_factory,
	//		rigid0, t0,
	//		rigid1, t1
	//	);
	//
	//	assert(joint == nullptr, "ZonaiPhysicsX :: Spherical Joint Initialize Error");
	//
	//	if (joint)
	//	{
	//		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	//	}
	//}

	SphericalJoint::SphericalJoint(physx::PxSphericalJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		joint = _pxJoint;
		rigidbody0 = _znBody0;
		rigidbody1 = _znBody1;
	}

	SphericalJoint::~SphericalJoint()
	{
		rigidbody0 = nullptr;
		rigidbody1 = nullptr;
		JointHelper::Release(joint);
		joint = nullptr;
	}

	void SphericalJoint::GetLimitAngle(float* _outY, float* _outZ)
	{
		assert(joint != nullptr);

		const auto cone = joint->getLimitCone();

		if (_outY)
		{
			*_outY = cone.yAngle;
		}
		if (_outZ)
		{
			*_outZ = cone.zAngle;
		}
	}

	void SphericalJoint::LimitEnable(bool _value)
	{
		assert(joint != nullptr);

		joint->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, _value);
	}

	void SphericalJoint::SetLimitCone(float _yAngle, float _zAngle)
	{
		assert(joint != nullptr);

		joint->setLimitCone({_yAngle, _zAngle});
	}

	void SphericalJoint::SetLimitConeWithSpring(float _yAngle, float _zAngle, float _stiffness, float _damping)
	{
		assert(joint != nullptr);

		joint->setLimitCone({_yAngle, _zAngle, {_stiffness, _damping}});
	}
}
