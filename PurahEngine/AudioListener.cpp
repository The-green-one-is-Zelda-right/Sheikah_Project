#include "AudioListener.h"

#include "GameObject.h"
#include "SoundManager.h"

PurahEngine::AudioListener::AudioListener()
	: listenerTransform(nullptr)
{

}

PurahEngine::AudioListener::~AudioListener()
{
	listenerTransform = nullptr;
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
}

void PurahEngine::AudioListener::Initialize()
{
	listenerTransform = GetGameObject()->GetTransform();
}

void PurahEngine::AudioListener::Update()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();

	Eigen::Vector3f pos = listenerTransform->GetWorldPosition();
	FMOD_VECTOR listenerPos(pos.x(), pos.y(), pos.z());

	Eigen::Vector3f forward = listenerTransform->GetFront();
	FMOD_VECTOR listenerForward(forward.x(), forward.y(), forward.z());

	Eigen::Vector3f up = listenerTransform->GetUp();
	FMOD_VECTOR listenerUp(up.x(), up.y(), up.z());

	soundManager.Set3DListenerAttributes(listenerPos, listenerForward, listenerUp);
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
