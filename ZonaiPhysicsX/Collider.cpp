#include "RigidBody.h"
#include "PxPhysicsAPI.h"

#include "Collider.h"

#include "ZnUtil.h"

namespace ZonaiPhysics
{
	Collider::Collider(physx::PxPhysics*& _factory, RigidBody* _rigid) noexcept :
		rigidbody(_rigid), shape()
	{

	}

	void Collider::SetTrigger(bool _flag) noexcept
	{
		assert(shape != nullptr);

		using namespace physx;
		rigidbody->pxBody->detachShape(*shape);

		if (_flag)
		{
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !_flag);
		}

		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, _flag);

		rigidbody->pxBody->attachShape(*shape);
	}

	void Collider::SetLayerID(uint32_t _id) noexcept
	{
		shape->setSimulationFilterData({ _id, 0, 0, 0 });
	}

	/// <summary>
	/// �̰� ��ü�� ��ġ�� �ٲٴ°ǵ�...
	/// �̰� ������?
	/// </summary>
	Eigen::Vector3f Collider::GetPosition() const noexcept
	{
		assert(rigidbody != nullptr);

		using namespace physx;
		return rigidbody->GetPosition();
	}

	void Collider::SetPosition(const Eigen::Vector3f& _position) noexcept
	{
		assert(rigidbody != nullptr);

		using namespace physx;
		rigidbody->SetPosition(_position);
	}

	Eigen::Vector3f Collider::GetLocalPosition() const noexcept
	{
		assert(shape != nullptr);

		// using namespace physx;
		// PxVec3 v = shape->getLocalPose().p;
		// return { v.x, v.y, v.z };

		return PhysxToEigen(shape->getLocalPose().p);
	}

	void Collider::SetLocalPosition(const Eigen::Vector3f& _position) noexcept
	{
		assert(rigidbody != nullptr);

		using namespace physx;
		rigidbody->pxBody->detachShape(*shape);

		PxTransform t = shape->getLocalPose();
		// t.p.x = _position.x();
		// t.p.y = _position.y();
		// t.p.z = _position.z();
		t.p = EigenToPhysx(_position);
		shape->setLocalPose(t);

		rigidbody->pxBody->attachShape(*shape);

		UpdateInertiaTensor();
	}

	Eigen::Quaternionf Collider::GetQuaternion() const noexcept
	{
		using namespace physx;
		return rigidbody->GetQuaternion();
	}

	void Collider::SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		using namespace physx;
		rigidbody->SetQuaternion(_quaternion);
	}

	Eigen::Quaternionf Collider::GetLocalQuaternion() const noexcept
	{
		assert(shape != nullptr);

		// using namespace physx;
		// PxQuat q = shape->getLocalPose().q;
		// return { q.w, q.x, q.y, q.z};

		return PhysxToEigen(shape->getLocalPose().q);
	}

	void Collider::SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		assert(shape != nullptr);

		using namespace physx;
		rigidbody->pxBody->detachShape(*shape);

		PxTransform t = shape->getLocalPose();
		// t.q.w = _quaternion.w();
		// t.q.x = _quaternion.x();
		// t.q.y = _quaternion.y();
		// t.q.z = _quaternion.z();
		t.q = EigenToPhysx(_quaternion);
		shape->setLocalPose(t);

		rigidbody->pxBody->attachShape(*shape);

		UpdateInertiaTensor();
	}

	void* Collider::GetUserData() const noexcept
	{
		return  userData;
	}

	void Collider::SetUserData(void* _userData) noexcept
	{
		userData = _userData;
	}

	void Collider::UpdateInertiaTensor() const noexcept
	{
		assert(shape != nullptr && rigidbody != nullptr);

		// auto pos = shape->getLocalPose().p;
		rigidbody->pxBody->setCMassLocalPose(shape->getLocalPose());
	}
} // namespace ZonaiPhysics