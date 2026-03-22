#pragma once
#include <Eigen/Dense>
#include <vector>

namespace ZonaiPhysics
{
	struct ZnTriangleMeshData
	{
		std::vector<Eigen::Vector3f>	points;
		std::vector<int>				indies;
	};
}
