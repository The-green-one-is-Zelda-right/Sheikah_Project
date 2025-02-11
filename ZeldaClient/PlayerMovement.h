#pragma once
#include "Eigen/Dense"
#include "PurahEngine.h"

namespace Phyzzle
{
	class Player;

	class PlayerMovement
	{
	public:
		explicit PlayerMovement(Player* player);

		void UpdateMovement();

		bool TryJump();  // 점프 시도
		bool CanMove();  // 이동 가능한 상태인지 확인
		bool TryPlayerMove(float moveSpeed);  // 실제 이동 실행

		void ApplyImpulse();
		void PlayerFlyingUpdate();

		void PlayerOnMovingPlatform(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider);
		void PlayerImpulseCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider);

	private:
		friend Player;

		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		Player* player;

		float impactThreshold = 50.f;  // 충돌 반응 최소 값
		Eigen::Vector3f playerFlyingVelocity;  // 플레이어 공중 속도
		float maxLinearVelocityY = 30.f;  // 최대 수직 속도

		float moveSpeed = 10.f;  // 기본 이동 속도
		float holdSpeed = 5.f;  // 어태치 상태 이동 속도
		float sensitivity = 90.f;  // 카메라 회전 감도
		float jumpPower = 10.f;  // 점프 힘
		float jumpCooldown = 0.1f;  // 점프 후 재사용 시간

		bool isGrounded = false;
		bool flying = false;
		float flyingTime = 0.5f;
		bool isStandableSlope = true;
		Eigen::Vector3f lastGroundNormal;

		Eigen::Vector3f onPlatformVelocity;

		float slopeLimit = 36.f;  // 경사 각도 제한
		float slideFriction = 0.3f;
	};
}
