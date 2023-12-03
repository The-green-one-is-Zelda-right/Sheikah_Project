#include "Collider.h"

#include <windows.h>

#include "RigidBody.h"
#include "ZonaiMath.h"
#include "PxPhysicsAPI.h"

namespace ZonaiPhysics
{
	Collider::Collider(physx::PxPhysics*& _factory, RigidBody* _rigid) noexcept : rigid_(_rigid), shape_()
	{

	}

	Collider::~Collider() noexcept
	{
		rigid_->getRigidDynamic()->detachShape(*shape_);
		// rigid_->detachShape(*shape_);
		shape_->release();
	}

	/// <summary>
	/// �̰� ��ü�� ��ġ�� �ٲٴ°ǵ�...
	/// �̰� ������?
	/// </summary>
	Vector3D Collider::GetPosition() const noexcept
	{
		using namespace physx;
		// PxVec3 v = rigid_->getGlobalPose().p;
		// return { v.x, v.y, v.z };
		return rigid_->GetPosition();
	}

	void Collider::SetPosition(const Vector3D& _position) noexcept
	{
		using namespace physx;
		// PxTransform t = rigid_->getGlobalPose();
		// t.p.x = _position.x;
		// t.p.y = _position.y;
		// t.p.z = _position.z;
		// rigid_->setGlobalPose(t);
		rigid_->SetPosition(_position);
	}

	Vector3D Collider::GetLocalPosition() const noexcept
	{
		using namespace physx;
		PxVec3 v = shape_->getLocalPose().p;
		return { v.x, v.y, v.z };
	}

	void Collider::SetLocalPosition(const Vector3D& _position) noexcept
	{
		using namespace physx;
		PxTransform t = shape_->getLocalPose();
		t.p.x = _position.x;
		t.p.y = _position.y;
		t.p.z = _position.z;
		shape_->setLocalPose(t);
	}

	Quaternion Collider::GetQuaternion() const noexcept
	{
		using namespace physx;
		// PxQuat q = rigid_->getGlobalPose().q;
		// return { q.w, q.x, q.y, q.z };
		return rigid_->GetQuaternion();
	}

	void Collider::SetQuaternion(const Quaternion& _quaternion) noexcept
	{
		using namespace physx;
		// PxTransform t = rigid_->getGlobalPose();
		// t.q.w = _quaternion.w;
		// t.q.x = _quaternion.x;
		// t.q.y = _quaternion.y;
		// t.q.z = _quaternion.z;
		// rigid_->setGlobalPose(t);
		rigid_->SetQuaternion(_quaternion);
	}

	Quaternion Collider::GetLocalQuaternion() const noexcept
	{
		using namespace physx;
		PxQuat q = shape_->getLocalPose().q;
		return { q.w, q.x, q.y, q.z };
	}

	void Collider::SetLocalQuaternion(const Quaternion& _quaternion) noexcept
	{
		using namespace physx;
		PxTransform t = shape_->getLocalPose();
		t.q.w = _quaternion.w;
		t.q.x = _quaternion.x;
		t.q.y = _quaternion.y;
		t.q.z = _quaternion.z;
		shape_->setLocalPose(t);
	}

	void Collider::SetTrigger(bool) noexcept
	{
		using namespace physx;
		// shape_->setFlags(PxShapeFlag::eTRIGGER_SHAPE)
	}

	void* Collider::GetUserData() const noexcept
	{
		return shape_->userData;
	}

	void Collider::SetUserData(void* _userData) noexcept
	{
		shape_->userData = _userData;
	}
} // namespace ZonaiPhysics