#pragma once
#include "ZnJoint.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	using namespace Eigen;

	class ZnPrismaticJoint : public ZnJoint
	{
	public:
		ZnPrismaticJoint() = default;
		~ZnPrismaticJoint() override = default;

	public:
		virtual float GetPosition() const = 0;

		virtual float GetVelocity() const = 0;

		virtual void SetLimit(float _lower, float _upper) = 0;
		virtual void SetLimitWithSpring(float _lower, float _upper, float _stiffness, float _damping) = 0;

		virtual void SetLimitEnable(bool) = 0;
		virtual bool IsLimitEnalbed() const = 0;
	};
}
