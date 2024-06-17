#include <Eigen/Dense>
#include <cmath>
#include <iostream>

#include "Transform.h"
#include "TimeController.h"
#include "Player.h"

#include "DefaultState.h"

#include "Holder.h"

namespace Phyzzle
{
	DefaultState::~DefaultState()
	= default;

	void DefaultState::StateEnter()
	{

	}

	void DefaultState::StateExit()
	{
	}

	void DefaultState::StateStay()
	{
		CameraUpdate();

		static Coroutine ta = CameraTemp();
		// static Coroutine taint = CameraTempint();

		if (!ta.done())
		{
			ta();
		}

		// std::optional value = taint.get_value();
		// if (value.has_value())
		// {
		// 
		// }
	}

	void DefaultState::Stick_L()
	{
		Move();

		auto direction = player->data.playerRigidbody->GetLinearVelocity();
		direction.y() = 0.f;
		LookToWorldDirection(direction);
	}

	void DefaultState::Stick_R()
	{
		// Around();
	}

	void DefaultState::Click_DLeft()
	{
		ChangeState(false);
	}

	void DefaultState::Click_DRight()
	{
		ChangeState(true);
	}

	void DefaultState::Pressing_DLeft()
	{

	}

	void DefaultState::Pressing_DRight()
	{

	}

	void DefaultState::Pressing_A()
	{

	}

	void DefaultState::Pressing_LB()
	{

	}

	void DefaultState::Pressing_RB()
	{

	}

	Coroutine<void> DefaultState::CameraTemp()
	{
		//int i = 0;
		//while (true)
		//{
		//	std::cout << "Coroutine" << i++ << std::endl;
		//	co_await WaitForSeconds(2000);
		//}

		co_return;
	}

	Coroutine<int> DefaultState::CameraTempint()
	{
		int i = 0;
		while (true)
		{
			std::cout << "Coroutine" << i++ << std::endl;
			co_await nullptr;
		}
	}

	void DefaultState::Click_A()
	{
		Jump();
	}

	void DefaultState::Click_LB()
	{
		if (!player->data.jumping)
		{
			player->ChangeState(player->data.state);
		}
	}

	void DefaultState::Click_RB()
	{
		// player->CameraReset();
	}

	void DefaultState::Jump() const
	{
		player->Jump();
	}

	void DefaultState::CameraUpdate()
	{
		CameraAround();
		CameraPositionFromAngle();
	}

	void DefaultState::CameraAround()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();

		// ��ƽ ���⿡ ���� ȸ�� ������ ���
		const float yawAngle = player->currInput.Rstick.X * player->data.sensitivity * deltaTime * player->currInput.Rstick.Size;
		{
			// ���� up �������� ī�޶� ȸ��
			player->data.cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
		}

		// ��ƽ ���⿡ ���� ȸ�� ������ ���
		const float pitchAngle = -player->currInput.Rstick.Y * player->data.sensitivity * deltaTime * player->currInput.Rstick.Size;
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
		}
	}

	void DefaultState::CameraPositionFromAngle()
	{
		auto currP = player->data.cameraCore->GetLocalPosition();

		float distance = 0.f;

		// ������ ���� ī�޶� ��ġ�� ������.
		if (player->data.xAngle >= 0.f)
		{
			const float ease = player->data.xAngle / player->data.limitHighAngle;
			auto EasingHigh = [](float x) -> float
				{
					return 1.0f - (1.0f - x) * (1.0f - x);
				};

			distance = player->data.coreDefaultPosition.z() + player->differenceHigh.z() * EasingHigh(ease);
		}
		else if (player->data.xAngle < 0.f)
		{
			const float ease = player->data.xAngle / player->data.limitLowAngle;
			auto EasingLow = [](float x) -> float
				{
					return 1.0f - std::pow(1.0f - x, 5.0f);
				};

			distance = player->data.coreDefaultPosition.z() + player->differenceLow.z() * EasingLow(ease);
		}

		
		Eigen::Vector3f modelPos = player->data.cameraArm->GetWorldPosition();
		Eigen::Vector3f dir = player->data.cameraCore->GetFront() * -1.f;
		unsigned int layers = player->data.cameraCollisionLayers;
		ZonaiPhysics::ZnQueryInfo info;

		bool hit = PurahEngine::Physics::Spherecast(
			0.3f,
			modelPos, Eigen::Quaternionf::Identity(),
			dir,
			std::fabs(distance),
			layers, info);

		if (hit)
		{
			currP = Eigen::Vector3f::UnitZ() * info.distance * -1.f;
		}
		else
		{
			currP = Eigen::Vector3f::UnitZ() * distance;
		}

		player->data.cameraCore->SetLocalPosition(currP);
	}

	void DefaultState::Move() const
	{
		player->PlayerMove(player->data.moveSpeed);
	}

	void DefaultState::Around() const
	{
		player->CameraAround();
	}

	void DefaultState::LookToWorldDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInWorldDirection(_to);
	}

	void DefaultState::LookToLocalDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInLocalDirection(_to);
	}

	// ���� �ɷ��� ������
	void DefaultState::ChangeState(bool _value) const
	{
		Player::State newState = Player::State::DEFAULT;

		const int size = player->stateChange.size() + 1;

		if (_value)
		{
			newState =
				static_cast<Player::State>(
					(player->data.state + 1)
					);

			newState = 
				static_cast<Player::State>(
					max(newState % size, 1)
					);

			if (newState == Player::State::DEFAULT)
			{
				newState = static_cast<Player::State>(
					newState + 1
					);
			}
		}
		else
		{

			newState =
				static_cast<Player::State>(
					player->data.state - 1
					);

			if (newState == Player::State::DEFAULT)
			{
				newState = static_cast<Player::State>(
					size - 1
					);
			}
		}

		player->data.state = newState;
	}
}
