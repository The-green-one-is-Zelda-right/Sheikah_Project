#include "NaDong.h"
#include <cassert>

#include "PurahEngine.h"

#include "TestMovement.h"
#include "CameraMovement.h"

using namespace PurahEngine;

void NaDong::Run()
{
	// ī�޶� ����
	GameObject* mainCameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
	Camera* mainCamera = mainCameraObject->AddComponent<Camera>();

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
