#include <Eigen/Dense>
#include <cmath>
#include <iostream>

#include "Transform.h"
#include "TimeController.h"

#include "EnumPlayerState.h"
#include "Player.h"

#include "PlayerController.h"
#include "PlayerStateMachine.h"
#include "PlayerMovement.h"
#include "PlayerAnimation.h"
#include "PlayerCamera.h"
#include "PlayerUIManager.h"

#include "DefaultState.h"

namespace Phyzzle
{
	DefaultState::~DefaultState()
	= default;

	void DefaultState::StateEnter()
	{
		player->data.holdObject = nullptr;
		player->data.holdObjectBody = nullptr;
	}

	void DefaultState::StateExit()
	{}

	void DefaultState::StateStay()
	{
		
	}

	void DefaultState::PostStateStay()
	{
		CameraUpdate();
	}

	void DefaultState::StateCancel()
	{}

	void DefaultState::Stick_L()
	{
		Move();

		auto direction = player->data.playerRigidbody->GetLinearVelocity();
		direction.y() = 0.f;
		if (direction.norm() >= 1e-2)
		{
			LookToWorldDirection(direction);
		}
	}

	void DefaultState::Stick_R()
	{

	}

	void DefaultState::Click_A()
	{
		Jump();
	}

	void DefaultState::Click_LB()
	{
		if (player->data.isGrounded)
		{
			player->ChangeAbilityState(AbilityState::ATTACH_SELECT);
		}
	}

	void DefaultState::Jump() const
	{
		if (player->GetMovement()->TryJump())
		{
			player->GetAnimation()->ChangePlayerAnimationState(AnimationState::JUMPING);

			// player->PlayJumping();
		}
	}

	void DefaultState::CameraUpdate()
	{
		player->GetCamera()->UpdateDefaultCamera();
	}

	void DefaultState::Move() const
	{
		if (player->GetMovement()->TryPlayerMove(player->data.moveSpeed))
		{
			if (player->data.isGrounded)
			{
				if (player->GetController()->currInput.Lstick.Size > 0.75f)
				{
					player->GetAnimation()->ChangePlayerAnimationState(AnimationState::RUN);
				}
				else
				{
					player->GetAnimation()->ChangePlayerAnimationState(AnimationState::WALK);
				}
				
				// player->PlayFootStep();
			}
			else
			{
				player->GetAnimation()->ChangePlayerAnimationState(AnimationState::JUMPING);
			}
		}
		else
		{
			if (player->data.isGrounded)
			{
				player->GetAnimation()->ChangePlayerAnimationState(AnimationState::IDLE);
			}
			else
			{
				player->GetAnimation()->ChangePlayerAnimationState(AnimationState::JUMPING);
			}
		}
	}

	void DefaultState::Around() const
	{
		player->GetCamera()->RotateCameraArm();
	}

	void DefaultState::LookToWorldDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInWorldDirection(_to);
	}

	void DefaultState::LookToLocalDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInLocalDirection(_to);
	}
}
