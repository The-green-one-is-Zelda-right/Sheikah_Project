#include "BeomJoon.h"

#include "PurahEngine.h"

using namespace PurahEngine;

void BeomJoon::Run()
{
	// ī�޶� ����
	GameObject* mainCameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
	Camera* mainCamera = mainCameraObject->AddComponent<Camera>();

	mainCameraObject->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, -10.0f });

	// ���� ī�޶�� ����
	SceneManager::GetInstance().SetMainCamera(mainCamera);

	// ����Ʈ ����
	GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"Light");
	DirectionalLight* light = lightObject->AddComponent<DirectionalLight>();
	
	// ����Ʈ�� ������ ���� ������ �鿩�� ���� �������� ����

	// ť�� ����
	GameObject* cubeObject = SceneManager::GetInstance().CreateGameObject(L"Cube");
	MeshRenderer* cube = cubeObject->AddComponent<MeshRenderer>();

	// MeshRenderer�� Ÿ���� Cube�� �����Ѵ�.
	// �⺻���� None���� �� ��� �ƹ��͵� ȭ�鿡 ���� �ʴ´�.(Debug ����� ��� ��Ÿ�� ������ �߻���Ų��.)
	cube->SetMesh(MeshRenderer::MeshType::Cube);
}
