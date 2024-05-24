#pragma once
#include "PurahEngineAPI.h"

#include "PurahSound.h"

#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <unordered_map>

namespace PurahEngine
{
	class Transform;
	class AudioSource;

	class PURAHENGINE_API SoundManager
	{
	public:
		void Initialize();
		void Finalize();

		void LoadSound(PurahSound newSound, FMOD::Sound** sound);
		
		void ReleaseSound(AudioSource* audioSource);

		void LoadBGMSound(PurahSound newSound, FMOD::Sound** sound);

		void LoadSfxSound(PurahSound newSound, FMOD::Sound** sound);

		void Play(PurahSound newSound, FMOD::Sound* sound, FMOD::Channel** channel);

		void PlayBGM(FMOD::Sound* sound, FMOD::Channel** channel);

		void PlaySfx(FMOD::Sound* sound, FMOD::Channel** channel);

		void Update();

		void Set3DListenerAttributes(FMOD_VECTOR pos, FMOD_VECTOR forward, FMOD_VECTOR up);

		FMOD::System* GetSystem() const;

	private:
		FMOD::System* system;
		FMOD::Channel* bgmChannel;
		FMOD::Channel* effectChannel;
		FMOD::ChannelGroup* effectChannelGroup;

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

