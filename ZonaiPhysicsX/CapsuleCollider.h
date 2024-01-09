#pragma once
#include "Collider.h"

namespace physx
{
	class PxPhysics;
	class PxShape;
	class PxRigidDynamic;
	class PxRigidActor;
	class PxMaterial;
}

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}

namespace ZonaiPhysics
{
	class RigidBody;

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider() noexcept = delete;
		CapsuleCollider(physx::PxPhysics*&, RigidBody*&, const float&, const float&, physx::PxMaterial*&) noexcept;
		~CapsuleCollider() noexcept override;
	};
} // ZonaiPhysics

