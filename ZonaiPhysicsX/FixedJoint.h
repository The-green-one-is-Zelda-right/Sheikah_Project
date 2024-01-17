#pragma once
#include "ZnFixedJoint.h"
#include "TemplateJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
	class PxFixedJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class FixedJoint : public TemplateJoint<ZnFixedJoint, physx::PxFixedJoint>
	{
	public:
							FixedJoint() noexcept = delete;
							FixedJoint(
								physx::PxPhysics*& _factory, 
								RigidBody* _object0, const ZnTransform& _transform0, 
								RigidBody* _object1, const ZnTransform& _transform1) noexcept;
							~FixedJoint() noexcept override;
	};
} // namespace ZonaiPhysics

