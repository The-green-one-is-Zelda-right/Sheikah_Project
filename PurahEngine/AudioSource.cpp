#include "AudioSource.h"

#include "GameObject.h"

#include "InputManager.h"

PurahEngine::AudioSource::AudioSource()
	: soundTransform(nullptr)
{

}

PurahEngine::AudioSource::~AudioSource()
{

}

void PurahEngine::AudioSource::Awake()
{

}

void PurahEngine::AudioSource::Initialize()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();

	soundTransform = GetGameObject()->GetTransform();

	
}

void PurahEngine::AudioSource::OnDataLoadComplete()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	soundManager.LoadSound(soundName, soundTransform, this, SoundType::EFFECT);
}

void PurahEngine::AudioSource::Update()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	auto& inputManager = PurahEngine::InputManager::Getinstance();

	Eigen::Vector3f soundPosition = soundTransform->GetWorldPosition();
	FMOD_VECTOR pos = { soundPosition.x(), soundPosition.y(), soundPosition.z() };

	if (inputManager.IsKeyDown(eKey::eKEY_0) == true)
	{
		soundManager.PlayEffect(L"EffectTest003.mp3", this);
	}
}

void PurahEngine::AudioSource::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioSource::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	PREDESERIALIZE_WSTRING(soundName);
}

void PurahEngine::AudioSource::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioSource::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(soundTransform);
}
