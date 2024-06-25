#pragma once
#include "ForceType.h"
#include "FreezeFlag.h"
#include "ZnObject.h"

namespace ZonaiPhysics
{
	using namespace Eigen;

	struct ZnBound3;

	class ZnRigidBody : public ZnObject
	{

	public:
								ZnRigidBody() = default;
		virtual					~ZnRigidBody() = default;

	public:
		/**
		���� ������ ��ü�� ����
		*/
		virtual void			WakeUp() = 0;
		
		/**
		���� ���θ� ��ȯ��
		*/
		virtual bool			IsSleeping() const = 0;

		/**
		�߷��� ������ �޴°�? 
		*/
		virtual void			UseGravity(bool) = 0;
		virtual bool			HasGravity() const = 0;

		/**
		Ű�׸�ƽ ����
		*/
		virtual void			SetKinematic(bool) = 0;
		virtual bool			IsKinematic() const = 0;

		virtual void			Disable(bool) const = 0;

		/**
		��ü�� �������� �����ϴ� �÷���
		*/
		virtual uint8_t			GetDynamicLockFlags() const = 0;
		virtual void			SetDynamicLockFlag(FreezeFlag flag, bool) = 0;
		virtual void			SetDynamicLockFlags(uint8_t flags) = 0;

	public:
		/**
		����

		��ü�� ����.
		0�̸� ������ �������� �����.
		*/
		virtual float			GetMass() const = 0;
		virtual void			SetMass(float) = 0;
		virtual void			SetDensity(float) = 0;

		virtual float			GetInvMass() const = 0;

		virtual Eigen::Vector3f	GetInertiaTensor() const = 0;

		/**
		���� ���� ���

		�����̳� �������� ��ҿ� ���� ��ü�� ������ ������.
		*/
		virtual float			GetLinearDamping() const = 0;
		virtual void			SetLinearDamping(float _damping) = 0;

		/**
		ȸ�� ���� ���

		�����̳� �������� ��ҿ� ���� ��ü�� ������ ������
		*/
		virtual float			GetAngularDamping() const = 0;
		virtual void			SetAngularDamping(float _damping) = 0;
		
		/**
		���ӵ�
		*/
		virtual Eigen::Vector3f	GetLinearVelocity() const = 0;
		virtual void			SetLinearVelocity(const Eigen::Vector3f& _velocity) = 0;
		
		/**
		���ӵ�
		*/
		virtual Eigen::Vector3f	GetAngularVelocity() const = 0;
		virtual void			SetAngularVelocity(const Eigen::Vector3f& _velocity) = 0;
		
		/**
		���ӵ� ����
		*/
		virtual float			GetMaxLinearVelocity() const = 0;
		virtual void			SetMaxLinearVelocity(const float&) = 0;

		/**
		���ӵ� ����
		*/
		virtual float			GetMaxAngularVelocity() const = 0;
		virtual void			SetMaxAngularVelocity(const float&) = 0;

		/**
		��ü�� ���� ���ϰų� ����
		*/
		virtual void			AddForce(const Eigen::Vector3f& _force, ForceType _type = ForceType::Force) = 0;
		virtual void			ClearForce() = 0;

		/**
		��ü�� ��ũ�� ���ϰų� ����
		*/
		virtual void			AddTorque(const Eigen::Vector3f& _torque, ForceType _type = ForceType::Force) = 0;
		virtual void			ClearTorque() = 0;

		// �־��� �࿡
		virtual ZnBound3		GetBoundingBox(const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot) = 0;
	};
}