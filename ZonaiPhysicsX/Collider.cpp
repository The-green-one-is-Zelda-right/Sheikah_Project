#include "RigidBody.h"
#include "PxPhysicsAPI.h"

#include "Collider.h"

namespace ZonaiPhysics
{
	Collider::Collider(physx::PxPhysics*& _factory, RigidBody* _rigid) noexcept : rigid_(_rigid), shape_()
	{

	}

	void Collider::SetTrigger(bool _flag) noexcept
	{
		using namespace physx;
		shape_->setFlag(PxShapeFlag::eTRIGGER_SHAPE, _flag);
	}

	/// <summary>
	/// 이건 강체의 위치를 바꾸는건데...
	/// 이게 맞을까?
	/// </summary>
	Eigen::Vector3f Collider::GetPosition() const noexcept
	{
		using namespace physx;
		return rigid_->GetPosition();
	}

	void Collider::SetPosition(const Eigen::Vector3f& _position) noexcept
	{
		using namespace physx;
		rigid_->SetPosition(_position);
	}

	Eigen::Vector3f Collider::GetLocalPosition() const noexcept
	{
		using namespace physx;
		PxVec3 v = shape_->getLocalPose().p;
		return { v.x, v.y, v.z };
	}

	void Collider::SetLocalPosition(const Eigen::Vector3f& _position) noexcept
	{
		using namespace physx;
		PxTransform t = shape_->getLocalPose();
		t.p.x = _position.x();
		t.p.y = _position.y();
		t.p.z = _position.z();
		rigid_->getRigidDynamic()->detachShape(*shape_);
		shape_->setLocalPose(t);
		rigid_->getRigidDynamic()->attachShape(*shape_);
		UpdateInertiaTensor();
	}

	Eigen::Quaternionf Collider::GetQuaternion() const noexcept
	{
		using namespace physx;
		return rigid_->GetQuaternion();
	}

	void Collider::SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		using namespace physx;
		rigid_->SetQuaternion(_quaternion);
	}

	Eigen::Quaternionf Collider::GetLocalQuaternion() const noexcept
	{
		using namespace physx;
		PxQuat q = shape_->getLocalPose().q;
		return { q.w, q.x, q.y, q.z};
	}

	void Collider::SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		using namespace physx;
		PxTransform t = shape_->getLocalPose();
		t.q.w = _quaternion.w();
		t.q.x = _quaternion.x();
		t.q.y = _quaternion.y();
		t.q.z = _quaternion.z();
		shape_->setLocalPose(t);
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
		auto pos = shape_->getLocalPose().p;
		rigid_->getRigidDynamic()->setCMassLocalPose(shape_->getLocalPose());
	}
} // namespace ZonaiPhysics