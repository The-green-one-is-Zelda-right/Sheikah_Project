#include "AudioClip.h"


PurahEngine::AudioClip::AudioClip()
	: mSound(nullptr), mChannel(nullptr), mMinDistance(1.0f), mMaxDistance(1000.0f)
{

}

PurahEngine::AudioClip::~AudioClip()
{
	mSound->release();
	mSound = nullptr;
}

PurahEngine::AudioClip::AudioClip(std::wstring fileName, SoundType type, float minDistance, float maxDistance)
	: mSound(nullptr), mChannel(nullptr)
{
	name = fileName;
	soundType = type;
	mMinDistance = minDistance;
	mMaxDistance = maxDistance;
}

void PurahEngine::AudioClip::CreateSound()
{
	soundManager.CreateSound(soundType, name, &mSound);
	if (soundType == SoundType::EFFECT)
	{
		mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);
	}
}

void PurahEngine::AudioClip::PlayAudio()
{
	soundManager.PlayAudio(soundType, mSound, &mChannel);
}

void PurahEngine::AudioClip::Stop()
{
	mChannel->stop();
}

void PurahEngine::AudioClip::Set3DAttributes(FMOD_VECTOR pos)
{
	mChannel->set3DAttributes(&pos, 0);
}
