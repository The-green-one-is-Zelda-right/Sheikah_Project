#include "NaDong.h"
#include <cassert>

#include "PurahEngine.h"

//#include "TestMovement.h"
//#include "CameraMovement.h"
#include "Test.h"
#include "Test2.h"

using namespace PurahEngine;

void NaDong::Run()
{
	//// ī�޶� ����
	//GameObject* mainCameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
	//Camera* mainCamera = mainCameraObject->AddComponent<Camera>();

	//// ���� ī�޶�� ����
	//SceneManager::GetInstance().SetMainCamera(mainCamera);

	//// ����Ʈ ����
	//GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"Light");
	//DirectionalLight* light = lightObject->AddComponent<DirectionalLight>();
	//
	//// ����Ʈ�� ������ ���� ������ �鿩�� ���� �������� ����

	//// ť�� ����
	//GameObject* cubeObject = SceneManager::GetInstance().CreateGameObject(L"Cube");
	//MeshRenderer* cube = cubeObject->AddComponent<MeshRenderer>();

	//// MeshRenderer�� Ÿ���� Cube�� �����Ѵ�.
	//// �⺻���� None���� �� ��� �ƹ��͵� ȭ�鿡 ���� �ʴ´�.(Debug ����� ��� ��Ÿ�� ������ �߻���Ų��.)
	//cube->SetMesh(MeshRenderer::MeshType::Cube);

	GameObject* object1 = SceneManager::GetInstance().CreateGameObject(L"1");
	GameObject* object2 = SceneManager::GetInstance().CreateGameObject(L"2");
	GameObject* object3 = SceneManager::GetInstance().CreateGameObject(L"3");
	GameObject* object4 = SceneManager::GetInstance().CreateGameObject(L"4");

	GameObject* cObject1 = SceneManager::GetInstance().CreateGameObject(L"c1");
	GameObject* cObject2 = SceneManager::GetInstance().CreateGameObject(L"c2");
	GameObject* cObject3 = SceneManager::GetInstance().CreateGameObject(L"c3");
	GameObject* cObject4 = SceneManager::GetInstance().CreateGameObject(L"c4");
	
	GameObject* ccObject1 = SceneManager::GetInstance().CreateGameObject(L"cc1");
	GameObject* ccObject2 = SceneManager::GetInstance().CreateGameObject(L"cc2");
	GameObject* ccObject3 = SceneManager::GetInstance().CreateGameObject(L"cc3");
	GameObject* ccObject4 = SceneManager::GetInstance().CreateGameObject(L"cc4");

	cObject1->GetComponent<Transform>()->SetParent(object1->GetComponent<Transform>());
	cObject2->GetComponent<Transform>()->SetParent(object2->GetComponent<Transform>());
	cObject3->GetComponent<Transform>()->SetParent(object3->GetComponent<Transform>());
	cObject4->GetComponent<Transform>()->SetParent(object4->GetComponent<Transform>());
	
	ccObject1->GetComponent<Transform>()->SetParent(cObject1->GetComponent<Transform>());
	ccObject2->GetComponent<Transform>()->SetParent(cObject2->GetComponent<Transform>());
	ccObject3->GetComponent<Transform>()->SetParent(cObject3->GetComponent<Transform>());
	ccObject4->GetComponent<Transform>()->SetParent(cObject4->GetComponent<Transform>());


	object1->AddComponent<Test>();
	object1->AddComponent<Test2>();
	object2->AddComponent<Test>();
	object3->AddComponent<Test>();
	object4->AddComponent<Test>();

	cObject1->AddComponent<Test>();
	cObject1->AddComponent<Test2>();
	cObject2->AddComponent<Test>();
	cObject3->AddComponent<Test>();
	cObject4->AddComponent<Test>();

	ccObject1->AddComponent<Test>();
	ccObject2->AddComponent<Test>();
	ccObject3->AddComponent<Test>();
	ccObject4->AddComponent<Test>();

}
