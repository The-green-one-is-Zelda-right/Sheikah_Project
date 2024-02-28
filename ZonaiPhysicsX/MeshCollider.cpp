#include "PxPhysicsAPI.h"
#include "ColliderHelper.h"

#include "MeshCollider.h"

namespace ZonaiPhysics
{
	MeshCollider::MeshCollider(physx::PxShape* _pxShape, RigidBody* _znBody) :
		Collider(_pxShape, _znBody)
	{
		pxShape->userData = this;
	}

	MeshCollider::~MeshCollider()
	{
		ColliderHelper::Release(pxShape);
		pxShape = nullptr;
		znBody = nullptr;
	}
}
