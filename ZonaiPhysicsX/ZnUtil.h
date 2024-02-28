#pragma once
#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)


namespace ZonaiPhysics
{
	Eigen::Vector3f PhysxToEigen(const physx::PxVec3& _vec);

	Eigen::Quaternionf PhysxToEigen(const physx::PxQuat& _quat);

	physx::PxVec3 EigenToPhysx(const Eigen::Vector3f& _vec);

	physx::PxQuat EigenToPhysx(const Eigen::Quaternionf& _quat);
}
