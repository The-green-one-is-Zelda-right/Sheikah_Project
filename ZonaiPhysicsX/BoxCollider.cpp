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
		rigidbody = _body;

		shape = _factory->createShape(PxBoxGeometry(_offset.x(), _offset.y(), _offset.z()), *_material);
		shape->userData = this;

		shape->setFlag(PxShapeFlag::eVISUALIZATION, true);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

		_body->getRigidDynamic()->attachShape(*shape);
	}

	BoxCollider::~BoxCollider() noexcept
	{
		shape->release();
		rigidbody->getRigidDynamic()->detachShape(*shape);
		
		rigidbody = nullptr;
	}
}
