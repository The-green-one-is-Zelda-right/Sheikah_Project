#include "GraphicsManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"
#include "InputManager.h"
#include <Eigen/Dense>

#include <cassert>

void PurahEngine::GraphicsManager::Initialize(HWND hWnd)
{
	zeldaGraphicsDLL = LoadLibrary(L"ZeldaGraphics.dll");
	if (zeldaGraphicsDLL == nullptr)
	{
		// DLL �ε� ����
		assert(0);
	}
	auto createZeldaRenderer = reinterpret_cast<IZeldaRenderer * (*)()>(GetProcAddress(zeldaGraphicsDLL, "CreateZeldaRenderer"));
	if (createZeldaRenderer == nullptr)
	{
		// DLL �Լ��� ã�� �� �����ϴ�.
		assert(0);
	}
	renderer = createZeldaRenderer();
	renderer->Initialize(1920, 1080, true, hWnd, false, 1000.0f, 1.0f);
	renderer->CreateBasicResources();
}

void PurahEngine::GraphicsManager::Run()
{
	// �׷��Ƚ� ���� �׽�Ʈ
	static PurahEngine::GameObject* testObject = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject");

	static bool run = true;
	static TextureID textureID = TextureID::ID_NULL;
	static ModelID modelID = ModelID::ID_NULL;
	static CameraID cameraID = CameraID::ID_NULL;
	Eigen::Matrix4f test = Eigen::Matrix4f::Identity();
	Eigen::Matrix4f camera;
	camera.normalized();
	camera <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, -10,
		0, 0, 0, 1;

	if (run == true)
	{
		run = false;
		auto resourceID = renderer->CreateCamera();
		renderer->SetMainCamera(resourceID);
		renderer->UpdateCamera(resourceID, camera, 3.141592654f / 4.0f, 1.0f, 1000.0f);
		textureID = renderer->CreateTexture(L"scd.jpg");
		if (textureID == TextureID::ID_NULL)
		{
			assert(0);
		}
	}
	renderer->BeginDraw(0.0f);
	float angle = 10.0f;
	Eigen::Matrix3f rotation_matrix;
	rotation_matrix = Eigen::AngleAxisf(angle, Eigen::Vector3f::UnitY());
	test.block<3, 3>(0, 0) = rotation_matrix;

	int a;
	int b;
	int c = a * (a < b) + b * (a <= b);

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('Q') == true)
	{
		//testObject->GetComponent<Transform>()->Rotate(Eigen::Vector3f::UnitY(), angle);
		testObject->GetComponent<Transform>()->Rotate(testObject->GetComponent<Transform>()->up, angle);
	}

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('E') == true)
	{
		testObject->GetComponent<Transform>()->Rotate(Eigen::Vector3f::UnitY(), -angle);
	}

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('W') == true)
	{
		testObject->GetComponent<Transform>()->SetLocalPosition(testObject->GetComponent<Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.1f, 0.0f));
	}
	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('S') == true)
	{
		testObject->GetComponent<Transform>()->SetLocalPosition(testObject->GetComponent<Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, -0.1f, 0.0f));
	}
	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('A') == true)
	{
		testObject->GetComponent<Transform>()->SetLocalPosition(testObject->GetComponent<Transform>()->GetLocalPosition() + Eigen::Vector3f(-0.1f, 0.0f, 0.0f));
	}
	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('D') == true)
	{
		testObject->GetComponent<Transform>()->SetLocalPosition(testObject->GetComponent<Transform>()->GetLocalPosition() + Eigen::Vector3f(0.1f, 0.0f, 0.0f));
	}

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('Z') == true)
	{
		testObject->GetComponent<Transform>()->SetLocalScale(testObject->GetComponent<Transform>()->GetLocalScale().cwiseProduct(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('C') == true)
	{
		testObject->GetComponent<Transform>()->SetLocalScale(testObject->GetComponent<Transform>()->GetLocalScale().cwiseQuotient(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	renderer->DrawCube(testObject->GetComponent<Transform>()->GetLocalMatrix(), textureID, false, 0, 1, 0, 1);
	renderer->EndDraw();
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
