#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "PhysicsSystem.h"
#include "TimeController.h"
#include "EngineSetting.h"
#include "DataManager.h"
#include "GameLoop.h"

PurahEngine::SceneManager::SceneManager()
	: mainCamera(nullptr), cameraPosition(Eigen::Vector3f(0.0f, 0.0f, -10.0f)), state(RunningState::AWAKE), physicsTime(0.0f), sceneBuffer(L"")
{

}

PurahEngine::SceneManager::~SceneManager()
{

}

PurahEngine::GameObject* PurahEngine::SceneManager::CreateGameObject(std::wstring objectName)
{
	GameObject* object = new GameObject(objectName);
	objectList.push_back(object);
	object->trans = object->AddComponentInit<Transform>();
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

std::wstring PurahEngine::SceneManager::GetName()
{
	return sceneName;
}

void PurahEngine::SceneManager::Update()
{
	PurahEngine::TimeController::GetInstance().Update("Simulate");
	float SdeltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime("Simulate");
	static float SimuleTime = 0;
	SimuleTime += SdeltaTime;

	PurahEngine::TimeController::GetInstance().Update("physics");
	float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime("physics");
	physicsTime += deltaTime;

	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->UpdateEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->LateUpdateEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	if (physicsTime >= 0.02f)
	{
		for (PurahEngine::GameObject* object : objectList)
		{
			if (object->trans->GetParent() == nullptr)
			{
				object->FixedUpdateEvent(eventQueue);
			}
		}
	}

	if (physicsTime >= 0.02f)
	{
		physicsTime -= 0.02f;
	}
}

void PurahEngine::SceneManager::LoadScene(const std::wstring fileName)
{
	sceneBuffer = fileName;

 	if (fileName == L"this")
	{
		sceneBuffer = sceneName;
	}
}

void PurahEngine::SceneManager::LoadScene(int sceneNumber)
{
	sceneBuffer = EngineSetting::GetInstance().GetScene(sceneNumber);
}

void PurahEngine::SceneManager::DeleteGameObject(GameObject* gameObject)
{
	auto objectIter = std::find(objectList.begin(), objectList.end(), gameObject);

	if (objectIter != objectList.end())
	{
		delete* objectIter;
		objectList.erase(objectIter);
		return;
	}

	for (int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->DeleteChild(gameObject);
	}

}

void PurahEngine::SceneManager::InitializationEvent()
{
	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->PostInitializeEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// Awake
	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->AwakeEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// OnEnable(활성화 직 후)
	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->EnableEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// Start
	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->StartEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// State Change
	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->StateChangeEvent();
		}
	}
}

void PurahEngine::SceneManager::DecommissionEvent()
{
	// OnDisable (비활성화 상태)
	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->DisableEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// OnDestroy (맨 마지막프레임에 오브젝트 파괴)
	for (PurahEngine::GameObject* object : objectList)
	{
		if (object->trans->GetParent() == nullptr)
		{
			object->DestroyEvent(destroyQueue);
		}
	}
	ExcuteDestroyQueue();
}

void PurahEngine::SceneManager::ExcuteEventQueue()
{
	while (!eventQueue.empty())
	{
		Component* component = eventQueue.front().first;
		auto f = eventQueue.front().second;
		eventQueue.pop();

		f(*component);
	}
}

void PurahEngine::SceneManager::ExcuteDestroyQueue()
{
	while (!destroyQueue.empty())
	{
		GameObject* object = destroyQueue.front();
		destroyQueue.pop();

		DeleteGameObject(object);
	}
}

void PurahEngine::SceneManager::LoadScene()
{
	if (sceneBuffer == L"")
	{
		return;
	}

	if (sceneBuffer == L"EXIT")
	{
		::PostQuitMessage(0);
		return;
	}

	auto& fManager = PurahEngine::FileManager::GetInstance();
	fManager.clear();
	DataManager::GetInstance().ClearDontDestroy();

	for (int i = 0; i < objectList.size(); i++)
	{
		bool checkDontDestroy = objectList[i]->GetDontDestroy();
		if (!checkDontDestroy)
		{
			delete objectList[i];
		}
		else
		{
			DataManager::GetInstance().DontDestroyOnLoad(objectList[i]);
		}
	}
	objectList.clear();
	sceneData = fManager.LoadData(sceneBuffer);

	ErrorType originError = GameLoop::errorType;
	GameLoop::errorType = ErrorType::Failed_Deserialize;
	Deserialize(sceneData);
	GameLoop::errorType = originError;

	LoadSceneCompleteEvent();

	LoadDontDestroyObject();

	// 필요하다면 여기서 sceneName 변경하는 코드 추가
	sceneName = sceneBuffer;
	sceneBuffer = L"";

}

void PurahEngine::SceneManager::LoadSceneCompleteEvent()
{
	for (int i = 0; i < objectList.size(); i++)
	{
		for (int j = 0; j < objectList[i]->componentList.size(); j++)
		{
			objectList[i]->componentList[j]->OnDataLoadComplete();
		}
	}
}

void PurahEngine::SceneManager::LoadDontDestroyObject()
{
	std::vector<GameObject*> dontDestroyList = DataManager::GetInstance().dontDestroyObjectList;
	int size = dontDestroyList.size();
	if (size != 0)
	{
		for (int i = 0; i < size; i++)
		{
			objectList.push_back(dontDestroyList[i]);
		}
	}
}

void PurahEngine::SceneManager::PreSerialize(json& jsonData) const
{

}

void PurahEngine::SceneManager::PreDeserialize(const json& jsonData)
{
	for (int i = 0; i < jsonData["gameObjects"].size(); i++)
	{
		std::string name = jsonData["gameObjects"][i]["name"];
		GameObject* object = CreateGameObject(std::wstring(name.begin(), name.end()));
		object->PreDeserialize(jsonData["gameObjects"][i]);
	}
}

void PurahEngine::SceneManager::PostSerialize(json& jsonData) const
{

}

void PurahEngine::SceneManager::PostDeserialize(const json& jsonData)
{
	for (int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->PostDeserialize(jsonData["gameObjects"][i]);
	}
}

void PurahEngine::SceneManager::Initialize()
{
	// 씬을 초기화할때 카메라를 씬에 생성해둔다.
	if (mainCamera == nullptr)
	{
		GameObject* object = CreateGameObject(L"MainCamera");
		Camera* tmp = object->AddComponentInit<Camera>();
		mainCamera = tmp;
		physicsTime = 0.0f;
	}
}

PurahEngine::SceneManager& PurahEngine::SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}