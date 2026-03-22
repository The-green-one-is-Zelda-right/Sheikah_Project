#pragma once
#include "EventCallback.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "ConvexCollider.h"

#include "MaterialEnum.h"
#include "ZnMaterial.h"

#include "RigidBodyHelper.h"

#include <Eigen/Dense>
#include "PxPhysicsAPI.h"
#include "omnipvd/PxOmniPvd.h"

#define PX_SUPPORT_PVD 1
#define PX_SUPPORT_OMNI_PVD 0

#if PX_SUPPORT_OMNI_PVD
#include "../pvdruntime/include/OmniPvdWriter.h"
#include "../pvdruntime/include/OmniPvdFileWriteStream.h"
#endif

#pragma warning (pop)

namespace physx
{
	class PxPhysics;
}

namespace ZonaiPhysics
{
	class ZnSimulationCallback;

	struct ZnControllerDecs;
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
	class SpringFlexJoint;

	struct ZnTransform;
}

namespace ZonaiPhysics
{
	template <typename Ty>
	concept znCollider = std::is_base_of_v<ZnCollider, Ty>;

	class ZnFactoryX
	{
	public:
		static void CreatePhysxFactory();
		static void PhysicsWithOmniPvd();
		static void SetSimulationCallback(ZnSimulationCallback* _instance);
		static void Release();

	public:
		static physx::PxScene*			CreateScene(void* _userScene, const Eigen::Vector3f& _gravity);
		static physx::PxMaterial*		CreateMaterial(const MaterialDesc& _desc);

		/// rigidbody
		static RigidBody*				CreateDynamicRigidBody(void* _userData);
		static RigidBody*				CreateStaticRigidBody(void* _userData);

		static physx::PxControllerManager* CreateControllerManager(physx::PxScene* _scene);
		static physx::PxCapsuleController* CreateCapsuleController(
			physx::PxControllerManager*, 
			float _radius, float _height, 
			const ZnControllerDecs& _desc, 
			float _density, physx::PxMaterial* _material);
		static physx::PxBoxController*		CreateBoxController(
			physx::PxControllerManager*, 
			const Eigen::Vector3f& _extend,
			const ZnControllerDecs& _desc,
			float _density, physx::PxMaterial* _material);

		static physx::PxShape*			CreateBoxShape(const Eigen::Vector3f& _extend, const physx::PxMaterial* _material);
		static physx::PxShape*			CreateSphereShape(float _radius, const physx::PxMaterial* _material);
		static physx::PxShape*			CreateCapsuleShape(float _radius, float _height, const physx::PxMaterial* _material);
		static physx::PxShape*			CreateTriagleMeshShape(physx::PxTriangleMesh* _mesh, const Eigen::Vector3f& _scale, const physx::PxMaterial* _material);
		static physx::PxShape*			CreateConvexMeshShape(physx::PxConvexMesh* _mesh, const Eigen::Vector3f& _scale, const physx::PxMaterial* _material);

		static physx::PxTriangleMesh*	CookTriagleMesh(FBXLoader::Model* _model);
		static physx::PxConvexMesh*		CookConvexMesh(FBXLoader::Model* _model);

		/// collider
		template <znCollider collider>
		static collider*				CreateZnCollider(void* _znBody, physx::PxShape* _pxShape)
		{
			assert(_znBody != nullptr);
			assert(_pxShape != nullptr);

			const auto znBody = static_cast<RigidBody*>(_znBody);

			auto zn_collider = new collider(_pxShape, znBody);
			if (!RigidBodyHelper::Attach(znBody->pxBody, _pxShape))
			{
				delete zn_collider;
				zn_collider = nullptr;
			}

			return zn_collider;
		}

		/// joint
		static FixedJoint*				CreateFixedJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static PrismaticJoint*			CreatePrismaticJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static DistanceJoint*			CreateDistanceJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static SphericalJoint*			CreateSphericalJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static HingeJoint*				CreateHingeJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static SpringFlexJoint*			CreateSpringFlexJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1);

	private:
		static ZonaiPhysics::EventCallback eventCallback;
		static physx::PxDefaultAllocator allocator;
		static physx::PxDefaultErrorCallback  errorCallback;
		static physx::PxDefaultCpuDispatcher* dispatcher;
		static physx::PxFoundation* foundation;
		static physx::PxPhysics* pxFactory;
		static physx::PxPvd* pxPvd;
		static physx::PxOmniPvd* omniPvd;
		// static PxCooking* pxCooking;
	};
}
