#pragma once
/// ZonaiPhysics�� RigidBody�� �������ش�.
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
		void		Awake() override;

	public:
		void		SetPosition(const Eigen::Vector3f& _pos) noexcept;
		const Eigen::Vector3f& GetPosition() noexcept;

	public:
		/**
		���� ������ ��ü�� ����
		*/
		void		WakeUp() noexcept;

		/**
		���� ���θ� ��ȯ��
		*/
		bool		IsSleeping() const noexcept;

		/**
		��ü�� �������� �����ϴ� �÷���
		*/
		uint8_t GetDynamicLockFlags() const noexcept;
		void		SetDynamicLockFlag(ZonaiPhysics::FreezeFlag flag, bool) noexcept;
		void		SetDynamicLockFlags(uint8_t flags) noexcept;

	public:
		/**
		����

		��ü�� ����.
		0�̸� ������ �������� �����.
		*/
		float		GetMass() const noexcept;
		void		SetMass(float) noexcept;

		/**
		���� ���� ���

		�����̳� �������� ��ҿ� ���� ��ü�� ������ ������.
		*/
		float		GetLinearDamping() const noexcept;
		void		SetLinearDamping(float _damping) noexcept;

		/**
		ȸ�� ���� ���

		�����̳� �������� ��ҿ� ���� ��ü�� ������ ������
		*/
		float		GetAngularDamping() const noexcept;
		void		SetAngularDamping(float _damping) noexcept;

		/**
		���ӵ�
		*/
		Eigen::Vector3f	GetLinearVelocity() const noexcept;
		void		SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept;

		/**
		���ӵ�
		*/
		Eigen::Vector3f	GetAngularVelocity() const noexcept;
		void		SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept;

		/**
		���ӵ� ����
		*/
		float		GetMaxLinearVelocity() const noexcept;
		void		SetMaxLinearVelocity(const float&) noexcept;

		/**
		���ӵ� ����
		*/
		float		GetMaxAngularVelocity() const noexcept;
		void		SetMaxAngularVelocity(const float&) noexcept;

		/**
		��ü�� ���� ���ϰų� ����
		*/
		void		AddForce(const Eigen::Vector3f& _force, ZonaiPhysics::ForceType _type = ZonaiPhysics::ForceType::Force) noexcept;
		void		ClearForce() noexcept;

		/**
		��ü�� ��ũ�� ���ϰų� ����
		*/
		void		AddTorque(const Eigen::Vector3f& _torque, ZonaiPhysics::ForceType _type = ZonaiPhysics::ForceType::Force) noexcept;
		void		ClearTorque() noexcept;

		void		SetKinematic(bool) noexcept;

		void		UseGravity(bool);

	public:
		void SimulateResult();

	private:
		bool awake{ true };
		bool lock[3]{ false, false, false };
		bool isKinematic{ false };
		bool useGravity{ false };

		// float maxLinearVelocity{ 0.f };
		// float maxAngularVelocity{ 0.f };
		// Eigen::Vector3f position{ 0.f, 0.f, 0.f };
		// Eigen::Quaternionf rotation{ Eigen::Quaternionf::Identity() };
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
