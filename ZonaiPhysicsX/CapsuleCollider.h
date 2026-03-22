#pragma once
#include "Collider.h"

namespace physx
{
	class PxShape;
}

namespace ZonaiPhysics
{
	class RigidBody;

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider() = delete;
		CapsuleCollider(physx::PxShape*, RigidBody*);
		~CapsuleCollider() override;

		Eigen::Quaternionf			GetLocalQuaternion() const override;
		void						SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) override;
	};
} // ZonaiPhysics

