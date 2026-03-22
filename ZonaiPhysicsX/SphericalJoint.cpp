#include "RigidBody.h"
#include "ZnTransform.h"
#include "ZnUtil.h"

#include "SphericalJoint.h"

#include "JointHelper.h"

namespace ZonaiPhysics
{
	SphericalJoint::SphericalJoint(physx::PxSphericalJoint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1)
	{
		assert(_pxJoint != nullptr);
		joint = _pxJoint;
		joint->userData = this;

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

	void SphericalJoint::SetLimitAngle(float _yAngle, float _zAngle)
	{
		assert(joint != nullptr);

		float yAngleRad = (physx::PxPi / 180.f * _yAngle);
		float zAngleRad = (physx::PxPi / 180.f * _zAngle);

		auto limit = joint->getLimitCone();

		limit.yAngle = yAngleRad;
		limit.zAngle = zAngleRad;

		joint->setLimitCone(limit);
	}

	void SphericalJoint::SetLimitRadian(float _yRadian, float _zRadian)
	{
		auto limit = joint->getLimitCone();

		limit.yAngle = _yRadian;
		limit.zAngle = _zRadian;

		joint->setLimitCone(limit);
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

	float SphericalJoint::GetRestitution() const
	{
		assert(joint != nullptr);

		const auto limit = joint->getLimitCone();

		return limit.restitution;
	}
}
