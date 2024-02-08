#pragma once
#include "ZnSphericalJoint.h"
#include "TemplateJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
	class PxSphericalJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class SphericalJoint : public TemplateJoint<ZnSphericalJoint, physx::PxSphericalJoint>
	{
	public:
		SphericalJoint() = delete;
		SphericalJoint(physx::PxSphericalJoint*, RigidBody*, RigidBody*);
		//SphericalJoint(
		//	physx::PxPhysics*& _factory,
		//	RigidBody* _object0, const ZnTransform& _transform0,
		//	RigidBody* _object1, const ZnTransform& _transform1);
		~SphericalJoint() override;

	public:
		void GetLimitAngle(float* _outY, float* _outZ) override;
		void LimitEnable(bool) override;
		void SetLimitCone(float _yAngle, float _zAngle) override;
		void SetLimitConeWithSpring(float _yAngle, float _zAngle, float _stiffness, float _damping) override;
	};
}
