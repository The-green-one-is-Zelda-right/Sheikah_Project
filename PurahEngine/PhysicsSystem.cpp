#include "PhysicsSystem.h"
#include "../ZonaiPhysicsBase/ZnPhysicsBase.h"
#include <cassert>

namespace PurahEngine
{

	void PhysicsSystem::Initialize() noexcept
	{
		ZonaiPhysicsXDLL = LoadLibrary(L"ZonaiPhysicsX.dll");

		if (ZonaiPhysicsXDLL == nullptr)
		{
			// DLL 로드 실패
			assert(0);
		}

		using ImportFunction = ZonaiPhysics::ZnPhysicsBase* (*) ();
		ImportFunction createZonaiPhysics{ reinterpret_cast<ImportFunction>(GetProcAddress(ZonaiPhysicsXDLL, "CreatePhysics")) };

		if (createZonaiPhysics == nullptr)
		{
			// DLL 함수를 찾을 수 없습니다.
			assert(0);
		}

		physics = createZonaiPhysics();

		if (physics == nullptr)
		{
			assert(0);
		}

		physics->Initialize();
	}

	void PhysicsSystem::Simulation(float _dt) noexcept
	{
		physics->Simulation(_dt);
	}

	void PhysicsSystem::Finalize() noexcept
	{
		physics->Finalize();

		FreeLibrary(ZonaiPhysicsXDLL);
	}

	ZonaiPhysics::ZnRigidBody* PhysicsSystem::CreateRigidBody(const std::wstring& _id) noexcept
	{
		return physics->CreateRigidBody(_id);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateBoxCollider(const std::wstring& _id, float x, float y, float z) noexcept
	{
		return physics->CreatBoxCollider(_id, x, y, z);
	}

	PurahEngine::PhysicsSystem& PurahEngine::PhysicsSystem::GetInstance()
	{
		static PhysicsSystem instance;
		return instance;
	}

	//ZonaiPhysics::ZnCollider* PhysicsSystem::CreatPlaneCollider(const std::wstring& _id, float x, float y) noexcept
	//{
	//	//return physics->CreatPlaneCollider(_id, x, y);
	//}

	//ZonaiPhysics::ZnCollider* PhysicsSystem::CreatSphereCollider(const std::wstring& _id, float radius) noexcept
	//{
	//	//return physics->CreatSphereCollider(_id, radius);
	//}

	//ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCapsuleCollider(const std::wstring& _id, float radius, float height) noexcept
	//{
	//	//return physics->CreateCapsuleCollider(_id, radius, height);
	//}

	//ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCustomCollider(const std::wstring& _id) noexcept
	//{
	//	//return physics->CreateCustomCollider(_id);
	//}

}