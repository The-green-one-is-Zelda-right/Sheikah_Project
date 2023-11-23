#pragma once
#include "ZnPhysicsBase.h"
#include "PxPhysicsAPI.h"
#include <map>
#include <string>

#define PVD_HOST "127.0.0.1"

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxMaterial;
	class PxPvd;
	class PxRigidDynamic;
}

namespace ZonaiPhysics
{
	class ZnRigidBody;
	class RigidBody;
	class ZnSoftBody;
	class SoftBody;
	class ZnCollider;
	class Collider;
	class ZnJoint;
	class Joint;
	class ZnTransform;
	class ZnPhysicsX;

	extern "C"

	{
		__declspec(dllexport) ZnPhysicsBase* CreatePhysics() noexcept;
	}

	class ZnPhysicsX : public ZnPhysicsBase
	{
	public:
		ZnPhysicsX() noexcept = default;
		virtual ~ZnPhysicsX() noexcept;

	public:
		virtual void			Initialize() noexcept override;
		virtual void			Simulation(float _dt) noexcept override;
		virtual void			Finalize() noexcept override;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody* CreateRigidBody(const std::wstring&) noexcept override;
		// virtual ZnRigidBody*	CreateRigidBody(const std::wstring&, ZnMaterial) noexcept = 0;

		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider* CreatBoxCollider(const std::wstring&, float x, float y, float z) noexcept override;
		virtual ZnCollider* CreatPlaneCollider(const std::wstring&, float x, float y) noexcept override;
		virtual ZnCollider* CreatSphereCollider(const std::wstring&, float radius) noexcept override;
		virtual ZnCollider* CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept override;
		virtual ZnCollider* CreateCustomCollider(const std::wstring&) noexcept override;
		
		/// <summary>
		/// Create Joint
		/// </summary>
		virtual ZnJoint*		CreatD6Joint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;			// D6 ����Ʈ		*��� ���� ��
		virtual ZnJoint*		CreatFixedJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;		// ���� ����Ʈ
		virtual ZnJoint*		CreatDistanceJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;		// �Ÿ� ����Ʈ
		virtual ZnJoint*		CreatSphericalJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;	// ���� ����Ʈ
		virtual ZnJoint*		CreatRevoluteJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;		// ȸ�� ����Ʈ
		virtual ZnJoint*		CreatPrismaticJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;	// ������ ����Ʈ
	
	private:
		physx::PxDefaultAllocator		allocator;
		physx::PxDefaultErrorCallback	errorCallback;
		physx::PxFoundation*			foundation;
		physx::PxPhysics*				physics;
		physx::PxDefaultCpuDispatcher*	dispatcher;
		physx::PxScene*					scene;
		physx::PxMaterial*				material;
		physx::PxPvd*					pvd;

	private:
		std::map<std::wstring, RigidBody*> bodies;
		bool first;
	};
} // namespace ZonaiPhysics

