#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"


namespace PurahEngine
{
	class CustomCollider : public Collider
	{
	public:
		CustomCollider() = default;
		~CustomCollider() override;

	public:
		void PreSerialize(json& jsonData) const override {};
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override {};
		void PostDeserialize(const json& jsonData) override {};

	protected:
		std::wstring modelName;
		Eigen::Quaternionf rotationOffset;
		Eigen::Vector3f scaleOffset;
	};
}