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
		virtual void SetMinDistance(float _lower) = 0;
		virtual void SetMaxDistance(float _upper) = 0;
		virtual void SetSpringArg(float _stiffness, float _damping) = 0;

		virtual void SetRestitution(float _restitution) const = 0;
		virtual float GetRestitution() const = 0;

		virtual void SetLimitEnable(bool) = 0;
		virtual bool IsLimitEnabled() const = 0;
	};
}
