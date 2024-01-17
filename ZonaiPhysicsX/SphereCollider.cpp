#include "PxPhysicsAPI.h"
#include "RigidBody.h"
#include "SphereCollider.h"

namespace ZonaiPhysics
{
	SphereCollider::SphereCollider(
		physx::PxPhysics*& _factory,
		RigidBody*& _body,
		const float& _offset,
		physx::PxMaterial*& _material
	) noexcept:
		Collider(_factory, _body)
	{
		using namespace physx;
		rigidbody = _body;

		shape = _factory->createShape(PxSphereGeometry(_offset), *_material);
		shape->userData = this;

		shape->setFlag(PxShapeFlag::eVISUALIZATION, true);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

		_body->getRigidDynamic()->attachShape(*shape);
	}

	SphereCollider::~SphereCollider() noexcept
	{
		shape->release();
		rigidbody->getRigidDynamic()->detachShape(*shape);

		rigidbody = nullptr;
	}
}
