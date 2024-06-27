#pragma once
#include "PurahEngineAPI.h"

#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <unordered_map>

namespace PurahEngine
{
	class Transform;
	class AudioSource;

	enum class SoundType
	{
		BGM,
		EFFECT,
		UI
	};

	class PURAHENGINE_API SoundManager
	{
	public:
		void Initialize();
		void Finalize();

		void CreateSound(SoundType soundType, std::wstring name, FMOD::Sound** sound);
		
		void ReleaseSound(AudioSource* audioSource);

		void CreateBGMSound(std::wstring name, FMOD::Sound** sound);

		void CreateSfxSound(std::wstring name, FMOD::Sound** sound);

		void CreateUISound(std::wstring name, FMOD::Sound** sound);

		void PlayAudio(SoundType soundType, FMOD::Sound* sound, FMOD::Channel** channel);

		void PlayBGM(FMOD::Sound* sound, FMOD::Channel** channel);

		void PlaySfx(FMOD::Sound* sound, FMOD::Channel** channel);

		void PlayUI(FMOD::Sound* sound, FMOD::Channel** channel);

		void Update();

		void Set3DListenerAttributes(FMOD_VECTOR pos, FMOD_VECTOR forward, FMOD_VECTOR up);

		FMOD::System* GetSystem() const;

	private:
		FMOD::System* system;
		FMOD::Channel* bgmChannel;
		FMOD::Channel* effectChannel;
		FMOD::ChannelGroup* bgmChannelGroup;
		FMOD::ChannelGroup* sfxChannelGroup;
		FMOD::ChannelGroup* masterChannelGroup;

	private:
		SoundManager();
		~SoundManager();
		SoundManager(const SoundManager& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		SoundManager& operator=(const SoundManager& ref) = delete;

	public:
		static SoundManager& GetInstance();
	};
}

