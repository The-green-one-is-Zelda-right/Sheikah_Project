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
		// 파일을 읽고 있는중인가?
		bool isRunning;
		std::unordered_map<int, void*> addressMap;

	private:
		FileManager();
		~FileManager();
		FileManager(const FileManager& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		FileManager& operator=(const FileManager& ref) = delete;

	public:
		static FileManager& GetInstance();
	};
}

