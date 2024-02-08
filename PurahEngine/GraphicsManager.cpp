#include "GraphicsManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "PhysicsSystem.h"
#include "Renderer.h"
#include "Camera.h"
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
	renderer = createZeldaRenderer();
#endif
	renderer->Initialize(1920, 1080, true, hWnd, false, 1000.0f, 1.0f);


}

void PurahEngine::GraphicsManager::Run()
{
	renderer->BeginDraw(1.0f);
	Camera* mainCamera = PurahEngine::SceneManager::GetInstance().GetMainCamera();
	Eigen::Matrix4f position = mainCamera->GetGameObject()->GetComponent<PurahEngine::Transform>()->GetWorldMatrix();
	
	mainCamera->SetRenderer(renderer);
	mainCamera->UpdateCamera(position, 3.141592654f / 4.0f, 1.0f, 1000.0f);

	for (auto render : rendererList)
	{
		render->Render(renderer);
	}

	renderer->EndDraw();
}

IZeldaRenderer* PurahEngine::GraphicsManager::GetRenderer()
{
	return renderer;
}

void PurahEngine::GraphicsManager::AddRenderer(Renderer* render)
{
	rendererList.push_back(render);
}

void PurahEngine::GraphicsManager::RemoveRenderer()
{

}

PurahEngine::GraphicsManager::GraphicsManager()
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
