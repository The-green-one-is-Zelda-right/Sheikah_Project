#pragma once
#include <Eigen/Dense>
#include "ZnBase.h"


namespace ZonaiPhysics
{
	using namespace Eigen;
	
	class ZnObject;

	class ZnJoint : public ZnBase
	{
	public:
		enum eOBJECT
		{
			eJOINT_OBJECT0 = 0,
			eJOINT_OBJECT1 = 1,
		};

	public:
							ZnJoint() = default;
		virtual				~ZnJoint() = default;

	public:
		/**
		������Ʈ�� ������
		*/
		virtual void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) = 0;
		virtual Eigen::Vector3f	GetLocalPosition(eOBJECT) const = 0;

		/**
		������Ʈ�� �����̼�
		*/
		virtual void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) = 0;
		virtual Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const = 0;

		/**
		������Ʈ0�� �������� ������Ʈ1�� ��� �ӵ��� ��ȯ��.
		*/
		virtual Eigen::Vector3f	GetRelativeLinearVelocity() const = 0;
		virtual Eigen::Vector3f	GetRelativeAngularVelocity() const = 0;

		/**
		����Ʈ �ı� ���� ������
		*/
		virtual void		SetBreakForce(float _force, float _torque) = 0;
		virtual void		GetBreakForce(float& _force, float& _torque) const = 0;

		/**
		����� ��ü���� �浹���� ����
		*/
		virtual void		EnableCollision(bool) = 0;

		virtual void		EnablePreprocessing(bool) = 0;

		virtual void		SetUserData(void* _userData) = 0;
	};
}