#include "KiHoon.h"
#include <cassert>

#include "PurahEngine.h"

#include "TestMovement.h"
#include "CameraMovement.h"

void KiHoon::Run()
{
	using namespace PurahEngine;

	static TextureID textureID = TextureID::ID_NULL;
	static ModelID modelID = ModelID::ID_NULL;
	static CameraID cameraID = CameraID::ID_NULL;
	static LightID lightID = LightID::ID_NULL;

	GameObject* light = SceneManager::GetInstance().CreateGameObject(L"Light");
	{
		DirectionalLight* directional = light->AddComponent<DirectionalLight>();
	}

	GameObject* camera = SceneManager::GetInstance().CreateGameObject(L"Camera");
	{
		Camera* cameraObject = camera->AddComponent<Camera>();
		SceneManager::GetInstance().SetMainCamera(cameraObject);
		camera->GetComponent<Transform>()->SetLocalPosition(Eigen::Vector3f(0, 0, -10));
		camera->AddComponent<CameraMovement>();
	}

	GameObject* testObject = SceneManager::GetInstance().CreateGameObject(L"testObject");
	{
		MeshRenderer* renderer = testObject->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);

		Transform* trnas = testObject->GetTransform();
		trnas->SetLocalPosition({ 0.0f, 3.0f, 0.0f });

		BoxCollider* collider = testObject->AddComponent<BoxCollider>();
		collider->SetSize({ 0.5f, 0.5f, 0.5f });

		RigidBody* rigid = testObject->AddComponent<RigidBody>();
		rigid->SetMass(10.0f);
		rigid->UseGravity(true);
	}

	GameObject* box2 = SceneManager::GetInstance().CreateGameObject(L"Box02");
	{
		MeshRenderer* renderer = box2->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);

		Transform* trnans = box2->GetTransform();
		trnans->SetLocalPosition({ 0.f, -5.f, 0.f });
		trnans->SetLocalScale({ 50.0f, 1.0f, 50.0f });

		BoxCollider* collider = box2->AddComponent<BoxCollider>();
		collider->SetSize({ 50.0f, 0.5f, 50.0f });

		RigidBody* rigid = box2->AddComponent<RigidBody>();
		rigid->SetMass(10.f);
		rigid->UseGravity(false);
		rigid->SetKinematic(true);
	}

	GameObject* box3 = SceneManager::GetInstance().CreateGameObject(L"Box03");
	{
		Transform* testTransform = testObject->GetTransform();

		MeshRenderer* renderer = box3->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);

		Transform* box3Trans = box3->GetComponent<Transform>();
		box3Trans->SetParent(testTransform);
		box3Trans->SetLocalPosition({ -2.0f, 0.0f, 0.0f });
		box3Trans->SetLocalScale({ 1.0f, 1.0f, 1.0f });

		BoxCollider* box3Collider = box3->AddComponent<BoxCollider>();
		box3Collider->SetDynamic(true);
		box3Collider->SetSize({ 0.5f, 0.5f, 0.5f });
	}
}
