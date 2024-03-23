#include "Controller.h"

#include "TimeController.h"
#include "Tween.h"
#include <algorithm>

namespace PurahEngine
{
	Controller::~Controller()
		= default;

	void Controller::Awake()
	{
		moveSpeed = 3.f;
		sensitivity = 45.f;
	}

	void Controller::Start()
	{
		GamePadManager::AddGamePad(0);

		gamePad = GamePadManager::GetGamePad(0);
		gamePad->SetDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		startPosition = modelCore->GetWorldPosition();
		startRotation = modelCore->GetWorldRotation();

		// playerRigidbody = gameObject->GetComponent<RigidBody>();
		startLinearVelocity = playerRigidbody->GetLinearVelocity();
		startAngularVelocity = playerRigidbody->GetAngularVelocity();
	}

	void Controller::Update()
	{
		GamePadInput();
		RotateCamera();
		// UpdateCamera();
		Move();
	}

	void Controller::GamePadInput()
	{
		LstickX = 0.f, LstickY = 0.f;
		RstickX = 0.f, RstickY = 0.f;
		LTrigger = 0.f, RTrigger = 0.f;

		if (gamePad->IsConnected())
		{
			LstickSize = gamePad->GetStickRatio(ePadStick::ePAD_STICK_L, LstickX, LstickY);
			RstickSize = gamePad->GetStickRatio(ePadStick::ePAD_STICK_R, RstickX, RstickY);

			LTrigger = gamePad->GetTriggerRatio(ePadTrigger::ePAD_TRIGGER_L);
			RTrigger = gamePad->GetTriggerRatio(ePadTrigger::ePAD_TRIGGER_R);

			if (gamePad->IsKeyDown(ePad::ePAD_A))
			{
				gamePad->VibrateRatio(LTrigger, RTrigger, 3.f);
			}
			if (gamePad->IsKeyDown(ePad::ePAD_B))
			{
				gamePad->VibrateRatio(0.f, 0.f);
			}
		}
	}

	void Controller::Move()
	{
		// 카메라의 전방 벡터를 계산
		Eigen::Vector3f cameraFront = cameraArm->GetFront();
		const Eigen::Vector3f forward = Eigen::Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Eigen::Vector3f right = cameraArm->GetRight();

		Eigen::Vector3f movementDirection = forward * LstickY + right * LstickX;

		std::wstring WString = std::to_wstring(LstickX) + L", " + std::to_wstring(LstickY) + L"\n";

		OutputDebugStringW(WString.c_str());

		movementDirection.y() = 0.f;

		// 속도 벡터를 계산
		Eigen::Vector3f movement = movementDirection * moveSpeed * LstickSize;

		Eigen::Vector3f velocity = playerRigidbody->GetLinearVelocity();

		// y축 운동을 방해하지 않는 걸로 중력은 적용되도록함.
		/// 단, 이런 방식이면 키 입력이 없다면 누군가랑 부딪쳐도 가만히 있을 듯
		velocity.x() = movement.x();
		velocity.z() = movement.z();

		// 속력을 적용시킴
		playerRigidbody->SetLinearVelocity(velocity);

		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();

		// Eigen::Vector3f localForward = modelCore->GetLocalRotation() * Eigen::Vector3f::UnitZ();

		if (!movementDirection.isZero())
		{
			Eigen::Vector3f localForward = parentWorld.conjugate() * movementDirection.normalized();

			// Calculate the rotation quaternion to align the current forward direction with the desired forward direction
			const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), localForward);

			// Set the rotation of the transform to the target rotation
			modelCore->SetLocalRotation(targetRotation);
		}
	}

	void Controller::RotateCamera()
	{
		TimeController& time = TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime("Simulate");

		// 스틱 기울기에 따라 회전 각도를 계산
		const float yawAngle = RstickX * sensitivity * deltaTime * RstickSize;
		{
			// 월드 up 기준으로 카메라를 회전
			cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
		}

		// 스틱 기울기에 따라 회전 각도를 계산
		const float pitchAngle = RstickY * sensitivity * deltaTime * RstickSize;
		{
			auto cameraRotationQuat = cameraArm->GetLocalRotation();
			Eigen::Vector3f cameraEulerAngles = cameraRotationQuat.toRotationMatrix().eulerAngles(0, 1, 2);

			// Get the current pitch angle in degrees
			float currentPitchDegrees = cameraEulerAngles(0) * 180.0f / std::numbers::pi;

			// Calculate the new pitch angle
			float newPitchDegrees = currentPitchDegrees + pitchAngle;

			if (newPitchDegrees < 180.f)
			{
				newPitchDegrees = std::clamp(newPitchDegrees, -1.f, 88.f);
			}
			else
			{
				newPitchDegrees = std::clamp(newPitchDegrees, 335.f, 361.f);
			}

			// 카메라 Right 벡터를 기준으로 회전하기 위해서 카메라의 월드 right를 구함.
			 const Eigen::Vector3f cameraRight = cameraArm->GetWorldRotation() * -Eigen::Vector3f::UnitX();

			//// 제한된 각도만큼 카메라 회전
			// float deltaPitch = newPitchDegrees - cameraEulerAngles(0);

			// 카메라의 right 기준으로 카메라를 회전
			 cameraArm->Rotate(cameraRight, pitchAngle);

			//// If the new pitch angle exceeds the limit, prevent further rotation
			//if (newPitchDegrees != currentPitchDegrees) 
			//{
			//	// Calculate the amount of rotation allowed
			//	//// float allowedRotation = newPitchDegrees - currentPitchDegrees;

			//	// Convert allowed rotation back to std::numbers::pi
			//	//// float allowedRotationRadians = allowedRotation * std::numbers::pi / 180.0f;

			//	// Rotate the camera by the allowed amount
			//	//// cameraArm->Rotate(cameraRight, allowedRotationRadians);

			//	Eigen::Quaternionf q;

			//	// Convert Euler angles to quaternions
			//	Eigen::AngleAxisf rollAngle(cameraEulerAngles.z(), Eigen::Vector3f::UnitX());
			//	Eigen::AngleAxisf pitchAngle(cameraEulerAngles.x(), Eigen::Vector3f::UnitY());
			//	Eigen::AngleAxisf yawAngle(cameraEulerAngles.y(), Eigen::Vector3f::UnitZ());

			//	// Combine the individual quaternions
			//	q = yawAngle * pitchAngle * rollAngle;

			//	cameraArm->SetLocalRotation(q);
			//}
		}
	}

	void Controller::HandsUp()
	{

	}

	void Controller::PreSerialize(json& jsonData) const
	{

	}

	void Controller::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(detect);
		PREDESERIALIZE_VALUE(moveSpeed);
		PREDESERIALIZE_VALUE(sensitivity);
	}

	void Controller::PostSerialize(json& jsonData) const
	{

	}

	void Controller::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(playerRigidbody);
		POSTDESERIALIZE_PTR(modelCore);
		POSTDESERIALIZE_PTR(cameraArm);
		POSTDESERIALIZE_PTR(cameraCore);
	}
}
