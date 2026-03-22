#pragma once
#include <Eigen/Dense>
#include "ZnResourceID.h"

namespace ZonaiPhysics
{
	struct ZnControllerDecs
	{
		Eigen::Vector3f position = Eigen::Vector3f::Zero();
		float stepOffset = 0.1f;
		float contactOffset = 0.1f;
		Eigen::Vector3f upDirection = Eigen::Vector3f::UnitY();
		float slopeLimitDeg = 60.f;
	};
}