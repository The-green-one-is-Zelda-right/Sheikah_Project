#pragma once

#include "PurahEngineAPI.h"

#include "Component.h"

namespace PurahEngine
{
	class PURAHENGINE_API UI : public Component
	{
	public:
		void Awake() override;

		Eigen::Vector2f GetScreenPosition();

		void SetLocalPosition(const Eigen::Vector2f& value);
		void SetSize(const Eigen::Vector2f& value);
		Eigen::Vector2f GetLocalPosition();
		Eigen::Vector2f GetSize();

	private:
		UI* FindParent();

	private:
		UI* parent = nullptr;

		// left up
		Eigen::Vector2f position;
		Eigen::Vector2f size;

	private:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}