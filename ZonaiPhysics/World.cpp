#include "World.h"

namespace ZonaiPhysics
{
	World::~World()
	= default;

	void World::Initialize()
	{

	}

	void World::Simulation(float _dt)
	{

	}

	void World::Finalize()
	{

	}

	ZnRigidBody* World::CreateRigidBody(const std::wstring&)
	{
	}

	ZnCollider* World::CreateBoxCollider(const std::wstring&, float x, float y, float z)
	{
	}

	ZnCollider* World::CreateSphereCollider(const std::wstring&, float radius)
	{
	}

	ZnJoint* World::CreatFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&)
	{
	}

	ZnJoint* World::CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&)
	{
	}
}
