#pragma once

#include "PurahEngineAPI.h"

#include "Component.h"

namespace PurahEngine
{
	class Image;
	class UI;

	class PURAHENGINE_API Button : public Component
	{
	public:
		void Awake() override;

		void Update() override;
		void LateUpdate() override;

		void Select();
		void Unselect();

		bool IsSelected();

	private:
		void ButtonEvent();

	private:
		UI* ui;
		bool selected;
		bool clicked;

	private:
		bool selectedWithAwake;
		Image* targetImage;
		std::wstring selectedImage;
		std::wstring unselectedImage;
		std::wstring pushedImage;

		std::vector<std::pair<GameObject*, bool>> objectOnOffEventList;
		std::wstring loadSceneEventBuffer;

	private:
		// Component��(��) ���� ��ӵ�
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}