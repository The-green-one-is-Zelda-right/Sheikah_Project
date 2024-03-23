#pragma once
#include "PurahEngineAPI.h"
#include "ZnFixedJoint.h"
#include "JointT.h"

namespace PurahEngine
{
	class PURAHENGINE_API FixedJoint : public JointT<ZonaiPhysics::ZnFixedJoint>
	{
	public:
		FixedJoint() = default;
		~FixedJoint() override;

	public:
		void Initialize() override;
		void OnDataLoadComplete() override;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		Eigen::Vector3f LocalAnchor;
		Eigen::Quaternionf LocalAnchorRotation;
		Eigen::Vector3f connectedLocalAnchor;
		Eigen::Quaternionf connectedLocalAnchorRotation;
	};
}

