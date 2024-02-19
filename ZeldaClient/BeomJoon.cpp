#include "BeomJoon.h"

#include "PurahEngine.h"

#include "CameraMovement.h"

using namespace PurahEngine;

void BeomJoon::Run()
{
	// ī�޶� ����
	GameObject* mainCameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
	Camera* mainCamera = mainCameraObject->AddComponent<Camera>();
	mainCameraObject->AddComponent<CameraMovement>();

	mainCameraObject->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, -10.0f });

	// ���� ī�޶�� ����
	SceneManager::GetInstance().SetMainCamera(mainCamera);

	// ����Ʈ ����
	GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"Light");
	
	// ����Ʈ�� ������ ���� ������ �鿩�� ���� �������� ����
	lightObject->GetTransform()->Rotate({ 0.0f, 1.0f, 0.0f }, 45.0f);
	DirectionalLight* light = lightObject->AddComponent<DirectionalLight>();

	// ť�� ����
	GameObject* cubeObject = SceneManager::GetInstance().CreateGameObject(L"Cube");
	MeshRenderer* cube = cubeObject->AddComponent<MeshRenderer>();

	// MeshRenderer�� Ÿ���� Cube�� �����Ѵ�.
	// �⺻���� None���� �� ��� �ƹ��͵� ȭ�鿡 ���� �ʴ´�.(Debug ����� ��� ��Ÿ�� ������ �߻���Ų��.)
	cube->SetMesh(MeshRenderer::MeshType::Cube);
	cube->SetTexture(L"scd.jpg");


	// ĳ���� ����
	GameObject* characterObject = SceneManager::GetInstance().CreateGameObject(L"Character");
	ModelRenderer* character = characterObject->AddComponent<ModelRenderer>();
	character->SetModelName(L"Character\\Character.fbx");

	characterObject->GetTransform()->SetLocalScale({ 0.02f, 0.02f, 0.02f });
}
