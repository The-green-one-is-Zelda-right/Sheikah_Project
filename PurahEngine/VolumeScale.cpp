#include "VolumeScale.h"

#include "Image.h"


void PurahEngine::VolumeScale::Awake()
{

}

void PurahEngine::VolumeScale::Start()
{

}

void PurahEngine::VolumeScale::Update()
{
	auto& soundManager = SoundManager::GetInstance();
	bgmScale = soundManager.GetBGMVolume();
	sfxScale = soundManager.GetSFXVolume();
}

void PurahEngine::VolumeScale::LateUpdate()
{
	if (soundType == SoundType::BGM)
	{
		if (targetImage != nullptr && offImage != L"")
		{
			if (bgmScale >= scaleBoundary)
			{
				targetImage->SetTexture(onImage);
			}
			else
			{
				targetImage->SetTexture(offImage);
			}
		}
	}
	else
	{
		if (sfxScale < scaleBoundary)
		{
			targetImage->SetTexture(offImage);
		}
		else
		{
			targetImage->SetTexture(onImage);
		}
	}
}

void PurahEngine::VolumeScale::PreSerialize(json& jsonData) const
{

}

void PurahEngine::VolumeScale::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	soundType = jsonData["soundType"];
	PREDESERIALIZE_WSTRING(onImage);
	PREDESERIALIZE_WSTRING(offImage);
	PREDESERIALIZE_VALUE(scaleBoundary);
}

void PurahEngine::VolumeScale::PostSerialize(json& jsonData) const
{

}

void PurahEngine::VolumeScale::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(targetImage);
}
