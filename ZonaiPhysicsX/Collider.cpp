#include "RigidBody.h"
#include "PxPhysicsAPI.h"

#include "Collider.h"

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
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, _flag);
	}

	/// <summary>
	/// 이건 강체의 위치를 바꾸는건데...
	/// 이게 맞을까?
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

		using namespace physx;
		PxVec3 v = shape->getLocalPose().p;
		return { v.x, v.y, v.z };
	}

	void Collider::SetLocalPosition(const Eigen::Vector3f& _position) noexcept
	{
		assert(rigidbody != nullptr);

		using namespace physx;
		PxTransform t = shape->getLocalPose();
		t.p.x = _position.x();
		t.p.y = _position.y();
		t.p.z = _position.z();
		rigidbody->getRigidDynamic()->detachShape(*shape);
		shape->setLocalPose(t);
		rigidbody->getRigidDynamic()->attachShape(*shape);
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

		using namespace physx;
		PxQuat q = shape->getLocalPose().q;
		return { q.w, q.x, q.y, q.z};
	}

	void Collider::SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		assert(shape != nullptr);

		using namespace physx;
		PxTransform t = shape->getLocalPose();
		t.q.w = _quaternion.w();
		t.q.x = _quaternion.x();
		t.q.y = _quaternion.y();
		t.q.z = _quaternion.z();
		rigidbody->getRigidDynamic()->detachShape(*shape);
		shape->setLocalPose(t);
		rigidbody->getRigidDynamic()->attachShape(*shape);
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

		auto pos = shape->getLocalPose().p;
		rigidbody->getRigidDynamic()->setCMassLocalPose(shape->getLocalPose());
	}
} // namespace ZonaiPhysics