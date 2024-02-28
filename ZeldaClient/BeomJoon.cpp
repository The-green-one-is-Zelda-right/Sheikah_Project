#include "BeomJoon.h"

#include "PurahEngine.h"

#include "CameraMovement.h"
#include "AnimationTestController.h"

using namespace PurahEngine;

void BeomJoon::Run()
{
	// 카메라 생성
	GameObject* mainCameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
	Camera* mainCamera = mainCameraObject->AddComponent<Camera>();
	mainCameraObject->AddComponent<CameraMovement>();

	mainCameraObject->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, -100.0f });

	// 메인 카메라로 설정
	SceneManager::GetInstance().SetMainCamera(mainCamera);

	// 라이트 생성
	GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"Light");
	
	// 라이트의 방향을 왼쪽 위에서 들여다 보는 방향으로 설정
	lightObject->GetTransform()->Rotate({ 0.0f, 1.0f, 0.0f }, 45.0f);
	DirectionalLight* light = lightObject->AddComponent<DirectionalLight>();

	// 큐브 생성
	GameObject* cubeObject = SceneManager::GetInstance().CreateGameObject(L"Cube");
	MeshRenderer* cube = cubeObject->AddComponent<MeshRenderer>();

	// MeshRenderer의 타입을 Cube로 설정한다.
	// 기본값은 None으로 이 경우 아무것도 화면에 뜨지 않는다.(Debug 모드의 경우 런타임 에러를 발생시킨다.)
	cube->SetMesh(MeshRenderer::MeshType::Cube);
	cube->SetTexture(L"scd.jpg");


	// 캐릭터 생성
	GameObject* characterObject = SceneManager::GetInstance().CreateGameObject(L"Character");
	ModelRenderer* character = characterObject->AddComponent<ModelRenderer>();
	character->SetModelName(L"Character\\Character.fbx");
	//character->SetModelName(L"C:\\Users\\KOCCA62\\Desktop\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying6.fbx");

	characterObject->GetTransform()->SetLocalScale({ 0.2f, 0.2f, 0.2f });

	// 애니메이션을 테스트하기 위한 컴포넌트 추가
	characterObject->AddComponent<Animator>();
	characterObject->AddComponent<AnimationTestController>();

}
