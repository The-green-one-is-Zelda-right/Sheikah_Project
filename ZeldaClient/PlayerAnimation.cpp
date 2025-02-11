#include "PlayerAnimation.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "PlayerController.h"

namespace Phyzzle
{
	PlayerAnimation::PlayerAnimation(Player* player)
		: player(player), currentAnimationState(Phyzzle::AnimationState::IDLE)
	{}

	void PlayerAnimation::UpdateAnimation()
	{
		if (previousAnimationState != currentAnimationState)
		{
			if (animationMap.contains(currentAnimationState))
				animationMap[currentAnimationState]();
		}
		else
		{
			animationSpeed = player->GetController()->GetPlayerInputData().Lstick.Size;
		
			if (animationSpeedMap.contains(currentAnimationState))
				animationSpeedMap[currentAnimationState](animationSpeed);
		}

		previousAnimationState = currentAnimationState;
	}

	void PlayerAnimation::AddAnimationState(
		AnimationState type, const std::wstring& animation,
		PurahEngine::Animator* animator)
	{
		animationMap[type] = [animation, animator]()
			{
				animator->Play(animation);
			};
	}

	void PlayerAnimation::AddAnimationSpeedController(
		AnimationState type, const std::wstring& animation, 
		PurahEngine::Animator* animator)
	{
		animationSpeedMap[type] = [animation, animator](float speed)
			{
				animator->SetPlaySpeed(animation, speed);
			};
	}

	void PlayerAnimation::ChangePlayerAnimationState(AnimationState newState)
	{
		currentAnimationState = newState;
	}
}