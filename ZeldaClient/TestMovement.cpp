#include "TestMovement.h"
#include "TimeController.h"
#include <cassert>
#define M_PI       3.14159265358979323846


PurahEngine::TestMovement::TestMovement()
{

}

PurahEngine::TestMovement::~TestMovement()
{

}

void PurahEngine::TestMovement::Update()
{
	//static int j = 0;
	//j++;
	//const float angle = 10.0f;

	//auto& inputManager = PurahEngine::InputManager::Getinstance();

	///// 게임패드 관련
	//
	//const auto rigid = GetGameObject()->GetComponent<RigidBody>();
	//const auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();
	//auto movement = Eigen::Vector3f{ 0.f, 0.f, 0.f };

	//if (inputManager.IsKeyPressed(eKey::eKEY_Q) == true)
	//{
	//	const auto axis = trans->up;
	//	trans->Rotate(axis, angle);
	//}
	//if (inputManager.IsKeyPressed(eKey::eKEY_E) == true)
	//{
	//	const auto axis = trans->up;
	//	trans->Rotate(axis, -angle);
	//	trans->Rotate(trans->front, -angle);
	//}

	//if (inputManager.IsKeyPressed(eKey::eKEY_W) == true)
	//{
	//	movement += Eigen::Vector3f(0.0f, 0.0f, 0.1f);
	//}
	//if (inputManager.IsKeyPressed(eKey::eKEY_S) == true)
	//{
	//	movement += Eigen::Vector3f(0.0f, 0.0f, -0.1f);
	//}
	//if (inputManager.IsKeyPressed(eKey::eKEY_A) == true)
	//{
	//	movement += Eigen::Vector3f(-0.1f, 0.0f, 0.0f);
	//}
	//if (inputManager.IsKeyPressed(eKey::eKEY_D) == true)
	//{
	//	movement += Eigen::Vector3f(0.1f, 0.0f, 0.0f);
	//}
	//if (inputManager.IsKeyPressed(eKey::eKEY_SPACE) == true)
	//{
	//	movement += Eigen::Vector3f(0.0f, 0.1f, 0.0f);
	//}

	//if (inputManager.IsKeyPressed(eKey::eKEY_N) == true)
	//{
	//	if (this->GetGameObject()->IsRootEnable() == true)
	//	{
	//		this->GetGameObject()->Disable();
	//	}
	//	else
	//	{
	//		this->GetGameObject()->Enable();
	//	}
	//}

	//if (inputManager.IsKeyPressed(eKey::eKEY_Z) == true)
	//{
	//	trans->SetLocalScale(trans->GetLocalScale().cwiseProduct(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	//}

	//if (inputManager.IsKeyPressed(eKey::eKEY_C) == true)
	//{
	//	trans->SetLocalScale(trans->GetLocalScale().cwiseQuotient(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	//}


	//static float rotationValue = 0.0f;

	//if (inputManager.IsKeyPressed(eKey::eKEY_B) == true)
	//{
	//	float roll = 0.0f;    // X 축 회전
	//	float pitch = rotationValue;   // Y 축 회전
	//	float yaw = 0.0f;     // Z 축 회전

	//	// 오일러 각도를 쿼터니언으로 변환
	//	Eigen::Quaternionf q;
	//	q = Eigen::AngleAxisf(yaw, Eigen::Vector3f::UnitZ())
	//		* Eigen::AngleAxisf(pitch, Eigen::Vector3f::UnitY())
	//		* Eigen::AngleAxisf(roll, Eigen::Vector3f::UnitX()) * Eigen::Quaternionf::Identity();
	//	float getDelta = PurahEngine::TimeController::GetInstance().GetDeltaTime("Init");
	//	rotationValue += PurahEngine::TimeController::GetInstance().GetDeltaTime("Init") * 10;
	//	GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalRotation(q);
	//}

	//const auto localPos = trans->GetLocalPosition();
	//const auto worldPos = trans->GetWorldPosition();

	//// Transform 에게 줘라
	////rigid->SetPosition(localPos + movement);

	//trans->SetLocalPosition(localPos + movement);

	///// Sound Test-----------------------------------------------------------
	//if (inputManager.IsKeyPressed(eKey::eKEY_8) == true)
	//{
	//	PurahEngine::SoundManager::GetInstance().Play(L"BGM_Test001");
	//}

	//if (inputManager.IsKeyPressed(eKey::eKEY_9) == true)
	//{
	//	PurahEngine::SoundManager::GetInstance().Play(L"BGM_Test002");
	//}

	//if (inputManager.IsKeyPressed(eKey::eKEY_0) == true)
	//{
	//	PurahEngine::SoundManager::GetInstance().Play(L"BGM_Test003");
	//}
}

void PurahEngine::TestMovement::FixedUpdate()
{

}

void PurahEngine::TestMovement::PreSerialize(json& jsonData) const
{

}

void PurahEngine::TestMovement::PreDeserialize(const json& jsonData)
{

}

void PurahEngine::TestMovement::PostSerialize(json& jsonData) const
{

}

void PurahEngine::TestMovement::PostDeserialize(const json& jsonData)
{

}
