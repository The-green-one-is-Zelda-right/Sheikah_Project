#pragma once
#include "EventCallback.h"
#include "ZnCollider.h"
#include "ZnJoint.h"

#include "ZnMaterial.h"
#include "ZnTransform.h"

#include "PxPhysicsAPI.h"

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


	class Joint;
	class FixedJoint;
	class PrismaticJoint;
	class DistanceJoint;
	class SphericalJoint;
	class HingeJoint;
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
		static RigidBody*				CreateRigidBody(void* _userData);

		/// collider
		static BoxCollider*				CreateBoxCollider(void* _userData, float _x, float _y, float _z, physx::PxMaterial* _material);
		static SphereCollider*			CreateSphereCollider(void* _userData, float _radius, physx::PxMaterial* _material);
		static CapsuleCollider*			CreateCapsuleCollider(void* _userData, float _radius, float _height, physx::PxMaterial* _material);

		/// joint
		static FixedJoint*				CreateFixedJoint(void* _userData0, const ZnTransform& tm0, void* _userData1, const ZnTransform& tm1);
		static PrismaticJoint*			CreatePrismaticJoint(void* _userData0, const ZnTransform& tm0, void* _userData1, const ZnTransform& tm1);
		static DistanceJoint*			CreateDistanceJoint(void* _userData0, const ZnTransform& tm0, void* _userData1, const ZnTransform& tm1);
		static SphericalJoint*			CreateSphericalJoint(void* _userData0, const ZnTransform& tm0, void* _userData1, const ZnTransform& tm1);
		static HingeJoint*				CreateHingeJoint(void* _userData0, const ZnTransform& tm0, void* _userData1, const ZnTransform& tm1);

	private:
		static EventCallback eventCallback;
		static physx::PxDefaultAllocator allocator;
		static physx::PxDefaultErrorCallback  errorCallback;
		static physx::PxDefaultCpuDispatcher* dispatcher;
		static physx::PxFoundation* foundation;
		static physx::PxPhysics* pxFactory;
		static physx::PxPvd* pxPvd;
	};
}