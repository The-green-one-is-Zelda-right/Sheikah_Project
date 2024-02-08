#pragma once
#include "ZnPhysicsBase.h"

namespace ZonaiPhysics
{
	class World : public ZnPhysicsBase
	{
	public:
		World() = default;
		~World() override;

		void Initialize() override;
		void Simulation(float _dt) override;
		void Finalize() override;

		ZnRigidBody* CreateRigidBody(const std::wstring&) override;
		ZnCollider* CreateBoxCollider(const std::wstring&, float x, float y, float z) override;
		ZnCollider* CreateSphereCollider(const std::wstring&, float radius) override;
		ZnJoint* CreatFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) override;
		ZnJoint* CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) override;
	};
}
