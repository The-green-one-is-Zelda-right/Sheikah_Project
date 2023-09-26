#pragma once
#include <windows.h>

namespace PurahEngine
{
	class GameLoop
	{
	public:
		void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
		// �ܺο��� ����� Run() �Լ�
		void Run(_In_ int nCmdShow);
		void Finalize();

	private:
		HWND hWnd;
		// ���������� Run()�Լ����� ���ư� �Լ�
		void run();

		//
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}

