#include "TestMovement.h"
#include "TimeController.h"
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

		auto prevPos = GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition();
		auto movement = Eigen::Vector3f(0.0f, 0.1f, 0.0f);
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(prevPos + movement);

		auto nowPos = GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition();

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


	static float rotationValue = 0.0f;

	auto rigid5 = GetGameObject()->GetComponent<PurahEngine::RigidBody>();
	auto worldpos = GetGameObject()->GetComponent<PurahEngine::Transform>()->GetWorldPosition();
	rigid5->SetPosition(worldpos);


	if (inputManager.IsKeyPressed('B') == true)
	{
		float roll = 0.0f;    // X �� ȸ��
		float pitch = rotationValue;   // Y �� ȸ��
		float yaw = 0.0f;     // Z �� ȸ��

		// ���Ϸ� ������ ���ʹϾ����� ��ȯ
		Eigen::Quaternionf q;
		q = Eigen::AngleAxisf(yaw, Eigen::Vector3f::UnitZ())
			* Eigen::AngleAxisf(pitch, Eigen::Vector3f::UnitY())
			* Eigen::AngleAxisf(roll, Eigen::Vector3f::UnitX()) * Eigen::Quaternionf::Identity();
		float getDelta = PurahEngine::TimeController::GetInstance().GetDeltaTime("Init");
		rotationValue += PurahEngine::TimeController::GetInstance().GetDeltaTime("Init") * 90;
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetWorldRotation(q);
	}

	const auto localPos = trans->GetLocalPosition();
	const auto rigid = GetGameObject()->GetComponent<RigidBody>();
	rigid->SetPosition(localPos + movement);
	// trans->SetLocalPosition(localPos + movement);

}
