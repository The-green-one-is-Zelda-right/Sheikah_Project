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

		virtual ~TemplateJoint() = default;

	protected:
		PhysxJoint* joint;
		RigidBody* rigidbody0;
		RigidBody* rigidbody1;

	public:
		/**
		������Ʈ�� ������
		*/
		virtual void SetLocalPosition(ZnJoint::eOBJECT _index, const Eigen::Vector3f& _localPos) override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto index = static_cast<PxJointActorIndex::Enum>(_index);
			PxTransform t = joint->getLocalPose(index);
			t.p = {_localPos.x(), _localPos.y(), _localPos.z()};
			joint->setLocalPose(index, t);
		}

		virtual Eigen::Vector3f GetLocalPosition(ZnJoint::eOBJECT _index) const override
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
		virtual void SetLocalQuaternion(ZnJoint::eOBJECT _index, const Quaternionf& _localQuat) override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto index = static_cast<PxJointActorIndex::Enum>(_index);
			PxTransform t = joint->getLocalPose(index);
			t.q = {_localQuat.x(), _localQuat.y(), _localQuat.z(), _localQuat.w()};
			joint->setLocalPose(index, t);
		}

		virtual Quaternionf GetLocalQuaternion(ZnJoint::eOBJECT _index) const override
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
		virtual Eigen::Vector3f GetRelativeLinearVelocity() const override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto& velo = joint->getRelativeLinearVelocity();
			return {velo.x, velo.y, velo.z};
		}

		virtual Eigen::Vector3f GetRelativeAngularVelocity() const override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto& velo = joint->getRelativeAngularVelocity();
			return {velo.x, velo.y, velo.z};
		}

		/**
		����Ʈ �ı� ���� ������
		*/
		virtual void SetBreakForce(float _force, float _torque) override
		{
			assert(joint != nullptr);

			joint->setBreakForce(_force, _torque);
		}

		virtual void GetBreakForce(float& _force, float& _torque) const override
		{
			assert(joint != nullptr);

			joint->getBreakForce(_force, _torque);
		}
	};
}
