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
	float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

	auto& inputManager = PurahEngine::InputManager::Getinstance();
	auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();
	
	const float rotationSpeed = 30.0f;

	// rotation
	if (inputManager.IsKeyPressed(eKey::eKEY_UP))
	{
		trans->Rotate(trans->GetWorldRotation() * Eigen::Vector3f::UnitX(), -rotationSpeed * deltaTime);
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_DOWN))
	{
		trans->Rotate(trans->GetWorldRotation() * Eigen::Vector3f::UnitX(), rotationSpeed * deltaTime);
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_LEFT))
	{
		trans->Rotate(Eigen::Vector3f::UnitY(), -rotationSpeed * deltaTime);
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_RIGHT))
	{
		trans->Rotate(Eigen::Vector3f::UnitY(), rotationSpeed * deltaTime);
	}

	
	const float moveSpeed = 6.0f;
	float moveScale = 1.0f;

	if (inputManager.IsKeyPressed(eKey::eKEY_CONTROL))
	{
		moveScale = 2.0f;
	}

	// position
	if (inputManager.IsKeyPressed(eKey::eKEY_W))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetFront() * moveSpeed * deltaTime * moveScale));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_A))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetRight() * moveSpeed * deltaTime * moveScale));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_S))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetFront() * moveSpeed * deltaTime * moveScale));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_D))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetRight() * moveSpeed * deltaTime * moveScale));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_Q))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetUp() * moveSpeed * deltaTime * moveScale));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_E))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetUp() * moveSpeed * deltaTime * moveScale));
	}
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
