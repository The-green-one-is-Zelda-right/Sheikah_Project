#pragma once
#include <unordered_map>
#include <memory>
#include "PurahEngine.h"
#include "EnumPlayerState.h"
#include "IState.h"

namespace Phyzzle
{
	class Player;

	class PlayerStateMachine
	{
	public:
		explicit PlayerStateMachine(Player* player);

		void UpdateAbilitChangeyState();
		void UpdateAbilityStayState();
		void PostUpdateAbilityState();

		void SetAbilityState(AbilityState newState);
		AbilityState CurrentAbilityStateEnum() const { return currentAbilityState; }
		IState* const CurrentAbilityStatePtr() { return states[currentAbilityState].get(); }

	private:
		friend Player;

		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		Player* player;
		AbilityState previousAbilityState;
		AbilityState currentAbilityState;
		AbilityState nextAbilityState;

#pragma region Select
		int searchAroundbufferSize = 64;
		float searchAroundDistance = 60.f;
		unsigned int searchAroundLayers = 0;

		unsigned int attachRaycastLayers = 0;
		float attachRaycastDistance = 40.f;
#pragma endregion Select

#pragma region Hold
		float targetPositionYSpeed = 4.f;
		float targetPositionZStep = 1.f;

		float minTargetPositionY = -4.f;
		float maxTargetPositionY = 10.f;
		float minTargetPositionZ = 1.f;
		float maxTargetPositionZ = 10.f;
		float targetPositionOffset = 1.5f;

		float linearSpringDamping = 3.f;		// 0 <= x <= 1
		float linearSpringFrequency = 100.f;	// 1.f rad/s ~ 10.f rad/s
		float linearMaxVelocity = 80.f;

		float angularSpringDamping = 2.f;
		float angularSpringFrequency = 80.f;
		float angularMaxVelocity = 100.f;

		float holdRotateAngle = 90.f;
		float arcRatio = 1.5f;
#pragma endregion Hold

		std::unordered_map<AbilityState, std::unique_ptr<IState>> states;
	};
}
