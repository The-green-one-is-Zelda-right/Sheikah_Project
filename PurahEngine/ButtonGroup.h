#pragma once

#include "PurahEngineAPI.h"

#include "Component.h"

#include <vector>

namespace PurahEngine
{
	class UI;
	class Button;

	class PURAHENGINE_API ButtonGroup : public Component
	{
	public:
		enum class Neighbor : int
		{
			Wait = -1,
			Left = 0,
			Right = 1,
			Up = 2,
			Down = 3
		};

		enum class LayoutType : int
		{
			Vertical = 0,
			Horizontal = 1
		};

	public:
		void Awake() override;
		void Start() override;
		void Update() override;
		void LateUpdate() override;

		void Select(int idx);

	private:
		void SelectNeighborGroup(Neighbor neighbor);
		void SelectNeighborGroupExecute(Neighbor neighbor);

	private:
		UI* ui;
		int selected;
		Neighbor selectNeighborBuffer;

	private:
		LayoutType layoutType;
		std::vector<Button*> contents;
		ButtonGroup* neighborGroups[4];
		int neighborGroupSelect[4];

	private:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}