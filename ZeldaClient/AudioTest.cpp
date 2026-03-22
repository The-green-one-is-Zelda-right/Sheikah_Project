#include "AudioTest.h"


PurahEngine::AudioTest::AudioTest()
{

}

PurahEngine::AudioTest::~AudioTest()
{

}

void PurahEngine::AudioTest::Awake()
{
	GetGameObject()->GetComponent<AudioSource>()->PlayAudio(L"BGMTest.mp3");
}

void PurahEngine::AudioTest::OnDataLoadComplete()
{

}

void PurahEngine::AudioTest::Update()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();

	if (inputManager.IsKeyPressed(eKey::eKEY_M))
	{
		GetGameObject()->GetComponent<AudioSource>()->PlayAudio(L"EffectTest003.mp3");
	}
}

void PurahEngine::AudioTest::OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const Collider*)
{
	
}

void PurahEngine::AudioTest::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioTest::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::AudioTest::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioTest::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(audioSource);
}
