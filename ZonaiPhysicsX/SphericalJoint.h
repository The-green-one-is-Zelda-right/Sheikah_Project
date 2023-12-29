#pragma once
#include <Eigen/Dense>
#include "ZnSphericalJoint.h"

namespace physx
{
	class PxSphericalJoint;
}

namespace ZonaiPhysics
{
	class SphericalJoint : public ZnSphericalJoint
	{
	public:
		SphericalJoint() noexcept;
		virtual ~SphericalJoint() noexcept = default;

	public:

	private:
		physx::PxSphericalJoint* joint;
	};
}

