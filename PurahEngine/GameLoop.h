#pragma once
#include "PurahEngineAPI.h"
#include <windows.h>

namespace PurahEngine
{
	class SceneManager;
	class GraphicsManager;

	class GameLoop
	{
	private:
		GameLoop();
		~GameLoop();
		GameLoop(const GameLoop& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		GameLoop& operator=(const GameLoop& ref) = delete;

	public:
		void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
		// 외부에서 사용할 Run() 함수
		void Run(_In_ int nCmdShow);
		void Finalize();

	private:
		HWND hWnd;
		// 내부적으로 Run()함수에서 돌아갈 함수
		void run();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		// Singleton
	public:
		static GameLoop& GetInstance();
	};
}

