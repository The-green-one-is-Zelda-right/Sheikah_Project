#include "RigidBody.h"
#include "PxPhysicsAPI.h"
#include "ZnLayer.h"

#include "ZnUtil.h"
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
		ColliderHelper::Release(pxShape);
		pxShape = nullptr;
		znBody = nullptr;
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
		assert(znBody != nullptr);

		return znBody->GetPosition();
	}

	void Collider::SetPosition(const Eigen::Vector3f& _position)
	{
		assert(znBody != nullptr);

		znBody->SetPosition(_position);
	}

	Eigen::Vector3f Collider::GetLocalPosition() const
	{
		assert(pxShape != nullptr);

		return ColliderHelper::GetLocalPosition(pxShape);
	}

	void Collider::SetLocalPosition(const Eigen::Vector3f& _position)
	{
		assert(pxShape != nullptr);

		RigidBodyHelper::Detach(znBody->pxBody, pxShape);
		ColliderHelper::SetLocalPosition(pxShape, _position);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		UpdateInertiaTensor();
	}

	Eigen::Quaternionf Collider::GetQuaternion() const
	{
		return znBody->GetQuaternion();
	}

	void Collider::SetQuaternion(const Eigen::Quaternionf& _quaternion)
	{
		assert(znBody != nullptr);

		znBody->SetQuaternion(_quaternion);
	}

	Eigen::Quaternionf Collider::GetLocalQuaternion() const
	{
		return ColliderHelper::GetLocalQuaternion(pxShape);
	}

	void Collider::SetLocalQuaternion(const Eigen::Quaternionf& _quaternion)
	{
		assert(pxShape != nullptr);

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

	void Collider::UpdateInertiaTensor() const
	{
		assert(pxShape != nullptr && znBody != nullptr);

		// auto pos = pxShape->getLocalPose().p;
		// znBody->pxBody->setCMassLocalPose(pxShape->getLocalPose());
	}
} // namespace ZonaiPhysics