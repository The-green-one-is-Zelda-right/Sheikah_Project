#pragma once
#include <string>
#include <windows.h>

namespace ZonaiPhysics
{
	class ZnPhysicsBase;
	class ZnRigidBody;
	class ZnCollider;
}

namespace PurahEngine
{
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
	public:
		void Initialize() noexcept;
		void Simulation(float _dt) noexcept;
		void Finalize() noexcept;

	public:
		ZonaiPhysics::ZnRigidBody*	CreateRigidBody(const std::wstring&) noexcept;

		ZonaiPhysics::ZnCollider*	CreateBoxCollider(const std::wstring&, float x, float y, float z) noexcept;
		ZonaiPhysics::ZnCollider*	CreatPlaneCollider(const std::wstring&, float x, float y) noexcept;
		ZonaiPhysics::ZnCollider*	CreatSphereCollider(const std::wstring&, float radius) noexcept;
		ZonaiPhysics::ZnCollider*	CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept;
		ZonaiPhysics::ZnCollider*	CreateCustomCollider(const std::wstring&) noexcept;

	public:
		static PhysicsSystem& GetInstance();
	};
}
