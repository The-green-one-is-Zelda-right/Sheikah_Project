#include "PlayerUIManager.h"
#include "Player.h"

namespace Phyzzle
{
	PlayerUIManager::PlayerUIManager(Player* player)
		: player(player)
	{
		// UI 요소 초기화
		// attachSelectUI = player->GetGameObject()->FindChildByName("AttachSelectUI");
		// attachHoldUI = player->GetGameObject()->FindChildByName("AttachHoldUI");
		// attachHoldCollisionUI = player->GetGameObject()->FindChildByName("AttachHoldCollisionUI");
	}

	void PlayerUIManager::UpdateUI()
	{
		// Player의 상태를 확인하여 UI 갱신
		switch (player->GetCurrentAbilityState())
		{
		case AbilityState::DEFAULT:
			break;

		case AbilityState::ATTACH_SELECT:
			break;

		case AbilityState::ATTACH_HOLD:
			break;

		case AbilityState::REWIND_SELECT:
			break;

		case AbilityState::LOCK_SELECT:
			break;

		default:
			break;
		}
	}

	void PlayerUIManager::HideAllUI()
	{
		Attach_Default->SetEnable(false);
		Catch_B->SetEnable(false);
		crossHead01->SetEnable(false);
		crossHead02->SetEnable(false);

		Attach_Hold_NoneStick->SetEnable(false);
		Attach_Hold_Stick->SetEnable(false);
		Rotation_NoneStick->SetEnable(false);
		Rotation_Stick->SetEnable(false);
		Stick_B->SetEnable(false);
	}

	void PlayerUIManager::ShowAttachDefaultUI()
	{
		Attach_Default->SetEnable(true);
		Attach_Hold_Stick->SetEnable(true);

		crossHead01->SetEnable(true);
	}

	void PlayerUIManager::HideAttachDefaultUI()
	{
		Attach_Default->SetEnable(false);
		Attach_Hold_Stick->SetEnable(false);

		crossHead01->SetEnable(false);
	}

	void PlayerUIManager::ShowAttachSelectUI()
	{
		Catch_B->SetEnable(true);
		crossHead02->SetEnable(true);
	}

	void PlayerUIManager::HideAttachSelectUI()
	{
		crossHead01->SetEnable(false);
		crossHead02->SetEnable(false);
	}

	void PlayerUIManager::ShowAttachTouchUI()
	{
		Stick_B->SetEnable(true);
	}

	void PlayerUIManager::HideAttachTouchUI()
	{
		Stick_B->SetEnable(false);
	}

	void PlayerUIManager::ShowAttachRotateUI()
	{

	}

	void PlayerUIManager::HideAttachRotateUI()
	{

	}

	void PlayerUIManager::ShowDettachUI()
	{

	}

	void PlayerUIManager::HideDettachUI()
	{

	}

	void PlayerUIManager::ShowAttachHoldDefaultUI()
	{

	}

	void PlayerUIManager::ShowAttachHoldRotateUI()
	{
		Attach_Hold_NoneStick->SetEnable(true);
		Attach_Hold_Stick->SetEnable(true);
		Rotation_NoneStick->SetEnable(true);
		Rotation_Stick->SetEnable(true);
	}

	void PlayerUIManager::HideAttachHoldDefaultUI()
	{

	}

	void PlayerUIManager::HideAttachHoldRotateUI()
	{
		Attach_Hold_NoneStick->SetEnable(true);
		Attach_Hold_Stick->SetEnable(true);
		Rotation_NoneStick->SetEnable(true);
		Rotation_Stick->SetEnable(true);
		Stick_B->SetEnable(true);
	}
}
