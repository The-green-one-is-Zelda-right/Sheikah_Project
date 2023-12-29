#pragma once
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
		오브젝트의 포지션
		*/
		virtual void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept = 0;
		virtual Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept = 0;

		/**
		오브젝트의 로테이션
		*/
		virtual void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept = 0;
		virtual Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept = 0;

		/**
		오브젝트0을 기준으로 오브젝트1의 상대 속도를 반환함.
		*/
		virtual Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept = 0;
		virtual Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept = 0;

		/**
		조인트 파괴 힘을 설정함
		*/
		virtual void		SetBreakForce(float _force, float _torque) noexcept = 0;
		virtual void		GetBreakForce(float& _force, float& _torque) const noexcept = 0;
	};
}