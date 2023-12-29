#pragma once
#include <Eigen/Dense>

#include "RigidBody.h"

namespace ZonaiPhysics
{
	class Collider
	{
	public:
		Eigen::Vector3f localPosition;			// local
		Eigen::Quaternionf localRotation;		// local

		RigidBody* body;
		void* userData;
	};
}
