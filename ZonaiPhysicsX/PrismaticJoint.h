#pragma once
#include "ZnPrismaticJoint.h"
#include "TemplateJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class PrismaticJoint : public TemplateJoint<ZnPrismaticJoint, physx::PxPrismaticJoint>
	{
	public:
		PrismaticJoint() noexcept = delete;
		PrismaticJoint(
			physx::PxPhysics*& _factory,
			RigidBody* _object0, const ZnTransform& _transform0,
			RigidBody* _object1, const ZnTransform& _transform1) noexcept;
		~PrismaticJoint() noexcept override;

	public:
		float GetPosition() const override;

		float GetVelocity() const override;

		void SetLimit(float _lower, float _upper) override;
		void SetLimit(float _lower, float _upper, float _stiffness, float _damping) override;

		void SetLimitEnable(bool) override;

		bool IsLimitEnalbed() const override;

	public:
		physx::PxTolerancesScale const* tolerances;
	};
} // namespace ZonaiPhysics

