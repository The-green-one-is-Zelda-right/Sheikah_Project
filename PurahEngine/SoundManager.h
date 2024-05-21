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

		void LoadSound(const std::wstring& soundName, PurahEngine::Transform* transform, AudioSource* audioSource, SoundType type);
		
		void ReleaseSound(AudioSource* audioSource);

		void LoadBGMSound(const std::wstring& soundName, AudioSource* audioSource);

		void LoadEffectSound(const std::wstring& soundName, Transform* transform, AudioSource* audioSource);

		void PlayBGM(const std::wstring& soundName, AudioSource* audioSource);

		void PlayEffect(const std::wstring& soundName, AudioSource* audioSource);

		void Update();

		void Set3DListenerAttributes(const PurahEngine::Transform* listenerTransform);

		FMOD::System* GetSystem() const;

	private:
		FMOD::System* system;
		FMOD::Channel* bgmChannel;
		FMOD::Channel* effectChannel;
		FMOD::ChannelGroup* effectChannelGroup;

		std::unordered_map<PurahEngine::AudioSource*, PurahSound> soundMap;

		FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };

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

