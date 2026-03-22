#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API SphereCollider : public Collider
	{
	public:
		SphereCollider() = default;
		~SphereCollider() override;

	public:
		void PostInitialize() override;

	public:
		void SetSize(float _radius);

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		float radius;
	};
}
