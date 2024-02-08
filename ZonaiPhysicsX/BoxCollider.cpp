#include <Eigen/Dense>
#include "RigidBody.h"
#include "PxPhysicsAPI.h"

#include "BoxCollider.h"

#include "ColliderHelper.h"

namespace ZonaiPhysics
{
	BoxCollider::BoxCollider(physx::PxShape* _pxShape, RigidBody* _znBody):
		Collider(_pxShape, _znBody)
	{
		pxShape->userData = this;
	}

	BoxCollider::~BoxCollider()
	{
		ColliderHelper::Release(pxShape);
		pxShape = nullptr;
		znBody = nullptr;
	}
}
