#pragma once
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
}

namespace ZonaiPhysics
{
	class ZnJoint;
	class ZnTransform;
	class RigidBody;

	template <typename Base, typename PhysxJoint>
	concept Joint = std::is_base_of_v<ZnJoint, Base>;

	template <Joint Base>
	class TemplateJoint : public Base
	{
	public:
		TemplateJoint() noexcept = delete;
		~TemplateJoint() noexcept override;

	protected:
		PhysxJoint* specificJoint;
		RigidBody* rigidbody[2];

	public:
		/**
		������Ʈ�� ������
		*/
		void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept override;
		Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept override;

		/**
		������Ʈ�� �����̼�
		*/
		void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept override;
		Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept override;

		/**
		������Ʈ0�� �������� ������Ʈ1�� ��� �ӵ��� ��ȯ��.
		*/
		Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept override;
		Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept override;

		/**
		����Ʈ �ı� ���� ������
		*/
		void		SetBreakForce(float _force, float _torque) noexcept override;
		void		GetBreakForce(float& _force, float& _torque) const noexcept override;
	};
}
