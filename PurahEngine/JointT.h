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
		void PostInitialize() override
		{
			SetBreakForce(breakForce, breakTorque);
			EnableCollision(enableCollision);
			EnablePreprocessing(enablePreprocessing);
		}

	protected:
		ZonaiJoint* joint;

	protected:
		RigidBody* body = nullptr;
		RigidBody* connectedBody = nullptr;
		Eigen::Vector3f LocalAnchor = Eigen::Vector3f::Zero();
		Eigen::Quaternionf LocalAnchorRotation = Eigen::Quaternionf::Identity();
		Eigen::Vector3f connectedLocalAnchor = Eigen::Vector3f::Zero();
		Eigen::Quaternionf connectedLocalAnchorRotation = Eigen::Quaternionf::Identity();

		float breakForce = FLT_MAX;
		float breakTorque = FLT_MAX;
		bool enableCollision = false;
		bool enablePreprocessing = true;
		std::function<void()> callback = nullptr;

	public:
		void		SetBreakCallback(const std::function<void()>& _callback) override
		{
			callback = _callback;
		}

	protected:
		void		BreakCallback() override
		{
			if (callback)
			{
				callback();
			}
		}
		void*		GetZnJoint() override
		{
			return joint;
		}
	public:
		void		SetRigidbody(RigidBody* _base, RigidBody* _other) noexcept override
		{
			body = _base;
			connectedBody = _other;
		}

		void		GetRigidbody(RigidBody*& _obj0, RigidBody*& _obj1) const noexcept
		{
			_obj0 = body;
			_obj1 = connectedBody;
		}

		void		SetAnchor(
			const Eigen::Vector3f& _localP0, const Eigen::Quaternionf& _localR0, 
			const Eigen::Vector3f& _localP1, const Eigen::Quaternionf& _localR1)
		{
			LocalAnchor = _localP0;
			LocalAnchorRotation = _localR0;
			connectedLocalAnchor = _localP1;
			connectedLocalAnchorRotation = _localR1;
		}

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

