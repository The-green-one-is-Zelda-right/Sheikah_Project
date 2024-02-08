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

	private:
		FMOD::System* system;

		FMOD::Channel* bgmChannel;
		FMOD::Channel* effectChannel;
		FMOD::ChannelGroup* effectChannelGroup;

		std::unordered_map<std::wstring, FMOD::Sound*> bgmSounds;
		std::unordered_map<std::wstring, FMOD::Sound*> effectSounds;

		FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };

	private:
		SoundManager();
		~SoundManager();

	public:
		static SoundManager& GetInstance();
	};
}

