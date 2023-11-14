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
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		GameLoop& operator=(const GameLoop& ref) = delete;

	public:
		void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
		// �ܺο��� ����� Run() �Լ�
		void Run(_In_ int nCmdShow);
		void Finalize();

	private:
		HWND hWnd;
		// ���������� Run()�Լ����� ���ư� �Լ�
		void run();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		// Singleton
	public:
		static GameLoop& GetInstance();
	};
}

