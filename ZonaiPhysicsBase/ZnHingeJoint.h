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
		// ���� ����
		virtual float GetAngle() const = 0;
		// ���� �ӵ�
		virtual float GetVelocity() const = 0;

		// ���� ����
		virtual void SetLimit(float _lower, float _upper) = 0;
		virtual void GetLimit(float& _upper, float& _lower) const = 0;
		
		// �ݹ� ��� (����Ʈ)
		virtual float GetRestitution() const = 0;
		virtual void SetRestitution(float _restitution) const = 0;

		// ƨ��� �� �ּ�ġ (����Ʈ)
		virtual float GetBounceThreshold() const = 0;
		virtual void SetBounceThreshold(float _bounceThreshold) const = 0;

		// ������ ���� ����
		virtual void SetSpringArg(float _stiffness, float _damping) = 0;
		virtual void GetSpringArg(float& _stiffness, float& _damping) = 0;
		
		// ������ ���ӵ�
		virtual void SetDriveVelocity(float _velocity) = 0;
		virtual float GetDriveVelocity() const = 0;

		// ���� �� ����
		virtual void SetDriveForceLimit(float) = 0;
		virtual float GetDriveForceLimit() const = 0;

		// ��� ����
		virtual void SetDriveGearRatio(float) = 0;
		virtual float GetDriveGearRatio() const = 0;

		// ���� Ȱ��ȭ
		virtual void SetLimitEnable(bool) = 0;
		// ���� Ȱ��ȭ
		virtual void SetDriveEnable(bool) = 0;
		// ���� �ӵ��� ������ �ӵ��� �ʰ��ϴ� ��� ���� �߰����� ����
		virtual void SetDriveFreespin(bool) = 0;
	};
}
