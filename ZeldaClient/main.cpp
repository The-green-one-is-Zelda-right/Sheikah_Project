#include <windows.h>
#include <iostream>
#include <cassert>
#include "PurahEngine.h"

#include "TestMovement.h"
#include "CameraMovement.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";

	std::string path = "PurahEngine.dll";
	HMODULE PurahEngineDll = ::LoadLibraryA(path.c_str());

	assert(PurahEngineDll != nullptr);

	auto CreateInitialize = reinterpret_cast<void (*)(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)>(GetProcAddress(PurahEngineDll, "Initialize"));
	auto CreateRun = reinterpret_cast<void (*)(_In_ int nCmdShow)>(GetProcAddress(PurahEngineDll, "Run"));
	auto CreateFinalize = reinterpret_cast<void (*)()>(GetProcAddress(PurahEngineDll, "Finalize"));

	// DLL 함수를 찾을 수 없습니다.
	assert(!(CreateInitialize == nullptr || CreateRun == nullptr || CreateFinalize == nullptr));

	//PurahEngine::Initialize(hInstance, gameName.c_str(), 1920, 1080); 역할
	CreateInitialize(hInstance, gameName.c_str(), 1920, 1080);

	auto renderer = PurahEngine::GraphicsManager::GetInstance().GetRenderer();
	static TextureID textureID = TextureID::ID_NULL;
	static ModelID modelID = ModelID::ID_NULL;
	static CameraID cameraID = CameraID::ID_NULL;


	

	PurahEngine::GameObject* testObject = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject");
	PurahEngine::GameObject* camera = PurahEngine::SceneManager::GetInstance().GetMainCamera()->GetGameObject();
	camera->AddComponent<PurahEngine::Camera>();
	testObject->AddComponent<PurahEngine::Renderer>();

	camera->GetComponent<PurahEngine::Transform>()->SetLocalPosition(Eigen::Vector3f(0, 0, -10));
	camera->GetComponent<PurahEngine::Camera>()->SetRenderer(renderer);
	camera->GetComponent<PurahEngine::Camera>()->CreateCamera();
	camera->GetComponent<PurahEngine::Camera>()->SetMainCamera();
	testObject->AddComponent<PurahEngine::TestMovement>();
	camera->AddComponent<PurahEngine::CameraMovement>();

	textureID = renderer->CreateTexture(L"scd.jpg");
	if (textureID == TextureID::ID_NULL)
	{
		assert(0);
	}
	testObject->GetComponent<PurahEngine::Renderer>()->AddTexture(textureID);


	//PurahEngine::Run(); 역할
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); 역할
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}
