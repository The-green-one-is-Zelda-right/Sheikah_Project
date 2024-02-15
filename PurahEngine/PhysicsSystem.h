#pragma once
#include <vector>
#include <windows.h>

#include "ColliderBase.h"

namespace PurahEngine
{
	class EventCallbackSystem;

	// template <ColliderType T> class Collider;
	// extern typedef Collider<ColliderType::DYNAMIC> DynamicCollider;
	// extern typedef Collider<ColliderType::STATIC> StaticCollider;
}

namespace ZonaiPhysics
{
	class Joint;
	class ZnPhysicsBase;
	class ZnRigidBody;
	class ZnCollider;
}

namespace PurahEngine
{
	// class Collider;
	class RigidBody;
	class Joint;

	class PhysicsSystem
	{
	public:
		PhysicsSystem() noexcept = default;
		~PhysicsSystem() noexcept = default;

		PhysicsSystem(const PhysicsSystem&) noexcept = delete;
		PhysicsSystem(PhysicsSystem&&) noexcept = delete;

		PhysicsSystem& operator=(const PhysicsSystem&) noexcept = delete;
		PhysicsSystem& operator=(PhysicsSystem&&) noexcept = delete;

	private:
		ZonaiPhysics::ZnPhysicsBase* physics;
		HMODULE ZonaiPhysicsXDLL;
		std::function<void()> releaseFuntion;

	public:
		void Initialize() noexcept;
		void PreStep() const;
		void Simulation(float _dt) const noexcept;
		void SimulateResult() const;
		void Finalize() const noexcept;

	public:
		void FreeObject(void*) const;

	public:
		std::vector<RigidBody*> bodies;
		std::vector<ColliderBase*> dynamicColliders;
		std::vector<ColliderBase*> staticColliders;
		std::vector<Joint*> joints;
		EventCallbackSystem* callbackSystem;

	public:
		ZonaiPhysics::ZnRigidBody*	CreateRigidBody(void* _gameObject) const noexcept;

		ZonaiPhysics::ZnCollider*	CreateBoxCollider(void* _gameObject, float x, float y, float z) const noexcept;
		// ZonaiPhysics::ZnCollider*	CreatPlaneCollider(void* _gameObject, float x, float y) noexcept;
		ZonaiPhysics::ZnCollider*	CreateSphereCollider(void* _gameObject, float radius) const noexcept;
		ZonaiPhysics::ZnCollider*	CreateCapsuleCollider(void* _gameObject, float radius, float height) const noexcept;
		// ZonaiPhysics::ZnCollider*	CreateCustomCollider(void* _gameObject) noexcept;

	public:
		static PhysicsSystem& GetInstance();
	};
}
