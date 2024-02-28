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

	class SphereCollider : public Collider
	{
	public:
		SphereCollider() = delete;
		SphereCollider(physx::PxShape*, RigidBody*);
		// SphereCollider(physx::PxPhysics*&, RigidBody*&, const float&, physx::PxMaterial*&);
		~SphereCollider() override;
	};
} // ZonaiPhysics

