#include "CoreSystem.h"

#include <fstream>
#include <cassert>
#include <vector>

#include "GraphicsResourceID.h"

void CoreSystem::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, unsigned int width, unsigned int height, bool screenresizeable, bool notitlebar, bool minimizable, bool maximizable)
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
	wcex.lpszClassName = gamename;
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);

	// �� �����츦 ����
	hWnd = CreateWindowW(
		gamename,
		gamename,
		screenresizeable ? WS_OVERLAPPEDWINDOW : (notitlebar ? (WS_POPUP) : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | (minimizable ? WS_MINIMIZEBOX : 0L) | (maximizable ? WS_MAXIMIZEBOX : 0L))), // WS_EX_TOPMOST | WS_POPUP,
		0,
		0,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	SetMenu(hWnd, NULL);

	resolutionX = width;
	resolutionY = height;
	lastWidth = width;
	lastHeight = height;

	isFullScreenMode = false;
	resizable = screenresizeable;


	zeldaGraphicsDLL = LoadLibrary(L"ZeldaGraphics.dll");
	if (zeldaGraphicsDLL == nullptr)
	{
		// DLL �ε� ����
		assert(0);
	}

	auto createZeldaRenderer = reinterpret_cast<IZeldaRenderer*(*)()>(GetProcAddress(zeldaGraphicsDLL, "CreateZeldaRenderer"));
	if (createZeldaRenderer == nullptr)
	{
		// DLL �Լ��� ã�� �� �����ϴ�.
		assert(0);
	}

	renderer = createZeldaRenderer();

	renderer->Initialize(1920, 1080, true, hWnd, false, 1000.0f, 1.0f);

	renderer->CreateBasicResources();
}

void CoreSystem::Finalize()
{
	renderer->Finalize();

	auto releaseZeldaRenderer = reinterpret_cast<void(*)(IZeldaRenderer*)>(GetProcAddress(zeldaGraphicsDLL, "ReleaseZeldaRenderer"));
	if (releaseZeldaRenderer == nullptr)
	{
		// DLL �Լ��� ã�� �� �����ϴ�.
		assert(0);
	}

	releaseZeldaRenderer(renderer);

	FreeLibrary(zeldaGraphicsDLL);
}

void CoreSystem::Run(_In_ int nCmdShow)
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

	isRun = true;

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

	isRun = false;
}

void CoreSystem::ChangeToFullScreenMode()
{
	if (isFullScreenMode == true)
	{
		return;
	}
	isFullScreenMode = true;

	RECT r;
	GetClientRect(hWnd, &r);
	lastWidth = r.right - r.left;
	lastHeight = r.bottom - r.top;

	SetWindowLongPtr(hWnd, GWL_STYLE, WS_EX_TOPMOST | WS_POPUP); //3d argument=style
	SetWindowPos(hWnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
}

void CoreSystem::ChangeToWindowMode()
{
	if (isFullScreenMode == false)
	{
		return;
	}
	isFullScreenMode = false;

	SetWindowLongPtr(hWnd, GWL_STYLE, resizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)); //3d argument=style
	SetWindowPos(hWnd, NULL, (GetSystemMetrics(SM_CXSCREEN) - lastWidth + 16) / 2, (GetSystemMetrics(SM_CYSCREEN) - lastHeight + 39) / 2, lastWidth + 16, lastHeight + 39, SWP_SHOWWINDOW);
}

void CoreSystem::ChangeWindowSize(unsigned int width, unsigned int height)
{
	if (isFullScreenMode == true || (lastWidth == width && lastHeight == height))
	{
		return;
	}
	lastWidth = width;
	lastHeight = height;

	SetWindowLongPtr(hWnd, GWL_STYLE, resizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)); //3d argument=style
	SetWindowPos(hWnd, NULL, (GetSystemMetrics(SM_CXSCREEN) - width + 16) / 2, (GetSystemMetrics(SM_CYSCREEN) - height + 39) / 2, width + 16, height + 39, SWP_SHOWWINDOW);
}

void CoreSystem::SetResizable(bool value)
{
	if (resizable == value)
	{
		return;
	}

	resizable = value;

	if (isFullScreenMode == false)
	{
		ChangeToFullScreenMode();
		ChangeToWindowMode();
	}
}

unsigned int CoreSystem::GetScreenWidth()
{
	return lastWidth;
}

unsigned int CoreSystem::GetScreenHeight()
{
	return lastHeight;
}

bool CoreSystem::IsFullScreenMode()
{
	return isFullScreenMode;
}

bool CoreSystem::IsRun()
{
	return isRun;
}
 
