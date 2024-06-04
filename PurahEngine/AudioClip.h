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

// AudioClip���� ����ü�� ������, ����, ����� ����.
// AudioSource�� ��ϵ� AudioClip�� �����ϴ� ���ҷθ� �ϴ°� ���� �� ����.