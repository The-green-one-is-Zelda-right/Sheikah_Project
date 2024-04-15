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
		// Component��(��) ���� ��ӵ�
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}