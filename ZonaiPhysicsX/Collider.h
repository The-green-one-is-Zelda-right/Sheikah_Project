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
							Collider(physx::PxShape*, RigidBody*) noexcept;
							//Collider(physx::PxPhysics*&, RigidBody*) noexcept;
		virtual				~Collider() noexcept = default;

	public:
		void				SetTrigger(bool) noexcept final;
		void				SetLayerData(const uint32_t&) noexcept final;

		/**
		��ġ
		*/
		Eigen::Vector3f		GetPosition() const noexcept final;
		void				SetPosition(const Eigen::Vector3f& _position) noexcept final;
		virtual  Eigen::Vector3f	GetLocalPosition() const noexcept final;
		virtual	 void		SetLocalPosition(const Eigen::Vector3f& _position) noexcept final;

		/**
		ȸ��
		*/
		Eigen::Quaternionf	GetQuaternion() const noexcept final;
		void				SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept final;
		virtual Eigen::Quaternionf	GetLocalQuaternion() const noexcept final;
		virtual void		SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) noexcept final;

		/**
		���� ������
		*/
		void*				GetUserData() const noexcept final;
		void				SetUserData(void* _userData) noexcept final;

	public:

		void				UpdateInertiaTensor() const noexcept;

	protected:
		RigidBody* rigidbody;
		physx::PxShape* shape;
	};
} // namespace ZonaiPhysics

