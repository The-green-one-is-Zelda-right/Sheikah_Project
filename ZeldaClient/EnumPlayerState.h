#pragma once


namespace Phyzzle
{
	enum class AbilityState
	{
		ATTACH_HOLD = -1,
		DEFAULT = 0,
		ATTACH_SELECT = 1,
		REWIND_SELECT = 2,
		LOCK_SELECT = 3
	};

	enum class AnimationState
	{
		IDLE,
		WALK,
		RUN,
		JUMP,
		JUMPING,
		LANDING,
		ABILITY_IDLE,
		ABILITY_FRONT,
		ABILITY_BACK,
		ABILITY_RIGHT,
		ABILITY_LEFT,
		EVENT,
	};
}