#include "PlayerMovement.h"
#include "EnumPlayerState.h"
#include "Player.h"
#include "PlayerController.h"
#include "PurahEngine.h"

namespace Phyzzle
{
	PlayerMovement::PlayerMovement(Player* player)
		: player(player), moveSpeed(10.f), holdSpeed(5.f), jumpPower(10.f), slopeLimit(36.f)
	{}

	void PlayerMovement::UpdateMovement()
	{
		ApplyImpulse();
		PlayerFlyingUpdate();
	}

	bool PlayerMovement::TryJump()
	{
		if (!CanMove())
			return false;

		player->GetRigidbody()->AddForce(Eigen::Vector3f::UnitY() * jumpPower, ZonaiPhysics::ForceType::Accelration);
		return true;
	}

	bool PlayerMovement::CanMove()
	{
		return isGrounded && isStandableSlope;
	}

	bool PlayerMovement::TryPlayerMove(float _moveSpeed)
	{
		using namespace Eigen;

		// 이동 방향 계산
		const Vector3f cameraFront = player->GetCameraCoreTrnasform()->GetFront();
		const Vector3f forward = Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Vector3f right = Vector3f::UnitY().cross(forward).normalized();
		Vector3f direction = forward * player->GetController()->GetPlayerInputData().Lstick.Y + right * player->GetController()->GetPlayerInputData().Lstick.X;

		// 목표 속도 설정
		Vector3f targetVelocity = _moveSpeed * direction;

		// 현재 속도 가져오기
		Vector3f currentVelocity = player->GetRigidbody()->GetLinearVelocity();
		currentVelocity.y() = std::clamp(currentVelocity.y(), -maxLinearVelocityY, maxLinearVelocityY);
		player->GetRigidbody()->SetLinearVelocity(currentVelocity);
		currentVelocity.y() = 0.f;

		// 추가 속도 계산
		Vector3f additionalVelocity = targetVelocity - currentVelocity;

		if (flying)
		{
			player->GetRigidbody()->AddForce(targetVelocity, ZonaiPhysics::Force);
		}
		else if (isGrounded)
		{
			player->GetRigidbody()->AddForce(additionalVelocity + onPlatformVelocity, ZonaiPhysics::ForceType::Accelration);
		}
		else
		{
			player->GetRigidbody()->AddForce(additionalVelocity * player->GetRigidbody()->GetMass(), ZonaiPhysics::Force);
		}

		onPlatformVelocity = Vector3f::Zero();

		return direction.norm() > 1e-6;
	}

	void PlayerMovement::ApplyImpulse()
	{
		player->GetRigidbody()->AddForce(playerFlyingVelocity, ZonaiPhysics::Accelration);
		playerFlyingVelocity = Eigen::Vector3f::Zero();
	}

	void PlayerMovement::PlayerFlyingUpdate()
	{
		if (!flying)
			return;

		static float acc = 0.f;
		acc += PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (acc >= flyingTime)
		{
			flying = false;
			acc = 0.f;
		}
	}

	void PlayerMovement::PlayerOnMovingPlatform(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		if (!collider->GetGameObject()->tag.IsContain(L"MovingGround"))
			return;

		PurahEngine::GameObject* gameobj = collider->GetGameObject();
		if (!gameobj)
			return;

		PurahEngine::RigidBody* groundBody = gameobj->GetComponent<PurahEngine::RigidBody>();
		if (!groundBody)
			return;

		Eigen::Vector3f movingGroundVelocity = groundBody->GetLinearVelocity();

		for (int i = 0; i < zn_collision.contactCount; i++)
		{
			// 충돌 지점이 플레이어의 아래쪽인지 확인
			if (zn_collision.contacts[i].point.y() < player->GetGameObject()->GetTransform()->GetWorldPosition().y() + 0.5f)
			{
				if (onPlatformVelocity.norm() < movingGroundVelocity.norm())
				{
					onPlatformVelocity = movingGroundVelocity;
					break;
				}
			}
		}
	}

	void PlayerMovement::PlayerImpulseCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		if (!collider->GetGameObject()->tag.IsContain(L"Shooter"))
			return;

		PurahEngine::GameObject* shooterObj = collider->GetGameObject();
		PurahEngine::RigidBody* shooterBody = shooterObj->GetComponent<PurahEngine::RigidBody>();

		if (!shooterBody)
			return;

		float impulses = zn_collision.impulses.norm();

		if (impulses >= impactThreshold)
		{
			Eigen::Vector3f knockbackForce = zn_collision.impulses;

			if (playerFlyingVelocity.norm() < knockbackForce.norm())
			{
				playerFlyingVelocity = knockbackForce;
				flying = true;
			}
		}
	}
}
