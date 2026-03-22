#include "Test2.h"
#include "TestSingleton.h"


PurahEngine::Test2::Test2()
{

}

PurahEngine::Test2::~Test2()
{

}

void PurahEngine::Test2::Awake()
{

}

void PurahEngine::Test2::Start()
{
	GetGameObject()->DontDestroyOnLoad();
}

void PurahEngine::Test2::Update()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();
	auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();

	// position
	if (inputManager.IsKeyPressed(eKey::eKEY_I))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetFront() * 0.1f));
		GetGameObject()->GetComponent<AudioSource>()->PlayAudio(L"footstep.mp3");
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_J))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetRight() * 0.1f));
		GetGameObject()->GetComponent<AudioSource>()->PlayAudio(L"footstep.mp3");
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_K))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetFront() * 0.1f));
		GetGameObject()->GetComponent<AudioSource>()->PlayAudio(L"footstep.mp3");
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_L))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetRight() * 0.1f));
		GetGameObject()->GetComponent<AudioSource>()->PlayAudio(L"footstep.mp3");
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_U))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetUp() * 0.1f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_O))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetUp() * 0.1f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_P))
	{
		GetGameObject()->EraseDontDestroy();
		GetGameObject()->DeleteComponent(this);
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_ESCAPE))
	{
		isPaused = !isPaused;
		if (isPaused)
		{
			TimeController::GetInstance().SetTimeScale(0.0f);
		}
		else
		{
			TimeController::GetInstance().SetTimeScale(1.0f);
		}
	}


	std::wcout << GetGameObject()->GetName() << std::endl;
	std::cout << trans->GetLocalPosition() << std::endl << std::endl;
	std::cout << TimeController::GetInstance().GetDeltaTime() << std::endl << std::endl;
}

void PurahEngine::Test2::FixedUpdate()
{

}

void PurahEngine::Test2::PreSerialize(json& jsonData) const
{

}

void PurahEngine::Test2::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::Test2::PostSerialize(json& jsonData) const
{

}

void PurahEngine::Test2::PostDeserialize(const json& jsonData)
{

}

