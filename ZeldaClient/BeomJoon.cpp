#include "BeomJoon.h"

#include "PurahEngine.h"

#include "CameraMovement.h"
#include "AnimationTestController.h"

using namespace PurahEngine;

void BeomJoon::Run()
{
	// ī�޶� ����
	GameObject* mainCameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
	Camera* mainCamera = mainCameraObject->AddComponent<Camera>();
	mainCameraObject->AddComponent<CameraMovement>();

	mainCameraObject->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, -100.0f });

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
	//character->SetModelName(L"C:\\Users\\KOCCA62\\Desktop\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying6.fbx");

	characterObject->GetTransform()->SetLocalScale({ 0.2f, 0.2f, 0.2f });

	// �ִϸ��̼��� �׽�Ʈ�ϱ� ���� ������Ʈ �߰�
	characterObject->AddComponent<Animator>();
	characterObject->AddComponent<AnimationTestController>();

}
