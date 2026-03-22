#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#pragma warning (pop)

#include "RigidBody.h"
#include "ColliderHelper.h"
#include "RigidBodyHelper.h"

#include "ZnUtil.h"
#include "CapsuleCollider.h"

namespace ZonaiPhysics
{
	CapsuleCollider::CapsuleCollider(physx::PxShape* _pxShape, RigidBody* _znBody) :
		Collider(_pxShape, _znBody)
	{

	}

	CapsuleCollider::~CapsuleCollider()
	{

	}

	Eigen::Quaternionf CapsuleCollider::GetLocalQuaternion() const
	{
		using namespace Eigen;

		const physx::PxQuat rotation(physx::PxPi / 2.f, physx::PxVec3(0.f, 0.f, 1.f));
		const Quaternionf rot = PhysxToEigen(rotation);

		const Quaternion curr = ColliderHelper::GetLocalQuaternion(pxShape);
		const Quaternionf rotInverse = rot.inverse();

		return rotInverse * curr;
	}

	void CapsuleCollider::SetLocalQuaternion(const Eigen::Quaternionf& _quaternion)
	{
		const physx::PxQuat rotation(physx::PxPi / 2.f, physx::PxVec3(0.f, 0.f, 1.f));

		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::SetLocalQuaternion(pxShape, PhysxToEigen(rotation) * _quaternion);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		UpdateInertiaTensor();
	}

}
