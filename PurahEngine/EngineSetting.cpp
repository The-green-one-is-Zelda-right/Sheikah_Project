#include "EngineSetting.h"

void PurahEngine::EngineSetting::SetTag(std::wstring tagName, GameObject* object)
{
	tagList[tagName].push_back(object);
}

std::vector<PurahEngine::GameObject*> PurahEngine::EngineSetting::GetTagObject(std::wstring tagName)
{
	return tagList[tagName];
}

void PurahEngine::EngineSetting::PreSerialize(json& jsonData) const
{

}

void PurahEngine::EngineSetting::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::EngineSetting::PostSerialize(json& jsonData) const
{

}

void PurahEngine::EngineSetting::PostDeserialize(const json& jsonData)
{

}

PurahEngine::EngineSetting& PurahEngine::EngineSetting::GetInstance()
{
	static EngineSetting instance;
	return instance;
}
