#include "FilterCallback.h"

#include "RigidBodyHelper.h"
#include "RigidBody.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"

#include "FixedJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "SphericalJoint.h"
#include "HingeJoint.h"

#include "ZnUtil.h"

#include "ZnTransform.h"

#include "ZnFactoryX.h"

#include "ConvexCollider.h"
#include "../ZeldaFBXLoader/FBXLoader.h"


namespace ZonaiPhysics
{
	EventCallback ZnFactoryX::eventCallback{};
	physx::PxDefaultAllocator ZnFactoryX::allocator{};
	physx::PxDefaultErrorCallback ZnFactoryX::errorCallback{};
	physx::PxDefaultCpuDispatcher* ZnFactoryX::dispatcher = nullptr;
	physx::PxFoundation* ZnFactoryX::foundation = nullptr;
	physx::PxPhysics* ZnFactoryX::pxFactory = nullptr;
	physx::PxPvd* ZnFactoryX::pxPvd = nullptr;
	// physx::PxCookingParams* ZnFactoryX::pxCooking = nullptr;

	void ZnFactoryX::CreatePhysxFactory()
	{
		/// SDK 생성
		{
			foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);

#ifdef _DEBUG
			pxPvd = PxCreatePvd(*foundation);
			physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
			pxPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif

			pxFactory = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pxPvd);

#ifdef _DEBUG
			PxInitExtensions(*pxFactory, pxPvd);
#endif

