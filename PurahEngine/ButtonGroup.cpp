#include "ButtonGroup.h"

#include "UnifiedInputManager.h"

#include "GameObject.h"
#include "UI.h"
#include "Button.h"

namespace PurahEngine
{
	void ButtonGroup::Awake()
	{
		ui = GetGameObject()->GetComponent<UI>();
		selected = -1;
		selectNeighborBuffer = Neighbor::Wait;
	}

	void ButtonGroup::Start()
	{
		bool existSelected = false;

		// 선택된 버튼의 번호를 찾고, 만약 여러개의 버튼이 켜져있다면 먼저 나온 버튼을 제외한 나머지를 끈다.
		for (size_t i = 0; i < contents.size(); i++)
		{
			if (contents[i]->IsSelected())
			{
				if (existSelected)
				{
					contents[i]->Unselect();
				}
				else
				{
					existSelected = true;
					selected = i;
				}
			}
		}
	}

	void ButtonGroup::Update()
	{
		if (selected == -1)
		{
			return;
		}

		int deltaV = 0;
		deltaV -= UnifiedInputManager::Getinstance().GetKey(eUnfInput::UI_Up) ? 1 : 0;
		deltaV += UnifiedInputManager::Getinstance().GetKey(eUnfInput::UI_Down) ? 1 : 0;

		int deltaH = 0;
		deltaH -= UnifiedInputManager::Getinstance().GetKey(eUnfInput::UI_Left) ? 1 : 0;
		deltaH += UnifiedInputManager::Getinstance().GetKey(eUnfInput::UI_Right) ? 1 : 0;

		switch (layoutType)
		{
			case PurahEngine::ButtonGroup::LayoutType::Vertical:
			{
				// 동시 입력 불가능
				// Vertical은 Up, Down 우선
				if (deltaV != 0)
				{
					int result = selected + deltaV;

					if (result < 0)
					{
						SelectNeighborGroup(Neighbor::Up);
					}
					else if (contents.size() <= result)
					{
						SelectNeighborGroup(Neighbor::Down);
					}
					else
					{
						contents[selected]->Unselect();
						contents[result]->Select();
						selected = result;
					}
				}
				else if (deltaH != 0)
				{
					if (deltaH < 0)
					{
						SelectNeighborGroup(Neighbor::Left);
					}
					else
					{
						SelectNeighborGroup(Neighbor::Right);
					}
				}

				break;
			}
			case PurahEngine::ButtonGroup::LayoutType::Horizontal:
			{
				// 동시 입력 불가능
				// Horizontal은 Left, Right 우선
				if (deltaH != 0)
				{
					int result = selected + deltaH;

					if (result < 0)
					{
						SelectNeighborGroup(Neighbor::Left);
					}
					else if (contents.size() <= result)
					{
						SelectNeighborGroup(Neighbor::Right);
					}
					else
					{
						contents[selected]->Unselect();
						contents[result]->Select();
						selected = result;
					}
				}
				else if (deltaV != 0)
				{
					if (deltaV < 0)
					{
						SelectNeighborGroup(Neighbor::Up);
					}
					else
					{
						SelectNeighborGroup(Neighbor::Down);
					}
				}

				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}

	void ButtonGroup::LateUpdate()
	{
		SelectNeighborGroupExecute(selectNeighborBuffer);
		selectNeighborBuffer = Neighbor::Wait;
	}

	void ButtonGroup::Select(int idx)
	{
		// 선택된 그룹을 다시 선택함
		assert(selected == -1);

		int selectIdx = std::min(std::max(0, idx), static_cast<int>(contents.size() - 1));

		if (selected != -1)
		{
			contents[idx]->Unselect();
		}

		if (selectIdx != -1)
		{
			contents[selectIdx]->Select();
			selected = selectIdx;
		}
	}

	void ButtonGroup::SelectNeighborGroup(Neighbor neighbor)
	{
		selectNeighborBuffer = neighbor;
	}

	void ButtonGroup::SelectNeighborGroupExecute(Neighbor neighbor)
	{
		if (neighbor == Neighbor::Wait || selected == -1)
		{
			return;
		}

		int idx = static_cast<int>(neighbor);

		if (neighborGroups[idx] == nullptr)
		{
			return;
		}

		contents[selected]->Unselect();

		int targetSelect = neighborGroupSelect[idx];

		if (targetSelect == -1)
		{
			targetSelect = selected;
		}

		neighborGroups[idx]->Select(targetSelect);
		selected = -1;
	}

	void ButtonGroup::PreSerialize(json& jsonData) const
	{
	}
	void ButtonGroup::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		int deserLayout = jsonData["layoutType"];
		layoutType = static_cast<LayoutType>(deserLayout);

		neighborGroupSelect[0] = jsonData["neighborGroupSelect"][0];
		neighborGroupSelect[1] = jsonData["neighborGroupSelect"][1];
		neighborGroupSelect[2] = jsonData["neighborGroupSelect"][2];
		neighborGroupSelect[3] = jsonData["neighborGroupSelect"][3];
	}
	void ButtonGroup::PostSerialize(json& jsonData) const
	{
	}
	void ButtonGroup::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(contents);
		for (int i = 0; i < 4; i++)
		{
			neighborGroups[i] = static_cast<ButtonGroup*>(PurahEngine::FileManager::GetInstance().GetAddress(jsonData["__ID__neighborGroups"][i]));
			if (neighborGroups[i] == this)
			{
				neighborGroups[i] = nullptr;
			}
		}
	}
}