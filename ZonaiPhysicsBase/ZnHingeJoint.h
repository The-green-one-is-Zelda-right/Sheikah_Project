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
		ZnHingeJoint() noexcept = default;
		~ZnHingeJoint() noexcept override = default;

	public:
		// ���� ����
		virtual float GetAngle() const = 0;
		// ���� �ӵ�
		virtual float GetVelocity() const = 0;

		// ���� ����
		virtual void SetLimit(float _lower, float _upper) = 0;
		// ���� ����(������)
		virtual void SetLimit(float _lower, float _upper, float _stiffness, float _damping) = 0;

		// 
		virtual void SetDriveVelocity(float _velocity) = 0;
		// 
		virtual float GetDriveVelocity() const = 0;

		// 
		virtual void SetDriveForceLimit(float) = 0;
		// 
		virtual float GetDriveForceLimit() const = 0;

		// 
		virtual void SetDriveGearRatio(float) = 0;
		// 
		virtual float GetDriveGearRatio() const = 0;

		// 
		virtual void SetLimitEnable(bool) = 0;
		// 
		virtual void SetDriveEnable(bool) = 0;
		// 
		virtual void SetDriveFreespin(bool) = 0;
	};
}