			// pxCooking = PxCreate
		}
	}

	void ZnFactoryX::SetSimulationCallback(ZnSimulationCallback* _instance)
	{
		assert(_instance != nullptr);

		eventCallback.setInstance(_instance);
	}

	void ZnFactoryX::Release()
	{
		PxCloseExtensions();
		PX_RELEASE(dispatcher);
		PX_RELEASE(pxFactory);
		if (pxPvd)
		{
			physx::PxPvdTransport* transport = pxPvd->getTransport();
			pxPvd->release();
			pxPvd = nullptr;
			PX_RELEASE(transport);
		}
		PX_RELEASE(foundation);

	}

	physx::PxScene* ZnFactoryX::CreateScene(void* _userScene, const Eigen::Vector3f& _gravity)
	{
		assert(_userScene != nullptr);

		physx::PxSceneDesc sceneDesc(pxFactory->getTolerancesScale());
		sceneDesc.gravity = EigenToPhysx(_gravity);
		dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = dispatcher;
		sceneDesc.simulationEventCallback = &eventCallback;
		sceneDesc.filterShader = FilterShader;

		const auto scene = pxFactory->createScene(sceneDesc);
		scene->userData = _userScene;

#ifdef _DEBUG
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 3.f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 3.f);

		physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif

		return scene;
	}

	physx::PxMaterial* ZnFactoryX::CreateMaterial(float _staticFriction, float _dynamicFriction, float _restitution)
	{
		return pxFactory->createMaterial(_dynamicFriction, _staticFriction, _restitution);
	}

	RigidBody* ZnFactoryX::CreateDynamicRigidBody(void* _userData)
	{
		assert(_userData != nullptr);

		const auto pxBody = pxFactory->createRigidDynamic(physx::PxTransform(physx::PxVec3{0.f, 0.f, 0.f}));
		assert(pxBody != nullptr);

		const auto znBody = new RigidBody(pxBody, _userData);
		return znBody;
	}

	RigidBody* ZnFactoryX::CreateStaticRigidBody(void* _userData)
	{
		assert(_userData != nullptr);

		const auto pxBody = pxFactory->createRigidStatic(physx::PxTransform(physx::PxVec3{ 0.f, 0.f, 0.f }));
		assert(pxBody != nullptr);

		// const auto znBody = new RigidBody(pxBody, _userData);
		// return znBody;
		return nullptr;
	}

	BoxCollider* ZnFactoryX::CreateBoxCollider(void* _znBody, const Eigen::Vector3f& _extend, const physx::PxMaterial* _material)
	{
		assert(_znBody != nullptr);

		const auto znBody = static_cast<RigidBody*>(_znBody);
		const auto pxShape = pxFactory->createShape(physx::PxBoxGeometry(_extend.x(), _extend.y(), _extend.z()), *_material);
		assert(pxShape != nullptr);

		const auto znBoxCollider = new BoxCollider(pxShape, znBody);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		return znBoxCollider;
	}

	SphereCollider* ZnFactoryX::CreateSphereCollider(void* _znBody, float _radius, const physx::PxMaterial* _material)
	{
		assert(_znBody != nullptr);

		const auto znBody = static_cast<RigidBody*>(_znBody);
		const auto pxShape = pxFactory->createShape(physx::PxSphereGeometry(_radius), *_material);
		assert(pxShape != nullptr);

		const auto znSphereCollider = new SphereCollider(pxShape, znBody);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		return znSphereCollider;
	}

	CapsuleCollider* ZnFactoryX::CreateCapsuleCollider(void* _znBody, float _radius, float _height, const physx::PxMaterial* _material)
	{
		assert(_znBody != nullptr);

		const auto znBody = static_cast<RigidBody*>(_znBody);
		const auto pxShape = pxFactory->createShape(physx::PxCapsuleGeometry(_radius, _height), *_material);
		assert(pxShape != nullptr);

		const auto znCapsuleCollider = new CapsuleCollider(pxShape, znBody);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		return znCapsuleCollider;
	}

	MeshCollider* ZnFactoryX::CreateMeshCollider(void* _znBody, const physx::PxMaterial* _material)
	{
		assert(_znBody != nullptr);

		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count;
		meshDesc.points.stride;
		meshDesc.points.data;

		meshDesc.triangles.count;
		meshDesc.triangles.stride;
		meshDesc.triangles.data;

		// meshDesc.flags = physx::PxMeshFlag::e16_BIT_INDICES;

		physx::PxCookingParams params(pxFactory->getTolerancesScale());
		// 메시 정리 비활성화 - 개발 구성에서 메시 유효성 검사 수행
		// params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
		// 엣지 사전 계산 비활성화, 각 삼각형에 대해 엣지가 설정됩니다. 이는 접촉 생성을 느리게 합니다.
		// params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
		// 내부 메시의 계층 구조를 낮춥니다.
		// params.meshCookingHint = physx::PxMeshCookingHint::eCOOKING_PERFORMANCE;
		//
//#ifdef _DEBUG
//// 메시가 정리되지 않은 상태로 쿠킹하기 전에 메시를 유효성 검사해야 합니다.
//		bool res = PxValidateTriangleMesh(params, meshDesc);
//		PX_ASSERT(res);
//#endif
//
//		physx::PxTriangleMesh* aTriangleMesh = PxCreateTriangleMesh(params, meshDesc,
//			pxFactory->getPhysicsInsertionCallback());

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum result;
		const bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
		if (!status)
			return NULL;

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh* triangleMesh = pxFactory->createTriangleMesh(readBuffer);

		const auto znBody = static_cast<RigidBody*>(_znBody);
		const auto pxShape = pxFactory->createShape(physx::PxTriangleMeshGeometry(triangleMesh), *_material);
		assert(pxShape != nullptr);

		const auto znMeshCollider = new MeshCollider(pxShape, znBody);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		return znMeshCollider;
	}

	ConvexCollider* ZnFactoryX::CreateConvexCollider(void* _znBody, const physx::PxMaterial* _material)
	{
		assert(_znBody != nullptr);

		FBXLoader::FBXLoader loader;

		// 이부분은 바뀔 예정
		// 정점 갯수, 정점 데이터, 정점 크기를 받아서 Convex를 만들어야함.
		const physx::PxVec3 convexVerts[] = {
			physx::PxVec3(0,1,0),
			physx::PxVec3(1,0,0),
			physx::PxVec3(-1,0,0),
			physx::PxVec3(0,0,1),
			physx::PxVec3(0,0,-1)
		};

		physx::PxConvexMeshDesc convexDesc;
		convexDesc.points.count = sizeof(convexVerts) / sizeof(physx::PxVec3);
		convexDesc.points.stride = sizeof(physx::PxVec3);
		convexDesc.points.data = convexVerts;
		convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		const physx::PxCookingParams params(pxFactory->getTolerancesScale());

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		if (!PxCookConvexMesh(params, convexDesc, buf, &result))
			return NULL;

		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convexMesh = pxFactory->createConvexMesh(input);

		const auto znBody = static_cast<RigidBody*>(_znBody);
		const auto pxShape = pxFactory->createShape(physx::PxConvexMeshGeometry(convexMesh), *_material);
		assert(pxShape != nullptr);

		const auto znConvexCollider = new ConvexCollider(pxShape, znBody);
		RigidBodyHelper::Attach(znBody->pxBody, pxShape);

		return znConvexCollider;
	}

	FixedJoint* ZnFactoryX::CreateFixedJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxFixedJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

		const auto znFixedJoint = new FixedJoint(joint, _znBody0, _znBody1);
		return znFixedJoint;
	}

	PrismaticJoint* ZnFactoryX::CreatePrismaticJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxPrismaticJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

		const auto znPrismaticJoint = new PrismaticJoint(joint, _znBody0, _znBody1, &pxFactory->getTolerancesScale());
		return znPrismaticJoint;
	}

	DistanceJoint* ZnFactoryX::CreateDistanceJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxDistanceJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

		const auto znDistanceJoint = new DistanceJoint(joint, _znBody0, _znBody1);
		return znDistanceJoint;
	}

	SphericalJoint* ZnFactoryX::CreateSphericalJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxSphericalJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

		auto znSphericalJoint = new SphericalJoint(joint, _znBody0, _znBody1);
		return znSphericalJoint;
	}

	HingeJoint* ZnFactoryX::CreateHingeJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxRevoluteJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

		const auto znHingeJoint = new HingeJoint(joint, _znBody0, _znBody1);
		return znHingeJoint;
	}
}
