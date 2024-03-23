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

		void LoadSound(const std::wstring& soundName, PurahEngine::Transform* transform, AudioSource* audioSource, SoundType type);
		
		void LoadBGMSound(const std::wstring& soundName, AudioSource* audioSource);

		void LoadEffectSound(const std::wstring& soundName, Transform* transform, AudioSource* audioSource);

		void PlayBGM(const std::wstring& soundName, AudioSource* audioSource);

		void PlayEffect(const std::wstring& soundName, AudioSource* audioSource);

		void Update();

		void SetListenerTransform(PurahEngine::Transform* transform);

		void SetObject3DAttributes();

		FMOD::System* GetSystem() const;

	private:
		FMOD::System* system;
		FMOD::Channel* bgmChannel;
		FMOD::Channel* effectChannel;
		FMOD::ChannelGroup* effectChannelGroup;

		PurahEngine::Transform* listenerTransform;

		std::unordered_map<PurahEngine::AudioSource*, PurahSound> soundMap;

		FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR listenerPosition = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR listenerForward = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR listenerUp = { 0.0f, 0.0f, 0.0f };


	private:
		SoundManager();
		~SoundManager();
		SoundManager(const SoundManager& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		SoundManager& operator=(const SoundManager& ref) = delete;

	public:
		static SoundManager& GetInstance();
	};
}

