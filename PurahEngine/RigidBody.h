#pragma once
/// ZonaiPhysics의 RigidBody를 래핑해준다.
#include "PurahEngineAPI.h"
#include "Component.h"
#include <Eigen/Dense>
#include "ZnRigidBody.h"

namespace ZonaiPhysics
{
	class ZnRigidBody;
}

namespace PurahEngine
{
	class PURAHENGINE_API RigidBody : public Component
	{
	public:
		RigidBody();
		~RigidBody() override;

	public:
		void Awake() override;

	public:
		/**
		수면 상태의 강체를 깨움
		*/
		void		WakeUp() noexcept;

		/**
		수면 여부를 반환함
		*/
		bool		IsSleeping() const noexcept;

		/**
		강체의 움직임을 제한하는 플래그
		*/
		ZonaiPhysics::DynamicLocks GetDynamicLockFlags() const noexcept;
		void		SetDynamicLockFlag(ZonaiPhysics::DynamicLock flag, bool) noexcept;
		void		SetDynamicLockFlags(ZonaiPhysics::DynamicLocks flags) noexcept;

	public:
		/**
		질량

		물체의 질량.
		0이면 무한한 질량으로 취급함.
		*/
		float		GetMass() const noexcept;
		void		SetMass(float) noexcept;

		/**
		선형 감쇠 계수

		저항이나 여러가지 요소에 의해 물체가 서서히 정지함.
		*/
		float		GetLinearDamping() const noexcept;
		void		SetLinearDamping(float _damping) noexcept;

		/**
		회전 감쇠 계수

		저항이나 여러가지 요소에 의해 물체가 서서히 정지함
		*/
		float		GetAngularDamping() const noexcept;
		void		SetAngularDamping(float _damping) noexcept;

		/**
		선속도
		*/
		Eigen::Vector3f	GetLinearVelocity() const noexcept;
		void		SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept;

		/**
		각속도
		*/
		Eigen::Vector3f	GetAngularVelocity() const noexcept;
		void		SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept;

		/**
		선속도 제한
		*/
		float		GetMaxLinearVelocity() const noexcept;
		void		SetMaxLinearVelocity(const float&) noexcept;

		/**
		각속도 제한
		*/
		float		GetMaxAngularVelocity() const noexcept;
		void		SetMaxAngularVelocity(const float&) noexcept;

		/**
		물체에 힘을 가하거나 지움
		*/
		void		AddForce(const Eigen::Vector3f& _force, ZonaiPhysics::ForceType _type = ZonaiPhysics::ForceType::Force) noexcept;
		void		ClearForce() noexcept;

		/**
		물체에 토크를 가하거나 지움
		*/
		void		AddTorque(const Eigen::Vector3f& _torque, ZonaiPhysics::ForceType _type = ZonaiPhysics::ForceType::Force) noexcept;
		void		ClearTorque() noexcept;

		void		SetKinematic(bool) noexcept;

	public:
		void SimulateResult();

	private:
		bool awake{ true };
		bool lock[3]{ false, false, false };
		bool isKinematic{ false };

		// float maxLinearVelocity{ 0.f };
		// float maxAngularVelocity{ 0.f };
		Eigen::Vector3f LinearVelocity{ 0.f, 0.f, 0.f };
		Eigen::Vector3f angularVelocity{ 0.f, 0.f, 0.f };
		float mass;
		float linearDamping;
		float angularDamping;
		Eigen::Vector3f force{};
		Eigen::Vector3f torque{};
		ZonaiPhysics::ZnRigidBody* body{};
	};
}
