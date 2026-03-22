#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider() = default;
		~CapsuleCollider() override;

	public:
		void PostInitialize() override;

	public:
		void SetSize(float _radius, float _halfHeight);

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		float radius;
		float height;
		std::string material;
	};
}

