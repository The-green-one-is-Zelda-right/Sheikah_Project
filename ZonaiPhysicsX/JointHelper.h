#pragma once
#include <windows.h>

#include "ZnJoint.h"

namespace ZonaiPhysics
{

	class JointHelper
	{
	public:
		static void Release(void*);

		static void SetLocalPosition(ZnJoint::eOBJECT _index, const Eigen::Vector3f& _localPos);
		static Eigen::Vector3f GetLocalPosition(ZnJoint::eOBJECT _index);

		static void SetLocalQuaternion(ZnJoint::eOBJECT _index, const Quaternionf& _localQuat);
		static Quaternionf GetLocalQuaternion(ZnJoint::eOBJECT _index);

		static Eigen::Vector3f GetRelativeLinearVelocity();
		static Eigen::Vector3f GetRelativeAngularVelocity();

		static void SetBreakForce(float _force, float _torque);

	};
}

