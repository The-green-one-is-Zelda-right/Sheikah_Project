#pragma once
#include <windows.h>

namespace PurahEngine
{
	class GameLoop
	{
	public:
		void Initialize();
		// �ܺο��� ����� Run() �Լ�
		void Run(_In_ int nCmdShow);
		void Finalize();

	private:
		HWND hWnd;
		// ���������� Run()�Լ����� ���ư� �Լ�
		void run();
	};
}

