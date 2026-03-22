#include "RespawnTrigger.h"

#include "RespawnSystem.h"

namespace Phyzzle
{
	void RespawnTrigger::RegisterTargetSystem(RespawnSystem* targetSystem, Mode mode, int level, PurahEngine::GameObject* targetObejct)
	{
		targetSystems.push_back({ targetSystem, mode, level, targetObejct });
	}

	void RespawnTrigger::OnTriggerEnter(const PurahEngine::Collider* other)
	{
		for (auto& [targetSystem, mode, level, targetObject] : targetSystems)
		{
			if (targetSystem == nullptr || targetObject == nullptr)
			{
				continue;
			}

			if (other->GetGameObject() == targetObject)
			{
				switch (mode)
				{
					case Mode::levelTrigger:
					{
						targetSystem->OnLevel(level);

						break;
					}
					case Mode::deathTrigger:
					{
						targetSystem->OnDeath();

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
	}

	void RespawnTrigger::PreSerialize(json& jsonData) const
	{
	}

	void RespawnTrigger::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}

	void RespawnTrigger::PostSerialize(json& jsonData) const
	{
	}

	void RespawnTrigger::PostDeserialize(const json& jsonData)
	{
	}
}