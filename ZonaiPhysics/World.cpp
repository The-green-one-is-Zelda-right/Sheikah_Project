#include "World.h"

namespace ZonaiPhysics
{
	World::~World() noexcept
	= default;

	void World::Initialize() noexcept
	{

	}

	void World::Simulation(float _dt) noexcept
	{

	}

	void World::Finalize() noexcept
	{

	}

	ZnRigidBody* World::CreateRigidBody(const std::wstring&) noexcept
	{
	}

	ZnCollider* World::CreateBoxCollider(const std::wstring&, float x, float y, float z) noexcept
	{
	}

	ZnCollider* World::CreateSphereCollider(const std::wstring&, float radius) noexcept
	{
	}

	ZnJoint* World::CreatFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept
	{
	}

	ZnJoint* World::CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept
	{
	}
}
