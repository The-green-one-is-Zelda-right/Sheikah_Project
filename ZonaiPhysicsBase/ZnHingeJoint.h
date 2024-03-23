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
		virtual void GetLimit(float& _upper, float& _lower) const = 0;
		
		// 반발 계수 (리미트)
		virtual float GetRestitution() const = 0;
		virtual void SetRestitution(float _restitution) const = 0;

		// 튕기는 힘 최소치 (리미트)
		virtual float GetBounceThreshold() const = 0;
		virtual void SetBounceThreshold(float _bounceThreshold) const = 0;

		// 스프링 관련 변수
		virtual void SetSpringArg(float _stiffness, float _damping) = 0;
		virtual void GetSpringArg(float& _stiffness, float& _damping) = 0;
		
		// 모터의 각속도
		virtual void SetDriveVelocity(float _velocity) = 0;
		virtual float GetDriveVelocity() const = 0;

		// 모터 힘 제한
		virtual void SetDriveForceLimit(float) = 0;
		virtual float GetDriveForceLimit() const = 0;

		// 기어 비율
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
