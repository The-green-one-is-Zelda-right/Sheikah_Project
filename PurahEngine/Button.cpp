#include "Button.h"

#include "Image.h"
#include "GameObject.h"
#include "UI.h"
#include "UnifiedInputManager.h"
#include "SceneManager.h"
#include "AudioSource.h"

namespace PurahEngine
{
	void Button::Awake()
	{
		ui = GetGameObject()->GetComponent<UI>();
		selected = !selectedWithAwake;
		clicked = false;
		if (selectedWithAwake)
			Select();
		else
			Unselect();
	}

	void Button::Update()
	{
		if (selected)
		{
			if (UnifiedInputManager::Getinstance().GetKeyDown(eUnfInput::UI_Ok))
			{
				clicked = true;
			}
		}
	}

	void Button::LateUpdate()
	{
		if (clicked)
		{
			ButtonEvent();
			clicked = false;
		}
	}

	void Button::Select()
	{
		if (selected == false)
		{
			selected = true;
			if (targetImage != nullptr && selectedImage != L"")
			{
				targetImage->SetTexture(selectedImage);
				if (GetGameObject()->GetComponent<AudioSource>() != nullptr)
				{
					GetGameObject()->GetComponent<AudioSource>()->PlayAudio();
				}
			}
		}
	}

	void Button::Unselect()
	{
		if (selected == true)
		{
			selected = false;
			if (targetImage != nullptr && unselectedImage != L"")
			{
				targetImage->SetTexture(unselectedImage);
			}
		}
	}

	bool Button::IsSelected()
	{
		return selected;
	}

	void Button::ButtonEvent()
	{
		if (GetGameObject()->GetComponent<AudioSource>() != nullptr)
		{
			GetGameObject()->GetComponent<AudioSource>()->PlayAudio(1);
		}
		for (auto& [targetGameObject, state] : objectOnOffEventList)
		{
			targetGameObject->SetEnable(state);
		}

		if (loadSceneEventBuffer != L"")
		{
			SceneManager::GetInstance().LoadScene(loadSceneEventBuffer);
		}
	}

	void Button::PreSerialize(json& jsonData) const
	{
	}

	void Button::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(selectedWithAwake);
		PREDESERIALIZE_WSTRING(selectedImage);
		PREDESERIALIZE_WSTRING(unselectedImage);
		PREDESERIALIZE_WSTRING(pushedImage);
		PREDESERIALIZE_WSTRING(loadSceneEventBuffer);
	}

	void Button::PostSerialize(json& jsonData) const
	{
	}

	void Button::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(targetImage);

		objectOnOffEventList.clear();
		for (int i = 0; i < jsonData[std::string("__ID__objectOnOffEventList")].size(); i++)
		{
			GameObject* targetObject = static_cast<GameObject*>(PurahEngine::FileManager::GetInstance().GetAddress(jsonData[std::string("__ID__objectOnOffEventList")][i][0]));
			bool state = jsonData[std::string("__ID__objectOnOffEventList")][i][1];

			objectOnOffEventList.push_back({ targetObject, state });
		}
	}
}