#include "GraphicsManager.h"
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
	renderer->CreateResources();
}

void PurahEngine::GraphicsManager::Run()
{
	// �׷��Ƚ� ���� �׽�Ʈ
	static bool run = true;
	static ResourceID textureID = ID_NULL;
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
		if (textureID == ID_NULL)
		{
			assert(0);
		}
	}
	renderer->BeginDraw();
	float angle = 3.14f / 4.0f;  // 90���� �������� ǥ��
	Eigen::Matrix3f rotation_matrix;
	rotation_matrix = Eigen::AngleAxisf(angle, Eigen::Vector3f::UnitY());
	test.block<3, 3>(0, 0) = rotation_matrix;

	renderer->DrawCube(test, textureID, false);
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
