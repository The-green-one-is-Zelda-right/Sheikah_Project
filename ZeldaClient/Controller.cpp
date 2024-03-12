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
		gamePad->SetDeadZone(5000);

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

			gamePad->GetStickRatio(ePadStick::ePAD_STICK_L, LstickX, LstickY);
			gamePad->GetStickRatio(ePadStick::ePAD_STICK_R, RstickX, RstickY);

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
		// ī�޶��� ���� ���͸� ���
		Eigen::Vector3f cameraFront = cameraArm->GetFront();
		const Eigen::Vector3f forward = Eigen::Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Eigen::Vector3f right = cameraArm->GetRight();

		Eigen::Vector3f movementDirection = forward * LstickY + right * LstickX;

		movementDirection.y() = 0.f;

		// �ӵ� ���͸� ���
		Eigen::Vector3f movement = movementDirection * moveSpeed;

		Eigen::Vector3f velocity = playerRigidbody->GetLinearVelocity();

		// y�� ��� �������� �ʴ� �ɷ� �߷��� ����ǵ�����.
		/// ��, �̷� ����̸� Ű �Է��� ���ٸ� �������� �ε��ĵ� ������ ���� ��
		velocity.x() = movement.x();
		velocity.z() = movement.z();

		// �ӷ��� �����Ŵ
		playerRigidbody->SetLinearVelocity(velocity);

		//if (LstickY == 0.f && LstickX == 0.f)
		//{
		//	return;
		//}

		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();
		const Eigen::Vector3f localForward = parentWorld.conjugate() * movementDirection.normalized();

		// Calculate the rotation quaternion to align the current forward direction with the desired forward direction
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), localForward);

		// Set the rotation of the transform to the target rotation
		modelCore->SetLocalRotation(targetRotation);
	}

	void Controller::RotateCamera()
	{
		{
			TimeController& time = TimeController::GetInstance();

			const float deltaTime = time.GetDeltaTime("Simulate");

			// ��ƽ ���⿡ ���� ȸ�� ������ ���
			const float yawAngle = RstickX * sensitivity * deltaTime;
			const float pitchAngle = RstickY * sensitivity * deltaTime;

			// ���� up �������� ī�޶� ȸ��
			cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);

			// ī�޶� Right ���͸� �������� ȸ���ϱ� ���ؼ� ī�޶��� ���� right�� ����.
			const Eigen::Vector3f cameraRight = cameraArm->GetWorldRotation() * -Eigen::Vector3f::UnitX();

			// ī�޶��� right �������� ī�޶� ȸ��
			cameraArm->Rotate(cameraRight, pitchAngle);
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
