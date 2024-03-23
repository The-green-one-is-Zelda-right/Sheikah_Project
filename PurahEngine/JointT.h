#pragma once
#include "Joint.h"
#include "ZnJoint.h"

namespace PurahEngine
{
	class RigidBody;

	template <typename Base>
	concept ZonaiJoint = std::is_base_of_v<ZonaiPhysics::ZnJoint, Base>;

	template <typename ZonaiJoint>
	class JointT : public Joint
	{
	public:
		JointT() = default;
		~JointT() override = default;

	protected:
		ZonaiJoint* joint;

	protected:
		RigidBody* connectedBody;
		float breakForce = FLT_MAX;
		float breakTorque = FLT_MAX;
		bool enableCollision = false;
		bool enablePreprocessing = true;

	public:
		/**
		오브젝트를 조인트로 연결함.
		둘 중 하나는 NULL일 수 있음.
		*/
		//virtual void		SetObject(ZnObject*, ZnObject*) noexcept = 0;
		//virtual void		GetObject(ZnObject*&, ZnObject*&) const noexcept = 0;

		void		SetLocalPosition(int _index, const Eigen::Vector3f& _localPos) override
		{
			assert(joint != nullptr);

			joint->SetLocalPosition(static_cast<ZonaiPhysics::ZnJoint::eOBJECT>(_index), _localPos);
		}

		Eigen::Vector3f	GetLocalPosition(int _index) const override
		{
			assert(joint != nullptr);

			return joint->GetLocalPosition(static_cast<ZonaiPhysics::ZnJoint::eOBJECT>(_index));
		}

		void		SetLocalQuaternion(int _index, const Eigen::Quaternionf& _localQuat) override
		{
			assert(joint != nullptr);

			joint->SetLocalQuaternion(static_cast<ZonaiPhysics::ZnJoint::eOBJECT>(_index), _localQuat);
		}

		Eigen::Quaternionf	GetLocalQuaternion(int _index) const override
		{
			assert(joint != nullptr);

			return joint->GetLocalQuaternion(static_cast<ZonaiPhysics::ZnJoint::eOBJECT>(_index));
		}

		Eigen::Vector3f	GetRelativeLinearVelocity() const override
		{
			assert(joint != nullptr);

			return joint->GetRelativeLinearVelocity();
		}

		Eigen::Vector3f	GetRelativeAngularVelocity() const override
		{
			assert(joint != nullptr);

			return joint->GetRelativeAngularVelocity();
		}

		void		SetBreakForce(float _force, float _torque) override
		{
			assert(joint != nullptr);

			joint->SetBreakForce(_force, _torque);
		}

		void		GetBreakForce(float& _force, float& _torque) const override
		{
			assert(joint != nullptr);

			joint->GetBreakForce(_force, _torque);
		}

		void		EnableCollision(bool _value) override
		{
			assert(joint != nullptr);

			joint->EnableCollision(_value);
		}

		void		EnablePreprocessing(bool _value) override
		{
			assert(joint != nullptr);

			joint->EnablePreprocessing(_value);
		}
	};
}

