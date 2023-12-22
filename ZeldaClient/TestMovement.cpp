#include "TestMovement.h"

PurahEngine::TestMovement::TestMovement()
{

}

PurahEngine::TestMovement::~TestMovement()
{

}

void PurahEngine::TestMovement::Update()
{
	float angle = 10.0f;

	auto& inputManager = PurahEngine::InputManager::Getinstance();

	if (inputManager.IsKeyPressed('Q') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->Rotate(GetGameObject()->GetComponent<PurahEngine::Transform>()->up, angle);
	}

	if (inputManager.IsKeyPressed('E') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->Rotate(Eigen::Vector3f::UnitY(), -angle);
	}

	if (inputManager.IsKeyPressed('W') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed('S') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, -0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed('A') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(-0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed('D') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.1f, 0.0f, 0.0f));
	}

	if (inputManager.IsKeyPressed('Z') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalScale(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalScale().cwiseProduct(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	if (inputManager.IsKeyPressed('C') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalScale(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalScale().cwiseQuotient(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}
}
