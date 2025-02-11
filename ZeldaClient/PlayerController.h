#pragma once
#include "PurahEngine.h"
#include "PlayerInputData.h"

class PurahEngine::IGamePad;

namespace Phyzzle
{
	class Player;

	class PlayerController
	{
	public:
		explicit PlayerController(Player* player);

		void HandleDebugToggle();

		void HandleInput();  // 입력 처리
		void HandleGamePadInput();
		void HandleKeyboardInput();

		Phyzzle::PlayerInput GetPlayerInputData();
		
	private:
		friend Player;

		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		void HandleStickInput();
		void HandleTriggerInput();
		void HandleButtonInput();
		void HandleButton(
			PurahEngine::ePad button, 
			void (Phyzzle::IState::* clickFunc)(), 
			void (Phyzzle::IState::* pressingFunc)(), 
			void (Phyzzle::IState::* upFunc)());

		void HandleMovementInput();
		void HandleCameraRotationInput();
		void HandleActionInput();
		void HandleAbilityInput();

		Phyzzle::Player* player;
		Phyzzle::PlayerInput currInput;
		PurahEngine::IGamePad* gamePad;
	};
}
