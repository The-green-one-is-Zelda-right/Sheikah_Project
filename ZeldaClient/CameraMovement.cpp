#include "CameraMovement.h"
#include <iostream>

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

	if (inputManager.IsKeyPressed(eKey::eKEY_UP))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_DOWN))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, -0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_LEFT))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(-0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_RIGHT))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_O))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.0f, -0.1f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_P))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.0f, 0.1f));
	}

	std::wcout << GetGameObject()->GetName() << std::endl;
	std::cout << trans->GetLocalPosition() << std::endl << std::endl;
}

void PurahEngine::CameraMovement::PreSerialize(json& jsonData) const
{

}

void PurahEngine::CameraMovement::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::CameraMovement::PostSerialize(json& jsonData) const
{

}

void PurahEngine::CameraMovement::PostDeserialize(const json& jsonData)
{

}
