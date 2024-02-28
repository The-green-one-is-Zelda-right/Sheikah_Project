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
		SoundManager(const SoundManager& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		SoundManager& operator=(const SoundManager& ref) = delete;

	public:
		static SoundManager& GetInstance();
	};
}

