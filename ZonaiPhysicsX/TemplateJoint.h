#pragma once
#include "ZnJoint.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

namespace physx
{
	class PxPhysics;
}

namespace ZonaiPhysics
{
	class ZnJoint;
	class ZnTransform;
	class RigidBody;

	// Base Ÿ���� ZnJoint�� ��ӹ޴°�?
	template <typename Base>
	concept isJoint = std::is_base_of_v<ZnJoint, Base>;

	// ZnJoint�� ����� �Լ��� ���ø����� �����ϴ� �ɷ� �ݺ��� �۾��� ����.
	template <isJoint Base, typename PhysxJoint>
	class TemplateJoint : public Base
	{
	public:
		TemplateJoint() : joint(), rigidbody0(), rigidbody1()
		{}

		~TemplateJoint() override = default;

	protected:
		PhysxJoint* joint;
		RigidBody* rigidbody0;
		RigidBody* rigidbody1;
		void* userData;

	public:
		void DisableConstraint(bool _value) override final
		{
			assert(joint != nullptr);

			joint->setConstraintFlag(physx::PxConstraintFlag::eDISABLE_CONSTRAINT, _value);
		}

		/**
		������Ʈ�� ������
		*/
		void SetLocalPosition(ZnJoint::eOBJECT _index, const Eigen::Vector3f& _localPos) override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto index = static_cast<PxJointActorIndex::Enum>(_index);
			PxTransform t = joint->getLocalPose(index);
			t.p = {_localPos.x(), _localPos.y(), _localPos.z()};
			joint->setLocalPose(index, t);
		}

		Eigen::Vector3f GetLocalPosition(ZnJoint::eOBJECT _index) const override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto index = static_cast<PxJointActorIndex::Enum>(_index);
			PxTransform t = joint->getLocalPose(index);
			return {t.p.x, t.p.y, t.p.z};
		}

		/**
		������Ʈ�� �����̼�
		*/
		void SetLocalQuaternion(ZnJoint::eOBJECT _index, const Quaternionf& _localQuat) override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto index = static_cast<PxJointActorIndex::Enum>(_index);
			PxTransform t = joint->getLocalPose(index);
			t.q = {_localQuat.x(), _localQuat.y(), _localQuat.z(), _localQuat.w()};
			joint->setLocalPose(index, t);
		}

		Quaternionf GetLocalQuaternion(ZnJoint::eOBJECT _index) const override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto index = static_cast<PxJointActorIndex::Enum>(_index);
			PxTransform t = joint->getLocalPose(index);
			return {t.q.x, t.q.y, t.q.z, t.q.w};
		}

		/**
		������Ʈ0�� �������� ������Ʈ1�� ��� �ӵ��� ��ȯ��.
		*/
		Eigen::Vector3f GetRelativeLinearVelocity() const override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto& velo = joint->getRelativeLinearVelocity();
			return {velo.x, velo.y, velo.z};
		}

		Eigen::Vector3f GetRelativeAngularVelocity() const override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto& velo = joint->getRelativeAngularVelocity();
			return {velo.x, velo.y, velo.z};
		}

		/**
		����Ʈ �ı� ���� ������
		*/
		void		SetBreakForce(float _force, float _torque) override
		{
			assert(joint != nullptr);

			joint->setBreakForce(_force, _torque);
		}

		void		GetBreakForce(float& _force, float& _torque) const override
		{
			assert(joint != nullptr);

			joint->getBreakForce(_force, _torque);
		}

		void		EnableCollision(bool _value) override
		{
			assert(joint != nullptr);

			joint->setConstraintFlag(physx::PxConstraintFlag::eCOLLISION_ENABLED, _value);
		}

		void		EnablePreprocessing(bool _value) override
		{
			assert(joint != nullptr);

			joint->setConstraintFlag(physx::PxConstraintFlag::eDISABLE_PREPROCESSING, !_value);
		}

		void SetUserData(void* _userData) override
		{
			assert(joint != nullptr);

			userData = _userData;
		}

		void* GetUserData() override
		{
			assert(joint != nullptr);

			return userData;
		}
	};
}
