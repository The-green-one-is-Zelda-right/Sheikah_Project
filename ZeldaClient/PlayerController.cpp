#include "PlayerController.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "PlayerStateMachine.h"
#include "PlayerCamera.h"
#include "IState.h"

namespace Phyzzle
{
	PlayerController::PlayerController(Player* player)
		: player(player), gamePad()
	{
		gamePad = PurahEngine::GamePadManager::GetGamePad(0);
	}

	void PlayerController::HandleDebugToggle()
	{
		//bool keyCtrl = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_CONTROL);
		//bool keyD = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_D);
		//bool keyShift = PurahEngine::InputManager::Getinstance().IsKeyReleased(PurahEngine::eKey::eKEY_SHIFT);

		//if (keyCtrl && keyD && keyShift)
		//{
		//	currInput.debugMode = !data.debugMode;
		//}
	}

	void PlayerController::HandleInput()
	{
		if (gamePad->IsConnected())
		{
			HandleGamePadInput();
		}
		else
		{
			HandleKeyboardInput();
		}

		HandleDebugToggle();
	}

	void PlayerController::HandleGamePadInput()
	{
		if (gamePad->IsConnected())
		{
			HandleStickInput();
			HandleTriggerInput();
			HandleButtonInput();
		}
	}

	void PlayerController::HandleStickInput()
	{
		currInput.Lstick.Size = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, currInput.Lstick.X, currInput.Lstick.Y);
		currInput.Rstick.Size = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, currInput.Rstick.X, currInput.Rstick.Y);

		player->GetStateMachine()->CurrentAbilityStatePtr()->Stick_L();
		player->GetStateMachine()->CurrentAbilityStatePtr()->Stick_R();
	}

	void PlayerController::HandleTriggerInput()
	{
		currInput.LTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
		currInput.RTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);

		player->GetStateMachine()->CurrentAbilityStatePtr()->Trigger_L();
		player->GetStateMachine()->CurrentAbilityStatePtr()->Trigger_R();
	}

	void PlayerController::HandleButtonInput()
	{
		HandleButton(PurahEngine::ePad::ePAD_SHOULDER_L, &IState::Click_LB, &IState::Pressing_LB, &IState::Up_LB);
		HandleButton(PurahEngine::ePad::ePAD_SHOULDER_R, &IState::Click_RB, &IState::Pressing_RB, &IState::Up_RB);
		HandleButton(PurahEngine::ePad::ePAD_A, &IState::Click_A, &IState::Pressing_A, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_B, &IState::Click_B, &IState::Pressing_B, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_X, &IState::Click_X, &IState::Pressing_X, &IState::Up_X);
		HandleButton(PurahEngine::ePad::ePAD_Y, &IState::Click_Y, &IState::Pressing_Y, &IState::Up_Y);
		HandleButton(PurahEngine::ePad::ePAD_UP, &IState::Click_DUp, &IState::Pressing_DUp, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_DOWN, &IState::Click_DDown, &IState::Pressing_DDown, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_LEFT, &IState::Click_DLeft, &IState::Pressing_DLeft, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_RIGHT, &IState::Click_DRight, &IState::Pressing_DRight, nullptr);
	}

	void PlayerController::HandleButton(
		PurahEngine::ePad button, 
		void (Phyzzle::IState::* clickFunc)(),
		void (Phyzzle::IState::* pressingFunc)(),
		void (Phyzzle::IState::* upFunc)())
	{
		if (gamePad->IsKeyDown(button))
		{
			(player->GetStateMachine()->CurrentAbilityStatePtr()->*clickFunc)();
		}
		else if (gamePad->IsKeyPressed(button))
		{
			if (pressingFunc)
			{
				(player->GetStateMachine()->CurrentAbilityStatePtr()->*pressingFunc)();
			}
		}
		else if (gamePad->IsKeyUp(button))
		{
			if (upFunc)
			{
				(player->GetStateMachine()->CurrentAbilityStatePtr()->*upFunc)();
			}
		}
	}

	void PlayerController::HandleKeyboardInput()
	{
		HandleMovementInput();
		player->GetStateMachine()->CurrentAbilityStatePtr()->Stick_L();
		player->GetStateMachine()->CurrentAbilityStatePtr()->Stick_R();

		HandleCameraRotationInput();
		HandleActionInput();
		HandleAbilityInput();
	}

	Phyzzle::PlayerInput PlayerController::GetPlayerInputData()
	{
		return currInput;
	}

	void PlayerController::HandleMovementInput()
	{
		bool moveForward = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_W);
		bool moveBackward = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_S);
		bool moveLeft = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_A);
		bool moveRight = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_D);

		currInput.Lstick.X = moveLeft ? -1.0f : (moveRight ? 1.0f : 0.0f);
		currInput.Lstick.Y = moveForward ? 1.0f : (moveBackward ? -1.0f : 0.0f);

		float magnitude = currInput.Lstick.X * currInput.Lstick.X + currInput.Lstick.Y * currInput.Lstick.Y;
		if (magnitude > 0.f)
		{
			magnitude = std::sqrt(magnitude);
			currInput.Lstick.Y /= magnitude;
			currInput.Lstick.X /= magnitude;
		}
		currInput.Lstick.Size = std::clamp(std::sqrt(magnitude), 0.0f, 1.0f);
	}

	void PlayerController::HandleCameraRotationInput()
	{
		bool rotateUp = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_UP);
		bool rotateDown = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_DOWN);
		bool rotateLeft = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_LEFT);
		bool rotateRight = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_RIGHT);

		currInput.Rstick.X = rotateLeft ? -1.0f : (rotateRight ? 1.0f : 0.0f);
		currInput.Rstick.Y = rotateUp ? 1.0f : (rotateDown ? -1.0f : 0.0f);

		float magnitude = currInput.Rstick.X * currInput.Rstick.X + currInput.Rstick.Y * currInput.Rstick.Y;
		if (magnitude > 0.f)
		{
			magnitude = std::sqrt(magnitude);
			currInput.Rstick.Y /= magnitude;
			currInput.Rstick.X /= magnitude;
		}
		currInput.Rstick.Size = std::clamp(std::sqrt(magnitude), 0.0f, 1.0f);
	}

	void PlayerController::HandleActionInput()
	{
		bool jump = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_SPACE);
		if (jump)
			player->GetStateMachine()->CurrentAbilityStatePtr()->Click_A();

		bool select = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_F);
		if (select)
			player->GetStateMachine()->CurrentAbilityStatePtr()->Click_B();

		bool attach = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_Z);
		if (attach)
			player->GetStateMachine()->CurrentAbilityStatePtr()->Click_X();
	}

	void PlayerController::HandleAbilityInput()
	{
		bool abillity = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_Q);
		if (abillity)
			player->GetStateMachine()->CurrentAbilityStatePtr()->Click_LB();

		bool rotate = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_E);
		if (rotate)
			player->GetStateMachine()->CurrentAbilityStatePtr()->Click_RB();
	}
}
