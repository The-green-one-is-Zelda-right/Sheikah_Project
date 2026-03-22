#pragma once
#include "PurahEngineAPI.h"
#include "JointT.h"
#include "ZnPrismaticJoint.h"	

namespace PurahEngine
{
	class PURAHENGINE_API SlideJoint : public JointT<ZonaiPhysics::ZnPrismaticJoint>
	{
	public:
		SlideJoint() = default;
		~SlideJoint() override;

	public:
		void PostInitialize() override;

	public:
		float GetPosition() const;

		float GetVelocity() const;

		void SetLimit(float _lower, float _upper) const;
		void SetMinDistance(float _lower) const;
		void SetMaxDistance(float _upper) const;
		void SetSpringArg(float _stiffness, float _damping) const;
		void SetBounciness(float _bounciness) const;

		void SetLimitEnable(bool) const;
		bool IsLimitEnabled() const;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		bool useSpring = false;
		float spring = 10.0f;
		float damper = 0.2f;

		bool useLimit = true;
		bool useMinDistance = true;
		float minDistance = 0.f;
		bool useMaxDistance = true;
		float maxDistance = 0.f;
		float bounciness = 0.f;
	};
}
