#include <Eigen/Dense>
#include "RigidBody.h"
#include "PxPhysicsAPI.h"

#include "BoxCollider.h"

namespace ZonaiPhysics
{
	BoxCollider::BoxCollider(
		physx::PxPhysics*& _factory, 
		RigidBody*& _body, 
		const Eigen::Vector3f& _offset, 
		physx::PxMaterial*& _material
	) noexcept : Collider(_factory, _body)
	{
		using namespace physx;
		rigid_ = _body;

		shape_ = _factory->createShape(PxBoxGeometry(_offset.x(), _offset.y(), _offset.z()), *_material);
		shape_->userData = this;

		shape_->setFlag(PxShapeFlag::eVISUALIZATION, true);
		shape_->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

		_body->getRigidDynamic()->attachShape(*shape_);
	}

	BoxCollider::~BoxCollider() noexcept
	{
		shape_->release();
		rigid_->getRigidDynamic()->detachShape(*shape_);
		
		rigid_ = nullptr;
	}
}
