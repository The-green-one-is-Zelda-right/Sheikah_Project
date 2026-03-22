#pragma once
#include "PurahEngineAPI.h"
#include "JointT.h"
#include "ZnSpringFlexJoint.h"

namespace PurahEngine
{
	class PURAHENGINE_API SpringFlexJoint : public JointT<ZonaiPhysics::ZnSpringFlexJoint>
	{
	public:
		SpringFlexJoint() = default;
		~SpringFlexJoint() override;

	public:
		void PostInitialize() override;

	public:
		void PreSerialize(json& jsonData) const override {};
		void PreDeserialize(const json& jsonData) override {};
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}

