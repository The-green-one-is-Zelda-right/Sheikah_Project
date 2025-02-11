#pragma once
#include "AttachSystem2.h"
#include "RewindSystem2.h"

namespace Phyzzle
{
	class Player;

	class PlayerAbility
	{
	public:
		explicit PlayerAbility(Player* player);

		void UpdateAbilityState();  // Attach & Rewind 상태 업데이트
		void ActivateAttach();      // 부착 시작
		void ActivateRewind();      // 되감기 시작
		void CancelAbility();       // 능력 취소

	private:
		struct AbilityData
		{
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
		};

		Player* player;
		std::unique_ptr<AttachSystem2> attachSystem;
		std::unique_ptr<RewindSystem2> rewindSystem;
	};
}
