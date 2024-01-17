#include "TestMovement.h"

PurahEngine::TestMovement::TestMovement()
{

}

PurahEngine::TestMovement::~TestMovement()
{

}

void PurahEngine::TestMovement::Update()
{
	const float angle = 10.0f;

	auto& inputManager = PurahEngine::InputManager::Getinstance();

	const auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();
	auto movement = Eigen::Vector3f{ 0.f, 0.f, 0.f };

	if (inputManager.IsKeyPressed('Q') == true)
	{
		const auto axis = GetGameObject()->GetComponent<PurahEngine::Transform>()->up;
		trans->Rotate(axis, angle);
	}
	if (inputManager.IsKeyPressed('E') == true)
	{
		const auto axis = Eigen::Vector3f::UnitY();
		trans->Rotate(axis, -angle);
	}

	if (inputManager.IsKeyPressed('W') == true)
	{
		movement += Eigen::Vector3f(0.0f, 0.1f, 0.0f);
	}
	if (inputManager.IsKeyPressed('S') == true)
	{
		movement += Eigen::Vector3f(0.0f, -0.1f, 0.0f);
	}
	if (inputManager.IsKeyPressed('A') == true)
	{
		movement += Eigen::Vector3f(-0.1f, 0.0f, 0.0f);
	}
	if (inputManager.IsKeyPressed('D') == true)
	{
		movement += Eigen::Vector3f(0.1f, 0.0f, 0.0f);
	}

	if (inputManager.IsKeyPressed('Z') == true)
	{
		trans->SetLocalScale(trans->GetLocalScale().cwiseProduct(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	if (inputManager.IsKeyPressed('C') == true)
	{
		trans->SetLocalScale(trans->GetLocalScale().cwiseQuotient(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	const auto localPos = trans->GetLocalPosition();
	const auto rigid = GetGameObject()->GetComponent<RigidBody>();
	rigid->SetPosition(localPos + movement);
	// trans->SetLocalPosition(localPos + movement);
}
