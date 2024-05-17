#define _CRT_SECURE_NO_WARNINGS

#include "GameLoop.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "InputManager.h"
#include "PhysicsSystem.h"
#include "TimeController.h"
#include "Timer.h"
#include "SoundManager.h"
#include "EngineSetting.h"
#include "UnifiedInputManager.h"
#include <cassert>


/// <summary>
///  �ӽ�
/// </summary>
#include "GamePadManager.h"
#include "../ZonaiPhysicsBase/ZnCollider.h"
#include "../ZonaiPhysicsBase/ZnRigidBody.h"

ZonaiPhysics::ZnCollider* collider;
ZonaiPhysics::ZnRigidBody* rigidBody;

static std::string timeInit = "Init";
PurahEngine::ErrorType PurahEngine::GameLoop::errorType = PurahEngine::ErrorType::UnKnown;

PurahEngine::GameLoop::GameLoop()
{

}

PurahEngine::GameLoop::~GameLoop()
{

}

void PurahEngine::GameLoop::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)
{
	SetUnhandledExceptionFilter(UnhandledExceptionFilter);

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

#ifdef _DEBUG
	{
		// �ܼ�â ����
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}
#endif

	SetMenu(hWnd, NULL);

	// Engine Setting �ʱ�ȭ (GraphicsManager, Physics Manager�� �ʱ�ȭ���� ���� �Ǿ�� �Ѵ�.)
	EngineSetting::GetInstance().Initialize();

	// �ð������� �ʱ�ȭ
	PurahEngine::TimeController::GetInstance().Initialize(timeInit);

	// Graphics dll �ʱ�ȭ(���� ���ɼ� ����)
	PurahEngine::GraphicsManager::GetInstance().Initialize(hWnd);

	// InputManager �ʱ�ȭ
	eKey key[] =
	{
		eKey::eKEY_W,
		eKey::eKEY_A,
		eKey::eKEY_S,
		eKey::eKEY_D,
		eKey::eKEY_O,
		eKey::eKEY_P,
		eKey::eKEY_Q,
		eKey::eKEY_E,
		eKey::eKEY_J,
		eKey::eKEY_I,
		eKey::eKEY_K,
		eKey::eKEY_L,
		eKey::eKEY_U,
		eKey::eKEY_M,

		eKey::eKEY_LEFT,
		eKey::eKEY_RIGHT,
		eKey::eKEY_UP,
		eKey::eKEY_DOWN,

		eKey::eKEY_ESCAPE,
		eKey::eKEY_SHIFT,
		eKey::eKEY_LSHIFT,
		eKey::eKEY_0,
		eKey::eKEY_1,
		eKey::eKEY_2,

		eKey::eKEY_SPACE,
		eKey::eKEY_CONTROL,
		eKey::eKEY_F1,
		eKey::eKEY_F2,
		eKey::eKEY_F3,
		eKey::eKEY_F4,
		eKey::eKEY_F5,
		eKey::eKEY_F6,
		eKey::eKEY_F7,
		eKey::eKEY_F8,

		eKey::eKEY_F11,
		eKey::eKEY_F12
	};
	PurahEngine::InputManager::Getinstance().Initialize(hWnd, key, sizeof(key) / sizeof(eKey));
	PurahEngine::GamePadManager::Instance().Initialize(hWnd, nullptr, 0);
	UnifiedInputManager::Getinstance().Initialize();

	// SceneManager �ʱ�ȭ
	PurahEngine::SceneManager::GetInstance().Initialize();

	// PhysicsSysyem �ʱ�ȭ
	PurahEngine::PhysicsSystem::GetInstance().Initialize();

	// SoundManager �ʱ�ȭ
	PurahEngine::SoundManager::GetInstance().Initialize();

	// 0�� �� �ε�
	SceneManager::GetInstance().LoadScene(EngineSetting::GetInstance().GetScene(0));
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
	SoundManager::GetInstance().Finalize();
	GraphicsManager::GetInstance().Finalize();
	PhysicsSystem::GetInstance().Finalize();
	GamePadManager::Instance().Release();

#ifdef _DEBUG
	FreeConsole();
#endif
}

