#include "VolumeSetting.h"
#include "GameObject.h"
#include "Button.h"
#include "UnifiedInputManager.h"
#include "AudioSource.h"

void PurahEngine::VolumeSetting::Awake()
{
	button = GetGameObject()->GetComponent<Button>();
}

void PurahEngine::VolumeSetting::Start()
{

}

void PurahEngine::VolumeSetting::Update()
{
	double soundVolume = 0.0f;
	bool isSelect = button->IsSelected();

	if (isSelect)
	{
		if (UnifiedInputManager::Getinstance().GetKeyDown(eUnfInput::UI_Right))
		{
			if (GetGameObject()->GetComponent<AudioSource>() != nullptr)
			{
				GetGameObject()->GetComponent<AudioSource>()->PlayAudio(1);
			}

			if (soundType == SoundType::BGM)
			{
				soundVolume = SoundManager::GetInstance().GetBGMVolume();
				soundVolume += 0.1f;
				SoundManager::GetInstance().SetBGMVolume(soundVolume);
			}
			else
			{
				soundVolume = SoundManager::GetInstance().GetSFXVolume();
				soundVolume += 0.1f;
				SoundManager::GetInstance().SetSFXVolume(soundVolume);
			}
		}
		else if (UnifiedInputManager::Getinstance().GetKeyDown(eUnfInput::UI_Left))
		{
			if (GetGameObject()->GetComponent<AudioSource>() != nullptr)
			{
				GetGameObject()->GetComponent<AudioSource>()->PlayAudio(1);
			}

			if (soundType == SoundType::BGM)
			{
				soundVolume = SoundManager::GetInstance().GetBGMVolume();
				soundVolume -= 0.1f;
				SoundManager::GetInstance().SetBGMVolume(soundVolume);
			}
			else
			{
				soundVolume = SoundManager::GetInstance().GetSFXVolume();
				soundVolume -= 0.1f;
				SoundManager::GetInstance().SetSFXVolume(soundVolume);
			}
		}
	}
}

void PurahEngine::VolumeSetting::LateUpdate()
{

}

void PurahEngine::VolumeSetting::PreSerialize(json& jsonData) const
{

}

void PurahEngine::VolumeSetting::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	soundType = jsonData["soundType"];
}

void PurahEngine::VolumeSetting::PostSerialize(json& jsonData) const
{

}

void PurahEngine::VolumeSetting::PostDeserialize(const json& jsonData)
{

}

/// PreDeserialize 고치고, Unity Component 만들것.