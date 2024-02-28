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

void PurahEngine::Test::PreSerialize(json& jsonData) const
{

}

void PurahEngine::Test::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	PREDESERIALIZE_VALUE(a);
	PREDESERIALIZE_VALUE(b);
	PREDESERIALIZE_VALUE(c);
}

void PurahEngine::Test::PostSerialize(json& jsonData) const
{

}

void PurahEngine::Test::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(trans);
}
