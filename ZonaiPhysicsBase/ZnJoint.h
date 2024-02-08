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
		오브젝트의 포지션
		*/
		virtual void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) = 0;
		virtual Eigen::Vector3f	GetLocalPosition(eOBJECT) const = 0;

		/**
		오브젝트의 로테이션
		*/
		virtual void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) = 0;
		virtual Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const = 0;

		/**
		오브젝트0을 기준으로 오브젝트1의 상대 속도를 반환함.
		*/
		virtual Eigen::Vector3f	GetRelativeLinearVelocity() const = 0;
		virtual Eigen::Vector3f	GetRelativeAngularVelocity() const = 0;

		/**
		조인트 파괴 힘을 설정함
		*/
		virtual void		SetBreakForce(float _force, float _torque) = 0;
		virtual void		GetBreakForce(float& _force, float& _torque) const = 0;
	};
}