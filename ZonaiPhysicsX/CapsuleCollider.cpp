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
		rigid_ = _body;

		shape_ = _factory->createShape(PxCapsuleGeometry(_radius, _height), *_material);
		shape_->userData = this;

		shape_->setFlag(PxShapeFlag::eVISUALIZATION, true);
		shape_->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

		_body->getRigidDynamic()->attachShape(*shape_);
	}

	CapsuleCollider::~CapsuleCollider() noexcept
	{
		shape_->release();
		rigid_->getRigidDynamic()->detachShape(*shape_);

		rigid_ = nullptr;
	}
}