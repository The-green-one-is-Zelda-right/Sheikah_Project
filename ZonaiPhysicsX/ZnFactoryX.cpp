#include "FilterCallback.h"

#include "RigidBodyHelper.h"
#include "RigidBody.h"

#include "FixedJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "SphericalJoint.h"
#include "HingeJoint.h"
#include "SpringFlexJoint.h"

#include "ZnUtil.h"

#include "ZnTransform.h"
#include "ZnControllerDecs.h"

#include "ZnFactoryX.h"

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
	physx::PxOmniPvd* ZnFactoryX::omniPvd = nullptr;
	// physx::PxCookingParams* ZnFactoryX::pxCooking = nullptr;

	void ZnFactoryX::CreatePhysxFactory()
	{
		/// SDK 생성
		{
			foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
#if PX_SUPPORT_PVD
			pxPvd = PxCreatePvd(*foundation);
			physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
			pxPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

			pxFactory = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pxPvd);

			PxInitExtensions(*pxFactory, pxPvd);
#endif

#if PX_SUPPORT_OMNI_PVD
			omniPvd = PxCreateOmniPvd(*foundation);
			if (!omniPvd)
			{
				printf("Error : could not create PxOmniPvd!");
				return;
			}
			OmniPvdWriter* omniWriter = omniPvd->getWriter();
			if (!omniWriter)
			{
				printf("Error : could not get an instance of PxOmniPvdWriter!");
				return;
			}
			OmniPvdFileWriteStream* fStream = omniPvd->getFileWriteStream();
			if (!fStream)
			{
				printf("Error : could not get an instance of PxOmniPvdFileWriteStream!");
				return;
			}
			fStream->setFileName("gOmniPvdName");
			omniWriter->setWriteStream(static_cast<OmniPvdWriteStream&>(*fStream));

			pxFactory = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, NULL, omniPvd);
			if (!pxFactory)
			{
				printf("Error : could not create a PhysX instance!");
				return;
			}

			if (pxFactory->getOmniPvd())
			{
				pxFactory->getOmniPvd()->startSampling();
			}
			else
			{
				printf("Error : could not start OmniPvd sampling!");
				return;
			}
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

#ifdef PX_SUPPORT_PVD
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 10.f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 10.f);

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

	physx::PxMaterial* ZnFactoryX::CreateMaterial(const MaterialDesc& _desc)
	{
		physx::PxMaterial* newMaterial = pxFactory->createMaterial(
			_desc.dynamicFriction, _desc.staticFriction, _desc.restitution);

		newMaterial->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(_desc.eFriction));
		newMaterial->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(_desc.eRestitution));

		return newMaterial;
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

	physx::PxControllerManager* ZnFactoryX::CreateControllerManager(physx::PxScene* _scene)
	{
		physx::PxControllerManager* manager = PxCreateControllerManager(*_scene);

		return manager;
	}

	physx::PxCapsuleController* ZnFactoryX::CreateCapsuleController(
		physx::PxControllerManager* _manager, 
		float _radius, float _height, 
		const ZnControllerDecs& _desc, 
		float _density, 
		physx::PxMaterial* _material)
	{
		using namespace physx;

		PxCapsuleControllerDesc desc;
		desc.height = _height;
		desc.radius = _radius;
		desc.position = PxExtendedVec3( _desc.position.x(), _desc.position.y(), _desc.position.z());
		desc.upDirection = EigenToPhysx(_desc.upDirection);
		desc.stepOffset = _desc.stepOffset;
		desc.slopeLimit = physx::PxDegToRad(_desc.slopeLimitDeg);
		desc.contactOffset = _desc.contactOffset;
		desc.density = _density;
		desc.material = _material;
		PxController* controller = _manager->createController(desc);
		PxCapsuleController* capsuleController = dynamic_cast<PxCapsuleController*>(controller);

		return capsuleController;
	}

	physx::PxBoxController* ZnFactoryX::CreateBoxController(
		physx::PxControllerManager* _manager, 
		const Eigen::Vector3f& _extend, 
		const ZnControllerDecs& _desc,
		float _density, 
		physx::PxMaterial* _material)
	{
		using namespace physx;

		PxBoxControllerDesc desc;
		desc.halfSideExtent = _extend.x();
		desc.halfHeight = _extend.y();
		desc.halfForwardExtent = _extend.z();
		desc.position = PxExtendedVec3(_desc.position.x(), _desc.position.y(), _desc.position.z());
		desc.upDirection = EigenToPhysx(_desc.upDirection);
		desc.stepOffset = _desc.stepOffset;
		desc.slopeLimit = PxDegToRad(_desc.slopeLimitDeg);
		desc.contactOffset = _desc.contactOffset;
		desc.density = _density;
		desc.material = _material;
		PxController* controller = _manager->createController(desc);
		PxBoxController* boxController = dynamic_cast<PxBoxController*>(controller);

		return boxController;
	}

	physx::PxShape* ZnFactoryX::CreateBoxShape(const Eigen::Vector3f& _extend, const physx::PxMaterial* _material)
	{
		return pxFactory->createShape(physx::PxBoxGeometry(_extend.x(), _extend.y(), _extend.z()), *_material, true);
	}

	physx::PxShape* ZnFactoryX::CreateSphereShape(float _radius, const physx::PxMaterial* _material)
	{
		return pxFactory->createShape(physx::PxSphereGeometry(_radius), *_material, true);
	}

	physx::PxShape* ZnFactoryX::CreateCapsuleShape(float _radius, float _height, const physx::PxMaterial* _material)
	{
		physx::PxShape* shape = pxFactory->createShape(physx::PxCapsuleGeometry(_radius, _height), *_material, true);

		const physx::PxQuat rotation(physx::PxPi / 2.f, physx::PxVec3(0.f, 0.f, 1.f));
		shape->setLocalPose(physx::PxTransform(rotation));

		return shape;
	}

	physx::PxShape* ZnFactoryX::CreateTriagleMeshShape(physx::PxTriangleMesh* _mesh, const Eigen::Vector3f& _scale,
		const physx::PxMaterial* _material)
	{
		physx::PxMeshScale scale(EigenToPhysx(_scale), EigenToPhysx(Quaternionf::Identity()));
		physx::PxTriangleMeshGeometry geom(_mesh, scale);
		physx::PxShape* pxShape = pxFactory->createShape(geom, *_material, true);

		return pxShape;
	}

	physx::PxShape* ZnFactoryX::CreateConvexMeshShape(physx::PxConvexMesh* _mesh, const Eigen::Vector3f& _scale,
		const physx::PxMaterial* _material)
	{
		physx::PxMeshScale scale(EigenToPhysx(_scale), EigenToPhysx(Quaternionf::Identity()));
		physx::PxConvexMeshGeometry geom(_mesh, scale);
		physx::PxShape* pxShape = pxFactory->createShape(geom, *_material, true);
		
		return pxShape;
	}

