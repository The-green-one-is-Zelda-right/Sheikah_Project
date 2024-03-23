#include "NaDong.h"
#include <cassert>

#include "PurahEngine.h"

//#include "TestMovement.h"
#include "CameraMovement.h"
#include "AudioListener.h"
#include "AudioSource.h"


using namespace PurahEngine;

void NaDong::Run()
{
	Eigen::Vector3f pos = { 0.0f, 0.0f, -10.0f };
	//// 카메라 생성
	GameObject* mainCameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
	Camera* mainCamera = mainCameraObject->AddComponent<Camera>();
	mainCameraObject->GetTransform()->SetLocalPosition(pos);
	mainCameraObject->AddComponent<AudioListener>();

	//// 메인 카메라로 설정
	SceneManager::GetInstance().SetMainCamera(mainCamera);

	//// 라이트 생성
	GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"Light");
	DirectionalLight* light = lightObject->AddComponent<DirectionalLight>();
	
	//// 라이트의 방향을 왼쪽 위에서 들여다 보는 방향으로 설정

	//// 큐브 생성
	GameObject* cubeObject = SceneManager::GetInstance().CreateGameObject(L"Cube");
	MeshRenderer* cube = cubeObject->AddComponent<MeshRenderer>();

	cubeObject->AddComponent<CameraMovement>();

	cubeObject->AddComponent<AudioSource>();

	//// MeshRenderer의 타입을 Cube로 설정한다.
	//// 기본값은 None으로 이 경우 아무것도 화면에 뜨지 않는다.(Debug 모드의 경우 런타임 에러를 발생시킨다.)
	cube->SetMesh(MeshRenderer::MeshType::Cube);

	//GameObject* object1 = SceneManager::GetInstance().CreateGameObject(L"1");
	//GameObject* object2 = SceneManager::GetInstance().CreateGameObject(L"2");
	//GameObject* object3 = SceneManager::GetInstance().CreateGameObject(L"3");
	//GameObject* object4 = SceneManager::GetInstance().CreateGameObject(L"4");
	//
	//
	//GameObject* cObject1 = SceneManager::GetInstance().CreateGameObject(L"c1");
	//GameObject* cObject2 = SceneManager::GetInstance().CreateGameObject(L"c2");
	//GameObject* cObject3 = SceneManager::GetInstance().CreateGameObject(L"c3");
	//GameObject* cObject4 = SceneManager::GetInstance().CreateGameObject(L"c4");
	//
	//GameObject* ccObject1 = SceneManager::GetInstance().CreateGameObject(L"cc1");
	//GameObject* ccObject2 = SceneManager::GetInstance().CreateGameObject(L"cc2");
	//GameObject* ccObject3 = SceneManager::GetInstance().CreateGameObject(L"cc3");
	//GameObject* ccObject4 = SceneManager::GetInstance().CreateGameObject(L"cc4");
	//
	//cObject1->GetComponent<Transform>()->SetParent(object1->GetComponent<Transform>());
	//cObject2->GetComponent<Transform>()->SetParent(object2->GetComponent<Transform>());
	//cObject3->GetComponent<Transform>()->SetParent(object3->GetComponent<Transform>());
	//cObject4->GetComponent<Transform>()->SetParent(object4->GetComponent<Transform>());
	//
	//ccObject1->GetComponent<Transform>()->SetParent(cObject1->GetComponent<Transform>());
	//ccObject2->GetComponent<Transform>()->SetParent(cObject2->GetComponent<Transform>());
	//ccObject3->GetComponent<Transform>()->SetParent(cObject3->GetComponent<Transform>());
	//ccObject4->GetComponent<Transform>()->SetParent(cObject4->GetComponent<Transform>());


	//object1->AddComponent<Test>();
	//object1->AddComponent<Test2>();
	//object1->AddComponent<Test3>();
	//object2->AddComponent<Test>();
	//object3->AddComponent<Test>();
	//object4->AddComponent<Test>();

	//cObject1->AddComponent<Test>();
	//cObject1->AddComponent<Test2>();
	//cObject2->AddComponent<Test>();
	//cObject3->AddComponent<Test>();
	//cObject4->AddComponent<Test>();

	//ccObject1->AddComponent<Test>();
	//ccObject2->AddComponent<Test>();
	//ccObject3->AddComponent<Test>();
	//ccObject4->AddComponent<Test>();

	//GameObject* testObject = SceneManager::GetInstance().CreateGameObject(L"test");
	//testObject->AddComponent<Test3>();

}
