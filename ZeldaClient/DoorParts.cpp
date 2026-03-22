#include "DoorParts.h"


namespace Phyzzle
{
	void DoorParts::Awake()
	{
		powerCounter = 0;

		originPos = GetGameObject()->GetTransform()->GetLocalPosition();
		targetPos = GetGameObject()->GetTransform()->GetLocalPosition() + movePos;

		done = false;

		state = State::Close;

		if (useRigidbody)
		{
			rigidbody = GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		}
	}

	void DoorParts::Update()
	{
		switch (state)
		{
			case DoorParts::State::Open:
			{
				MoveToTarget(targetPos);
				break;
			}
			case DoorParts::State::Close:
			{
				MoveToTarget(originPos);
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}

	void DoorParts::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= 1)
		{
			state = State::Open;
			done = false;
		}
	}

	void DoorParts::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < 1)
		{
			state = State::Close;
			done = false;
		}
	}

	void DoorParts::MoveToTarget(const Eigen::Vector3f& target)
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (useRigidbody)
		{
			Eigen::Vector3f currentPosition = GetGameObject()->GetTransform()->GetLocalPosition();
			Eigen::Vector3f moveVec = (target - currentPosition).normalized();

			rigidbody->AddForce(moveVec * speed * deltaTime, ZonaiPhysics::Velocity_Change);
		}
		else
		{
			if (done)
			{
				return;
			}

			Eigen::Vector3f currentPosition = GetGameObject()->GetTransform()->GetLocalPosition();

			Eigen::Vector3f moveVec = target - currentPosition;
			float remainingSize = moveVec.norm();
			Eigen::Vector3f moveVecNormal = moveVec.normalized();

			float moveSize = movePos.norm();

			if (remainingSize > moveSize * speed * deltaTime)
			{
				GetGameObject()->GetTransform()->SetLocalPosition(currentPosition + (moveVec * moveSize * speed * deltaTime));
			}
			else
			{
				GetGameObject()->GetTransform()->SetLocalPosition(target);
				done = true;
			}
		}
	}

	void DoorParts::PreSerialize(json& jsonData) const
	{
	}

	void DoorParts::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(useRigidbody);
		PREDESERIALIZE_VECTOR3F(movePos);
		PREDESERIALIZE_VALUE(speed);
	}

	void DoorParts::PostSerialize(json& jsonData) const
	{
	}

	void DoorParts::PostDeserialize(const json& jsonData)
	{
	}
}