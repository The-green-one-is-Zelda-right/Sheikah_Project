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

	class BoxCollider : public Collider
	{
	public:
		BoxCollider() noexcept = delete;
		BoxCollider(physx::PxPhysics*&, RigidBody*&, const ZonaiMath::Vector3D&, physx::PxMaterial*&) noexcept;
		virtual ~BoxCollider() noexcept;

	};
} // ZonaiPhysics

