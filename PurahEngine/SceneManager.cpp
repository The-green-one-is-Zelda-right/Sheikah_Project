#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

PurahEngine::SceneManager::SceneManager()
	: mainCamera(nullptr), cameraPosition(Eigen::Vector3f(0.0f, 0.0f, -10.0f))
{

}

PurahEngine::SceneManager::~SceneManager()
{

}

PurahEngine::GameObject* PurahEngine::SceneManager::CreateGameObject(std::wstring objectName)
{
	GameObject* object = new GameObject(objectName);
	objectList.push_back(object);
	object->AddComponent<Transform>();
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

void PurahEngine::SceneManager::Initialize()
{
	// 씬을 초기화할때 카메라를 씬에 생성해둔다.
	if (mainCamera == nullptr)
	{
		//GameObject* object = CreateGameObject(L"MainCamera");
		//Camera* tmp = object->AddComponent<Camera>();
		//Transform* cameraTransform = object->GetComponent<Transform>();
		//cameraTransform->SetLocalPosition(cameraPosition);
		//tmp->UpdateCamera(cameraTransform->GetLocalMatrix(), 3.141592654f / 4.0f, 1.0f, 1000.0f);
		//mainCamera = tmp;
	}
}

PurahEngine::SceneManager& PurahEngine::SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}