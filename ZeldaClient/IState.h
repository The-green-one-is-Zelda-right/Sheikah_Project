#pragma once
#include <coroutine>
#include "ePad.h"
#include "Player.h"

namespace Phyzzle
{
	class Player;

	class IState
	{
	public:
		IState() = delete;
		IState(Player* _player) : player(_player) {}
		virtual ~IState() = default;

	protected:
		Player* player;

	public:
		virtual void StateEnter() = 0;

		virtual void StateStay() = 0;
		virtual void PostStateStay() = 0;
		virtual void StateExit() = 0;
		
		virtual void StateCancel() = 0;

	public:
		virtual void Stick_L() {}
		virtual void Stick_R() {}

		virtual void Trigger_L() {}
		virtual void Trigger_R() {}

		virtual void Click_DUp() {}
		virtual void Pressing_DUp() {}

		virtual void Click_DDown() {}
		virtual void Pressing_DDown() {}

		virtual void Click_DLeft() {}
		virtual void Pressing_DLeft() {}

		virtual void Click_DRight() {}
		virtual void Pressing_DRight() {}

		virtual void Click_A() {}
		virtual void Pressing_A() {}

		virtual void Click_B() {}
		virtual void Pressing_B() {}

		virtual void Click_X() {}
		virtual void Pressing_X() {}
		virtual void Up_X() {}

		virtual void Click_Y() {}
		virtual void Pressing_Y() {}
		virtual void Up_Y() {}

		virtual void Click_LB() {}
		virtual void Pressing_LB() {}
		virtual void Up_LB() {}

		virtual void Click_RB() {}
		virtual void Pressing_RB() {}
		virtual void Up_RB() {}
	};
}
