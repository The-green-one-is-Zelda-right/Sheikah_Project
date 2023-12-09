#include "ZonaiMath.h"

#include "PxPhysicsAPI.h"

#include "BoxCollider.h"
#include "RigidBody.h"

#include "ZnPhysicsX.h"


namespace ZonaiPhysics
{
	void ZnPhysicsX::Initialize() noexcept
	{
		using namespace physx;
		foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
		pvd = PxCreatePvd(*foundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);
		PxInitExtensions(*physics, pvd);

		PxSceneDesc sceneDesc(physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		dispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		scene = physics->createScene(sceneDesc);

		PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		material = physics->createMaterial(0.5f, 0.5f, 0.6f);
	}

	void ZnPhysicsX::Simulation(float _dt) noexcept
	{
		scene->simulate(_dt);
		scene->fetchResults(true);
	}

	void ZnPhysicsX::Finalize() noexcept
	{
		using namespace physx;
		PX_RELEASE(scene);
		PX_RELEASE(dispatcher);
		PxCloseExtensions();
		PX_RELEASE(physics);
		if (pvd)
		{
			PxPvdTransport* transport = pvd->getTransport();
			pvd->release();	pvd = NULL;
			PX_RELEASE(transport);
		}
		PX_RELEASE(foundation);
	}

	/// <summary>
	/// 강체를 만들어서 반환
	/// </summary>
	ZnRigidBody* ZnPhysicsX::CreateRigidBody(const std::wstring& _id) noexcept
	{
		RigidBody* result = FindRigidBody(_id);

		if (result == nullptr)
		{
			result = new RigidBody(physics);
			bodies.insert(std::make_pair(_id, result));
			scene->addActor(*result->getRigidDynamic());
		}

		result->getRigidDynamic()->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);

		return result;
	}

	/// <summary>
	/// 강체를 찾아서 거기에 콜라이더를 붙임.
	/// </summary>
	ZnCollider* ZnPhysicsX::CreatBoxCollider(const std::wstring& _id, float x, float y, float z) noexcept
	{
		RigidBody* body = FindRigidBody(_id);

		if (body == nullptr)
		{
			body = new RigidBody(physics);
			body->getRigidDynamic()->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
			bodies.insert(std::make_pair(_id, body));
			scene->addActor(*body->getRigidDynamic());
		}

		Collider* newRigidBody = new BoxCollider(physics, body, ZonaiMath::Vector3D(x, y, z), material);

		return newRigidBody;
	}

	RigidBody* ZnPhysicsX::FindRigidBody(const std::wstring& _id) noexcept
	{
		auto itr = bodies.find(_id);
		if (itr != bodies.end())
		{
			return itr->second;
		}

		return nullptr;
	}

// 	ZnCollider* ZnPhysicsX::CreatPlaneCollider(const std::wstring&, float x, float y) noexcept
// 	{
// 
// 	}
// 
// 	ZnCollider* ZnPhysicsX::CreatSphereCollider(const std::wstring&, float radius) noexcept
// 	{
// 
// 	}
// 
// 	ZnCollider* ZnPhysicsX::CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept
// 	{
// 
// 	}
// 
// 	ZnCollider* ZnPhysicsX::CreateCustomCollider(const std::wstring&) noexcept
// 	{
// 
// 	}

	extern "C"
	{
		ZnPhysicsBase* CreatePhysics()
		{
			return new ZnPhysicsX();
		}
	}

} // namespace ZonaiPhysics