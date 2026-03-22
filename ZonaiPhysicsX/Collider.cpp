#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#pragma warning (pop)

#include "RigidBody.h"
#include "ZnLayer.h"

#include "ZnUtil.h"
#include "ZnBound3.h"
#include "ColliderHelper.h"
#include "RigidBodyHelper.h"

#include "Collider.h"

namespace ZonaiPhysics
{
	Collider::Collider(physx::PxShape* _pxShape, RigidBody* _znBody) :
		znBody(_znBody), pxShape(_pxShape)
	{
		ColliderHelper::SetLayer(pxShape, 0);
		pxShape->userData = this;
	}

	Collider::~Collider()
	{
		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::Release(&pxShape);
		znBody = nullptr;
	}

	void Collider::Disable(bool _value)
	{
		znBody->Disable(_value);
	}

	void Collider::DisableQuery(bool _flag)
	{
		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::DisableQuery(pxShape, _flag);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);
	}

	void Collider::DisableSimulation(bool _flag)
	{
		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::DisableSimulation(pxShape, _flag);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);
	}

	void Collider::SetTrigger(bool _flag)
	{
		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::SetTrigger(pxShape, _flag);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);
	}

	void Collider::SetLayerData(const uint32_t& _id)
	{
		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::SetLayer(pxShape, _id);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);
	}

	/// <summary>
	/// 이건 강체의 위치를 바꾸는건데...
	/// 이게 맞을까?
	/// </summary>
	Eigen::Vector3f Collider::GetPosition() const
	{
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		return znBody->GetPosition();
	}

	void Collider::SetPosition(const Eigen::Vector3f& _position, bool _wakeup)
	{
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		znBody->SetPosition(_position, _wakeup);
	}

	Eigen::Vector3f Collider::GetLocalPosition() const
	{
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		return ColliderHelper::GetLocalPosition(pxShape);
	}

	void Collider::SetLocalPosition(const Eigen::Vector3f& _position)
	{
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::SetLocalPosition(pxShape, _position);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		UpdateInertiaTensor();
	}

	Eigen::Quaternionf Collider::GetQuaternion() const
	{
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		return znBody->GetQuaternion();
	}

	void Collider::SetQuaternion(const Eigen::Quaternionf& _quaternion, bool _wakeup)
	{
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		znBody->SetQuaternion(_quaternion, _wakeup);
	}

	Eigen::Quaternionf Collider::GetLocalQuaternion() const
	{
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		return ColliderHelper::GetLocalQuaternion(pxShape);
	}

	void Collider::SetLocalQuaternion(const Eigen::Quaternionf& _quaternion)
	{
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::SetLocalQuaternion(pxShape, _quaternion);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		UpdateInertiaTensor();
	}

	void* Collider::GetUserData() const
	{
		return  userData;
	}

	void Collider::SetUserData(void* _userData)
	{
		userData = _userData;
	}

	ZnBound3 Collider::GetBoundingBox(const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot)
	{
		return ColliderHelper::GetBoundingBox(pxShape, _pos, _rot);
	}

	void Collider::SetMaterial(const ZnMaterialID& _id)
	{
		ColliderHelper::SetMaterial(pxShape, _id);
	}

	void Collider::SetMaterials(ZnMaterialID* _pointer, int size)
	{
		ColliderHelper::SetMaterials(pxShape, _pointer, size);
	}

	void Collider::UpdateInertiaTensor() const
	{
		NULL_POINTER_REFERENCE(pxShape, ZnCollider)
		NULL_POINTER_REFERENCE(znBody, ZnCollider)

		// auto pos = pxShape->getLocalPose().p;
		// znBody->pxBody->setCMassLocalPose(pxShape->getLocalPose());
	}
} // namespace ZonaiPhysics