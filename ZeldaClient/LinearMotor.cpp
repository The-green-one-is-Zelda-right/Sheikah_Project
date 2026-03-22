#include "LinearMotor.h"

namespace Phyzzle
{
	void LinearMotor::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= 1)
		{
			state = State::Push;
			elapsedTime = 0.0f;
			// rigid->UseGravity(false);
		}
	}

	void LinearMotor::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < 1)
		{
			state = State::Pull;
			// rigid->UseGravity(true);
		}
	}

	void LinearMotor::Awake()
	{
		rigid = GetGameObject()->GetComponent<PurahEngine::RigidBody>();
	}

	void LinearMotor::Start()
	{
		state = State::Pull;
	}

	void LinearMotor::Update()
	{
		StateUpdate();
	}

	void LinearMotor::StateUpdate()
	{
		switch (state)
		{
		case State::Push:
		{
			Eigen::Vector3f currentVelocity = rigid->GetLinearVelocity();
			Eigen::Vector3f targetVelocity = moveDirection.normalized() * pushPower;
			Eigen::Vector3f additionalVelocity = targetVelocity - currentVelocity;
			rigid->AddForce(additionalVelocity, (ZonaiPhysics::ForceType)pushPowerType);
			break;
		}
		case State::Pull:
		{
			Eigen::Vector3f currentVelocity = rigid->GetLinearVelocity();
			Eigen::Vector3f targetVelocity = -moveDirection.normalized() * pullPower;
			Eigen::Vector3f additionalVelocity = targetVelocity - currentVelocity;
			rigid->AddForce(additionalVelocity, (ZonaiPhysics::ForceType)pullPowerType);
			break;
		}
		case State::Wait:
		case State::Sleep:
		{
			// Do Nothing
			break;
		}
		default:
		{
			assert(0);
			break;
		}
		}
	}

	//Eigen::Vector3f GetTargetVelocity()
	//{

	//}

	void LinearMotor::PreSerialize(json& jsonData) const
	{
	}

	void LinearMotor::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VECTOR3F(moveDirection);
		PREDESERIALIZE_VALUE(pushPower);
		PREDESERIALIZE_VALUE(pullPower);
		PREDESERIALIZE_VALUE(pushPowerType);
		PREDESERIALIZE_VALUE(pullPowerType);

		PREDESERIALIZE_VALUE(pushTime);
		PREDESERIALIZE_VALUE(pullTime);

		PREDESERIALIZE_VALUE(powerOnWithStart);
	}

	void LinearMotor::PostSerialize(json& jsonData) const
	{
	}

	void LinearMotor::PostDeserialize(const json& jsonData)
	{
	}
}