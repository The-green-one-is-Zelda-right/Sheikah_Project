#pragma once
#include "SoundManager.h"

namespace PurahEngine
{
	class SoundManager;



	class AudioClip
	{
	public:
		AudioClip();
		~AudioClip();

		AudioClip(std::wstring fileName, SoundType type, float minDistance, float maxDistance);

		void CreateSound();
		void PlayAudio();
		void Stop();
		void Set3DAttributes(FMOD_VECTOR pos);
		void SetLoop();


	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;

		std::wstring name;
		SoundType soundType;
		float mMinDistance;
		float mMaxDistance;
	private:
		SoundManager& soundManager = SoundManager::GetInstance();

	};
}

// AudioClip에서 구조체를 가지고, 실행, 등록을 하자.
// AudioSource는 등록된 AudioClip을 관리하는 역할로만 하는게 맞을 것 같다.