#pragma once
#include "ForceType.h"
#include "ZnObject.h"

namespace ZonaiPhysics
{
	using DynamicLocks = unsigned char;
	
	enum class DynamicLock : DynamicLocks
	{
		LOCK_LINEAR_X = (1 << 0),
		LOCK_LINEAR_Y = (1 << 1),
		LOCK_LINEAR_Z = (1 << 2),
		LOCK_ANGULAR_X = (1 << 3),
		LOCK_ANGULAR_Y = (1 << 4),
		LOCK_ANGULAR_Z = (1 << 5)
	};

	using namespace Eigen;

	class ZnRigidBody : public ZnObject
	{

	public:
							ZnRigidBody() noexcept = default;
		virtual				~ZnRigidBody() noexcept = default;

	public:
		/**
		수면 상태의 강체를 깨움
		*/
		virtual void		WakeUp() noexcept = 0;
		
		/**
		수면 여부를 반환함
		*/
		virtual bool		IsSleeping() const noexcept = 0;

		/**
		중력의 영향을 받는가? 
		*/
		virtual void		UseGravity(bool) noexcept = 0;

		/**
		키네마틱 설정
		*/
		virtual void		SetKinematic(bool) noexcept = 0;

		/**
		강체의 움직임을 제한하는 플래그
		*/
		virtual DynamicLocks GetDynamicLockFlags() const noexcept = 0;
		virtual void		SetDynamicLockFlag(DynamicLock flag, bool) noexcept = 0;
		virtual void		SetDynamicLockFlags(DynamicLocks flags) noexcept = 0;

	public:
		/**
		질량

		물체의 질량.
		0이면 무한한 질량으로 취급함.
		*/
		virtual float		GetMass() const noexcept = 0;
		virtual void		SetMass(float) noexcept = 0;
		virtual float		GetInvMass() const noexcept = 0;

		virtual Eigen::Vector3f	GetInertiaTensor() const noexcept = 0;
		virtual void		SetInertiaTensor(const Eigen::Vector3f&) noexcept = 0;

		/**
		선형 감쇠 계수

		저항이나 여러가지 요소에 의해 물체가 서서히 정지함.
		*/
		virtual float		GetLinearDamping() const noexcept = 0;
		virtual void		SetLinearDamping(float _damping) noexcept = 0;

		/**
		회전 감쇠 계수

		저항이나 여러가지 요소에 의해 물체가 서서히 정지함
		*/
		virtual float		GetAngularDamping() const noexcept = 0;
		virtual void		SetAngularDamping(float _damping) noexcept = 0;
		
		/**
		선속도
		*/
		virtual Eigen::Vector3f	GetLinearVelocity() const noexcept = 0;
		virtual void		SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept = 0;
		
		/**
		각속도
		*/
		virtual Eigen::Vector3f	GetAngularVelocity() const noexcept = 0;
		virtual void		SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept = 0;
		
		/**
		선속도 제한
		*/
		virtual float		GetMaxLinearVelocity() const noexcept = 0;
		virtual void		SetMaxLinearVelocity(const float&) noexcept = 0;

		/**
		각속도 제한
		*/
		virtual float		GetMaxAngularVelocity() const noexcept = 0;
		virtual void		SetMaxAngularVelocity(const float&) noexcept = 0;

		virtual void		SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque, ForceType _type) noexcept = 0;

		/**
		물체에 힘을 가하거나 지움
		*/
		virtual void		AddForce(const Eigen::Vector3f& _force, ForceType _type = ForceType::Force) noexcept = 0;
		virtual void		ClearForce() noexcept = 0;

		/**
		물체에 토크를 가하거나 지움
		*/
		virtual void		AddTorque(const Eigen::Vector3f& _torque, ForceType _type = ForceType::Force) noexcept = 0;
		virtual void		ClearTorque() noexcept = 0;

	};
}