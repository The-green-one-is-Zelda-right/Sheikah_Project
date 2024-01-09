#pragma once
#include "ZnJoint.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	using namespace Eigen;

	class ZnPrismaticJoint : public ZnJoint
	{
	public:
		ZnPrismaticJoint() noexcept = default;
		~ZnPrismaticJoint() noexcept override = default;
	};
}
