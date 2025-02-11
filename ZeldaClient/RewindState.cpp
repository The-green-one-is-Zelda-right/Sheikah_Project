#include "Player.h"

#include "RewindState.h"

namespace Phyzzle
{
	RewindState::~RewindState()
	= default;

	void RewindState::StateEnter()
	{
	}

	void RewindState::StateExit()
	{
	}

	void RewindState::PostStateStay()
	{

	}

	void RewindState::StateStay()
	{

	}

	void RewindState::Stick_L()
	{
		Aim();
	}

	void RewindState::Stick_R()
	{

	}

	void RewindState::Trigger_L()
	{

	}

	void RewindState::Trigger_R()
	{

	}

	void RewindState::Click_DUp()
	{

	}

	void RewindState::Click_DDown()
	{

	}

	void RewindState::Click_DLeft()
	{

	}

	void RewindState::Click_DRight()
	{

	}

	// 능력 준비 중일 때 A를 누르면 취소 됨
	void RewindState::Click_A()
	{
		Cancel();
	}

	// 능력 준비 중일 때 B를 눌러 오브젝트를 선택함.
	void RewindState::Click_B()
	{
		Select();
	}

	void RewindState::Click_X()
	{

	}

	void RewindState::Click_Y()
	{

	}

	void RewindState::Click_LB()
	{
		Cancel();
	}

	void RewindState::Click_RB()
	{

	}

	void RewindState::Aim()
	{

	}

	void RewindState::Cancel()
	{
		player->ChangeAbilityState(AbilityState::DEFAULT);
	}

	void RewindState::Select()
	{
	}
	void RewindState::StateCancel()
	{
	}
}
