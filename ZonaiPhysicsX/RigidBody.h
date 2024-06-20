#pragma once
#include <vector>

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#pragma warning (pop)

#include "ForceType.h"
#include "ZnRigidBody.h"

namespace physx
{
	class PxRigidDynamic;
}

namespace ZonaiPhysics
{
	class Collider;

	class RigidBody : public ZnRigidBody
	{
	public:
							RigidBody() = delete;
							RigidBody(physx::PxRigidDynamic* _pxBody, void* _userData = nullptr);
							~RigidBody() override;

	public:
		void				WakeUp() override;
		bool				IsSleeping() const override;

		void				UseGravity(bool) override;
		bool				HasGravity() const override;
		void				SetKinematic(bool) override;
		bool				IsKinematic() const override;

		void				Disable(bool) const;

		uint8_t				GetDynamicLockFlags() const override;
		void				SetDynamicLockFlag(FreezeFlag flag, bool) override;
		void				SetDynamicLockFlags(uint8_t flags) override;
		
		void*				GetUserData() const override;
		void				SetUserData(void*) override;

	public:
		// ����
		float				GetMass() const override;
		void				SetMass(float) override;
		void				SetDensity(float) override;
		float				GetInvMass() const override;

		Eigen::Vector3f		GetInertiaTensor() const override;

		float				GetLinearDamping() const override;
		void				SetLinearDamping(float) override;

		float				GetAngularDamping() const override;
		void				SetAngularDamping(float) override;
		
	public:
		// �ӵ�
		Eigen::Vector3f		GetLinearVelocity() const override;
		void				SetLinearVelocity(const Eigen::Vector3f&) override;

		Eigen::Vector3f		GetAngularVelocity() const override;
		void				SetAngularVelocity(const Eigen::Vector3f&) override;
		
		float				GetMaxLinearVelocity() const override;
		void				SetMaxLinearVelocity(const float&) override;

		float				GetMaxAngularVelocity() const override;
		void				SetMaxAngularVelocity(const float&) override;

		Eigen::Vector3f		GetPosition() const override;
		void				SetPosition(const Eigen::Vector3f&) override;

		Eigen::Quaternionf	GetQuaternion() const override;
		void				SetQuaternion(const Eigen::Quaternionf&) override;

		void				AddForce(const Eigen::Vector3f&, ForceType) override;
		void				ClearForce() override;
	
		void				AddTorque(const Eigen::Vector3f&, ForceType) override;
		void				ClearTorque() override;

	public:
		physx::PxRigidDynamic* pxBody;
	};
} // namespace ZonaiPhysics

