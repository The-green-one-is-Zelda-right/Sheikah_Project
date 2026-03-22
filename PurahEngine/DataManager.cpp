#include "DataManager.h"
#include "GameObject.h"
#include "FileManager.h"
#include "SceneManager.h"

void PurahEngine::DataManager::DontDestroyOnLoad(GameObject* gameObject)
{
	if (find(dontDestroyObjectList.begin(), dontDestroyObjectList.end(), gameObject) == dontDestroyObjectList.end())
	{
		dontDestroyObjectList.push_back(gameObject);
	}
}

void PurahEngine::DataManager::ClearDontDestroy()
{
	dontDestroyObjectList.clear();
}

void PurahEngine::DataManager::PreSerialize(json& jsonData) const
{

}

void PurahEngine::DataManager::PreDeserialize(const json& jsonData)
{

}

void PurahEngine::DataManager::PostSerialize(json& jsonData) const
{

}

void PurahEngine::DataManager::PostDeserialize(const json& jsonData)
{

}

PurahEngine::DataManager::DataManager()
{

}

PurahEngine::DataManager::~DataManager()
{

}


PurahEngine::DataManager& PurahEngine::DataManager::GetInstance()
{
	static DataManager instance;
	return instance;
}