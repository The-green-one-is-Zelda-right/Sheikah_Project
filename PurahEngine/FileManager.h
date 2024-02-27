#pragma once
#include "PurahEngineAPI.h"
#include "nlohmann/json.hpp"

#include <unordered_map>
using json = nlohmann::json;

namespace PurahEngine
{
	class PURAHENGINE_API FileManager
	{
	public:
		json LoadData(const std::wstring& filename);

		void SetAddress(int id, void* address);
		void* GetAddress(int id);
		void clear();

	private:
		// ������ �а� �ִ����ΰ�?
		bool isRunning;
		std::unordered_map<int, void*> addressMap;

	private:
		FileManager();
		~FileManager();
		FileManager(const FileManager& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		FileManager& operator=(const FileManager& ref) = delete;

	public:
		static FileManager& GetInstance();
	};
}

