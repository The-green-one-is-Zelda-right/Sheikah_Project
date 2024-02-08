#pragma once
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>


namespace ZonaiPhysics
{
	Eigen::Vector3f PhysxToEigen(const physx::PxVec3& _vec);

	Eigen::Quaternionf PhysxToEigen(const physx::PxQuat& _quat);

	physx::PxVec3 EigenToPhysx(const Eigen::Vector3f& _vec);

	physx::PxQuat EigenToPhysx(const Eigen::Quaternionf& _quat);
}
