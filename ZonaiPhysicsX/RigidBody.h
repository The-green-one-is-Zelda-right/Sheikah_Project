#pragma once
#include <vector>
#include <Eigen/Dense>

#include "ForceType.h"
#include "ZnRigidBody.h"

namespace physx
{
	class PxPhysics;
	class PxRigidDynamic;
	class PxRigidActor;
}

namespace ZonaiPhysics
{
	class Collider;

	class RigidBody : public ZnRigidBody
	{
	public:
							RigidBody() noexcept;
							RigidBody(physx::PxPhysics*&) noexcept;
							~RigidBody() noexcept override;

	public:
		void			WakeUp() noexcept override;
		bool			IsSleeping() const noexcept override;

		void			UseGravity(bool) noexcept override;

		void			SetKinematic(bool) noexcept override;

		void			CanSimulate(bool) const noexcept;

		DynamicLocks	GetDynamicLockFlags() const noexcept override;
		void			SetDynamicLockFlag(DynamicLock flag, bool) noexcept override;
		void			SetDynamicLockFlags(DynamicLocks flags) noexcept override;
		
		void*			GetUserData() const noexcept override;
		void			SetUserData(void*) noexcept override;

	public:
		// Áú·®
		float			GetMass() const noexcept override;
		void			SetMass(float) noexcept override;
		float			GetInvMass() const noexcept override;

		Eigen::Vector3f	GetInertiaTensor() const noexcept override;
		void			SetInertiaTensor(const Eigen::Vector3f&) noexcept override;

		float			GetLinearDamping() const noexcept override;
		void			SetLinearDamping(float) noexcept override;

		float			GetAngularDamping() const noexcept override;
		void			SetAngularDamping(float) noexcept override;
		
	public:
		// ¼Óµµ
		Eigen::Vector3f	GetLinearVelocity() const noexcept override;
		void			SetLinearVelocity(const Eigen::Vector3f&) noexcept override;

		Eigen::Vector3f	GetAngularVelocity() const noexcept override;
		void			SetAngularVelocity(const Eigen::Vector3f&) noexcept override;
		
		float			GetMaxLinearVelocity() const noexcept override;
		void			SetMaxLinearVelocity(const float&) noexcept override;

		float			GetMaxAngularVelocity() const noexcept override;
		void			SetMaxAngularVelocity(const float&) noexcept override;

		Eigen::Vector3f	GetPosition() const noexcept override;
		void			SetPosition(const Eigen::Vector3f&) noexcept override;

		Eigen::Quaternionf	GetQuaternion() const noexcept override;
		void			SetQuaternion(const Eigen::Quaternionf&) noexcept override;

		// Èû
		void			SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque, ForceType _type) noexcept override;

		void			AddForce(const Eigen::Vector3f&, ForceType) noexcept override;
		void			ClearForce() noexcept override;
	
		void			AddTorque(const Eigen::Vector3f&, ForceType) noexcept override;
		void			ClearTorque() noexcept override;

	public:
		__declspec(property(get = GetLinearDamping, put = SetLinearDamping)) 
			float linerDamping;

		__declspec(property(get = GetAngularDamping, put = SetAngularDamping)) 
			float angularDamping;

		__declspec(property(get = GetLinearVelocity, put = SetLinearVelocity))
			Eigen::Vector3f linerVelocity;

		__declspec(property(get = GetAngularVelocity, put = SetAngularVelocity)) 
			Eigen::Vector3f angularVelocity;

		__declspec(property(get = GetMaxLinearVelocity, put = SetMaxLinearVelocity))
			float maxLinearVelocity;

		__declspec(property(get = GetMaxAngularVelocity, put = SetMaxAngularVelocity))
			float maxAngularVelocity;

		__declspec(property(get = GetPosition, put = SetPosition))
			Eigen::Vector3f position;

		__declspec(property(get = GetQuaternion, put = SetQuaternion))
			Eigen::Quaternionf quaternion;

	public:
		physx::PxRigidDynamic* getRigidDynamic() const noexcept;

	private:
		physx::PxRigidDynamic* rigidbody_;
	};
} // namespace ZonaiPhysics

