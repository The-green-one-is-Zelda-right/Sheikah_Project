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
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		TestSingleton& operator=(const TestSingleton& ref) = delete;
	};
}

