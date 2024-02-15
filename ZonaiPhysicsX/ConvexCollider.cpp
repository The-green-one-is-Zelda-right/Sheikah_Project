#include "PxPhysicsAPI.h"
#include "ColliderHelper.h"

#include "ConvexCollider.h"

namespace ZonaiPhysics
{
	ConvexCollider::ConvexCollider(physx::PxShape* _pxShape, RigidBody* _znBody) :
		Collider(_pxShape, _znBody)
	{
		_pxShape->userData = this;
	}

	ConvexCollider::~ConvexCollider()
	{
		ColliderHelper::Release(pxShape);
		pxShape = nullptr;
		znBody = nullptr;
	}
}
