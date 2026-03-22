#pragma once
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnQueryDesc
	{
		Eigen::Vector3f position = Eigen::Vector3f::Zero();
		Eigen::Quaternionf rotation = Eigen::Quaternionf::Identity();
		Eigen::Vector3f direction = Eigen::Vector3f::Zero();
		float distance = 0.f;
		unsigned long queryLayer;
		float offset = 0.f;
	};
}