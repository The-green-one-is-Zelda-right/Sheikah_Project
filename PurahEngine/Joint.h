#pragma once
#include "Component.h"
#include "ZnJoint.h"

namespace PurahEngine
{
	class Joint : public Component
	{
	public:
		Joint();
		~Joint() override;

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
		virtual void		SetLocalPosition(int _index, const Eigen::Vector3f&) noexcept = 0;
		virtual Eigen::Vector3f	GetLocalPosition(int _index) const noexcept = 0;

		/**
		������Ʈ�� �����̼�
		*/
		virtual void		SetLocalQuaternion(int _index, const Eigen::Quaternionf&) noexcept = 0;
		virtual Eigen::Quaternionf	GetLocalQuaternion(int _index) const noexcept = 0;

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

