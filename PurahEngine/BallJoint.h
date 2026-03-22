#pragma once
#include "PurahEngineAPI.h"
#include "JointT.h"
#include "ZnSphericalJoint.h"

namespace PurahEngine
{
	class PURAHENGINE_API BallJoint : public JointT<ZonaiPhysics::ZnSphericalJoint>
	{
	public:
		BallJoint() = default;
		~BallJoint() override;

	public:
		void PostInitialize() override;

	public:
		void GetLimitAngle(float& _outY, float& _outZ) const;

		float GetYAngle() const;
		float GetZAngle() const;

		void LimitEnable(bool _value) const;

		// 회전 '각도' 제한
		void SetLimitAngle(float _yAngle, float _zAngle) const;
		void SetLimitRadian(float _yAngle, float _zAngle) const;
		void SetSpringArg(float _stiffness, float _damping) const;
		void SetBounciness(float _bounciness) const;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		bool useSpring = false;
		float spring = 0.0f;
		float damping = 0.0f;

		bool useLimit = false;
		float swingLimitY = 40.0f;
		float swingLimitZ = 40.0f;
		float bounciness = 0.0f;
	};
}
