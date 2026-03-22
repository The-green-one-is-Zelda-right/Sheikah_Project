#include "FileManager.h"
#include <fstream>

#include <Windows.h>

json PurahEngine::FileManager::LoadData(const std::wstring& filename)
{
	json jsonData;
	std::ifstream data(filename);
	if (data.is_open())
	{
		data >> jsonData;

		data.close();
	}
	else
	{
		MessageBox(0, (L"Failed to Load Data \"" + filename + L"\"").c_str(), L"PurahEngine::FileManager Error", MB_OK);
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
