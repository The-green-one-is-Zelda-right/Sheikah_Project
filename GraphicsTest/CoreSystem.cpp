#include "CoreSystem.h"

#include <fstream>
#include <cassert>
#include <vector>
#include <chrono>

#include "GraphicsResourceID.h"

// 암시적 링크 사용
#define ZELDA_GRAPHICS_IMPLICIT_LINK

#ifdef ZELDA_GRAPHICS_IMPLICIT_LINK
#include "ZeldaGraphics.h"
#pragma comment(lib, "ZeldaGraphics.lib")
#endif // ZELDA_GRAPHICS_IMPLICIT_LINK

void CoreSystem::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, unsigned int width, unsigned int height, bool screenresizeable, bool notitlebar, bool minimizable, bool maximizable)
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
	wcex.lpszClassName = gamename;
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);

	// 그 윈도우를 생성
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

#ifdef ZELDA_GRAPHICS_IMPLICIT_LINK
	renderer = ZeldaGraphics::CreateZeldaRenderer();
#else
	zeldaGraphicsDLL = LoadLibrary(L"ZeldaGraphics.dll");
	if (zeldaGraphicsDLL == nullptr)
	{
		// DLL 로드 실패
		assert(0);
	}

	auto createZeldaRenderer = reinterpret_cast<IZeldaRenderer * (*)()>(GetProcAddress(zeldaGraphicsDLL, "CreateZeldaRenderer"));
	if (createZeldaRenderer == nullptr)
	{
		// DLL 함수를 찾을 수 없습니다.
		assert(0);
	}

	renderer = createZeldaRenderer();
#endif

	renderer->Initialize(1920, 1080, true, hWnd, false, 1000.0f, 1.0f);
}

void CoreSystem::Finalize()
{
	renderer->Finalize();

#ifdef ZELDA_GRAPHICS_IMPLICIT_LINK
	ZeldaGraphics::ReleaseZeldaRenderer(renderer);
#else
	auto releaseZeldaRenderer = reinterpret_cast<void(*)(IZeldaRenderer*)>(GetProcAddress(zeldaGraphicsDLL, "ReleaseZeldaRenderer"));
	if (releaseZeldaRenderer == nullptr)
	{
		// DLL 함수를 찾을 수 없습니다.
		assert(0);
	}

	releaseZeldaRenderer(renderer);

	FreeLibrary(zeldaGraphicsDLL);
#endif
}

