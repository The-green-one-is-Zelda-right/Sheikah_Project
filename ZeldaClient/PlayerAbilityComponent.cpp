#include "PlayerAbilityComponent.h"

#include "AttachHoldState.h"
#include "AttachSelectState.h"
#include "DefaultState.h"
#include "RewindState.h"

namespace Phyzzle
{
	PlayerAbilityComponent::~PlayerAbilityComponent() = default;

	void PlayerAbilityComponent::BindPlayer(Player* _player)
	{
		player = _player;
	}

	void PlayerAbilityComponent::InitializeRuntime()
	{
		if (player == nullptr || !stateSystem.empty())
		{
			return;
		}

		stateSystem.emplace(Player::DEFAULT, std::make_unique<DefaultState>(player));
		stateSystem.emplace(Player::ATTACH_SELECT, std::make_unique<AttachSelectState>(player));
		stateSystem.emplace(Player::ATTACH_HOLD, std::make_unique<AttachHoldState>(player));
		stateSystem.emplace(Player::REWIND_SELECT, std::make_unique<RewindState>(player));
	}

	void PlayerAbilityComponent::CancelCurrentState()
	{
		if (IState* state = GetActiveState())
		{
			state->StateCancel();
		}
	}

	bool PlayerAbilityComponent::UpdateStateChange()
	{
		if (prevState == currState)
		{
			return false;
		}

		if (IState* prev = GetState(prevState))
		{
			prev->StateExit();
		}

		if (IState* curr = GetState(currState))
		{
			curr->StateEnter();
		}

		return true;
	}

	void PlayerAbilityComponent::UpdateStateStay()
	{
		if (prevState != currState)
		{
			return;
		}

		if (IState* state = GetActiveState())
		{
			state->StateStay();
		}
	}

	void PlayerAbilityComponent::PostUpdateState()
	{
		if (prevState != currState)
		{
			return;
		}

		if (IState* state = GetActiveState())
		{
			state->PostStateStay();
		}
	}

	void PlayerAbilityComponent::AdvanceFrame()
	{
		prevState = currState;
		currState = nextState;
	}

	void PlayerAbilityComponent::RequestState(AbilityState _state)
	{
		nextState = _state;
	}

	PlayerAbilityComponent::AbilityState PlayerAbilityComponent::GetCurrentState() const
	{
		return currState;
	}

	void PlayerAbilityComponent::HandleGamePadInput()
	{
		if (player == nullptr || player->gamePad == nullptr || !player->gamePad->IsConnected())
		{
			return;
		}

		player->currInput.Lstick.Size = player->gamePad->GetStickRatio(
			PurahEngine::ePadStick::ePAD_STICK_L,
			player->currInput.Lstick.X,
			player->currInput.Lstick.Y
		);
		player->currInput.Rstick.Size = player->gamePad->GetStickRatio(
			PurahEngine::ePadStick::ePAD_STICK_R,
			player->currInput.Rstick.X,
			player->currInput.Rstick.Y
		);

		if (IState* state = GetActiveState())
		{
			state->Stick_L();
			state->Stick_R();
		}

		player->currInput.LTrigger = player->gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
		player->currInput.RTrigger = player->gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);

		if (IState* state = GetActiveState())
		{
			state->Trigger_L();
			state->Trigger_R();
		}

		HandleButton(PurahEngine::ePad::ePAD_SHOULDER_L, &IState::Click_LB, &IState::Pressing_LB, &IState::Up_LB);
		HandleButton(PurahEngine::ePad::ePAD_SHOULDER_R, &IState::Click_RB, &IState::Pressing_RB, &IState::Up_RB);
		HandleButton(PurahEngine::ePad::ePAD_A, &IState::Click_A, &IState::Pressing_A, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_B, &IState::Click_B, &IState::Pressing_B, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_X, &IState::Click_X, &IState::Pressing_X, &IState::Up_X);
		HandleButton(PurahEngine::ePad::ePAD_Y, &IState::Click_Y, &IState::Pressing_Y, &IState::Up_Y);
		HandleButton(PurahEngine::ePad::ePAD_UP, &IState::Click_DUp, &IState::Pressing_DUp, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_DOWN, &IState::Click_DDown, &IState::Pressing_DDown, nullptr);

		if (currState == Player::ATTACH_HOLD)
		{
			HandleButton(PurahEngine::ePad::ePAD_LEFT, &IState::Click_DLeft, &IState::Pressing_DLeft, nullptr);
			HandleButton(PurahEngine::ePad::ePAD_RIGHT, &IState::Click_DRight, &IState::Pressing_DRight, nullptr);
		}
		else
		{
			if (player->gamePad->IsKeyDown(PurahEngine::ePad::ePAD_LEFT))
			{
				player->SelectPreviousAbility();
			}

			if (player->gamePad->IsKeyDown(PurahEngine::ePad::ePAD_RIGHT))
			{
				player->SelectNextAbility();
			}
		}
	}

	void PlayerAbilityComponent::HandleKeyboardStateInput()
	{
		if (IState* state = GetActiveState())
		{
			state->Stick_L();
			state->Stick_R();
		}
	}

	void PlayerAbilityComponent::HandleKeyboardActionInput()
	{
		IState* state = GetActiveState();
		if (state == nullptr)
		{
			return;
		}

		auto& inputManager = PurahEngine::InputManager::Getinstance();

		if (inputManager.IsKeyDown(PurahEngine::eKey::eKEY_SPACE))
		{
			state->Click_A();
		}

		if (inputManager.IsKeyDown(PurahEngine::eKey::eKEY_F))
		{
			state->Click_B();
		}

		if (inputManager.IsKeyDown(PurahEngine::eKey::eKEY_Z))
		{
			state->Click_X();
		}
	}

	void PlayerAbilityComponent::HandleKeyboardAbilityInput()
	{
		IState* state = GetActiveState();
		if (state == nullptr)
		{
			return;
		}

		auto& inputManager = PurahEngine::InputManager::Getinstance();

		if (inputManager.IsKeyDown(PurahEngine::eKey::eKEY_1))
		{
			player->SelectAbility(Player::ATTACH_SELECT);
		}

		if (inputManager.IsKeyDown(PurahEngine::eKey::eKEY_2))
		{
			player->SelectAbility(Player::REWIND_SELECT);
		}

		if (inputManager.IsKeyDown(PurahEngine::eKey::eKEY_Q))
		{
			state->Click_LB();
		}

		if (inputManager.IsKeyDown(PurahEngine::eKey::eKEY_E))
		{
			state->Click_RB();
		}
	}

	void PlayerAbilityComponent::HandleButton(
		PurahEngine::ePad button,
		void (IState::* clickFunc)(),
		void (IState::* pressingFunc)(),
		void (IState::* upFunc)())
	{
		IState* state = GetActiveState();
		if (state == nullptr || player == nullptr || player->gamePad == nullptr)
		{
			return;
		}

		if (player->gamePad->IsKeyDown(button))
		{
			(state->*clickFunc)();
		}
		else if (player->gamePad->IsKeyPressed(button))
		{
			if (pressingFunc != nullptr)
			{
				(state->*pressingFunc)();
			}
		}
		else if (player->gamePad->IsKeyUp(button))
		{
			if (upFunc != nullptr)
			{
				(state->*upFunc)();
			}
		}
	}

	IState* PlayerAbilityComponent::GetState(AbilityState _state) const
	{
		auto iter = stateSystem.find(_state);
		if (iter == stateSystem.end())
		{
			return nullptr;
		}

		return iter->second.get();
	}

	IState* PlayerAbilityComponent::GetActiveState() const
	{
		return GetState(currState);
	}
}
