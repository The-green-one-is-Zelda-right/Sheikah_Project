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
		오브젝트의 포지션
		*/
		void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept override;
		Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept override;

		/**
		오브젝트의 로테이션
		*/
		void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept override;
		Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept override;

		/**
		오브젝트0을 기준으로 오브젝트1의 상대 속도를 반환함.
		*/
		Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept override;
		Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept override;

		/**
		조인트 파괴 힘을 설정함
		*/
		void		SetBreakForce(float _force, float _torque) noexcept override;
		void		GetBreakForce(float& _force, float& _torque) const noexcept override;
	};
}
