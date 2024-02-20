#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "PhysicsSystem.h"
#include "TimeController.h"

PurahEngine::SceneManager::SceneManager()
	: mainCamera(nullptr), cameraPosition(Eigen::Vector3f(0.0f, 0.0f, -10.0f)), state(RunningState::AWAKE), physicsTime(0.0f)
{

}

PurahEngine::SceneManager::~SceneManager()
{

}

PurahEngine::GameObject* PurahEngine::SceneManager::CreateGameObject(std::wstring objectName)
{
	GameObject* object = new GameObject(objectName);
	objectList.push_back(object);
	object->trans = object->AddComponent<Transform>();
	return object;
}

PurahEngine::Camera* PurahEngine::SceneManager::GetMainCamera()
{
	return mainCamera;
}

void PurahEngine::SceneManager::SetMainCamera(Camera* camera)
{
	mainCamera = camera;
}

void PurahEngine::SceneManager::SetName(std::wstring name)
{
	sceneName = name;
}

void PurahEngine::SceneManager::Update()
{
	PurahEngine::TimeController::GetInstance().Update("Simulate");
	float SdeltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime("Simulate");
	static float SimuleTime = 0;
	SimuleTime += SdeltaTime;

	PhysicsSystem::GetInstance().SimulateResult();

	PurahEngine::TimeController::GetInstance().Update("physics");
	float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime("physics");
	physicsTime += deltaTime;

	if (state == RunningState::AWAKE)
	{
		for (PurahEngine::GameObject* object : objectList)
		{
			object->AwakeEvent();
		}
		state = RunningState::START;
	}
	else if (state == RunningState::START)
	{
		for (PurahEngine::GameObject* object : objectList)
		{
			object->StartEvent();
		}
		state = RunningState::UPDATE;
	}
	else if (state == RunningState::UPDATE)
	{
		for (PurahEngine::GameObject* object : objectList)
		{
			object->UpdateEvent();
			object->LateUpdateEvent();
		}
	}

	if (physicsTime >= 0.02f)
	{
		for (PurahEngine::GameObject* object : objectList)
		{
			object->FixedUpdateEvent();

			object->OnCollisionEnter();
			object->OnCollisionStay();
			object->OnCollisionExit();

			object->OnTriggerEnter();
			object->OnTriggerStay();
			object->OnTriggerExit();
		}
	}


	/*for (PurahEngine::GameObject* object : objectList)
	{
		if (state == RunningState::START)
		{
			object->StartEvent();
		}
		if (physicsTime >= 0.02f)
		{
			object->FixedUpdateEvent();

			object->OnCollisionEnter();
			object->OnCollisionStay();
			object->OnCollisionExit();

			object->OnTriggerEnter();
			object->OnTriggerStay();
			object->OnTriggerExit();
		}
		if (object->isRun == true)
		{
			object->UpdateEvent();
			object->LateUpdateEvent();
		}
		object->isRun = true;
	}*/
	if (physicsTime >= 0.02f)
	{
		physicsTime -= 0.02f;
	}

}

void PurahEngine::SceneManager::Initialize()
{
	// 씬을 초기화할때 카메라를 씬에 생성해둔다.
	if (mainCamera == nullptr)
	{
		GameObject* object = CreateGameObject(L"MainCamera");
		Camera* tmp = object->AddComponent<Camera>();
		mainCamera = tmp;
		physicsTime = 0.0f;
	}
}

PurahEngine::SceneManager& PurahEngine::SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}