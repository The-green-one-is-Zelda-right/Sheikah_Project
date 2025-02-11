#pragma once
#include <string>
#include <map>
#include "PurahEngine.h"
#include "EnumPlayerState.h"

namespace Phyzzle
{
	class Player;

	class PlayerAnimation
	{
	public:
		explicit PlayerAnimation(Player* player);

		void UpdateAnimation();

		void AddAnimationState(
			AnimationState type, const std::wstring& animation,
			PurahEngine::Animator* animator);
		void AddAnimationSpeedController(
			AnimationState type, const std::wstring& animation,
			PurahEngine::Animator* animator);

		void ChangePlayerAnimationState(AnimationState);
		void SetAnimationSpeed(float speed) { animationSpeed = speed; }

	private:
		friend class Player;

		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		struct AnimationData
		{
			std::wstring idleAnimation;
			std::wstring walkingAnimation;
			std::wstring runningAnimation;
			std::wstring jumpAnimation;
			std::wstring jumpingAnimation;
			std::wstring landingAnimation;

			std::wstring holdIdleAnimation;
			std::wstring holdFrontAnimation;
			std::wstring holdBackAnimation;
			std::wstring holdRightAnimation;
			std::wstring holdLeftAnimation;

			float animationSpeed;
		} animData;

		Player* player;
		float animationSpeed;
		AnimationState previousAnimationState;
		AnimationState currentAnimationState;
		std::map<AnimationState, std::function<void()>> animationMap;
		std::map<AnimationState, std::function<void(float)>> animationSpeedMap;
	};
}
