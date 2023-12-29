#pragma once
#include <Eigen/Dense>
#include "ZnCollider.h"

namespace physx
{
	class PxPhysics;
	class PxShape;
	class PxRigidDynamic;
	class PxRigidActor;
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
		void				SetTrigger(bool) noexcept final;

		/**
		위치
		*/
		Eigen::Vector3f		GetPosition() const noexcept final;
		void				SetPosition(const Eigen::Vector3f& _position) noexcept final;
		virtual  Eigen::Vector3f	GetLocalPosition() const noexcept final;
		virtual	 void		SetLocalPosition(const Eigen::Vector3f& _position) noexcept final;

		/**
		회전
		*/
		Eigen::Quaternionf	GetQuaternion() const noexcept final;
		void				SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept final;
		virtual Eigen::Quaternionf	GetLocalQuaternion() const noexcept final;
		virtual void		SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) noexcept final;

		/**
		유저 데이터
		*/
		void*				GetUserData() const noexcept final;
		void				SetUserData(void* _userData) noexcept final;

	public:

	public:
		__declspec(property(get = GetPosition, put = SetPosition))
			Eigen::Vector3f position;

		__declspec(property(get = GetQuaternion, put = SetQuaternion))
			Eigen::Quaternionf quaternion;

		__declspec(property(get = GetLocalPosition, put = SetLocalPosition))
			Eigen::Vector3f localPosition;

		__declspec(property(get = GetLocalQuaternion, put = SetLocalQuaternion))
			Eigen::Quaternionf localQuaternion;

	protected:
		RigidBody* rigid_;
		physx::PxShape* shape_;
	};
} // namespace ZonaiPhysics

