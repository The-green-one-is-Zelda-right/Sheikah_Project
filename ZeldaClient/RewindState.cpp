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

	// �ɷ� �غ� ���� �� A�� ������ ��� ��
	void RewindState::Click_A()
	{
		Cancel();
	}

	// �ɷ� �غ� ���� �� B�� ���� ������Ʈ�� ������.
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
		player->ChangeState(Player::State::DEFAULT);
	}

	void RewindState::Select()
	{
	}
}
