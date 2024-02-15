#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{

	class TestSingleton
	{
	public:
		std::vector<std::wstring> log;

	public:
		static TestSingleton& Getinstance();

	private:
		TestSingleton();
		~TestSingleton();
		TestSingleton(const TestSingleton& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		TestSingleton& operator=(const TestSingleton& ref) = delete;
	};
}

