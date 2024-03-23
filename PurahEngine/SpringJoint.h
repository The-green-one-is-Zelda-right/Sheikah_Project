#pragma once
#include "PurahEngineAPI.h"
#include "JointT.h"
#include "ZnDistanceJoint.h"

namespace PurahEngine
{
	class PURAHENGINE_API SpringJoint : public JointT<ZonaiPhysics::ZnDistanceJoint>
	{
	public:
		SpringJoint() = default;
		~SpringJoint() override;

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

		Eigen::Vector3f connectedLocalAnchor;

		bool useSpring = false;
		float spring = 10.0f;
		float damper = 0.2f;

		bool useMinDistance = true;
		float minDistance = 0.f;
		bool useMaxDistance = true;
		float maxDistance = 0.f;
		float Tolerance = 0.025f;
	};
}

