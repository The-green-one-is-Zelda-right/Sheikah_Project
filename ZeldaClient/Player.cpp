#include "EasingFunc.h"
#include "TimeController.h"
#include <algorithm>
#include <iostream>

#include "PhysicsSystem.h"

#include "Player.h"

namespace Phyzzle
{
	Player::~Player()
		= default;

	void Player::Awake()
	{
	}

	void Player::Start()
	{
		PurahEngine::GamePadManager::AddGamePad(0);

		gamePad = PurahEngine::GamePadManager::GetGamePad(0);
		gamePad->SetDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		startPosition = cameraCore->GetLocalPosition();
		startRotation = cameraCore->GetLocalRotation();

		highPosition = Eigen::Vector3f(0.f, 0.f, -20.f);
		lowPosition = Eigen::Vector3f(0.f, 0.f, -2.f);
		differenceHigh = highPosition - startPosition;
		differenceLow = lowPosition - startPosition;
	}

	void Player::Update()
	{
		GamePadInput();
		RotateCamera();

		Move();

		if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_A))
		{
			Jump();
		}
	}

	void Player::OnCollisionEnter(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		if (collider->GetGameObject()->GetName() == L"Ground")
		{
			jumping = false;
		}
	}

	void Player::GamePadInput()
	{
		LstickX = 0.f, LstickY = 0.f;
		RstickX = 0.f, RstickY = 0.f;
		LTrigger = 0.f, RTrigger = 0.f;

		if (gamePad->IsConnected())
		{
			LstickSize = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, LstickX, LstickY);
			RstickSize = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, RstickX, RstickY);

			LTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
			RTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);
		}
	}

	void Player::Move()
	{
		// ī�޶��� ���� ���͸� ���
		Eigen::Vector3f cameraFront = cameraArm->GetFront();
		const Eigen::Vector3f forward = Eigen::Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Eigen::Vector3f right = cameraArm->GetRight();

		Eigen::Vector3f movementDirection = forward * LstickY + right * LstickX;

		movementDirection.y() = 0.f;

		// �ӵ� ���͸� ���
		movement = movementDirection * moveSpeed * LstickSize;

		Eigen::Vector3f velocity = playerRigidbody->GetLinearVelocity();

		// y�� ��� �������� �ʴ� �ɷ� �߷��� ����ǵ�����.
		/// ��, �̷� ����̸� Ű �Է��� ���ٸ� �������� �ε��ĵ� ������ ���� ��
		velocity.x() = movement.x();
		velocity.z() = movement.z();

		// �ӷ��� �����Ŵ
		playerRigidbody->SetLinearVelocity(velocity);

		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();

		// Eigen::Vector3f localForward = modelCore->GetLocalRotation() * Eigen::Vector3f::UnitZ();

		static bool lastbool = false;
		static bool currentbool = false;

		lastbool = currentbool;
		currentbool = movementDirection.isZero();

		if (lastbool != currentbool)
		{
			if (currentbool)
			{
				animator->Play(L"Armature|Armature|Armature|idle");
			}
			else
			{
				animator->Play(L"Armature|Armature|Armature|running");
			}
			return;
		}

		if (currentbool)
		{
			return;
		}

		animator->SetPlaySpeed(L"Armature|Armature|Armature|running", LstickSize);


		Eigen::Vector3f localForward = parentWorld.conjugate() * movementDirection.normalized();

		// Calculate the rotation quaternion to align the current forward direction with the desired forward direction
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), localForward);

		// Set the rotation of the transform to the target rotation
		modelCore->SetLocalRotation(targetRotation);
	}

	void Player::RotateCamera()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();

		// ��ƽ ���⿡ ���� ȸ�� ������ ���
		const float yawAngle = RstickX * sensitivity * deltaTime * RstickSize;
		{
			// ���� up �������� ī�޶� ȸ��
			cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
		}

		// ��ƽ ���⿡ ���� ȸ�� ������ ���
		const float pitchAngle = -RstickY * sensitivity * deltaTime * RstickSize;
		{
			//auto cameraRotationQuat = cameraArm->GetLocalRotation();
			//Eigen::Vector3f cameraEulerAngles = cameraRotationQuat.toRotationMatrix().eulerAngles(1, 0, 2);
			//
			//OutputDebugStringW(
			//		(
			//		std::to_wstring(cameraEulerAngles.x()* 180.0f / std::numbers::pi_v<float>) + L" " + 
			//		std::to_wstring(cameraEulerAngles.y()* 180.0f / std::numbers::pi_v<float>) + L" " +
			//		std::to_wstring(cameraEulerAngles.z()* 180.0f / std::numbers::pi_v<float>) + L" " +
			//		L'\n'
			//		) .c_str());
			//
			//// Get the current pitch angle in degrees
			//float currentPitchDegrees = cameraEulerAngles(1) * 180.0f / std::numbers::pi_v<float>;
			//
			//// Calculate the new pitch angle
			//float newPitchDegrees = currentPitchDegrees - pitchAngle;

			float deltaAngle = 0.f;

			xAngle += pitchAngle;

			if (xAngle > limitHighAngle)
			{
				deltaAngle = 0.f;
				xAngle = limitHighAngle;
			}
			else if (xAngle < limitLowAngle)
			{
				deltaAngle = 0.f;
				xAngle = limitLowAngle;
			}
			else
			{
				deltaAngle = pitchAngle;
			}

			// ī�޶� Right ���͸� �������� ȸ���ϱ� ���ؼ� ī�޶��� ���� right�� ����.
			 const Eigen::Vector3f cameraRight = cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitX();

			// ī�޶��� right �������� ī�޶� ȸ��
			 cameraArm->Rotate(cameraRight, deltaAngle);


			if (xAngle > 0.f)
			 {
				const float ease = xAngle / limitHighAngle;
				 auto EasingHigh = [](float x) -> float
					 {
						 return 1.0f - (1.0f - x) * (1.0f - x);
					 };
			 	cameraCore->SetLocalPosition(startPosition + differenceHigh * EasingHigh(ease));
			 }
			 else if (xAngle < 0.f)
			 {
				const float ease = xAngle / limitLowAngle;
				 auto EasingLow = [](float x) -> float
					 {
						 return 1.0f - std::pow(1.0f - x, 5.0f);
					 };
			 	cameraCore->SetLocalPosition(startPosition + differenceLow * EasingLow(ease));
			 }
			 else
			 {
			 	cameraCore->SetLocalPosition(startPosition);
			 }
		}
	}

	void Player::Jump()
	{
		if (!jumping)
		{
			auto curr = playerRigidbody->GetLinearVelocity();
			curr.y() = jumpPower;
			playerRigidbody->SetLinearVelocity(curr);
			jumping = true;
		}
	}

	void Player::HandsUp()
	{

	}

	void Player::PreSerialize(json& jsonData) const
	{

	}

	void Player::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(detect);
		PREDESERIALIZE_VALUE(moveSpeed);
		PREDESERIALIZE_VALUE(sensitivity);

		PREDESERIALIZE_VALUE(jumpPower);
	}

	void Player::PostSerialize(json& jsonData) const
	{

	}

	void Player::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(playerRigidbody);
		POSTDESERIALIZE_PTR(modelCore);
		POSTDESERIALIZE_PTR(cameraArm);
		POSTDESERIALIZE_PTR(cameraCore);
		POSTDESERIALIZE_PTR(animator);
	}
}
