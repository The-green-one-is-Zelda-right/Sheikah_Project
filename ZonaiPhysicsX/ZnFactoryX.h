#pragma once
#include "EventCallback.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#include "PxPhysicsAPI.h"
#pragma warning (pop)

namespace physx
{
	class PxPhysics;
}

namespace ZonaiPhysics
{
	class ZnSimulationCallback;

	class RigidBody;

	class Collider;
	class BoxCollider;
	class SphereCollider;
	class CapsuleCollider;
	class ConvexCollider;
	class MeshCollider;

	class Joint;
	class FixedJoint;
	class PrismaticJoint;
	class DistanceJoint;
	class SphericalJoint;
	class HingeJoint;

	class ZnTransform;
}

namespace ZonaiPhysics
{
	class ZnFactoryX
	{
	public:
		static void CreatePhysxFactory();
		static void SetSimulationCallback(ZnSimulationCallback* _instance);
		static void Release();

	public:
		static physx::PxScene*			CreateScene(void* _userScene, const Eigen::Vector3f& _gravity);
		static physx::PxMaterial*		CreateMaterial(float _staticFriction, float _dynamicFriction, float _restitution);

		/// rigidbody
		static RigidBody*				CreateDynamicRigidBody(void* _userData);
		static RigidBody*				CreateStaticRigidBody(void* _userData);

		/// collider
		static BoxCollider*				CreateBoxCollider(void* _znBody, const Eigen::Vector3f& _extend, const physx::PxMaterial* _material);
		static SphereCollider*			CreateSphereCollider(void* _znBody, float _radius, const physx::PxMaterial* _material);
		static CapsuleCollider*			CreateCapsuleCollider(void* _znBody, float _radius, float _height, const physx::PxMaterial* _material);
		static MeshCollider*			CreateMeshCollider(void* _znBody , const physx::PxMaterial* _material);
		static ConvexCollider*			CreateConvexCollider(void* _znBody , const physx::PxMaterial* _material);

		/// joint
		static FixedJoint*				CreateFixedJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static PrismaticJoint*			CreatePrismaticJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static DistanceJoint*			CreateDistanceJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static SphericalJoint*			CreateSphericalJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static HingeJoint*				CreateHingeJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);

	private:
		static ZonaiPhysics::EventCallback eventCallback;
		static physx::PxDefaultAllocator allocator;
		static physx::PxDefaultErrorCallback  errorCallback;
		static physx::PxDefaultCpuDispatcher* dispatcher;
		static physx::PxFoundation* foundation;
		static physx::PxPhysics* pxFactory;
		static physx::PxPvd* pxPvd;
		// static PxCooking* pxCooking;
	};
}