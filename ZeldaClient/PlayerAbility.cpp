#include "PlayerAbility.h"
#include "Player.h"
#include "PlayerStateMachine.h"

namespace Phyzzle
{
	PlayerAbility::PlayerAbility(Player* player)
		: player(player),
		attachSystem(std::make_unique<AttachSystem2>(player)),
		rewindSystem(std::make_unique<AttachSystem2>(player)) 
	{}

	void PlayerAbility::UpdateAbilityState()
	{
		auto stateMachine = player->GetController()->GetStateMachine();
		auto currentState = stateMachine->GetCurrentState();

		if (currentState == Player::AbilityState::ATTACH_SELECT)
		{
			ActivateAttach();
		}
		else if (currentState == Player::AbilityState::REWIND)
		{
			ActivateRewind();
		}
	}

	void PlayerAbility::ActivateAttach()
	{
		if (attachSystem->TryAttach())
		{
			player->GetUIManager()->ShowAbilityIcon("Attach");  // ✅ UI 업데이트
		}
	}

	void PlayerAbility::ActivateRewind()
	{
		if (rewindSystem->StartRewind())
		{
			player->GetUIManager()->UpdateRewindProgress(1.0f);  // ✅ UI 업데이트
		}
	}

	void PlayerAbility::CancelAbility()
	{
		attachSystem->CancelAttach();
		rewindSystem->StopRewind();
		player->GetUIManager()->HideAbilityIcon();  // ✅ 능력 취소 UI 처리
	}
}
