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
		오브젝트를 조인트로 연결함.
		둘 중 하나는 NULL일 수 있음.
		*/
		//virtual void		SetObject(ZnObject*, ZnObject*) noexcept = 0;
		//virtual void		GetObject(ZnObject*&, ZnObject*&) const noexcept = 0;

		/**
		오브젝트의 포지션
		*/
		virtual void		SetLocalPosition(int _index, const Eigen::Vector3f&) noexcept = 0;
		virtual Eigen::Vector3f	GetLocalPosition(int _index) const noexcept = 0;

		/**
		오브젝트의 로테이션
		*/
		virtual void		SetLocalQuaternion(int _index, const Eigen::Quaternionf&) noexcept = 0;
		virtual Eigen::Quaternionf	GetLocalQuaternion(int _index) const noexcept = 0;

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

