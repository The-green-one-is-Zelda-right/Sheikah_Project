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

	void SphericalJoint::GetLimitAngle(float& _outY, float& _outZ)
	{
		assert(joint != nullptr);

		const auto cone = joint->getLimitCone();

		_outY = cone.yAngle;
		_outZ = cone.zAngle;
	}

	float SphericalJoint::GetYAngle() const
	{
		assert(joint != nullptr);

		return joint->getSwingYAngle();
	}

	float SphericalJoint::GetZAngle() const
	{
		assert(joint != nullptr);

		return joint->getSwingZAngle();
	}

	void SphericalJoint::LimitEnable(bool _value)
	{
		assert(joint != nullptr);

		joint->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, _value);
	}

	void SphericalJoint::SetLimitCone(float _yAngle, float _zAngle)
	{
		assert(joint != nullptr);

		float yAngleRad = (physx::PxPi / 180.f * _yAngle);
		float zAngleRad = (physx::PxPi / 180.f * _zAngle);

		joint->setLimitCone({ yAngleRad, zAngleRad });
	}

	void SphericalJoint::SetSpringArg(float _stiffness, float _damping)
	{
		assert(joint != nullptr);

		auto limit = joint->getLimitCone();

		limit.damping = _damping;
		limit.stiffness = _stiffness;

		joint->setLimitCone(limit);
	}

	void SphericalJoint::SetRestitution(float _restitution) const
	{
		assert(joint != nullptr);

		auto limit = joint->getLimitCone();

		limit.restitution = _restitution;

		joint->setLimitCone(limit);
	}
}
