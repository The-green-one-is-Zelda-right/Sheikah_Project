#include <windows.h>
#include <iostream>
#include <cassert>
#include "PurahEngine.h"

#include "TestMovement.h"
#include "CameraMovement.h"
#include "../ZeldaGraphics/ZeldaGraphicsDefine.h"

namespace PurahEngine
{
	class BoxCollider;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";

	std::string path = "PurahEngine.dll";
	HMODULE PurahEngineDll = ::LoadLibraryA(path.c_str());

	assert(PurahEngineDll != nullptr);

	auto CreateInitialize = reinterpret_cast<void (*)(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)>(GetProcAddress(PurahEngineDll, "Initialize"));
	auto CreateRun = reinterpret_cast<void (*)(_In_ int nCmdShow)>(GetProcAddress(PurahEngineDll, "Run"));
	auto CreateFinalize = reinterpret_cast<void (*)()>(GetProcAddress(PurahEngineDll, "Finalize"));

	// DLL �Լ��� ã�� �� �����ϴ�.
	assert(!(CreateInitialize == nullptr || CreateRun == nullptr || CreateFinalize == nullptr));

	//PurahEngine::Initialize(hInstance, gameName.c_str(), 1920, 1080); ����
	CreateInitialize(hInstance, gameName.c_str(), 1920, 1080);

	auto renderer = PurahEngine::GraphicsSystem::GetInstance().GetRenderer();
	static TextureID textureID = TextureID::ID_NULL;
	static ModelID modelID = ModelID::ID_NULL;
	static CameraID cameraID = CameraID::ID_NULL;

	const auto light = renderer->CreateDirectionalLight(
		{ 0.2f, 0.2f, 0.2f },
		{ 1.f, 1.f, 1.f },
		{ 1.f, 1.f,1.f },
		{ 1.f, -1.f, 1.f }
	);

	renderer->DrawLight(light);

	// const auto light = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Light");

	//PurahEngine::GameObject* box = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box01");

	//auto boxTrans = box->GetComponent<PurahEngine::Transform>();
	//boxTrans->SetLocalPosition({ 0, 5, 0 });

	//auto boxCollider = box->AddComponent<PurahEngine::BoxCollider>();
	//boxCollider->SetSize({ 0.5f, 0.5f, 0.5f });

	//auto rigid = box->AddComponent<PurahEngine::RigidBody>();
	//rigid->SetMass(10.f);
	//rigid->UseGravity(true);


	PurahEngine::GameObject* box2 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box02");

	auto box2Trans = box2->GetComponent<PurahEngine::Transform>();
	box2Trans->SetLocalPosition({0.f, -5.f, 0.f});

	auto box2Collider = box2->AddComponent<PurahEngine::BoxCollider>();
	box2Collider->SetSize({ 100, 0.5f, 100 });

	auto rigid2 = box2->AddComponent<PurahEngine::RigidBody>();
	rigid2->SetMass(10.f);
	rigid2->UseGravity(false);
	rigid2->SetKinematic(true);



	PurahEngine::GameObject* testObject = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject");
	testObject->AddComponent<PurahEngine::Renderer>();
	testObject->AddComponent<PurahEngine::TestMovement>();

	auto boxTrans = testObject->GetComponent<PurahEngine::Transform>();
	boxTrans->SetLocalPosition({ 0, 5, 0 });

	auto boxCollider = testObject->AddComponent<PurahEngine::BoxCollider>();
	boxCollider->SetSize({ 0.5f, 0.5f, 0.5f });

	auto rigid = testObject->AddComponent<PurahEngine::RigidBody>();
	rigid->SetMass(10.f);
	rigid->UseGravity(true);

	boxCollider->Awake();
	box2Collider->Awake();
	rigid->Awake();
	rigid2->Awake();




	PurahEngine::GameObject* camera = PurahEngine::SceneManager::GetInstance().GetMainCamera()->GetGameObject();
	camera->GetComponent<PurahEngine::Transform>()->SetLocalPosition(Eigen::Vector3f(0, 0, -10));
	camera->AddComponent<PurahEngine::CameraMovement>();

	const auto cameraComponent = camera->AddComponent<PurahEngine::Camera>();
	cameraComponent->SetRenderer(renderer);
	cameraComponent->CreateCamera();
	cameraComponent->SetMainCamera();

	textureID = renderer->CreateTexture(L"scd.jpg");
	if (textureID == TextureID::ID_NULL)
	{
		assert(0);
	}

	// testObject->GetComponent<PurahEngine::Renderer>()->AddTexture(textureID);
	auto cube = testObject->AddComponent<PurahEngine::Renderer>();
	auto cube2 = box2->AddComponent<PurahEngine::Renderer>();
	cube->AddTexture(textureID);
	cube2->AddTexture(textureID);

	//PurahEngine::Run(); ����
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); ����
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}
