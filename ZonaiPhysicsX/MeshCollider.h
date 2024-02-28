#pragma once
#include "Collider.h"

namespace ZonaiPhysics
{
	class RigidBody;

	class MeshCollider : public Collider
	{
	public:
		MeshCollider() = delete;
		MeshCollider(physx::PxShape*, RigidBody*);
		~MeshCollider() override;
	};
}

