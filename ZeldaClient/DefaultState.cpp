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
		// 상태가 시작되면 암은 캐릭터가
		using namespace Eigen;
		player->data.cameraCore->SetLocalPosition(player->data.coreDefaultPosition);
		player->data.cameraCore->SetLocalRotation(player->data.coreDefaultRotation);
		player->data.cameraArm->SetLocalPosition(player->data.armDefaultPosition);

		auto rot = player->data.modelCore->GetLocalRotation();
		player->data.cameraArm->SetLocalRotation(rot);

		player->data.xAngle = 0.f;
	}

	void DefaultState::StateExit()
	{
	}

	void DefaultState::StateStay()
	{
		CameraUpdate();

		static Coroutine ta = CameraTemp();

		if (!ta.done())
		{
			ta();
		}
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
		int i = 0;
		while (i < 100)
		{
			std::cout << "Coroutine " << i++ << std::endl;
			CO_WAIT;
		}

		co_return;
	}

	Coroutine<int> DefaultState::CameraTempint()
	{
		int i = 0;
		while (true)
		{
			std::cout << "Coroutine" << i++ << std::endl;
			// co_await nullptr;
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

		// 스틱 기울기에 따라 회전 각도를 계산
		const float yawAngle = player->currInput.Rstick.X * player->data.sensitivity * deltaTime * player->currInput.Rstick.Size;
		{
			// 월드 up 기준으로 카메라를 회전
			player->data.cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
		}

		// 스틱 기울기에 따라 회전 각도를 계산
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

			// 카메라 Right 벡터를 기준으로 회전하기 위해서 카메라의 월드 right를 구함.
			const Eigen::Vector3f cameraRight = player->data.cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitX();

			// 카메라의 right 기준으로 카메라를 회전
			player->data.cameraArm->Rotate(cameraRight, deltaAngle);
		}
	}

	void DefaultState::CameraPositionFromAngle()
	{
		auto currP = player->data.cameraCore->GetLocalPosition();

		float distance = 0.f;

		// 각도에 따라 카메라 위치를 움직임.
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

	// 현재 능력을 변경함
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
