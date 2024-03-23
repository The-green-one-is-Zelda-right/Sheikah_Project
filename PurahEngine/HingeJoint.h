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
		void Initialize() override;
		void OnDataLoadComplete() override;

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
