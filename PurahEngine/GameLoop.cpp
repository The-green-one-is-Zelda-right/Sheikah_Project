#include "GameLoop.h"

PurahEngine::GameLoop::GameLoop()
{

}

PurahEngine::GameLoop::~GameLoop()
{

}

void PurahEngine::GameLoop::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)
{
	// ���� �� �����츦 ���
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // ���ν���
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = gameName;
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);

	// �� �����츦 ����
	hWnd = CreateWindowW(gameName, gameName, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	SetMenu(hWnd, NULL);


}

void PurahEngine::GameLoop::Run(_In_ int nCmdShow)
{
	// ������ �ڵ��� ���� �߿�
	if (!hWnd)
	{
		return;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���ѷ��� (�ȿ� �޽��� ������ ���� ���� ���� ���Ե�)
	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);
		}
		else
		{
			// ���� ����
			run();
		}
	}
}

void PurahEngine::GameLoop::Finalize()
{

}

void PurahEngine::GameLoop::run()
{

}

LRESULT CALLBACK PurahEngine::GameLoop::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

PurahEngine::GameLoop& PurahEngine::GameLoop::GetInstance()
{
	static GameLoop instance;
	return instance;
}
