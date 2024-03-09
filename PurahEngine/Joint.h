#pragma once
#include "Component.h"
#include "ZnJoint.h"

namespace PurahEngine
{
	template <typename Base>
	concept ZonaiJoint = std::is_base_of_v<ZonaiPhysics::ZnJoint, Base>;

	template <typename ZonaiJoint>
	class Joint : public Component
	{
	public:
		Joint();
		~Joint() override;

	protected:
		ZonaiJoint* joint;

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
		virtual void		SetLocalPosition(int _index, const Eigen::Vector3f& _localPos) noexcept
		{
			assert(joint != nullptr);

			joint->SetLocalPosition((ZonaiPhysics::ZnJoint::eOBJECT)_index, _localPos);
		}

		virtual Eigen::Vector3f	GetLocalPosition(int _index) const noexcept
		{
			assert(joint != nullptr);

			return joint->GetLocalPosition((ZonaiPhysics::ZnJoint::eOBJECT)_index);
		}

		/**
		������Ʈ�� �����̼�
		*/
		virtual void		SetLocalQuaternion(int _index, const Eigen::Quaternionf& _localQuat) noexcept
		{
			assert(joint != nullptr);

			joint->SetLocalQuaternion((ZonaiPhysics::ZnJoint::eOBJECT)_index, _localQuat);
		}

		virtual Eigen::Quaternionf	GetLocalQuaternion(int _index) const noexcept
		{
			assert(joint != nullptr);

			return joint->GetLocalQuaternion((ZonaiPhysics::ZnJoint::eOBJECT)_index);
		}

		/**
		������Ʈ0�� �������� ������Ʈ1�� ��� �ӵ��� ��ȯ��.
		*/
		virtual Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept
		{
			assert(joint != nullptr);

			return joint->GetRelativeLinearVelocity();
		}
		
		virtual Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept
		{
			assert(joint != nullptr);

			return joint->GetRelativeAngularVelocity();
		}

		/**
		����Ʈ �ı� ���� ������
		*/
		virtual void		SetBreakForce(float _force, float _torque) noexcept
		{
			assert(joint != nullptr);

			joint->SetBreakForce(_force, _torque);
		}

		virtual void		GetBreakForce(float& _force, float& _torque) const noexcept
		{
			assert(joint != nullptr);

			joint->GetBreakForce(_force, _torque);
		}
	};
}

