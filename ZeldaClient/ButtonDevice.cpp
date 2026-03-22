#include "ButtonDevice.h"

#include "ElectricalDevice.h"
#include "Door.h"

namespace Phyzzle
{
	void ButtonDevice::Awake()
	{		
		originPos = pushableObject->GetTransform()->GetLocalPosition();

		powerElapsed = 0.0f;
		pushElapsed = 0.0f;

		isLastButtonDown = false;
		isPowerOn = false;
		isButtonDown = false;

		pushableRigidbody = pushableObject->GetComponent<PurahEngine::RigidBody>();
	}

	void ButtonDevice::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		float dist = originPos.y() - pushableObject->GetTransform()->GetLocalPosition().y();

		if (dist >= activationDepth)
		{
			isButtonDown = true;
		}
		else if (dist <= deactivationDepth)
		{
			isButtonDown = false;
		}

		switch (buttonType)
		{
			case Phyzzle::ButtonDevice::ButtonType::Toggle:
			{
				if (isButtonDown)
				{
					pushElapsed += deltaTime;
					if (pushElapsed >= executeDelay)
					{
						TargetPowerOn();
						FastenButton();
					}
				}
				else
				{
					pushElapsed = 0.0f;
				}
				break;
			}
			case Phyzzle::ButtonDevice::ButtonType::Clickable:
			{
				if (isButtonDown)
				{
					pushElapsed += deltaTime;
					if (pushElapsed >= executeDelay)
					{
						TargetPowerOn();
					}
				}
				else
				{
					TargetPowerOff();
					pushElapsed = 0.0f;
				}
				break;
			}
			case Phyzzle::ButtonDevice::ButtonType::Timer:
			{
				TimerButtonUpdate();

				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}

		// Eigen::Vector3f currentVelocity = pushableRigidbody->GetLinearVelocity();
		// Eigen::Vector3f additionalVelocity = targetVelocity - currentVelocity;

	}

	void ButtonDevice::FixedUpdate()
	{
		//Eigen::Vector3f currentVelocity = pushableRigidbody->GetLinearVelocity();
		//Eigen::Vector3f targetVelocity = { 0.0f, releasedSpeed, 0.0f };
		//Eigen::Vector3f additionalVelocity = targetVelocity - currentVelocity;

		//pushableRigidbody->AddForce(additionalVelocity, ZonaiPhysics::Velocity_Change);

		Eigen::Vector3f targetVelocity = { 0.0f, releasedSpeed, 0.0f };
		pushableRigidbody->AddForce(targetVelocity, ZonaiPhysics::Velocity_Change);
	}

	void ButtonDevice::TargetPowerOn()
	{
		if (isPowerOn == true)
		{
			return;
		}

		isPowerOn = true;
		powerElapsed = 0.0f;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOn();
		}
	}

	void ButtonDevice::TargetPowerOff()
	{
		if (isPowerOn == false)
		{
			return;
		}

		isPowerOn = false;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOff();
		}
	}

	void ButtonDevice::FastenButton()
	{
		pushableRigidbody->SetKinematic(true);
	}

	void ButtonDevice::TimerButtonUpdate()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		switch (timerType)
		{
			case Phyzzle::ButtonDevice::TimerType::Hold:
			{
				if (isButtonDown)
				{
					pushElapsed += deltaTime;
					if (pushElapsed >= time + executeDelay)
					{
						TargetPowerOn();
					}
				}
				else
				{
					pushElapsed = 0.0f;
				}

				break;
			}
			case Phyzzle::ButtonDevice::TimerType::Push:
			{
				if (isPowerOn)
				{
					powerElapsed += deltaTime;

					if (powerElapsed >= time)
					{
						TargetPowerOff();
						pushElapsed = 0.0f;
						powerElapsed = 0.0f;
					}
				}
				
				if (isButtonDown)
				{
					pushElapsed += deltaTime;
					if (pushElapsed >= executeDelay)
					{
						TargetPowerOn();
						powerElapsed = 0.0f;
					}
				}
				else
				{
					pushElapsed = 0.0f;
				}

				break;
			}
			case Phyzzle::ButtonDevice::TimerType::Release:
			{
				if (isPowerOn)
				{
					powerElapsed += deltaTime;

					if (powerElapsed >= time)
					{
						TargetPowerOff();
						pushElapsed = 0.0f;
						powerElapsed = 0.0f;
					}
				}

				if (isButtonDown)
				{
					pushElapsed += deltaTime;
				}
				else
				{
					if (pushElapsed >= executeDelay)
					{
						TargetPowerOn();
						powerElapsed = 0.0f;
					}
					pushElapsed = 0.0f;
				}

				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}
	
	void ButtonDevice::PreSerialize(json& jsonData) const
	{
	}
	void ButtonDevice::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		int buttonTypeInt = jsonData["buttonType"];
		buttonType = static_cast<ButtonType>(buttonTypeInt);
		int timerTypeInt = jsonData["timerType"];
		timerType = static_cast<TimerType>(timerTypeInt);

		PREDESERIALIZE_VALUE(time);
		PREDESERIALIZE_VALUE(releasedSpeed);
		PREDESERIALIZE_VALUE(activationDepth);
		PREDESERIALIZE_VALUE(deactivationDepth);
		PREDESERIALIZE_VALUE(executeDelay);
	}
	void ButtonDevice::PostSerialize(json& jsonData) const
	{
	}
	void ButtonDevice::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(pushableObject);
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}