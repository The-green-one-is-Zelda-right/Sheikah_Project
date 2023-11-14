#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

PurahEngine::SceneManager::SceneManager()
	: mainCamera(nullptr)
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

void PurahEngine::SceneManager::Initialize()
{
	// ���� �ʱ�ȭ�Ҷ� ī�޶� ���� �����صд�.
	if (mainCamera == nullptr)
	{
		GameObject* object = CreateGameObject(L"MainCamera");
		Camera* tmp = object->AddComponent<Camera>();
		mainCamera = tmp;
	}
}

PurahEngine::SceneManager& PurahEngine::SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}