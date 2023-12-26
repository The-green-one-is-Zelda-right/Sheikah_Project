#pragma once
#include "ZnRigidBody.h"
#include <Eigen/Dense>


namespace ZonaiPhysics
{
	using namespace Eigen;
	
	class ZnObject;

	class ZnJoint
	{
	public:
		enum class eOBJECT
		{
			eJOINT_OBJECT0 = 0,
			eJOINT_OBJECT1 = 1,
		};

	public:
							ZnJoint() noexcept = default;
		virtual				~ZnJoint() noexcept = default;

	public:
		/**
		������Ʈ�� ����Ʈ�� ������.
		�� �� �ϳ��� NULL�� �� ����.
		*/
		//virtual void		SetObject(ZnObject*, ZnObject*) noexcept = 0;
		//virtual void		GetObject(ZnObject*&, ZnObject*&) const noexcept = 0;
		
		/**
		������Ʈ�� ������
		*/
		virtual void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept = 0;
		virtual Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept = 0;

		/**
		������Ʈ�� �����̼�
		*/
		virtual void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept = 0;
		virtual Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept = 0;

		/**
		������Ʈ0�� �������� ������Ʈ1�� ��� �ӵ��� ��ȯ��.
		*/
		virtual Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept = 0;
		virtual Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept = 0;

		/**
		����Ʈ �ı� ���� ������
		*/
		virtual void		SetBreakForce(float _force, float _torque) noexcept = 0;
		virtual void		GetBreakForce(float& _force, float& _torque) const noexcept = 0;
	};
}