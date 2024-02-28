#pragma once
#include "ForceType.h"
#include "FreezeFlag.h"
#include "ZnObject.h"

namespace ZonaiPhysics
{
	using namespace Eigen;

	class ZnRigidBody : public ZnObject
	{

	public:
								ZnRigidBody() = default;
		virtual					~ZnRigidBody() = default;

	public:
		/**
		수면 상태의 강체를 깨움
		*/
		virtual void			WakeUp() = 0;
		
		/**
		수면 여부를 반환함
		*/
		virtual bool			IsSleeping() const = 0;

		/**
		중력의 영향을 받는가? 
		*/
		virtual void			UseGravity(bool) = 0;

		/**
		키네마틱 설정
		*/
		virtual void			SetKinematic(bool) = 0;

		/**
		강체의 움직임을 제한하는 플래그
		*/
		virtual uint8_t			GetDynamicLockFlags() const = 0;
		virtual void			SetDynamicLockFlag(FreezeFlag flag, bool) = 0;
		virtual void			SetDynamicLockFlags(uint8_t flags) = 0;

	public:
		/**
		질량

		물체의 질량.
		0이면 무한한 질량으로 취급함.
		*/
		virtual float			GetMass() const = 0;
		virtual void			SetMass(float) = 0;
		virtual float			GetInvMass() const = 0;

		virtual Eigen::Vector3f	GetInertiaTensor() const = 0;

		/**
		선형 감쇠 계수

		저항이나 여러가지 요소에 의해 물체가 서서히 정지함.
		*/
		virtual float			GetLinearDamping() const = 0;
		virtual void			SetLinearDamping(float _damping) = 0;

		/**
		회전 감쇠 계수

		저항이나 여러가지 요소에 의해 물체가 서서히 정지함
		*/
		virtual float			GetAngularDamping() const = 0;
		virtual void			SetAngularDamping(float _damping) = 0;
		
		/**
		선속도
		*/
		virtual Eigen::Vector3f	GetLinearVelocity() const = 0;
		virtual void			SetLinearVelocity(const Eigen::Vector3f& _velocity) = 0;
		
		/**
		각속도
		*/
		virtual Eigen::Vector3f	GetAngularVelocity() const = 0;
		virtual void			SetAngularVelocity(const Eigen::Vector3f& _velocity) = 0;
		
		/**
		선속도 제한
		*/
		virtual float			GetMaxLinearVelocity() const = 0;
		virtual void			SetMaxLinearVelocity(const float&) = 0;

		/**
		각속도 제한
		*/
		virtual float			GetMaxAngularVelocity() const = 0;
		virtual void			SetMaxAngularVelocity(const float&) = 0;

		/**
		물체에 힘을 가하거나 지움
		*/
		virtual void			AddForce(const Eigen::Vector3f& _force, ForceType _type = ForceType::Force) = 0;
		virtual void			ClearForce() = 0;

		/**
		물체에 토크를 가하거나 지움
		*/
		virtual void			AddTorque(const Eigen::Vector3f& _torque, ForceType _type = ForceType::Force) = 0;
		virtual void			ClearTorque() = 0;
	};
}