void PurahEngine::GameLoop::run()
{
	TimeController::GetInstance().Update(timeInit);

	Timer::PreUpdate();
	const float deltaTime = TimeController::GetInstance().GetDeltaTime(timeInit);

	PhysicsSystem::GetInstance().PreStep();
	PhysicsSystem::GetInstance().Simulation(deltaTime);
	PhysicsSystem::GetInstance().SimulateResult();

	InputManager::Getinstance().Update();
	GamePadManager::Instance().Update();
	// InputManager, GamePadManager ���Ŀ� �����ؾ��Ѵ�.
	UnifiedInputManager::Getinstance().Update();

	Timer::Update();
	if (InputManager::Getinstance().IsKeyPressed(eKey::eKEY_SHIFT) && InputManager::Getinstance().IsKeyDown(eKey::eKEY_ESCAPE))
	{
		SceneManager::GetInstance().LoadScene(L"DataExportTestWorldObjectInfo.json");
	}

	SceneManager::GetInstance().LoadScene();
	SceneManager::GetInstance().InitializationEvent();

	SceneManager::GetInstance().Update();
	SoundManager::GetInstance().Update();

	GraphicsManager::GetInstance().UpdateAnimator(deltaTime);
	GraphicsManager::GetInstance().Render(deltaTime);
	SceneManager::GetInstance().DecommissionEvent();


	if (TimeController::GetInstance().GetDeltaTime() != 0.f)
	{
		static int currFPS = 0;
		static float count = 0;
		static int minFPS = FLT_MAX;
		static int maxFPS = FLT_MIN;

		minFPS = min(minFPS, currFPS);
		maxFPS = max(maxFPS, currFPS);

		currFPS = (int) (1.f / TimeController::GetInstance().GetFPS());
		count += TimeController::GetInstance().GetDeltaTime();

		if (count > 1.f)
		{
			SetWindowText((HWND)hWnd,
				(
					L"fps : " + std::to_wstring(currFPS) +
					L", min : " + std::to_wstring(minFPS) +
					L", max : " + std::to_wstring(maxFPS)
					).c_str()
			);

			count -= 1.f;
			minFPS = INT_MAX;
			maxFPS = INT_MIN;
		}
	}

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

		//case WM_SIZE:
		//{
		//	UINT width = LOWORD(lParam);
		//	UINT height = HIWORD(lParam);

		//	UINT menubarHeight = 11u;

		//	OutputDebugString((L"" + std::to_wstring(width) + L"" + std::to_wstring(height - menubarHeight)).c_str());
		//	break;
		//}

		case WM_ENTERSIZEMOVE:
		{
			TimeController::GetInstance().PauseAll();
			break;
		}

		case WM_EXITSIZEMOVE:
		{
			TimeController::GetInstance().ResumeAll();
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

LONG WINAPI PurahEngine::GameLoop::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	switch (errorType)
	{
		case PurahEngine::ErrorType::UnKnown:
		{
			MessageBox(0, L"Unknown Error Type\n", L"PurahEngine::GameLoop Error", MB_OK);
			break;
		}
		case PurahEngine::ErrorType::Failed_Deserialize:
		{
			MessageBox(0, L"Failed to deserialize data\n", L"PurahEngine::GameLoop Error", MB_OK);
			break;
		}
		default:
		{
			MessageBox(0, L"Not Defined Error Type\nPlease Check PurahEngine::GameLoop::UnhandledExceptionFilter", L"PurahEngine::GameLoop Error", MB_OK);
			break;
		}
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

PurahEngine::GameLoop& PurahEngine::GameLoop::GetInstance()
{
	static GameLoop instance;
	return instance;
}
