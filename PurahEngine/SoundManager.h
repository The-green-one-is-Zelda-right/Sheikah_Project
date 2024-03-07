#pragma once
#include "PurahEngineAPI.h"

#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <unordered_map>

namespace PurahEngine
{

	class PURAHENGINE_API SoundManager
	{
	public:
		enum class SoundType
		{
			BGM,
			EFFECT
		};
	public:
		void Initialize();

		void LoadSound(const std::wstring& soundName, const std::wstring& filePath, SoundType type);

		void Play(const std::wstring& soundName);

		void Update();

		void SetListenerPosition(FMOD_VECTOR lPosition, FMOD_VECTOR lFront, FMOD_VECTOR lUp);

	private:
		FMOD::System* system;

		FMOD::Channel* bgmChannel;
		FMOD::Channel* effectChannel;
		FMOD::ChannelGroup* effectChannelGroup;

		std::unordered_map<std::wstring, FMOD::Sound*> bgmSounds;
		std::unordered_map<std::wstring, FMOD::Sound*> effectSounds;

		FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR listenerPosition = { 0.0f, 0.0f, 0.0f };

		FMOD_VECTOR soundPosition = { 0.0f, 0.0f, 0.0f };

		FMOD_VECTOR forward = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR up = { 0.0f, 0.0f, 0.0f };


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