#include "Bunny.h"

	physx::PxTriangleMesh* ZnFactoryX::CookTriagleMesh(FBXLoader::Model* _model)
	{
		using namespace physx;

		std::vector<physx::PxVec3> vertices;
		std::vector<physx::PxU32> indies;

		physx::PxU32 accIndex = 0;

		for (size_t i = 0; i < _model->meshList.size(); i++)
		{
			auto& mesh = _model->meshList[i];
			auto& points = mesh->vertices;
			auto& index = mesh->indices;

			for (size_t j = 0; j < points.size(); j++)
			{
				vertices.emplace_back(points[j].position.x, points[j].position.y, points[j].position.z);
			}

			for (size_t j = 0; j < index.size(); j++)
			{
				indies.emplace_back(accIndex + index[j]);
			}

			accIndex += index.size();
		}

		// 정점 정보
		PxTriangleMeshDesc meshDesc;

		meshDesc.points.count = (physx::PxU32)vertices.size();
		meshDesc.points.stride = sizeof(Eigen::Vector3f);
		meshDesc.points.data = &vertices.front();

		meshDesc.triangles.count = (physx::PxU32)(indies.size() / 3);
		meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
		meshDesc.triangles.data = &indies.front();

		const PxTolerancesScale scale;
		PxCookingParams params(scale);
		params.midphaseDesc.setToDefault(PxMeshMidPhase::eBVH34);


		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum result;
		const bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
		if (!status)
		{
			ZONAI_CAUTUON(ZonaiPhysics, Create Mesh Collider Error!)
			return NULL;
		}

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh* triangleMesh = pxFactory->createTriangleMesh(readBuffer);

		return triangleMesh;
	}

	physx::PxConvexMesh* ZnFactoryX::CookConvexMesh(FBXLoader::Model* _model)
	{
		std::vector<physx::PxVec3> vertices;
		
		for (size_t i = 0; i < _model->meshList.size(); i++)
		{
			auto& mesh = _model->meshList[i];
			auto& points = mesh->vertices;

			for (size_t i = 0; i < points.size(); i++)
			{
				vertices.emplace_back(points[i].position.x, points[i].position.y, points[i].position.z);
			}
		}

		// 정점 정보
		physx::PxConvexMeshDesc convexDesc;
		convexDesc.points.count = vertices.size();
		convexDesc.points.stride = sizeof(physx::PxVec3);
		convexDesc.points.data = &vertices.front();
		convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxCookingParams params(pxFactory->getTolerancesScale());
		params.midphaseDesc.setToDefault(physx::PxMeshMidPhase::eBVH34);

		// 정점 메모리 버퍼
		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		const bool status = PxCookConvexMesh(params, convexDesc, buf, &result);
		if (!status)
		{
			ZONAI_CAUTUON(ZonaiPhysics, Create Convex Collider Error!)
			return nullptr;
		}

		// 버퍼를 바탕으로 Mesh 생성
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convexMesh = pxFactory->createConvexMesh(input);

		return convexMesh;
	}

	FixedJoint* ZnFactoryX::CreateFixedJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxFixedJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znFixedJoint = new FixedJoint(joint, _znBody0, _znBody1);
		return znFixedJoint;
	}

	PrismaticJoint* ZnFactoryX::CreatePrismaticJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxPrismaticJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znPrismaticJoint = new PrismaticJoint(joint, _znBody0, _znBody1, &pxFactory->getTolerancesScale());
		return znPrismaticJoint;
	}

	DistanceJoint* ZnFactoryX::CreateDistanceJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxDistanceJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znDistanceJoint = new DistanceJoint(joint, _znBody0, _znBody1);
		return znDistanceJoint;
	}

	SphericalJoint* ZnFactoryX::CreateSphericalJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxSphericalJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		auto znSphericalJoint = new SphericalJoint(joint, _znBody0, _znBody1);
		return znSphericalJoint;
	}

	HingeJoint* ZnFactoryX::CreateHingeJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxRevoluteJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znHingeJoint = new HingeJoint(joint, _znBody0, _znBody1);
		return znHingeJoint;
	}

	SpringFlexJoint* ZnFactoryX::CreateSpringFlexJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = new D1SpringFlexJoint(*pxFactory, *pxbody0, t0, *pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znSpringFlex = new SpringFlexJoint(joint, _znBody0, _znBody1);
		return znSpringFlex;
	}
}
