#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider() = default;
		~CapsuleCollider() override = default;

	public:
		void Initialize() override;
		void OnDataLoadComplete() override;

	public:
		void SetSize(float _radius, float _halfHeight);

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	public:
		float radius;
		float height;
	};
}

