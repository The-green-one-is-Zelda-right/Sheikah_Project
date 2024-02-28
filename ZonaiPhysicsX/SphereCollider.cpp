#include "PxPhysicsAPI.h"
#include "RigidBody.h"
#include "SphereCollider.h"

#include "ColliderHelper.h"

namespace ZonaiPhysics
{
	SphereCollider::SphereCollider(physx::PxShape* _pxShape, RigidBody* _znBody) :
		Collider(_pxShape, _znBody)
	{
		pxShape->userData = this;
	}

	SphereCollider::~SphereCollider()
	{
		ColliderHelper::Release(pxShape);
		pxShape = nullptr;
		znBody = nullptr;
	}
}
