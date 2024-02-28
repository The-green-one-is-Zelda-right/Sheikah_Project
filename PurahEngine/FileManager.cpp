#include "FileManager.h"
#include <fstream>


json PurahEngine::FileManager::LoadData(const std::wstring& filename)
{
	json jsonData;
	std::ifstream data(filename);
	if (data.is_open())
	{
		data >> jsonData;

		data.close();
	}

	return jsonData;
}


void PurahEngine::FileManager::SetAddress(int id, void* address)
{
	addressMap[id] = address;
}


void* PurahEngine::FileManager::GetAddress(int id)
{
	auto iter = addressMap.find(id);

	if (iter == addressMap.end())
	{
		return nullptr;
	}

	return iter->second;
}


void PurahEngine::FileManager::clear()
{
	addressMap.clear();
}

PurahEngine::FileManager::FileManager()
	: isRunning(false)
{

}

PurahEngine::FileManager::~FileManager()
{

}

PurahEngine::FileManager& PurahEngine::FileManager::GetInstance()
{
	static FileManager instance;
	return instance;
}
