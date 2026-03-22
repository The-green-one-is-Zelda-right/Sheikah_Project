#pragma once
#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#include "windows.h"
#include "FBXData.h"
#pragma warning (pop)

#define ZONAI_CAUTUON(string, heder)	\
MessageBox(0,							\
	L#string,							\
	L#heder,							\
	MB_OK | MB_ICONEXCLAMATION			\
);

#define NULL_POINTER_REFERENCE(pointer, error_path)		\
if(pointer == nullptr)									\
{														\
	ZONAI_CAUTUON(Null Pointer Reference, error_path)	\
};

namespace ZonaiPhysics
{
	Eigen::Vector3f PhysxToEigen(const physx::PxVec3& _vec);

	Eigen::Quaternionf PhysxToEigen(const physx::PxQuat& _quat);

	physx::PxVec3 LoadDataToPhysX(const FBXLoader::Float3& _point);

	physx::PxQuat LoadDataToPhysX(const FBXLoader::Quaternion& _quat);

	physx::PxVec3 EigenToPhysx(const Eigen::Vector3f& _vec);

	physx::PxQuat EigenToPhysx(const Eigen::Quaternionf& _quat);


}
