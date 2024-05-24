#pragma once
#include "SoundManager.h"
#include "PurahSound.h"

namespace PurahEngine
{
	class SoundManager;

	class AudioClip
	{
	public:
		AudioClip();
		~AudioClip();

		void CreateSound();
		void Play();
		void Stop();
		void Set3DAttributes(FMOD_VECTOR pos);
		void SetLoop();


	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;

		PurahSound newSound;
	private:
		SoundManager& soundManager = SoundManager::GetInstance();

	};
}

// AudioClip���� ����ü�� ������, ����, ����� ����.
// AudioSource�� ��ϵ� AudioClip�� �����ϴ� ���ҷθ� �ϴ°� ���� �� ����.