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
		void Initialize() override;
		void OnDataLoadComplete() override;

	public:
		void GetLimitAngle(float& _outY, float& _outZ) const;

		float GetYAngle() const;
		float GetZAngle() const;

		void LimitEnable(bool _value) const;

		// 회전 '각도' 제한
		void SetLimitCone(float _yAngle, float _zAngle) const;
		void SetSpringArg(float _stiffness, float _damping) const;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		Eigen::Vector3f LocalAnchor;
		Eigen::Quaternionf LocalAnchorRotation;

		Eigen::Vector3f connectedLocalAnchor;
		Eigen::Quaternionf connectedLocalAnchorRotation;

		bool useSpring = false;
		float spring = 0.0f;
		float damping = 0.0f;

		bool useLimit = false;
		float swingLimitY = 40.0f;
		float swingLimitZ = 40.0f;
		float bounciness = 0.0f;
	};
}
