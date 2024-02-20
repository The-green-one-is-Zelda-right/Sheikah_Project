#include "Test.h"
#include "TestSingleton.h"
#include "InputManager.h"

PurahEngine::Test::Test()
{

}

PurahEngine::Test::~Test()
{

}

void PurahEngine::Test::Awake()
{
	TestSingleton::Getinstance().log.push_back(GetGameObject()->GetName() + (L" Awake"));
}

void PurahEngine::Test::Start()
{
	TestSingleton::Getinstance().log.push_back(GetGameObject()->GetName() + (L" Start"));
}

void PurahEngine::Test::Update()
{
	TestSingleton::Getinstance().log.push_back(GetGameObject()->GetName() + (L" Update"));
}
