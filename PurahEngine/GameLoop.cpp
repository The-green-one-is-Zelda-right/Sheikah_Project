#include "GameLoop.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "InputManager.h"
#include "PhysicsSystem.h"
#include "TimeController.h"
#include "SoundManager.h"
#include <cassert>


/// <summary>
///  임시
/// </summary>
#include "../ZonaiPhysicsBase/ZnCollider.h"
#include "../ZonaiPhysicsBase/ZnRigidBody.h"
#include "../ZonaiMath/ZonaiMath.h"

ZonaiPhysics::ZnCollider* collider;
ZonaiPhysics::ZnRigidBody* rigidBody;

static std::string timeInit = "Init";

PurahEngine::GameLoop::GameLoop()
{

}

PurahEngine::GameLoop::~GameLoop()
{

}

void PurahEngine::GameLoop::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)
{
	// 내가 쓸 윈도우를 등록
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // 프로시저
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

	// 그 윈도우를 생성
	hWnd = CreateWindowW(gameName, gameName, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	SetMenu(hWnd, NULL);

	// 시간관리자 초기화
	PurahEngine::TimeController::GetInstance().Initialize(timeInit);

	// Graphics dll 초기화(변경 가능성 농후)
	PurahEngine::GraphicsManager::GetInstance().Initialize(hWnd);

	// InputManager 초기화
	PurahEngine::InputManager::Getinstance().Initialize();

	// SceneManager 초기화
	PurahEngine::SceneManager::GetInstance().Initialize();

	// PhysicsSysyem 초기화
	PurahEngine::PhysicsSystem::GetInstance().Initialize();

	// SoundManager 초기화
	PurahEngine::SoundManager::GetInstance().Initialize();

}

void PurahEngine::GameLoop::Run(_In_ int nCmdShow)
{
	// 윈도우 핸들이 가장 중요
	if (!hWnd)
	{
		return;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 무한루프 (안에 메시지 루프와 게임 루프 등이 포함됨)
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
			// 엔진 동작
			run();
		}
	}
}

void PurahEngine::GameLoop::Finalize()
{
	PurahEngine::GraphicsManager::GetInstance().Finalize();
}

void PurahEngine::GameLoop::run()
{
	PurahEngine::TimeController::GetInstance().Update(timeInit);
	float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime(timeInit);

	PurahEngine::PhysicsSystem::GetInstance().PreStep();
	PurahEngine::PhysicsSystem::GetInstance().Simulation(0.02f);

	PurahEngine::InputManager::Getinstance().Update();
	PurahEngine::SceneManager::GetInstance().Update();
	PurahEngine::SoundManager::GetInstance().Update();

	PurahEngine::GraphicsManager::GetInstance().Run(deltaTime);
}

LRESULT CALLBACK PurahEngine::GameLoop::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

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
