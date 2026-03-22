#include "ObjectGenerator.h"

namespace Phyzzle
{
	void ObjectGenerator::PowerOn()
	{
		powerCount += 1;

		if (powerCount == 1)
		{
			switch (generatorType)
			{
				case ObjectGenerator::GeneratorType::Always:
				{
					break;
				}
				case ObjectGenerator::GeneratorType::MaintainingPower:
				{
					break;
				}
				case ObjectGenerator::GeneratorType::PowerOn:
				{
					for (PurahEngine::GameObject* object : targetObjects)
					{
						if (object->GetEnable() == false)
						{
							object->SetEnable(true);
							object->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition());
							object->GetTransform()->SetLocalRotation(Eigen::Quaternionf::Identity());
							object->GetComponent<PurahEngine::RigidBody>()->WakeUp();
						}
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
	}

	void ObjectGenerator::PowerOff()
	{
		powerCount -= 1;
	}

	void ObjectGenerator::Awake()
	{
		delayElapsed = generateDelay;
		powerCount = 0;
	}

	void ObjectGenerator::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (delayElapsed < generateDelay)
		{
			delayElapsed += deltaTime;
		}

		switch (generatorType)
		{
			case ObjectGenerator::GeneratorType::Always:
			{
				for (PurahEngine::GameObject* object : targetObjects)
				{
					GenerateObject(object);
				}

				break;
			}
			case ObjectGenerator::GeneratorType::MaintainingPower:
			{
				if (powerCount != 0)
				{
					for (PurahEngine::GameObject* object : targetObjects)
					{
						GenerateObject(object);
					}
				}

				break;
			}
			case ObjectGenerator::GeneratorType::PowerOn:
			{

				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}

	void ObjectGenerator::GenerateObject(PurahEngine::GameObject* object)
	{
		if (object->GetEnable())
		{
			return;
		}

		if (delayElapsed < generateDelay)
		{
			return;
		}

		delayElapsed = 0.0f;

		object->SetEnable(true);
		object->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition());
		object->GetTransform()->SetLocalRotation(Eigen::Quaternionf::Identity());
		object->GetComponent<PurahEngine::RigidBody>()->WakeUp();
	}

	void ObjectGenerator::PreSerialize(json& jsonData) const
	{
	}
	void ObjectGenerator::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(generatorType);
		PREDESERIALIZE_VALUE(generateDelay);
	}
	void ObjectGenerator::PostSerialize(json& jsonData) const
	{
	}
	void ObjectGenerator::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetObjects);
	}
}