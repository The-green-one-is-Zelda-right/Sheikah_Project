#include "PlayerStateMachine.h"
#include "DefaultState.h"
#include "AttachSelectState.h"
#include "AttachHoldState.h"
#include "LockState.h"
#include "RewindState.h"

namespace Phyzzle
{
	PlayerStateMachine::PlayerStateMachine(Player* player)
		: player(player), 
		currentAbilityState(AbilityState::DEFAULT)
	{
		// 가능한 모든 상태 초기화
		states[AbilityState::DEFAULT] = std::make_unique<DefaultState>(player);
		states[AbilityState::ATTACH_SELECT] = std::make_unique<AttachSelectState>(player);
		states[AbilityState::ATTACH_HOLD] = std::make_unique<AttachHoldState>(player);
		// states[AbilityState::LOCK] = std::make_unique<LockState>(player);
		// states[AbilityState::REWIND] = std::make_unique<RewindState>(player);
	}

	void PlayerStateMachine::UpdateAbilitChangeyState()
	{
		if (previousAbilityState == currentAbilityState)
			return;

		if (states.contains(previousAbilityState))
			states[previousAbilityState]->StateExit();

		if (states.contains(currentAbilityState))
			states[currentAbilityState]->StateEnter();
	}

	void PlayerStateMachine::UpdateAbilityStayState()
	{
		if (previousAbilityState != currentAbilityState)
			return;

		if (states.contains(currentAbilityState))
			states[currentAbilityState]->StateStay();
	}

	void PlayerStateMachine::PostUpdateAbilityState()
	{
		if (previousAbilityState != currentAbilityState)
			return;

		if (states.contains(currentAbilityState))
			states[currentAbilityState]->PostStateStay();

		previousAbilityState = currentAbilityState;
		currentAbilityState = nextAbilityState;
	}

	void PlayerStateMachine::SetAbilityState(AbilityState newState)
	{
		nextAbilityState = newState;
	}
}
