#include <Eigen/Dense>
#include <cmath>
#include <iostream>

#include "Transform.h"
#include "TimeController.h"
#include "Player.h"

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
		player->UseSelectedAbility();
	}

	void DefaultState::Jump() const
	{
		if (player->TryJump())
		{
			player->ChangePlayerAnimationState(Player::JUMPING);

			player->PlayJumping();
		}
	}

	void DefaultState::CameraUpdate()
	{
		player->UpdateDefaultCamera();
	}

	void DefaultState::Move() const
	{
		if (player->TryPlayerMove(player->data.moveSpeed))
		{
			if (player->data.isGrounded)
			{
				if (player->currInput.Lstick.Size > 0.75f)
				{
					player->ChangePlayerAnimationState(Player::RUN);
				}
				else
				{
					player->ChangePlayerAnimationState(Player::WALK);
				}
				
				player->PlayFootStep();
			}
			else
			{
				player->ChangePlayerAnimationState(Player::JUMPING);
			}
		}
		else
		{
			if (player->data.isGrounded)
			{
				player->ChangePlayerAnimationState(Player::IDLE);
			}
			else
			{
				player->ChangePlayerAnimationState(Player::JUMPING);
			}
		}
	}

	void DefaultState::Around() const
	{
		player->RotateCameraArm();
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
