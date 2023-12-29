#pragma once
#include "ZnPhysicsBase.h"

namespace ZonaiPhysics
{
	class World : public ZnPhysicsBase
	{
	public:
		World() noexcept = default;
		~World() noexcept override;

		void Initialize() noexcept override;
		void Simulation(float _dt) noexcept override;
		void Finalize() noexcept override;

		ZnRigidBody* CreateRigidBody(const std::wstring&) noexcept override;
		ZnCollider* CreateBoxCollider(const std::wstring&, float x, float y, float z) noexcept override;
		ZnCollider* CreateSphereCollider(const std::wstring&, float radius) noexcept override;
		ZnJoint* CreatFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;
		ZnJoint* CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;
	};
}
