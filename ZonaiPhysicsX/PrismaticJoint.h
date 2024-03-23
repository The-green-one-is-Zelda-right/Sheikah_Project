#pragma once
#include "ZnPrismaticJoint.h"
#include "TemplateJoint.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

namespace physx
{
	class PxPhysics;
	class PxPrismaticJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class PrismaticJoint : public TemplateJoint<ZnPrismaticJoint, physx::PxPrismaticJoint>
	{
	public:
		PrismaticJoint() = delete;
		PrismaticJoint(physx::PxPrismaticJoint*, RigidBody*, RigidBody*, const physx::PxTolerancesScale*);
		//PrismaticJoint(
		//	physx::PxPhysics*& _factory,
		//	RigidBody* _object0, const ZnTransform& _transform0,
		//	RigidBody* _object1, const ZnTransform& _transform1);
		~PrismaticJoint() override;

	public:
		float GetPosition() const override;

		float GetVelocity() const override;

		void SetLimit(float _lower, float _upper) override;
		void SetSpringArg(float _stiffness, float _damping) override;

		void SetLimitEnable(bool) override;

		bool IsLimitEnalbed() const override;

	public:
		const physx::PxTolerancesScale* tolerances;
	};
} // namespace ZonaiPhysics
