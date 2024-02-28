#pragma once
#include "Collider.h"

namespace ZonaiPhysics
{
	class ConvexCollider : public Collider
	{
	public:
		ConvexCollider() = delete;
		ConvexCollider(physx::PxShape* _pxShape, RigidBody* _znBody);
		~ConvexCollider() override;
	};
}

