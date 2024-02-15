#include "GraphicsManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "PhysicsSystem.h"
#include "Renderer.h"
#include "Camera.h"
#include "GraphicsResourceManager.h"

#include <Eigen/Dense>
#include <cassert>

#ifdef YH_DEBUG
#pragma comment (lib, "../Debug/ZeldaGraphicsAdapter.lib")
#elif YH_RELEASE
#pragma comment (lib, "../Release/ZeldaGraphicsAdapter.lib")
#endif


void PurahEngine::GraphicsManager::Initialize(HWND hWnd)
{
#ifdef YH_RENDERER
	renderer = new IZeldaRendererAdapter();
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
	graphicsModule = createZeldaRenderer();
#endif

	graphicsModule->Initialize(1920, 1080, true, hWnd, false);

	resourceManager = new GraphicsResourceManager(graphicsModule);
}

void PurahEngine::GraphicsManager::Finalize()
{
#ifdef YH_RENDERER
	assert(0); // Not implement to delete YH Renderer 
#else
	auto releaseZeldaRenderer = reinterpret_cast<void(*)(IZeldaRenderer*)>(GetProcAddress(zeldaGraphicsDLL, "ReleaseZeldaRenderer"));
	if (releaseZeldaRenderer == nullptr)
	{
		// DLL 함수를 찾을 수 없습니다.
		assert(0);
	}

	releaseZeldaRenderer(graphicsModule);

	FreeLibrary(zeldaGraphicsDLL);
#endif

	if (resourceManager != nullptr)
	{
		delete resourceManager;
	}
}

void PurahEngine::GraphicsManager::Run(float deltaTime)
{
	graphicsModule->BeginDraw(deltaTime);

	// Camera
	Camera* mainCamera = PurahEngine::SceneManager::GetInstance().GetMainCamera();
	mainCamera->Render(graphicsModule);

	// Light
	for (auto light : lightList)
	{
		light->Render(graphicsModule);
	}

	// Object
	for (auto render : rendererList)
	{
		render->Render(graphicsModule);
	}

	graphicsModule->EndDraw();
}

void PurahEngine::GraphicsManager::AddRenderer(IRenderer* renderer)
{
	rendererList.push_back(renderer);
}

void PurahEngine::GraphicsManager::RemoveRenderer(IRenderer* renderer)
{
	rendererList.erase(remove(rendererList.begin(), rendererList.end(), renderer), rendererList.end());
}

void PurahEngine::GraphicsManager::AddLight(IRenderer* renderer)
{
	lightList.push_back(renderer);
}

void PurahEngine::GraphicsManager::RemoveLight(IRenderer* renderer)
{
	lightList.erase(remove(lightList.begin(), lightList.end(), renderer), lightList.end());
}

PurahEngine::GraphicsManager::GraphicsManager() :
	graphicsModule(nullptr),
	zeldaGraphicsDLL(NULL),
	resourceManager(nullptr)
{

}

PurahEngine::GraphicsManager::~GraphicsManager()
{

}

PurahEngine::GraphicsManager& PurahEngine::GraphicsManager::GetInstance()
{
	static GraphicsManager instance;
	return instance;
}
