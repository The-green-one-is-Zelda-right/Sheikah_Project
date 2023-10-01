#pragma once
#include "PurahEngineAPI.h"
#include <windows.h>

namespace PurahEngine
{
	class GameLoop
	{
	public:
		PURAHENGINE_API void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
		// �ܺο��� ����� Run() �Լ�
		PURAHENGINE_API void Run(_In_ int nCmdShow);
		PURAHENGINE_API void Finalize();

	private:
		HWND hWnd;
		// ���������� Run()�Լ����� ���ư� �Լ�
		void run();

		//
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}

