#include "HoldState.h"

namespace Phyzzle
{
	HoldState::~HoldState()
	= default;

	void HoldState::Input()
	{

	}

	void HoldState::StateEnter()
	{

	}

	void HoldState::StateExit()
	{

	}

	void HoldState::operator()()
	{

	}

	void HoldState::Stick_L()
	{

	}

	void HoldState::Stick_R()
	{

	}

	void HoldState::Trigger_L()
	{

	}

	void HoldState::Trigger_R()
	{

	}

	void HoldState::Click_DUp()
	{

	}

	void HoldState::Click_DDown()
	{
	}

	void HoldState::Click_DLeft()
	{
	}

	void HoldState::Click_DRight()
	{
	}

	void HoldState::Click_A()
	{
	}

	void HoldState::Click_B()
	{
	}

	void HoldState::Click_X()
	{
	}

	void HoldState::Click_Y()
	{
	}

	void HoldState::Click_LB()
	{
		Cancel();
	}

	void HoldState::Click_RB()
	{
	}

	void HoldState::Cancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	void HoldState::XRotate() const
	{
		
	}

	void HoldState::YRotate() const
	{

	}
}
