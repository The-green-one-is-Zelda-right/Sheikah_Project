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
		void		Initialize() override;
		void		PostInitialize() override;
		
		void		Awake() override;

		void		OnEnable() override;
		void		OnDisable() override;

	public:
		void		SetPosition(const Eigen::Vector3f& _pos) noexcept;
		Eigen::Vector3f GetPosition() noexcept;

		void		SetRotation(const Eigen::Quaternionf& _rot) noexcept;
		Eigen::Quaternionf GetRotation() noexcept;

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
		uint8_t		GetDynamicLockFlags() const noexcept;
		void		SetDynamicLockFlag(ZonaiPhysics::FreezeFlag flag, bool) noexcept;
		void		SetDynamicLockFlags(uint8_t flags) noexcept;

	public:
		/**
		질량

		물체의 질량.
		0이면 무한한 질량으로 취급함.
		*/
		float		GetMass() const noexcept;
		void		SetMass(float) noexcept;
		void		SetDensity(float);


		Eigen::Vector3f	GetInertiaTensor() const;
		void			SetInertiaTensor(const Eigen::Vector3f& tensor);

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
		// float		GetMaxLinearVelocity() const noexcept;
		void		SetMaxLinearVelocity(const float&) noexcept;

		/**
		각속도 제한
		*/
		// float		GetMaxAngularVelocity() const noexcept;
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
		bool		IsKinematic() const;

		void		UseGravity(bool);
		bool		HasGravity() const;

		/**
		오브젝트의 바운딩 박스를 계산함

		매개변수를 입력하지 않으면
		오브젝트의 위치와 회전을 기반으로 바운딩 박스를 계산해줌

		매개 변수는 계산될 기저를 결정하는 위치와 회전
		*/
		ZonaiPhysics::ZnBound3 GetBoundingBox(
			const Eigen::Vector3f& _pos = Eigen::Vector3f::Zero(), 
			const Eigen::Quaternionf& _rot = Eigen::Quaternionf::Identity()
		);

		ZonaiPhysics::ZnBound3 ComputeBoundingBoxAtTransform(
			const Eigen::Vector3f& _pos = Eigen::Vector3f::Zero(),
			const Eigen::Quaternionf& _rot = Eigen::Quaternionf::Identity()
		);

	public:
		void SimulateResult();

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		bool awake{ true };
		bool isKinematic{ false };
		bool useGravity{ true };

		// float maxLinearVelocity{ 0.f };
		// float maxAngularVelocity{ 0.f };
		// Eigen::Vector3f position{ 0.f, 0.f, 0.f };
		// Eigen::Quaternionf rotation{ Eigen::Quaternionf::Identity() };
		uint8_t freeze{ 0 };
		Eigen::Vector3f linearVelocity{ 0.f, 0.f, 0.f };
		Eigen::Vector3f angularVelocity{ 0.f, 0.f, 0.f };
		bool useAutoMass{ false };
		float mass{ 1.f };
		float density{};
		float linearDamping{ 0.1f };
		float angularDamping{ 0.1f };
		Eigen::Vector3f force{ 0.f, 0.f, 0.f };
		Eigen::Vector3f torque{ 0.f, 0.f, 0.f };

	private:
		ZonaiPhysics::ZnRigidBody* body{};

	private:
		friend class Transform;

		friend class DistanceJoint;
		friend class SpringJoint;
		friend class HingeJoint;
		friend class SlideJoint;
		friend class BallJoint;
		friend class FixedJoint;
	};
}
