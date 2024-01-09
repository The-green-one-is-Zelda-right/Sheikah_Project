#pragma once
#include "ZnJoint.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	using namespace Eigen;

	class ZnFixedJoint : public ZnJoint
	{
	public:
					ZnFixedJoint() noexcept = default;
					~ZnFixedJoint() noexcept override = default;
	};
}