void CoreSystem::Run(_In_ int nCmdShow)
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
			// 엔진 동작
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
	static auto current_time = std::chrono::high_resolution_clock::now();
	static auto last_time = std::chrono::high_resolution_clock::now();
	current_time = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_time);
	auto deltaTime = static_cast<float>(duration.count()) / 1000000.0f;

	if (deltaTime < 0.016f)
	{
		Sleep(16 - static_cast<int>(deltaTime * 1000.0f));
		deltaTime = 0.016f;
	}

	if (deltaTime != 0.0f)
	{
		static int lastFPS = 0;
		static int currentFPS = 0;

		lastFPS = currentFPS;
		currentFPS = 1.0f / deltaTime;

		if (currentFPS != lastFPS)
		{
			//OutputDebugString((std::to_wstring(currentFPS) + L"\n").c_str());
		}
	}

	last_time = current_time;

	static Eigen::Matrix4f cameraMatrix;

	static std::vector<std::wstring> animationList;
	static std::vector<std::wstring> animationList2;
	static std::vector<float> animationPlayTimeList;
	static std::vector<float> animationPlayTimeList2;
	 
	static float pointLightRange_Max = 200.0f;
	static float pointLightRange_Min = 10.0f;
	static float pointLightRange_Speed = 50.0f;
	static float pointLightRange = 10.0f;
	static Eigen::Vector3f pointLightPos = { 0.0f, 50.0f, 0.0f };

	static bool firstRun = true;
	if (firstRun)
	{
		firstRun = false;

		scdTextureID = renderer->CreateTexture(L"scd.jpg");
		hnsTextureID = renderer->CreateTexture(L"hns.jpg");
		msTextureID = renderer->CreateTexture(L"exit_cursor.png");
		//fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Building\\Building.fbx");
		//fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Ganondorf (TotK).fbx");
		fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying6.fbx");
		//fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Sponza\\sponza.fbx");
		//fbxID = renderer->CreateModel(L"C:\\Users\\BEOMJOON\\Downloads\\Capoeira.fbx");
		//fbxID = renderer->CreateModel(L"Box2.fbx");
		//fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Cylinder\\Cylinder.fbx");
		//fbxID = renderer->CreateModel(L"C:\\Users\\KOCCA62\\Desktop\\Timmy_Shooting\\Timmy_Shooting.fbx");

		fbxID2 = renderer->CreateModel(L"D:\\GA4th4Q_Project\\Tree\\5_Project\\ZeldaEngine\\Resources\\FBX\\Boss\\Boss.fbx");
		//fbxID2 = renderer->CreateModel(L"C:\\Users\\BEOMJOON\\Downloads\\Capoeira.fbx");
		
		dirLightID = renderer->CreateDirectionalLight({ 0.2f, 0.2f, 0.2f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f });
		pointLightID = renderer->CreatePointLight({ 0.2f, 0.2f, 0.2f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, pointLightPos, pointLightRange);

		mainCameraID = renderer->CreateCamera();

		renderer->SetMainCamera(mainCameraID);

		cameraMatrix <<
			1.0f, 0.0f, 0.0f, 3.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, -10.0f,
			0.0f, 0.0f, 0.0f, 1.0f;

		animationList = renderer->GetAnimationListByModel(fbxID);
		animationPlayTimeList = renderer->GetAnimationPlayTime(fbxID);

		animationList2 = renderer->GetAnimationListByModel(fbxID2);
		animationPlayTimeList2 = renderer->GetAnimationPlayTime(fbxID2);
	}

	const static float moveSpeed = 0.5f;
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
		moveDelta *= 10;
		rotateX *= 10;
		rotateY *= 10;
		if (GetAsyncKeyState(VK_SHIFT))
		{
			moveDelta *= 10;
			rotateX *= 10;
			rotateY *= 10;
		}

		if (GetAsyncKeyState(VK_F1)) renderer->SetDebugMode(DebugMode::Normal);
		if (GetAsyncKeyState(VK_F2)) renderer->SetDebugMode(DebugMode::DeferredDebugAll);
		if (GetAsyncKeyState(VK_F3)) renderer->SetDebugMode(DebugMode::DeferredDebug0);
		if (GetAsyncKeyState(VK_F4)) renderer->SetDebugMode(DebugMode::DeferredDebug1);
		if (GetAsyncKeyState(VK_F5)) renderer->SetDebugMode(DebugMode::DeferredDebug2);
		if (GetAsyncKeyState(VK_F6)) renderer->SetDebugMode(DebugMode::DeferredDebug3);
		if (GetAsyncKeyState(VK_F7)) renderer->SetDebugMode(DebugMode::DeferredDebug4);
		if (GetAsyncKeyState(VK_F8)) renderer->SetDebugMode(DebugMode::DeferredDebug5);
		if (GetAsyncKeyState(VK_F11)) renderer->SetRendererMode(RendererMode::OffWireFrameMode);
		if (GetAsyncKeyState(VK_F12)) renderer->SetRendererMode(RendererMode::OnWireFrameMode);
	}

	if (GetAsyncKeyState('O'))
	{
		pointLightRange += (pointLightRange_Speed * deltaTime);
		if (pointLightRange > pointLightRange_Max)
		{
			pointLightRange = pointLightRange_Max;
		}
		renderer->UpdateLight(pointLightID, { 0.2f, 0.2f, 0.2f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {}, pointLightPos, pointLightRange, 0.0f);
	}
	if (GetAsyncKeyState('P'))
	{
		pointLightRange -= (pointLightRange_Speed * deltaTime);
		if (pointLightRange < pointLightRange_Min)
		{
			pointLightRange = pointLightRange_Min;
		}
		renderer->UpdateLight(pointLightID, { 0.2f, 0.2f, 0.2f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {}, pointLightPos, pointLightRange, 0.0f);
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

	renderer->UpdateCamera(mainCameraID, cameraMatrix, 3.141592654f / 4.0f, 0.1f, 1000.0f);

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
		0, 0.1, 0, 0,
		0, 0, 0.1, 30,
		0, 0, 0, 1;

	// 애니메이션 테스트 코드
	const static int repeatPlay = 5;

	static float animationTime = 0.0f;
	static int animationNumber = 0;
	static int playCount = 0;
	animationTime += deltaTime;

	if (animationList.size() != 0)
	{
		while (animationTime >= animationPlayTimeList[animationNumber])
		{
			animationTime -= animationPlayTimeList[animationNumber];
			playCount += 1;

			if (playCount >= repeatPlay)
			{
				playCount = 0;
				animationNumber += 1;

				if (animationNumber >= animationList.size())
				{
					animationNumber = 0;
				}
			}
		}
	}

	static float animationTime2 = 0.0f;
	static int animationNumber2 = 0;
	static int playCount2 = 0;
	animationTime2 += deltaTime;

	if (animationList2.size() != 0)
	{
		while (animationTime2 >= animationPlayTimeList2[animationNumber2])
		{
			animationTime2 -= animationPlayTimeList2[animationNumber2];
			playCount2 += 1;

			if (playCount2 >= repeatPlay)
			{
				playCount2 = 0;
				animationNumber2 += 1;

				if (animationNumber2 >= animationList2.size())
				{
					animationNumber2 = 0;
				}
			}
		}
	}
	
	Eigen::Matrix4f rightPosMatrix;
	rightPosMatrix <<
		0.1, 0, 0, 900,
		0, 0.1, 0, 0,
		0, 0, 0.1, 30,
		0, 0, 0, 1;

	static int scdX = 0;
	scdX += 4;

	if (scdX > 1920)
	{
		scdX -= 1920;
	}

	renderer->DrawLight(dirLightID);
	renderer->DrawLight(pointLightID);

	renderer->DrawSprite({ scdX, 0 }, msTextureID);
	renderer->DrawSprite({ 1920 - scdX - 280, 800 }, msTextureID);
	//renderer->DrawSprite({ 1920 - scdX - 280, 800 }, hnsTextureID);


	renderer->DrawModel(rightPosMatrix, fbxID, false);
	//renderer->DrawModel(ganonMatrix, fbxID, false);

	renderer->DrawAnimation(ganonMatrix, fbxID, animationNumber2 != 0 ? animationList[animationNumber] : L"", animationTime, false);
	//renderer->DrawAnimation(ganonMatrix, fbxID, animationNumber2 != 0 ? animationList[animationNumber] : L"", 4.0f, false);
	//renderer->DrawAnimation(ganonMatrix, fbxID2, animationNumber2 != 0 ? animationList2[animationNumber2] : L"", animationTime2, false);

	Eigen::Matrix4f cubeMatrix = Eigen::Matrix4f::Identity();
	cubeMatrix(0, 0) = 10.0f;
	cubeMatrix(1, 1) = 10.0f;
	cubeMatrix(2, 2) = 10.0f;

	cubeMatrix(0, 3) = 3.0f;
	cubeMatrix(1, 3) = 1.0f;
	cubeMatrix(2, 3) = 10.0f;

	renderer->DrawCube(cubeMatrix, scdTextureID, false, 1.0f, 0.0f, 0.0f, 1.0f);
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
			// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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