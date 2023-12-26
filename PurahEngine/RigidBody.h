#pragma once
/// ZonaiPhysics�� RigidBody�� �������ش�.
#include "Component.h"
#include <Eigen/Dense>
#include "ZnRigidBody.h"

namespace ZonaiPhysics
{
	class ZnRigidBody;
}

namespace PurahEngine
{
	class RigidBody : public Component
	{
	public:
		RigidBody();
		~RigidBody() override;

	public:
		void Awake() override;

	public:
		/**
		��ġ
		*/
		Eigen::Vector3f	GetPosition() const noexcept;
		void		SetPosition(const Eigen::Vector3f& _position) noexcept;

		/**
		ȸ��
		*/
		Eigen::Quaternionf	GetQuaternion() const noexcept;
		void		SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept;

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
		ZonaiPhysics::DynamicLocks GetDynamicLockFlags() const noexcept;
		void		SetDynamicLockFlag(ZonaiPhysics::DynamicLock flag, bool) noexcept;
		void		SetDynamicLockFlags(ZonaiPhysics::DynamicLocks flags) noexcept;

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

	private:
		ZonaiPhysics::ZnRigidBody* body;
	};
}
