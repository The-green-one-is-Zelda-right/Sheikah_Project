#pragma once
#include "PurahEngineAPI.h"

namespace PurahEngine
{
	class PURAHENGINE_API FileManager
	{
	public:

	private:
		// ������ �а� �ִ����ΰ�?
		bool isRunning;


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

