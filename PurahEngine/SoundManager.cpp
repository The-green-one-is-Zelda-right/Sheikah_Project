#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "SoundManager.h"
#include "Transform.h"

#include <cassert>
#include <locale>
#include <codecvt>

#ifdef _DEBUG
#include <iostream>
#endif


void PurahEngine::SoundManager::Initialize()
{
	FMOD_RESULT result;

	// Create the main system object.
	result = FMOD::System_Create(&system);

	assert(result == FMOD_OK);

	// Initialize FMOD.
	result = system->init(100, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// Get Master Channel Group & Create BGM, SFX Channel Group
	result = system->getMasterChannelGroup(&masterChannelGroup);
	assert(result == FMOD_OK);
	result = system->createChannelGroup("BGMGroup", &bgmChannelGroup);
	assert(result == FMOD_OK);
	result = system->createChannelGroup("SFXGroup", &sfxChannelGroup);
	assert(result == FMOD_OK);

	//bgmChannelGroup->setVolume(1.0f);
	//sfxChannelGroup->setVolume(1.0f);

	// add BGM, SFX Channel Group to Master Channel Group
	result = masterChannelGroup->addGroup(bgmChannelGroup);
	assert(result == FMOD_OK);
	result = masterChannelGroup->addGroup(sfxChannelGroup);
	assert(result == FMOD_OK);

	result = system->set3DSettings(1.0, 1.0, 1.0);
}

void PurahEngine::SoundManager::Finalize()
{
	system->release();
}

void PurahEngine::SoundManager::CreateSound(SoundType soundType, std::wstring name, FMOD::Sound** sound)
{
	switch (soundType)
	{
		case SoundType::BGM:
		{
			CreateBGMSound(name, sound);
			break;
		}
		case SoundType::EFFECT:
		{
			CreateSfxSound(name, sound);
			break;
		}
		case SoundType::UI:
		{
			CreateUISound(name, sound);
			break;
		}
	}
}

void PurahEngine::SoundManager::ReleaseSound(AudioSource* audioSource)
{

}

void PurahEngine::SoundManager::CreateBGMSound(std::wstring name, FMOD::Sound** sound)
{
	FMOD_RESULT result;

#ifdef _DEBUG
	std::wstring filePath = L"../Sound/BGM/Test/" + name;
#else
	std::wstring filePath = L"Sound/BGM/" + name;
#endif

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_LOOP_NORMAL, 0, sound);
	assert(result == FMOD_OK);
}

void PurahEngine::SoundManager::CreateSfxSound(std::wstring name, FMOD::Sound** sound)
{
	FMOD_RESULT result;

#ifdef _DEBUG
	std::wstring filePath = L"../Sound/SFX/Test/" + name;
#else
	std::wstring filePath = L"Sound/SFX/" + name;
#endif

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);
	
	result = system->createSound(str.c_str(), FMOD_3D_LINEARSQUAREROLLOFF, 0, sound);
	assert(result == FMOD_OK);
}

void PurahEngine::SoundManager::CreateUISound(std::wstring name, FMOD::Sound** sound)
{
	FMOD_RESULT result;

#ifdef _DEBUG
	std::wstring filePath = L"../Sound/UI/" + name;
#else
	std::wstring filePath = L"Sound/UI/" + name;
#endif

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_2D, 0, sound);
	assert(result == FMOD_OK);
}

void PurahEngine::SoundManager::PlayAudio(SoundType soundType, FMOD::Sound* sound, FMOD::Channel** channel)
{
	switch (soundType)
	{
		case SoundType::BGM:
		{
			PlayBGM(sound, channel);
			break;
		}
		case SoundType::EFFECT:
		{
			PlaySfx(sound, channel);
			break;
		}
		case SoundType::UI:
		{
			PlayUI(sound, channel);
			break;
		}
	}
}

void PurahEngine::SoundManager::PlayBGM(FMOD::Sound* sound, FMOD::Channel** channel)
{
	FMOD_RESULT result;
	bool isBGMPlaying = true;
	(*channel)->isPlaying(&isBGMPlaying);

	if (isBGMPlaying)
	{
		(*channel)->stop();
	}

	result = system->playSound(sound, bgmChannelGroup, false, channel);
	assert(result == FMOD_OK);
	bgmChannel = (*channel);
}

void PurahEngine::SoundManager::PlaySfx(FMOD::Sound* sound, FMOD::Channel** channel)
{
	FMOD_RESULT result;
	bool isPlaying = true;
	(*channel)->isPlaying(&isPlaying);
	if (!isPlaying)
	{
		result = system->playSound(sound, sfxChannelGroup, false, channel);
		assert(result == FMOD_OK);
	}

	(*channel)->setPaused(false);
	isPlaying = (*channel)->getCurrentSound(&sound);
}

void PurahEngine::SoundManager::PlayUI(FMOD::Sound* sound, FMOD::Channel** channel)
{
	FMOD_RESULT result;
	bool isPlaying = true;
	(*channel)->isPlaying(&isPlaying);
	if (!isPlaying)
	{
		result = system->playSound(sound, sfxChannelGroup, false, channel);
		assert(result == FMOD_OK);
	}

	(*channel)->setPaused(false);
	isPlaying = (*channel)->getCurrentSound(&sound);
}

void PurahEngine::SoundManager::SetBGMVolume(float volume)
{
	bgmVolume = std::clamp(volume, 0.00f, 1.00f);
	bgmChannelGroup->setVolume(bgmVolume);
}

void PurahEngine::SoundManager::SetSFXVolume(float volume)
{
	sfxVolume = std::clamp(volume, 0.0f, 1.0f);
	sfxChannelGroup->setVolume(sfxVolume);
}

float PurahEngine::SoundManager::GetBGMVolume()
{
	bgmChannelGroup->getVolume(&bgmVolume);
	bgmVolume = std::round(bgmVolume * 10) / 10.0f;
	//return (bgmVolume * 10);
	return bgmVolume;
}

float PurahEngine::SoundManager::GetSFXVolume()
{
	sfxChannelGroup->getVolume(&sfxVolume);
	sfxVolume = std::round(sfxVolume * 10) / 10.0f;
	//return (sfxVolume * 10);
	return sfxVolume;
}

void PurahEngine::SoundManager::Update()
{
	system->update();
	bgmChannelGroup->setVolume(bgmVolume);
	sfxChannelGroup->setVolume(sfxVolume);

}

void PurahEngine::SoundManager::Set3DListenerAttributes(FMOD_VECTOR pos, FMOD_VECTOR forward, FMOD_VECTOR up)
{
	system->set3DListenerAttributes(0, &pos, 0, &forward, &up);
}

FMOD::System* PurahEngine::SoundManager::GetSystem() const
{
	return system;
}

PurahEngine::SoundManager::SoundManager()
	: system(nullptr), bgmChannel(nullptr), effectChannel(nullptr), sfxChannelGroup(nullptr), bgmChannelGroup(nullptr), masterChannelGroup(nullptr), bgmVolume(1.0f), sfxVolume(1.0f)
{

}

PurahEngine::SoundManager::~SoundManager()
{

}

PurahEngine::SoundManager& PurahEngine::SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}
