#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "SoundManager.h"
#include "Transform.h"

#include <cassert>
#include <locale>
#include <codecvt>

void PurahEngine::SoundManager::Initialize()
{
	FMOD_RESULT result;

	// Create the main system object.
	result = FMOD::System_Create(&system);

	assert(result == FMOD_OK);

	// Initialize FMOD.
	result = system->init(100, FMOD_INIT_NORMAL, nullptr);

	assert(result == FMOD_OK);

	result = system->set3DSettings(1.0, 1.0, 1.0);

	system->createChannelGroup("effectChannelGroup", &effectChannelGroup);
}

void PurahEngine::SoundManager::LoadSound(const std::wstring& soundName, PurahEngine::Transform* transform, AudioSource* audioSource, SoundType type)
{
	switch (type)
	{
	case SoundType::BGM:
	{
		LoadBGMSound(soundName, audioSource);
		break;
	}
	case SoundType::EFFECT:
	{
		LoadEffectSound(soundName, transform, audioSource);
		break;
	}
	}
}

void PurahEngine::SoundManager::LoadBGMSound(const std::wstring& soundName, AudioSource* audioSource)
{
	FMOD_RESULT result;

	PurahEngine::PurahSound newSound;
	std::wstring filePath = L"../Sound/Effect/Test/" + soundName;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_LOOP_NORMAL, 0, &(newSound.sound));
	assert(result == FMOD_OK);

	newSound.soundName = soundName;
	newSound.isPlaying = false;
	newSound.channel = bgmChannel;
	newSound.type = SoundType::BGM;

	soundMap[audioSource] = newSound;
}

void PurahEngine::SoundManager::LoadEffectSound(const std::wstring& soundName, Transform* transform, AudioSource* audioSource)
{
	FMOD_RESULT result;

	PurahEngine::PurahSound newSound;
	std::wstring filePath = L"../Sound/Effect/Test/" + soundName;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_3D, 0, &(newSound.sound));
	assert(result == FMOD_OK);

	newSound.soundName = soundName;
	newSound.transform = transform;
	newSound.isPlaying = false;
	newSound.channel = 0;
	newSound.type = SoundType::EFFECT;
	newSound.lastPos = { 0.0f, 0.0f, 0.0f };

	newSound.channel->setChannelGroup(effectChannelGroup);
	newSound.sound->set3DMinMaxDistance(0.5f, 1000.0f);

	soundMap[audioSource] = newSound;
}

void PurahEngine::SoundManager::PlayBGM(const std::wstring& soundName, AudioSource* audioSource)
{
	auto purahSound = soundMap.at(audioSource);
	if (purahSound.soundName == soundName)
	{
		bool isBGMPlaying = true;
		purahSound.channel->isPlaying(&isBGMPlaying);

		if (isBGMPlaying)
		{
			purahSound.channel->stop();
		}

		system->playSound(purahSound.sound, 0, false, &purahSound.channel);
	}
}

void PurahEngine::SoundManager::PlayEffect(const std::wstring& soundName, AudioSource* audioSource)
{
	auto& purahSound = soundMap.at(audioSource);

	if (purahSound.soundName == soundName)
	{
		bool isPlaying = true;
		purahSound.channel->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			system->playSound(purahSound.sound, 0, false, &purahSound.channel);
		}

		position.x = purahSound.transform->GetWorldPosition().x();
		position.y = purahSound.transform->GetWorldPosition().y();
		position.z = purahSound.transform->GetWorldPosition().z();

		auto r = purahSound.channel->set3DAttributes(&position, 0);
		purahSound.channel->setPaused(false);
		purahSound.isPlaying = purahSound.channel->getCurrentSound(&purahSound.sound);
	}
}

void PurahEngine::SoundManager::Update()
{
	FMOD_VECTOR lastPos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	vel.x = (listenerPosition.x - lastPos.x);
	vel.y = (listenerPosition.y - lastPos.y);
	vel.z = (listenerPosition.z - lastPos.z);

	if (listenerTransform != nullptr)
	{
		Eigen::Vector3f pos = listenerTransform->GetWorldPosition();
		listenerPosition.x = pos.x();
		listenerPosition.y = pos.y();
		listenerPosition.z = pos.z();

		Eigen::Vector3f forward = listenerTransform->GetFront();
		listenerForward.x = forward.x();
		listenerForward.y = forward.y();
		listenerForward.z = forward.z();

		Eigen::Vector3f up = listenerTransform->GetUp();
		listenerUp.x = up.x();
		listenerUp.y = up.y();
		listenerUp.z = up.z();
	}

	SetObject3DAttributes();
	system->set3DListenerAttributes(0, &listenerPosition, &vel, &listenerForward, &listenerUp);

	system->update();
	lastPos = listenerPosition;
}

void PurahEngine::SoundManager::SetListenerTransform(PurahEngine::Transform* transform)
{
	listenerTransform = transform;
}

void PurahEngine::SoundManager::SetObject3DAttributes()
{
	for (auto iter = soundMap.begin(); iter != soundMap.end(); iter++)
	{
		auto purahSound = soundMap.at(iter->first);

		Eigen::Vector3f pos = purahSound.transform->GetWorldPosition();
		position.x = pos.x();
		position.y = pos.y();
		position.z = pos.z();

		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
		vel.x = (position.x - purahSound.lastPos.x);
		vel.y = (position.y - purahSound.lastPos.y);
		vel.z = (position.z - purahSound.lastPos.z);

		auto r = purahSound.channel->set3DAttributes(&position, &vel);

		purahSound.lastPos = position;
	}
}

FMOD::System* PurahEngine::SoundManager::GetSystem() const
{
	return system;
}

PurahEngine::SoundManager::SoundManager()
	: system(nullptr), bgmChannel(nullptr), effectChannel(nullptr), effectChannelGroup(nullptr), listenerTransform(nullptr)
{

}

PurahEngine::SoundManager::~SoundManager()
{
	system->release();
}

PurahEngine::SoundManager& PurahEngine::SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}