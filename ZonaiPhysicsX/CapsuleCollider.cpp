#include "PxPhysicsAPI.h"
#include "RigidBody.h"
#include "CapsuleCollider.h"

namespace ZonaiPhysics
{
	CapsuleCollider::CapsuleCollider(
		physx::PxPhysics*& _factory,
		RigidBody*& _body,
		const float& _radius,
		const float& _height,
		physx::PxMaterial*& _material
	) noexcept : Collider(_factory, _body)
	{
		using namespace physx;
		rigidbody = _body;

		shape = _factory->createShape(PxCapsuleGeometry(_radius, _height), *_material);
		shape->userData = this;

		shape->setFlag(PxShapeFlag::eVISUALIZATION, true);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

		_body->getRigidDynamic()->attachShape(*shape);
	}

	CapsuleCollider::~CapsuleCollider() noexcept
	{
		shape->release();
		rigidbody->getRigidDynamic()->detachShape(*shape);

		rigidbody = nullptr;
	}
}