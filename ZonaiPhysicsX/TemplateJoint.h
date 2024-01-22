#pragma once
#include "PxPhysicsAPI.h"

#include "ZnJoint.h"
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

	// Base Ÿ���� ZnJoint�� ��ӹ޴°�?
	template <typename Base>
	concept isJoint = std::is_base_of_v<ZnJoint, Base>;

	// ZnJoint�� ����� �Լ��� ���ø����� �����ϴ� �ɷ� �ݺ��� �۾��� ����.
	template <isJoint Base, typename PhysxJoint>
	class TemplateJoint : public Base
	{
	public:
		TemplateJoint() noexcept : joint(), rigidbody()
		{
		}

		virtual ~TemplateJoint() noexcept = default;

	protected:
		PhysxJoint* joint;
		RigidBody* rigidbody[2];

	public:
		/**
		������Ʈ�� ������
		*/
		virtual void SetLocalPosition(ZnJoint::eOBJECT _index, const Vector3f& _localPos) noexcept override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto index = static_cast<PxJointActorIndex::Enum>(_index);
			PxTransform t = joint->getLocalPose(index);
			t.p = {_localPos.x(), _localPos.y(), _localPos.z()};
			joint->setLocalPose(index, t);
		}

		virtual Vector3f GetLocalPosition(ZnJoint::eOBJECT _index) const noexcept override
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
		virtual void SetLocalQuaternion(ZnJoint::eOBJECT _index, const Quaternionf& _localQuat) noexcept override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto index = static_cast<PxJointActorIndex::Enum>(_index);
			PxTransform t = joint->getLocalPose(index);
			t.q = {_localQuat.x(), _localQuat.y(), _localQuat.z(), _localQuat.w()};
			joint->setLocalPose(index, t);
		}

		virtual Quaternionf GetLocalQuaternion(ZnJoint::eOBJECT _index) const noexcept override
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
		virtual Vector3f GetRelativeLinearVelocity() const noexcept override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto& velo = joint->getRelativeLinearVelocity();
			return {velo.x, velo.y, velo.z};
		}

		virtual Vector3f GetRelativeAngularVelocity() const noexcept override
		{
			assert(joint != nullptr);

			using namespace physx;
			const auto& velo = joint->getRelativeAngularVelocity();
			return {velo.x, velo.y, velo.z};
		}

		/**
		����Ʈ �ı� ���� ������
		*/
		virtual void SetBreakForce(float _force, float _torque) noexcept override
		{
			assert(joint != nullptr);

			joint->setBreakForce(_force, _torque);
		}

		virtual void GetBreakForce(float& _force, float& _torque) const noexcept override
		{
			assert(joint != nullptr);

			joint->getBreakForce(_force, _torque);
		}
	};
}