void CoreSystem::run()
{
	static Eigen::Matrix4f cameraMatrix;

	static bool firstRun = true;
	if (firstRun)
	{
		firstRun = false;

		scdTextureID = renderer->CreateTexture(L"scd.jpg");
		//fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Building\\Building.fbx");
		//fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Ganondorf (TotK).fbx");
		fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying.fbx");

		mainCameraID = renderer->CreateCamera();

		renderer->SetMainCamera(mainCameraID);

		cameraMatrix <<
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, -10.0f,
			0.0f, 0.0f, 0.0f, 1.0f;
	}

	const static float moveSpeed = 0.1f;
	const static float rotateSpeed = 0.03f;

	float rotateX = 0.0f;
	float rotateY = 0.0f;
	Eigen::Vector4f moveDelta = Eigen::Vector4f::Zero();

	if (GetAsyncKeyState('W')) moveDelta += (cameraMatrix * Eigen::Vector4f::UnitZ()) * moveSpeed;
	if (GetAsyncKeyState('S')) moveDelta -= (cameraMatrix * Eigen::Vector4f::UnitZ()) * moveSpeed;
	if (GetAsyncKeyState('A')) moveDelta -= (cameraMatrix * Eigen::Vector4f::UnitX()) * moveSpeed;
	if (GetAsyncKeyState('D')) moveDelta += (cameraMatrix * Eigen::Vector4f::UnitX()) * moveSpeed;
	if (GetAsyncKeyState('Q')) moveDelta -= (cameraMatrix * Eigen::Vector4f::UnitY()) * moveSpeed;
	if (GetAsyncKeyState('E')) moveDelta += (cameraMatrix * Eigen::Vector4f::UnitY()) * moveSpeed;
	if (GetAsyncKeyState(VK_UP)) rotateX -= rotateSpeed;
	if (GetAsyncKeyState(VK_DOWN)) rotateX += rotateSpeed;
	if (GetAsyncKeyState(VK_LEFT)) rotateY -= rotateSpeed;
	if (GetAsyncKeyState(VK_RIGHT)) rotateY += rotateSpeed;

	if (GetAsyncKeyState(VK_CONTROL))
	{
		moveDelta *= 500;
		rotateX *= 500;
		rotateY *= 500;
	}

	Eigen::Matrix4f cameraRotateY;
	cameraRotateY <<
		cos(rotateY), 0, sin(rotateY), 0,
		0, 1, 0, 0,
		-sin(rotateY), 0, cos(rotateY), 0,
		0, 0, 0, 1;
	Eigen::Matrix4f cameraRotateX;
	cameraRotateX <<
		1, 0, 0, 0,
		0, cos(rotateX), -sin(rotateX), 0,
		0, sin(rotateX), cos(rotateX), 0,
		0, 0, 0, 1;
	Eigen::Matrix4f cameraMove;
	cameraMove <<
		1, 0, 0, moveDelta.x(),
		0, 1, 0, moveDelta.y(),
		0, 0, 1, moveDelta.z(),
		0, 0, 0, 1;

	cameraMatrix = cameraMove * cameraMatrix * cameraRotateX * cameraRotateY;

	renderer->UpdateCamera(mainCameraID, cameraMatrix, 3.141592654f / 4.0f, 1.0f, 10000.0f);

	renderer->BeginDraw(0.016f);

	static float falling = 0.0f;

	falling += -0.03f;

	if (falling < -5.0f)
	{
		falling = 5.0f;
	}

	Eigen::Matrix4f fallingMatrix;
	fallingMatrix <<
		1, 0, 0, 0,
		0, 1, 0, falling,
		0, 0, 1, 0,
		0, 0, 0, 1;

	static float rotation = 0.0f;

	rotation += 3.141592f * 0.01f;

	if (rotation > 3.141592f * 2.0f)
	{
		rotation -= 3.141592f * 2.0f;
	}

	Eigen::Matrix4f worldMatrix;
	worldMatrix <<
		cos(rotation), 0, sin(rotation), 0,
		0, 1, 0, 0,
		-sin(rotation), 0, cos(rotation), 0,
		0, 0, 0, 1;

	static float rotation2 = 0.0f;

	rotation2 += 3.141592f * 0.015f;

	if (rotation2 > 3.141592f * 2.0f)
	{
		rotation2 -= 3.141592f * 2.0f;
	}

	Eigen::Matrix4f worldMatrix2;
	worldMatrix2 <<
		cos(rotation2), 0, -sin(rotation2), -3.0f,
		0, 1, 0, 0,
		sin(rotation2), 0, cos(rotation2), 0,
		0.0f, 0, 0, 1;

	Eigen::Matrix4f ganonMatrix;
	ganonMatrix <<
		0.1, 0, 0, 0,
		0, 0.1, 0, -10,
		0, 0, 0.1, 30,
		0, 0, 0, 1;

	//renderer->DrawModel(worldMatrix, fbxID, false);
	renderer->DrawModel(Eigen::Matrix4f::Identity(), fbxID, false);

	renderer->DrawCube(fallingMatrix * worldMatrix, scdTextureID, false, 1.0f, 0.0f, 0.0f, 1.0f);
	//renderer->DrawCube(fallingMatrix * worldMatrix2, ID_NULL, false, 0.0f, 1.0f, 1.0f, 1.0f);



	
	renderer->EndDraw();
}

LRESULT CoreSystem::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
			EndPaint(hWnd, &ps);
			break;
		}

		case WM_ENTERSIZEMOVE:
		{
			break;
		}

		case WM_MOVE:
		{
			break;
		}

		case WM_EXITSIZEMOVE:
		{
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		case WM_SYSKEYDOWN:
		{
			if (wParam == VK_MENU)
			{

				return 0;
			}

			if ((wParam & 0xFFF0) == SC_KEYMENU)
			{
				return 0;
			}

			break;
		}

		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

CoreSystem& CoreSystem::GetInstance()
{
	static CoreSystem instance;
	return instance;
}

CoreSystem::CoreSystem() : firstRun(true), runningState(RunningState::INITIALIZATION), isRun(false)
{

}

CoreSystem::~CoreSystem()
{

}