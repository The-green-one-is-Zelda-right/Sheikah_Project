#pragma once
#include "ZnPhysicsBase.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
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
	class ZnPhysicsX;

	class ZnRigidBody;
	class RigidBody;

	class ZnSoftBody;
	class SoftBody;

	class ZnCollider;
	class Collider;

	class ZnJoint;
	class Joint;
	class FixedJoint;

	class ZnTransform;

	struct ZnRaycastInfo;
	struct ZnMaterial;

	enum class ColliderID
	{
		Box				= 0,
		Plane			= 1,
		Sphere			= 2,
		Capsule			= 3,
		Custom			= 4,
	};

	enum class JointID
	{
		D6				= 0,
		Fixed			= 1,
		Distance		= 2,
		Spherical		= 3,
		Revolute		= 4,
		Prismatic		= 5,
	};

	class ZnPhysicsX : public ZnPhysicsBase
	{
	public:
		ZnPhysicsX() noexcept = default;
		virtual ~ZnPhysicsX() noexcept = default;

	public:
		virtual void			Initialize() noexcept override;
		virtual void			Simulation(float _dt) noexcept override;
		virtual void			Finalize() noexcept override;
		virtual void			WorldClear() noexcept;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*	CreateRigidBody(const std::wstring&) noexcept override;

		/// <summary>
		/// Create Collider
		/// </summary>
		ZnCollider*				CreateBoxCollider(const std::wstring&, float x, float y, float z) noexcept override;
// 		ZnCollider*				CreatePlaneCollider(const std::wstring&, float x, float y) noexcept override;
 		ZnCollider*				CreateSphereCollider(const std::wstring&, float radius) noexcept override;
 		ZnCollider*				CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept override;
// 		ZnCollider*				CreateCustomCollider(const std::wstring&) noexcept override;
	
// 		/// <summary>
// 		/// Create Joint
// 		/// </summary>
 		// ZnD6Joint*				CreateD6Joint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;			// D6 조인트		*사실 뭔지 모름
		ZnFixedJoint*			CreateFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;		// 고정 조인트
		ZnDistanceJoint*		CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;		// 거리 조인트
		ZnSphericalJoint*		CreateSphericalJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;	// 구형 조인트
		ZnHingeJoint*			CreateHingeJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;		// 회전 조인트
		ZnPrismaticJoint*		CreatePrismaticJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;	// 프리즘 조인트

	public:
		bool					Raycast(const Eigen::Vector3f&, const Eigen::Vector3f&, float, ZnRaycastInfo&) noexcept override;

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

		physx::PxRaycastHit hitInfo;
		physx::PxRaycastBuffer hit;
		physx::PxQueryFilterData d;

		physx::PxMaterial*				material;

	private:
		std::map<std::wstring, RigidBody*> bodies;
	};

	extern "C"
	{
		__declspec(dllexport) ZnPhysicsBase* CreatePhysics();
	}
} // namespace ZonaiPhysics

