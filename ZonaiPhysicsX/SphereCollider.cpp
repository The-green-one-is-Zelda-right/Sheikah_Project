
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
		rigid_ = _body;

		shape_ = _factory->createShape(PxSphereGeometry(_offset), *_material);
		shape_->userData = this;

		shape_->setFlag(PxShapeFlag::eVISUALIZATION, true);
		shape_->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

		_body->getRigidDynamic()->attachShape(*shape_);
	}

	SphereCollider::~SphereCollider() noexcept
	{
		shape_->release();
		rigid_->getRigidDynamic()->detachShape(*shape_);

		rigid_ = nullptr;
	}
}
