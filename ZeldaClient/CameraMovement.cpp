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
	auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();

	if (inputManager.IsKeyPressed(VK_UP) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed(VK_DOWN) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, -0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed(VK_LEFT) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(-0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed(VK_RIGHT) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed('O') == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.0f, -0.1f));
	}	
	if (inputManager.IsKeyPressed('P') == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.0f, 0.1f));
	}
	//if(inputManager.IsKeyPressed(VK_WHEEL))
}
