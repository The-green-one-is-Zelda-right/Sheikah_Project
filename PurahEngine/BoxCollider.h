#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API BoxCollider final : public Collider
	{
	public:
		BoxCollider() = default;
		~BoxCollider() override = default;

		void Awake() override;

	public:
		void SetSize(const Eigen::Vector3f& _size);

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	public:
		Eigen::Vector3f size{ 1.f, 1.f, 1.f };
	};
}
