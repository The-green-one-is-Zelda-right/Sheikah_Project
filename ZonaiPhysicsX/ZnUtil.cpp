#include "ZnUtil.h"

namespace ZonaiPhysics
{
	Eigen::Vector3f PhysxToEigen(const physx::PxVec3& _vec)
	{
		return { _vec.x, _vec.y ,_vec.z };
	}

	Eigen::Quaternionf PhysxToEigen(const physx::PxQuat& _quat)
	{
		return { _quat.w, _quat.x, _quat.y ,_quat.z };
	}

	physx::PxVec3 EigenToPhysx(const Eigen::Vector3f& _vec)
	{
		return { _vec.x(), _vec.y() ,_vec.z() };
	}

	physx::PxQuat EigenToPhysx(const Eigen::Quaternionf& _quat)
	{
		return { _quat.x(), _quat.y(), _quat.z() ,_quat.w() };
	}
}
