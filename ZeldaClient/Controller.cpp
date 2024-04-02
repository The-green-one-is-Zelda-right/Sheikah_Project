#include "GameObject.h"
#include "GamePadManager.h"


#include "InputManager.h"
#include "TimeController.h"

#include "Controller.h"

namespace Phyzzle
{
	std::vector<PurahEngine::Transform*> Controller::controllerable{};

	Controller::~Controller()
	{
		controllerable.erase(std::ranges::find(controllerable, transform));
	}

	void Controller::Awake()
	{
		transform = GetGameObject()->GetTransform();
		controllerable.push_back(transform);

		PurahEngine::GamePadManager::AddGamePad(0);

		gamePad = PurahEngine::GamePadManager::GetGamePad(0);
		gamePad->SetDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}

	void Controller::Update()
	{
		Input();
		Move();
		Around();
	}

	void Controller::Input()
	{
		auto& input = PurahEngine::InputManager::Getinstance();

		if (input.IsKeyDown(PurahEngine::eKey::eKEY_1))
		{
			auto itr = std::ranges::find(controllerable, transform);
			if (itr != controllerable.begin() && itr != controllerable.end())
			{
				--itr;
				if (itr != controllerable.begin() && itr != controllerable.end())
				{
					transform = *itr;
				}
			}
		}
		if (input.IsKeyDown(PurahEngine::eKey::eKEY_2))
		{
			auto itr = std::ranges::find(controllerable, transform);
			if (itr != controllerable.begin() && itr != controllerable.end())
			{
				++itr;
				if (itr != controllerable.begin() && itr != controllerable.end())
				{
					transform = *itr;
				}
			}
		}
		if (input.IsKeyDown(PurahEngine::eKey::eKEY_F1))
		{
			Keybord = Keybord ? false : true;
		}

		if (gamePad->IsConnected())
		{
			LstickSize = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, LstickX, LstickY);
			RstickSize = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, RstickX, RstickY);

			LTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
			RTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);
		}

		if (Keybord)
		{
			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_W))
			{
				LstickSize = 1.f;
				LstickY = 1.f;
			}
			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_S))
			{
				LstickSize = 1.f;
				LstickY = -1.0f;
			}
			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_A))
			{
				LstickSize = 1.f;
				LstickX = -1.0f;
			}
			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_D))
			{
				LstickSize = 1.f;
				LstickX = 1.0f;
			}

			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_Q))
			{
				LTrigger = 1.f;
			}
			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_E))
			{
				RTrigger = 1.f;
			}

			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_UP))
			{
				RstickSize = 1.f;
				RstickY = 1.0f;
			}
			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_DOWN))
			{
				RstickSize = 1.f;
				RstickY = -1.0f;
			}
			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_LEFT))
			{
				RstickSize = 1.f;
				RstickX = -1.0f;
			}
			if (input.IsKeyPressed(PurahEngine::eKey::eKEY_RIGHT))
			{
				RstickSize = 1.f;
				RstickX = 1.0f;
			}
		}
	}

	void Controller::Move()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();

		// ī�޶��� ���� ���͸� ���
		const Eigen::Vector3f front = transform->GetFront();
		const Eigen::Vector3f right = transform->GetRight();
		const Eigen::Vector3f up = transform->GetUp();

		const Eigen::Vector3f movementDirection = front * LstickY + right * LstickX + up * (-LTrigger + RTrigger);
		transform->SetLocalPosition(transform->GetLocalPosition() + movementDirection * moveSpeed * deltaTime);
	}

	void Controller::Around()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();

		// ��ƽ ���⿡ ���� ȸ�� ������ ���
		const float yawAngle = RstickX * sensitivity * deltaTime * RstickSize;
		{
			// ���� up �������� ī�޶� ȸ��
			transform->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
		}

		// ��ƽ ���⿡ ���� ȸ�� ������ ���
		const float pitchAngle = -RstickY * sensitivity * deltaTime * RstickSize;
		{
			float deltaAngle = 0.f;

			xAngle += pitchAngle;

			if (xAngle > 80.f)
			{
				deltaAngle = 0.f;
				xAngle = 80.f;
			}
			else if (xAngle < -80.f)
			{
				deltaAngle = 0.f;
				xAngle = -80.f;
			}
			else
			{
				deltaAngle = pitchAngle;
			}

			// ī�޶� Right ���͸� �������� ȸ���ϱ� ���ؼ� ī�޶��� ���� right�� ����.
			const Eigen::Vector3f cameraRight = transform->GetWorldRotation() * Eigen::Vector3f::UnitX();

			// ī�޶��� right �������� ī�޶� ȸ��
			transform->Rotate(cameraRight, deltaAngle);
		}
	}

	void Controller::PreSerialize(json& jsonData) const
	{
	}

	void Controller::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_VALUE(sensitivity);
		PREDESERIALIZE_VALUE(moveSpeed);
	}

	void Controller::PostSerialize(json& jsonData) const
	{
	}

	void Controller::PostDeserialize(const json& jsonData)
	{
	}
}
