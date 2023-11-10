#pragma once
#include "ZnPhysicsBase.h"
#include "PxPhysicsAPI.h"

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
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;
	class ZnTransform;
	class World;

	extern "C"
	{
		__declspec(dllexport) ZnPhysicsBase* CreatePhysics() noexcept;
	}

	class World : public ZnPhysicsBase
	{
	public:
		World() noexcept = default;
		virtual ~World() noexcept;

	public:
		virtual void			Initialize() noexcept override;
		virtual void			Simulation(float _dt) noexcept override;
		virtual void			Finalize() noexcept override;

	public:
		/// <summary>
		/// Create Component
		/// </summary>
		virtual ZnRigidBody*	CreateRigidBody() noexcept override;
		virtual ZnSoftBody*		CreateSoftBody() noexcept override;
		virtual ZnCollider*		CreateCollider() noexcept override;
		virtual ZnJoint*		CreateJoint() noexcept override;


		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*	CreateRigidBody() noexcept;
		
		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*		CreatBoxCollider(float width, float height) noexcept;
		virtual ZnCollider*		CreatPlaneCollider() noexcept;
		virtual ZnCollider*		CreatSphereCollider() noexcept;
		virtual ZnCollider*		CreateCapsuleCollider() noexcept;
		virtual ZnCollider*		CreateCustomCollider() noexcept;
		
		/// <summary>
		/// Create Joint
		/// </summary>
		virtual ZnJoint*		CreatD6Joint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;			// D6 ����Ʈ		*��� ���� ��
		virtual ZnJoint*		CreatFixedJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;			// ���� ����Ʈ
		virtual ZnJoint*		CreatDistanceJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;		// �Ÿ� ����Ʈ
		virtual ZnJoint*		CreatSphericalJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;		// ���� ����Ʈ
		virtual ZnJoint*		CreatRevoluteJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;		// ȸ�� ����Ʈ
		virtual ZnJoint*		CreatPrismaticJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;		// ������ ����Ʈ
	
	private:
		physx::PxDefaultAllocator		allocator;
		physx::PxDefaultErrorCallback	errorCallback;
		physx::PxFoundation*			foundation;
		physx::PxPhysics*				physics;
		physx::PxDefaultCpuDispatcher*	dispatcher;
		physx::PxScene*					scene;
		physx::PxMaterial*				material;
		physx::PxPvd*					pvd;
	};
} // namespace ZonaiPhysics

