#pragma once
#include "ZnPhysicsBase.h"

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

	class ZnMaterial;
	class ZnPhysicsX;

	class ZnPhysicsX : public ZnPhysicsBase
	{
	public:
		ZnPhysicsX() noexcept = default;
		virtual ~ZnPhysicsX() noexcept = default;

	public:
		virtual void			Initialize() noexcept override;
		virtual void			Simulation(float _dt) noexcept override;
		virtual void			Finalize() noexcept override;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*	CreateRigidBody(const std::wstring&) noexcept override;

		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*		CreatBoxCollider(const std::wstring&, float x, float y, float z) noexcept override;
// 		virtual ZnCollider*		CreatPlaneCollider(const std::wstring&, float x, float y) noexcept override;
// 		virtual ZnCollider*		CreatSphereCollider(const std::wstring&, float radius) noexcept override;
// 		virtual ZnCollider*		CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept override;
// 		virtual ZnCollider*		CreateCustomCollider(const std::wstring&) noexcept override;
// 		
// 		/// <summary>
// 		/// Create Joint
// 		/// </summary>
// 		virtual ZnJoint*		CreatD6Joint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;			// D6 조인트		*사실 뭔지 모름
// 		virtual ZnJoint*		CreatFixedJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;		// 고정 조인트
// 		virtual ZnJoint*		CreatDistanceJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;		// 거리 조인트
// 		virtual ZnJoint*		CreatSphericalJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;	// 구형 조인트
// 		virtual ZnJoint*		CreatRevoluteJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;		// 회전 조인트
// 		virtual ZnJoint*		CreatPrismaticJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;	// 프리즘 조인트
	
	public:
// 		virtual ZnMaterial*		CreateMaterial(const float&, const float&, const float&) noexcept;
// 		virtual ZnMaterial*		CreateMaterial(const Vector3D&) noexcept;

	private:
		RigidBody*				FindRigidBody(const std::wstring&) noexcept;
		Collider*				CreateCollider(const std::wstring&) noexcept;

	private:
		physx::PxDefaultAllocator		allocator;
		physx::PxDefaultErrorCallback	errorCallback;
		physx::PxFoundation*			foundation;
		physx::PxPhysics*				physics;
		physx::PxDefaultCpuDispatcher*	dispatcher;
		physx::PxScene*					scene;
		physx::PxPvd*					pvd;

		physx::PxMaterial*				material;

	private:
		std::map<std::wstring, RigidBody*> bodies;
		bool first;
	};

	extern "C"
	{
		__declspec(dllexport) ZnPhysicsBase* CreatePhysics();
	}
} // namespace ZonaiPhysics

