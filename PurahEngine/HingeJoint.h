#pragma once
#include "PurahEngineAPI.h"
#include "JointT.h"
#include "ZnHingeJoint.h"

namespace PurahEngine
{
	class PURAHENGINE_API HingeJoint : public JointT<ZonaiPhysics::ZnHingeJoint>
	{
	public:
		HingeJoint() = default;
		~HingeJoint() override;

	public:
		void PostInitialize() override;

	public:
		// ���� ����
		float GetAngle() const;
		// ���� �ӵ�
		float GetVelocity() const;

		// ���� ����
		void SetLimit(float _lower, float _upper) const;
		void GetLimit(float& _upper, float& _lower) const;

		// �ݹ� ��� (����Ʈ)
		float GetRestitution() const;
		void SetRestitution(float _restitution) const;

		// ƨ��� �� �ּ�ġ (����Ʈ)
		float GetBounceThreshold() const;
		void SetBounceThreshold(float _bounceThreshold) const;

		// ������ ���� ����
		void SetSpringArg(float _stiffness, float _damping) const;
		void GetSpringArg(float& _stiffness, float& _damping) const;

		// ������ ���ӵ�
		void SetDriveVelocity(float _velocity) const;
		float GetDriveVelocity() const;

		// ���� �� ����
		void SetDriveForceLimit(float) const;
		float GetDriveForceLimit() const;

		// ��� ����
		void SetDriveGearRatio(float) const;
		float GetDriveGearRatio() const;

		// ���� Ȱ��ȭ
		void SetLimitEnable(bool) const;
		// ���� Ȱ��ȭ
		void SetDriveEnable(bool) const;
		// ���� �ӵ��� ������ �ӵ��� �ʰ��ϴ� ��� ���� �߰����� ����
		void SetDriveFreespin(bool) const;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
        bool useSpring = false;
        float spring = 0.0f;
        float damper = 0.0f;
        float targetPosition = 0.0f;

        bool setMotor = false;
        float velocity = 0.0f;
        float force = 0.0f;
        bool freeSpin = false;

        bool useLimit = false;
        float angleLower = 0.0f;
        float angleUpper = 0.0f;
        float bounciness = 0.0f;
        float bounceThreshold = 0.5f;
	};
}
