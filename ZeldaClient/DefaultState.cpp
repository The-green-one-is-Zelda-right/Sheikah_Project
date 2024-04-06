#include <Eigen/Dense>

#include "Transform.h"
#include "TimeController.h"
#include "Player.h"

#include "DefaultState.h"

#include "Holder.h"

namespace Phyzzle
{
	DefaultState::~DefaultState()
	= default;

	void DefaultState::operator()()
	{
		Move();
		Around();
	}

	void DefaultState::Stick_L()
	{
	}

	void DefaultState::Stick_R()
	{
	}

	void DefaultState::Trigger_L()
	{
	}

	void DefaultState::Trigger_R()
	{
	}

	void DefaultState::Click_DUp()
	{
	}

	void DefaultState::Click_DDown()
	{
	}

	void DefaultState::Click_DLeft()
	{
	}

	void DefaultState::Click_DRight()
	{
	}

	void DefaultState::Click_A()
	{
		Jump();
	}

	void DefaultState::Click_B()
	{
		Hold();
	}

	void DefaultState::Click_X()
	{
	}

	void DefaultState::Click_Y()
	{
	}

	void DefaultState::Click_LB()
	{
	}

	void DefaultState::Click_RB()
	{
		// �������� ���
	}

	void DefaultState::Push() const
	{

	}

	void DefaultState::Hold() const
	{
		auto target = player->data.holder->GetHolderableBody();
		if (target)
		{
			// player->ChangeState(Player::State::HOLD);
		}
	}

	void DefaultState::Jump() const
	{
		if (!player->data.jumping)
		{
			auto curr = player->data.playerRigidbody->GetLinearVelocity();
			curr.y() = player->data.jumpPower;
			player->data.playerRigidbody->SetLinearVelocity(curr);
			player->data.jumping = true;
		}
	}

	void DefaultState::Move() const
	{
		// ī�޶��� ���� ���͸� ���
		Eigen::Vector3f cameraFront = player->data.cameraArm->GetFront();
		const Eigen::Vector3f forward = Eigen::Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Eigen::Vector3f right = player->data.cameraArm->GetRight();

		Eigen::Vector3f movementDirection = forward * player->pad.LstickY + right * player->pad.LstickX;

		movementDirection.y() = 0.f;

		// �ӵ� ���͸� ���
		Eigen::Vector3f movement = movementDirection * player->data.moveSpeed * player->pad.LstickSize;

		Eigen::Vector3f velocity = player->data.playerRigidbody->GetLinearVelocity();

		// y�� ��� �������� �ʴ� �ɷ� �߷��� ����ǵ�����.
		/// ��, �̷� ����̸� Ű �Է��� ���ٸ� �������� �ε��ĵ� ������ ���� ��
		velocity.x() = movement.x();
		velocity.z() = movement.z();

		// �ӷ��� �����Ŵ
		player->data.playerRigidbody->SetLinearVelocity(velocity);

		const Eigen::Quaternionf parentWorld = player->gameObject->GetTransform()->GetWorldRotation();

		{
			static bool lastbool = false;
			static bool currentbool = false;

			lastbool = currentbool;
			currentbool = movementDirection.isZero();

			if (lastbool != currentbool)
			{
				if (currentbool)
				{
					player->data.animator->Play(L"Armature|Armature|Armature|idle");
				}
				else
				{
					player->data.animator->Play(L"Armature|Armature|Armature|running");
				}
				return;
			}

			if (currentbool)
			{
				return;
			}

			player->data.animator->SetPlaySpeed(L"Armature|Armature|Armature|running", player->pad.LstickSize);
		}

		const Eigen::Vector3f localForward = parentWorld.conjugate() * movementDirection.normalized();

		// Calculate the rotation quaternion to align the current forward direction with the desired forward direction
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), localForward);

		// Set the rotation of the transform to the target rotation
		player->data.modelCore->SetLocalRotation(targetRotation);
	}

	void DefaultState::Around() const
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();

		// ��ƽ ���⿡ ���� ȸ�� ������ ���
		const float yawAngle = player->pad.RstickX * player->data.sensitivity * deltaTime * player->pad.RstickSize;
		{
			// ���� up �������� ī�޶� ȸ��
			player->data.cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
		}

		// ��ƽ ���⿡ ���� ȸ�� ������ ���
		const float pitchAngle = -player->pad.RstickY * player->data.sensitivity * deltaTime * player->pad.RstickSize;
		{
			float deltaAngle = 0.f;

			player->data.xAngle += pitchAngle;

			if (player->data.xAngle > player->data.limitHighAngle)
			{
				deltaAngle = 0.f;
				player->data.xAngle = player->data.limitHighAngle;
			}
			else if (player->data.xAngle < player->data.limitLowAngle)
			{
				deltaAngle = 0.f;
				player->data.xAngle = player->data.limitLowAngle;
			}
			else
			{
				deltaAngle = pitchAngle;
			}

			// ī�޶� Right ���͸� �������� ȸ���ϱ� ���ؼ� ī�޶��� ���� right�� ����.
			const Eigen::Vector3f cameraRight = player->data.cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitX();

			// ī�޶��� right �������� ī�޶� ȸ��
			player->data.cameraArm->Rotate(cameraRight, deltaAngle);

			if (player->data.xAngle > 0.f)
			{
				const float ease = player->data.xAngle / player->data.limitHighAngle;
				auto EasingHigh = [](float x) -> float
					{
						return 1.0f - (1.0f - x) * (1.0f - x);
					};
				player->data.cameraCore->SetLocalPosition(player->startPosition + player->differenceHigh * EasingHigh(ease));
			}
			else if (player->data.xAngle < 0.f)
			{
				const float ease = player->data.xAngle / player->data.limitLowAngle;
				auto EasingLow = [](float x) -> float
					{
						return 1.0f - std::pow(1.0f - x, 5.0f);
					};
				player->data.cameraCore->SetLocalPosition(player->startPosition + player->differenceLow * EasingLow(ease));
			}
			else
			{
				player->data.cameraCore->SetLocalPosition(player->startPosition);
			}
		}
	}
}
