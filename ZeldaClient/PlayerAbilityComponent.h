#pragma once

#include <memory>
#include <unordered_map>

#include "Player.h"
#include "IState.h"

namespace Phyzzle
{
	class PlayerAbilityComponent final
	{
	public:
		using AbilityState = Player::AbilityState;

	public:
		~PlayerAbilityComponent();
		void BindPlayer(Player* _player);
		void InitializeRuntime();
		void CancelCurrentState();

		bool UpdateStateChange();
		void UpdateStateStay();
		void PostUpdateState();
		void AdvanceFrame();

		void RequestState(AbilityState _state);
		AbilityState GetCurrentState() const;

		void HandleGamePadInput();
		void HandleKeyboardStateInput();
		void HandleKeyboardActionInput();
		void HandleKeyboardAbilityInput();

	private:
		void HandleButton(
			PurahEngine::ePad button,
			void (IState::* clickFunc)(),
			void (IState::* pressingFunc)(),
			void (IState::* upFunc)());
		IState* GetState(AbilityState _state) const;
		IState* GetActiveState() const;

	private:
		Player* player = nullptr;
		std::unordered_map<AbilityState, std::unique_ptr<IState>> stateSystem;
		AbilityState prevState = Player::DEFAULT;
		AbilityState currState = Player::DEFAULT;
		AbilityState nextState = Player::DEFAULT;
	};
}
