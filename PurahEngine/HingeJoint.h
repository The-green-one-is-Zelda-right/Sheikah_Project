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
		// 관절 각도
		float GetAngle() const;
		// 관절 속도
		float GetVelocity() const;

		// 관절 제한
		void SetLimit(float _lower, float _upper) const;
		void GetLimit(float& _upper, float& _lower) const;

		// 반발 계수 (리미트)
		float GetRestitution() const;
		void SetRestitution(float _restitution) const;

		// 튕기는 힘 최소치 (리미트)
		float GetBounceThreshold() const;
		void SetBounceThreshold(float _bounceThreshold) const;

		// 스프링 관련 변수
		void SetSpringArg(float _stiffness, float _damping) const;
		void GetSpringArg(float& _stiffness, float& _damping) const;

		// 모터의 각속도
		void SetDriveVelocity(float _velocity) const;
		float GetDriveVelocity() const;

		// 모터 힘 제한
		void SetDriveForceLimit(float) const;
		float GetDriveForceLimit() const;

		// 기어 비율
		void SetDriveGearRatio(float) const;
		float GetDriveGearRatio() const;

		// 제한 활성화
		void SetLimitEnable(bool) const;
		// 모터 활성화
		void SetDriveEnable(bool) const;
		// 기존 속도가 모터의 속도를 초과하는 경우 힘을 추가하지 않음
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
