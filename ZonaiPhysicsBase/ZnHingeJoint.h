#pragma once
#include "ZnJoint.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	using namespace Eigen;

	//
	class ZnHingeJoint : public ZnJoint
	{
	public:
		ZnHingeJoint() = default;
		~ZnHingeJoint() override = default;

	public:
		// 관절 각도
		virtual float GetAngle() const = 0;
		// 관절 속도
		virtual float GetVelocity() const = 0;

		// 관절 제한
		virtual void SetLimit(float _lower, float _upper) = 0;
		// 관절 제한(스프링)
		virtual void SetLimitWithSpring(float _lower, float _upper, float _stiffness, float _damping) = 0;

		// 모터의 각속도
		virtual void SetDriveVelocity(float _velocity) = 0;
		virtual float GetDriveVelocity() const = 0;

		// 모터 힘 제한
		virtual void SetDriveForceLimit(float) = 0;
		virtual float GetDriveForceLimit() const = 0;

		// 
		virtual void SetDriveGearRatio(float) = 0;
		virtual float GetDriveGearRatio() const = 0;

		// 제한 활성화
		virtual void SetLimitEnable(bool) = 0;
		// 모터 활성화
		virtual void SetDriveEnable(bool) = 0;
		// 기존 속도가 모터의 속도를 초과하는 경우 힘을 추가하지 않음
		virtual void SetDriveFreespin(bool) = 0;
	};
}
