#include "LockState.h"

#include "EnumPlayerState.h"
#include "Player.h"

namespace Phyzzle
{
	void LockState::StateEnter()
	{

	}

	void LockState::StateExit()
	{

	}

	void LockState::StateStay()
	{

	}

	void LockState::PostStateStay()
	{

	}

	void LockState::Stick_L()
	{

	}

	void LockState::Stick_R()
	{
		CameraAround();
	}

	void LockState::Click_B()
	{
		IState::Click_B();
	}

	void LockState::Click_LB()
	{
		Cancel();
	}

	void LockState::CameraAround()
	{
		player->RotateCameraArm();
	}

	bool LockState::Aim()
	{
		const float distance = 40.f;
		// const bool hit = player->RaycastFromCamera(distance, &result, &pzObject, nullptr);
		bool hit = false;


		if (!hit)
			return false;

		if (result->IsKinematic())
			return false;

		if (result->GetGameObject()->tag.IsContain(L"Phyzzle Player"))
			return false;

		return true;
	}

	void LockState::Select()
	{

	}

	void LockState::Cancel() const
	{
		player->ChangeAbilityState(AbilityState::DEFAULT);
	}
	void LockState::StateCancel()
	{
	}
}
