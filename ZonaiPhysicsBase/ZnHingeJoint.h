#pragma once
#include "ZnJoint.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	using namespace Eigen;

	class ZnHingeJoint : public ZnJoint
	{
	public:
		ZnHingeJoint() noexcept = default;
		~ZnHingeJoint() noexcept override = default;

	public:
	};
}
