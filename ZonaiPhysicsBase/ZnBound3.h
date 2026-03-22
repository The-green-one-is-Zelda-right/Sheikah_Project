#pragma once
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnBound3
	{
		ZnBound3() 
			: minimum(), maximum()
		{}

		ZnBound3(const Eigen::Vector3f& min, const Eigen::Vector3f& max) 
			: minimum(min), maximum(max)
		{}

		ZnBound3(const ZnBound3& bound)
			: minimum(bound.minimum), maximum(bound.maximum)
		{}

		Eigen::Vector3f minimum;
		Eigen::Vector3f maximum;
	};
}