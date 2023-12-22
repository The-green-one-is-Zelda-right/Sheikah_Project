#include "CameraMovement.h"

PurahEngine::CameraMovement::CameraMovement()
{

}

PurahEngine::CameraMovement::~CameraMovement()
{

}

void PurahEngine::CameraMovement::Update()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();

	if (inputManager.IsKeyPressed(VK_UP) == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed(VK_DOWN) == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, -0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed(VK_LEFT) == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(-0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed(VK_RIGHT) == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.1f, 0.0f, 0.0f));
	}
}
