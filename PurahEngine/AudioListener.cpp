#include "AudioListener.h"

#include "GameObject.h"
#include "SoundManager.h"

PurahEngine::AudioListener::AudioListener()
	: listenerTransform(nullptr)
{

}

PurahEngine::AudioListener::~AudioListener()
{

}

void PurahEngine::AudioListener::Initialize()
{
	listenerTransform = GetGameObject()->GetTransform();
}

void PurahEngine::AudioListener::Update()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	soundManager.SetListenerTransform(listenerTransform);
}

void PurahEngine::AudioListener::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioListener::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::AudioListener::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioListener::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(listenerTransform);
}
