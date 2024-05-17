#pragma once
#include "PurahEngineAPI.h"
#include <windows.h>

namespace PurahEngine
{
	class SceneManager;
	class GraphicsManager;

	enum class ErrorType
	{
		UnKnown,
		Failed_Deserialize
	};

	class GameLoop
	{
	private:
		GameLoop();
		~GameLoop();
		GameLoop(const GameLoop& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		GameLoop& operator=(const GameLoop& ref) = delete;

	public:
		void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
		// �ܺο��� ����� Run() �Լ�
		void Run(_In_ int nCmdShow);
		void Finalize();

	public:
		static ErrorType errorType;

	private:
		bool moving = false;
		HWND hWnd;

		// ���������� Run()�Լ����� ���ư� �Լ�
		void run();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);

		// Singleton
	public:
		static GameLoop& GetInstance();
	};
}

