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
		���� ������ ��ü�� ����
		*/
		virtual void		WakeUp() noexcept = 0;
		
		/**
		���� ���θ� ��ȯ��
		*/
		virtual bool		IsSleeping() const noexcept = 0;

		/**
		��ü�� �������� �����ϴ� �÷���
		*/
		virtual DynamicLocks GetDynamicLockFlags() const noexcept = 0;
		virtual void		SetDynamicLockFlag(DynamicLock flag, bool) noexcept = 0;
		virtual void		SetDynamicLockFlags(DynamicLocks flags) noexcept = 0;

	public:
		/**
		����

		��ü�� ����.
		0�̸� ������ �������� �����.
		*/
		virtual float		GetMass() const noexcept = 0;
		virtual void		SetMass(float) noexcept = 0;
		virtual float		GetInvMass() const noexcept = 0;

		virtual Eigen::Vector3f	GetInertiaTensor() const noexcept = 0;
		virtual void		SetInertiaTensor(const Eigen::Vector3f&) noexcept = 0;

		/**
		���� ���� ���

		�����̳� �������� ��ҿ� ���� ��ü�� ������ ������.
		*/
		virtual float		GetLinearDamping() const noexcept = 0;
		virtual void		SetLinearDamping(float _damping) noexcept = 0;

		/**
		ȸ�� ���� ���

		�����̳� �������� ��ҿ� ���� ��ü�� ������ ������
		*/
		virtual float		GetAngularDamping() const noexcept = 0;
		virtual void		SetAngularDamping(float _damping) noexcept = 0;
		
		/**
		���ӵ�
		*/
		virtual Eigen::Vector3f	GetLinearVelocity() const noexcept = 0;
		virtual void		SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept = 0;
		
		/**
		���ӵ�
		*/
		virtual Eigen::Vector3f	GetAngularVelocity() const noexcept = 0;
		virtual void		SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept = 0;
		
		/**
		���ӵ� ����
		*/
		virtual float		GetMaxLinearVelocity() const noexcept = 0;
		virtual void		SetMaxLinearVelocity(const float&) noexcept = 0;

		/**
		���ӵ� ����
		*/
		virtual float		GetMaxAngularVelocity() const noexcept = 0;
		virtual void		SetMaxAngularVelocity(const float&) noexcept = 0;

		virtual void		SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque, ForceType _type) noexcept = 0;

		/**
		��ü�� ���� ���ϰų� ����
		*/
		virtual void		AddForce(const Eigen::Vector3f& _force, ForceType _type = ForceType::Force) noexcept = 0;
		virtual void		ClearForce() noexcept = 0;

		/**
		��ü�� ��ũ�� ���ϰų� ����
		*/
		virtual void		AddTorque(const Eigen::Vector3f& _torque, ForceType _type = ForceType::Force) noexcept = 0;
		virtual void		ClearTorque() noexcept = 0;

		virtual void		SetKinematic(bool) noexcept = 0;
	};
}