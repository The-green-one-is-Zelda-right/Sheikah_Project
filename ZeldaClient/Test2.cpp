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

}

void PurahEngine::Test2::Update()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();
	const auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();
	const auto name = GetGameObject()->GetName();
	bool isRootable = GetGameObject()->IsRootEnable();

	if (inputManager.IsKeyPressed(eKey::eKEY_N) == true)
	{
		if (trans->GetParent() != nullptr)
		{
			if (isRootable == false)
			{
				GetGameObject()->Disable();
				TestSingleton::Getinstance().log.push_back(GetGameObject()->GetName() + (L"			             Disable"));
			}
		}
		else
		{
			if (isRootable == true)
			{
				GetGameObject()->Disable();
				TestSingleton::Getinstance().log.push_back(GetGameObject()->GetName() + (L"				         Disable"));
			}
		}

	}
}

void PurahEngine::Test2::FixedUpdate()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();
	const auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();
	const auto name = GetGameObject()->GetName();
	bool isRootable = GetGameObject()->IsRootEnable();

	if (inputManager.IsKeyPressed(eKey::eKEY_M) == true)
	{
		if (trans->GetParent() != nullptr)
		{
			if (isRootable == true)
			{
				GetGameObject()->SetEnable(true);
				GetGameObject()->Enable();
				TestSingleton::Getinstance().log.push_back(GetGameObject()->GetName() + (L"				       Enable"));
			}
		}
		else
		{
			if (isRootable == false)
			{
				GetGameObject()->Enable();
				TestSingleton::Getinstance().log.push_back(GetGameObject()->GetName() + (L"				       Enable"));
			}
		}
	}
}

void PurahEngine::Test2::PreSerialize(json& jsonData) const
{

}

void PurahEngine::Test2::PreDeserialize(const json& jsonData)
{

}

void PurahEngine::Test2::PostSerialize(json& jsonData) const
{

}

void PurahEngine::Test2::PostDeserialize(const json& jsonData)
{

}

