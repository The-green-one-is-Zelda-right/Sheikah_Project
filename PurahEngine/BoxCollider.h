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

	public:
		void Initialize() override;
		void OnDataLoadComplete() override;

	public:
		void SetSize(const Eigen::Vector3f& _size);

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		Eigen::Vector3f size{ 1.f, 1.f, 1.f };
	};
}
