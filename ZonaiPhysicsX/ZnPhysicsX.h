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
		ZnCollider*		CreatBoxCollider(const std::wstring&, float x, float y, float z) noexcept override;
// 		virtual ZnCollider*		CreatPlaneCollider(const std::wstring&, float x, float y) noexcept override;
 		ZnCollider*		CreatSphereCollider(const std::wstring&, float radius) noexcept override;
// 		virtual ZnCollider*		CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept override;
// 		virtual ZnCollider*		CreateCustomCollider(const std::wstring&) noexcept override;
// 		
// 		/// <summary>
// 		/// Create Joint
// 		/// </summary>
 		// ZnJoint*		CreatD6Joint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;			// D6 ����Ʈ		*��� ���� ��
		ZnJoint*		CreatFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;		// ���� ����Ʈ
		ZnJoint*		CreatDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;		// �Ÿ� ����Ʈ
// 		virtual ZnJoint*		CreatSphericalJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;	// ���� ����Ʈ
// 		virtual ZnJoint*		CreatRevoluteJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;		// ȸ�� ����Ʈ
// 		virtual ZnJoint*		CreatPrismaticJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept override;	// ������ ����Ʈ

	public:
		virtual bool Raycast(const Eigen::Vector3f&, const Eigen::Vector3f&, float, ZnRaycastInfo&) noexcept override;

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

