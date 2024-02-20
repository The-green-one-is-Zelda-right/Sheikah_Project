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

	if (inputManager.IsKeyPressed('N') == true)
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

	if (inputManager.IsKeyPressed('M') == true)
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

