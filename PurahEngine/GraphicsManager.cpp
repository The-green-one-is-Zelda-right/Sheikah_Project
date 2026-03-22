#include "GraphicsManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "PhysicsSystem.h"
#include "Renderer.h"
#include "ModelRenderer.h"
#include "Animator.h"
#include "Camera.h"
#include "GraphicsResourceManager.h"
#include "EngineSetting.h"

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
	OutputDebugStringW(std::to_wstring(GetLastError()).c_str());
	if (zeldaGraphicsDLL == nullptr)
	{
		// DLL 로드 실패
		assert(0);
		return;
	}
	auto createZeldaRenderer = reinterpret_cast<IZeldaRenderer * (*)()>(GetProcAddress(zeldaGraphicsDLL, "CreateZeldaRenderer"));

	if (createZeldaRenderer == nullptr)
	{
		// DLL 함수를 찾을 수 없습니다.
		assert(0);
		return;
	}
	graphicsModule = createZeldaRenderer();
#endif

	EngineSetting& setting = EngineSetting::GetInstance();

	graphicsModule->SetExtraInitOption(setting.GetShadowAreaRange(), setting.GetShadowAreaOffset(), setting.GetShadowMapSize());

	graphicsModule->Initialize(1920, 1080, false, hWnd, false);
	resourceManager = new GraphicsResourceManager(graphicsModule);

	// 모델링 파일 미리 로드
	std::vector<std::wstring> preloadmodels = setting.GetPreLoadModels();
	for (int i = 0; i < preloadmodels.size(); i++)
	{
		RegisterDefaultModelFile(preloadmodels[i]);
	}
}

void PurahEngine::GraphicsManager::Finalize()
{
	// 리소스 매니저가 먼저 해제된 후에, graphicsModule이 해제되어야한다.
	if (resourceManager != nullptr)
	{
		delete resourceManager;
	}

	graphicsModule->Finalize();

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
}

void PurahEngine::GraphicsManager::RegisterDefaultModelFile(const std::wstring& fileName)
{
	resourceManager->GetModelID(fileName);
}

void PurahEngine::GraphicsManager::UpdateAnimator(float deltaTime)
{
	for (auto iter = animatorList.begin(); iter != animatorList.end(); iter++)
	{
		Animator* animator = *iter;
		animator->UpdateAnimation(deltaTime);
	}
}

void PurahEngine::GraphicsManager::Render(float deltaTime)
{
	InputManager& inputManager = InputManager::Getinstance();
	if (inputManager.IsKeyPressed(eKey::eKEY_CONTROL))
	{
		if (inputManager.IsKeyDown(eKey::eKEY_F1)) graphicsModule->SetDebugMode(DebugMode::Normal);
		if (inputManager.IsKeyDown(eKey::eKEY_F2)) graphicsModule->SetDebugMode(DebugMode::DeferredDebugAll);
		if (inputManager.IsKeyDown(eKey::eKEY_F3)) graphicsModule->SetDebugMode(DebugMode::DeferredDebug0);
		if (inputManager.IsKeyDown(eKey::eKEY_F4)) graphicsModule->SetDebugMode(DebugMode::DeferredDebug1);
		if (inputManager.IsKeyDown(eKey::eKEY_F5)) graphicsModule->SetDebugMode(DebugMode::DeferredDebug2);
		if (inputManager.IsKeyDown(eKey::eKEY_F6)) graphicsModule->SetDebugMode(DebugMode::DeferredDebug3);
		if (inputManager.IsKeyDown(eKey::eKEY_F7)) graphicsModule->SetDebugMode(DebugMode::DeferredDebug4);
		if (inputManager.IsKeyDown(eKey::eKEY_F8)) graphicsModule->SetDebugMode(DebugMode::DeferredDebug5);


		if (inputManager.IsKeyDown(eKey::eKEY_F11)) graphicsModule->SetRendererMode(RendererMode::OffWireFrameMode);
		if (inputManager.IsKeyDown(eKey::eKEY_F12)) graphicsModule->SetRendererMode(RendererMode::OnWireFrameMode);

	}

	EngineSetting& setting = EngineSetting::GetInstance();

	graphicsModule->SetExtraOption(setting.GetShadowMapDepthBias(), setting.GetPointLightDepthBias());

	graphicsModule->BeginDraw(deltaTime);

	// Camera
	Camera* mainCamera = PurahEngine::SceneManager::GetInstance().GetMainCamera();
	if (mainCamera->GetGameObject()->IsRootEnable())
	{
		mainCamera->Render(graphicsModule);
	}

	// Light
	for (auto light : lightList)
	{
		if (light->IsRootEnable())
		{
			light->Render(graphicsModule);
		}
	}

	// Object
	for (auto render : rendererList)
	{
		if (render->IsRootEnable())
		{
			render->Render(graphicsModule);
		}
	}


	// String
	for (auto& renderInfo : stringList)
	{
		graphicsModule->DrawString(renderInfo.str, renderInfo.x, renderInfo.y, renderInfo.width, renderInfo.height, renderInfo.fontSize, { renderInfo.r, renderInfo.g, renderInfo.b, renderInfo.a });
	}
	stringList.clear();

	graphicsModule->EndDraw();
}

void PurahEngine::GraphicsManager::DrawString(const std::wstring& string, float x, float y, float width, float height, float fontSize, float r, float g, float b, float a)
{
	StringRenderInfo renderInfo;
	renderInfo.str = string;
	renderInfo.x = x;
	renderInfo.y = y;
	renderInfo.width = width;
	renderInfo.height = height;
	renderInfo.fontSize = fontSize;
	renderInfo.r = r;
	renderInfo.g = g;
	renderInfo.b = b;
	renderInfo.a = a;
	stringList.push_back(renderInfo);
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

void PurahEngine::GraphicsManager::AddAnimator(Animator* animator)
{
	animatorList.push_back(animator);
}

void PurahEngine::GraphicsManager::RemoveAnimator(Animator* animator)
{
	animatorList.erase(remove(animatorList.begin(), animatorList.end(), animator), animatorList.end());
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
