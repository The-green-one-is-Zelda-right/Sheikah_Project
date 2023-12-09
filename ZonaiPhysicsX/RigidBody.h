#pragma once
#include <vector>

#include "ForceType.h"
#include "ZnRigidBody.h"

#define OVERRIDE noexcept override

namespace ZonaiMath
{
	class Vector3D;
}

namespace physx
{
	class PxPhysics;
	class PxRigidDynamic;
	class PxRigidActor;
}

namespace ZonaiPhysics
{
	using namespace ZonaiMath;

	class Collider;

	class RigidBody : public ZnRigidBody
	{
	public:
							RigidBody() noexcept = delete;
							RigidBody(physx::PxPhysics*&) noexcept;
		virtual				~RigidBody() noexcept;

	public:
		virtual void		WakeUp() noexcept override;
		virtual bool		IsSleeping() const noexcept override;

		virtual DynamicLocks GetDynamicLockFlags() const noexcept override;
		virtual void		SetDynamicLockFlag(DynamicLock flag, bool) noexcept override;
		virtual void		SetDynamicLockFlags(DynamicLocks flags) noexcept override;
		
		virtual void*		GetUserData() const noexcept override;
		virtual void		SetUserData(void*) noexcept override;

	public:
		// Áú·®
		virtual float		GetMass() const noexcept override;
		virtual void		SetMass(float) noexcept override;
		virtual float		GetInvMass() const noexcept override;

		virtual Vector3D	GetInertiaTensor() const noexcept override;
		virtual void		SetInertiaTensor(const Vector3D&) noexcept override;

		virtual float		GetLinearDamping() const noexcept override;
		virtual void		SetLinearDamping(float) noexcept override;

		virtual float		GetAngularDamping() const noexcept override;
		virtual void		SetAngularDamping(float) noexcept override;
		
	public:
		// ¼Óµµ
		virtual Vector3D	GetLinearVelocity() const noexcept override;
		virtual void		SetLinearVelocity(const Vector3D&) noexcept override;

		virtual Vector3D	GetAngularVelocity() const noexcept override;
		virtual void		SetAngularVelocity(const Vector3D&) noexcept override;
		
		virtual float		GetMaxLinearVelocity() const noexcept override;
		virtual void		SetMaxLinearVelocity(const float&) noexcept override;

		virtual float		GetMaxAngularVelocity() const noexcept override;
		virtual void		SetMaxAngularVelocity(const float&) noexcept override;

		virtual Vector3D	GetPosition() const noexcept override;
		virtual void		SetPosition(const Vector3D&) noexcept override;

		virtual Quaternion	GetQuaternion() const noexcept override;
		virtual void		SetQuaternion(const Quaternion&) noexcept override;

		// Èû
		virtual void		AddForce(const Vector3D&, ForceType) noexcept override;
		virtual void		ClearForce() noexcept override;
	
		virtual void		AddTorque(const Vector3D&, ForceType) noexcept override;
		virtual void		ClearTorque() noexcept override;

	public:
		__declspec(property(get = GetLinearDamping, put = SetLinearDamping)) 
			float linerDamping;

		__declspec(property(get = GetAngularDamping, put = SetAngularDamping)) 
			float angularDamping;

		__declspec(property(get = GetLinearVelocity, put = SetLinearVelocity))
			Vector3D linerVelocity;

		__declspec(property(get = GetAngularVelocity, put = SetAngularVelocity)) 
			Vector3D angularVelocity;

		__declspec(property(get = GetMaxLinearVelocity, put = SetMaxLinearVelocity))
			float maxLinearVelocity;

		__declspec(property(get = GetMaxAngularVelocity, put = SetMaxAngularVelocity))
			float maxAngularVelocity;

		__declspec(property(get = GetPosition, put = SetPosition))
			Vector3D position;

		__declspec(property(get = GetQuaternion, put = SetQuaternion))
			Quaternion quaternion;

	public:
		void AddCollider(Collider*) noexcept;
		void CanSimulate() noexcept;
		physx::PxRigidDynamic* getRigidDynamic() const noexcept;

	private:
		physx::PxRigidDynamic* rigidbody_;
		std::vector<ZonaiPhysics::Collider*> shapes_;
	};
} // namespace ZonaiPhysics

