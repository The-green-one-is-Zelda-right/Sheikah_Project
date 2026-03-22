#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API BoxCollider final : public Collider
	{
	public:
		BoxCollider() = default;
		~BoxCollider() override;

	public:
		void PostInitialize() override;

	public:
		void SetSize(const Eigen::Vector3f& _size);

	public:
		void PreDeserialize(const json& jsonData) override;

	public:
		Eigen::Vector3f size{ 1.f, 1.f, 1.f };
	};
}
