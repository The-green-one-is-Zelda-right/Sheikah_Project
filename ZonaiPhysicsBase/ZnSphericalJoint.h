#pragma once
#include "ZnJoint.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ZnSphericalJoint : public ZnJoint
	{
	public:
					ZnSphericalJoint() = default;
					~ZnSphericalJoint() override = default;

	public:
		virtual void GetLimitAngle(float& _outY, float& _outZ) = 0;

		virtual float GetYAngle() const = 0;
		virtual float GetZAngle() const = 0;

		virtual void LimitEnable(bool) = 0;

		// 회전 '각도' 제한
		virtual void SetLimitCone(float _yAngle, float _zAngle) = 0;
		virtual void SetSpringArg(float _stiffness, float _damping) = 0;
	};
}
