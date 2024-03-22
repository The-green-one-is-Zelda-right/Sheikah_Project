#pragma once
#include "ZnSphericalJoint.h"
#include "TemplateJoint.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

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
		void GetLimitAngle(float& _outY, float& _outZ) override;

		float GetYAngle() const override;
		float GetZAngle() const override;

		void LimitEnable(bool) override;

		void SetLimitAngle(float _yAngle, float _zAngle) override;
		void SetLimitRadian(float _yRadian, float _zRadian) override;

		void SetSpringArg(float _stiffness, float _damping) override;
		void SetRestitution(float _restitution) const override;
		float GetRestitution() const override;
	};
}
