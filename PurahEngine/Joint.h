#pragma once
#include "Component.h"
#include <functional>

namespace PurahEngine
{
	class RigidBody;

	class Joint : public Component
	{
	public:
		Joint() = default;
		~Joint() override = default;

	public:
		virtual void				SetBreakCallback(const std::function<void()>& _callback) = 0;

	protected:
		friend class EventCallbackSystem;
		friend class PhysicsSystem;
		virtual void				BreakCallback() = 0;
		virtual void*				GetZnJoint() = 0;

	public:
		virtual void				SetRigidbody(RigidBody* _base, RigidBody* _other) noexcept = 0;
		virtual void				GetRigidbody(RigidBody*& _obj0, RigidBody*& _obj1) const noexcept = 0;

		/**
		오브젝트의 포지션
		*/
		virtual void				SetLocalPosition(int _index, const Eigen::Vector3f& _localPos) = 0;
		virtual Eigen::Vector3f		GetLocalPosition(int _index) const = 0;

		/**
		오브젝트의 로테이션
		*/
		virtual void				SetLocalQuaternion(int _index, const Eigen::Quaternionf& _localQuat) = 0;
		virtual Eigen::Quaternionf	GetLocalQuaternion(int _index) const = 0;

		/**
		오브젝트0을 기준으로 오브젝트1의 상대 속도를 반환함.
		*/
		virtual Eigen::Vector3f		GetRelativeLinearVelocity() const = 0;
		virtual Eigen::Vector3f		GetRelativeAngularVelocity() const = 0;

		/**
		조인트 파괴 힘을 설정함
		*/
		virtual void				SetBreakForce(float _force, float _torque) = 0;
		virtual void				GetBreakForce(float& _force, float& _torque) const = 0;

		/**
		연결된 강체끼리 충돌할지 여부
		*/
		virtual void		EnableCollision(bool) = 0;
		virtual void		EnablePreprocessing(bool) = 0;
	};
}