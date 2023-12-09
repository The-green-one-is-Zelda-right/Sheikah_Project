#pragma once
#include "ZnCollider.h"

namespace physx
{
	class PxPhysics;
	class PxShape;
	class PxRigidDynamic;
	class PxRigidActor;
}

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}

namespace ZonaiPhysics
{
	class RigidBody;

	class Collider : public ZnCollider
	{
	public:
		Collider() noexcept = delete;
		Collider(physx::PxPhysics*&, RigidBody*) noexcept;
		virtual				~Collider() noexcept = default;

	public:
		/**
		위치
		*/
		virtual ZonaiMath::Vector3D	GetPosition() const noexcept final;
		virtual void		SetPosition(const ZonaiMath::Vector3D& _position) noexcept final;
		virtual ZonaiMath::Vector3D	GetLocalPosition() const noexcept final;
		virtual void		SetLocalPosition(const ZonaiMath::Vector3D& _position) noexcept final;

		/**
		회전
		*/
		virtual ZonaiMath::Quaternion	GetQuaternion() const noexcept final;
		virtual void		SetQuaternion(const ZonaiMath::Quaternion& _quaternion) noexcept final;
		virtual ZonaiMath::Quaternion	GetLocalQuaternion() const noexcept final;
		virtual void		SetLocalQuaternion(const ZonaiMath::Quaternion& _quaternion) noexcept final;

		virtual void		SetTrigger(bool) noexcept final;

		/**
		유저 데이터
		*/
		virtual void*		GetUserData() const noexcept final;
		virtual void		SetUserData(void* _userData) noexcept final;

	public:
		__declspec(property(get = GetPosition, put = SetPosition))
			ZonaiMath::Vector3D position;

		__declspec(property(get = GetQuaternion, put = SetQuaternion))
			ZonaiMath::Quaternion quaternion;

		__declspec(property(get = GetLocalPosition, put = SetLocalPosition))
			ZonaiMath::Vector3D localPosition;

		__declspec(property(get = GetLocalQuaternion, put = SetLocalQuaternion))
			ZonaiMath::Quaternion localQuaternion;

	protected:
		RigidBody* rigid_;
		physx::PxShape* shape_;
	};
} // namespace ZonaiPhysics

