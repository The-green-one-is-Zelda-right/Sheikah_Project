#include "SceneLoadSupport.h"

namespace Phyzzle
{
	void SceneLoadSupport::Awake()
	{
		waitLoad = false;
		elapsedTime = false;
	}

	void SceneLoadSupport::Start()
	{
		if (!waitLoad && loadWithStart)
		{
			waitLoad = true;
			elapsedTime = 0.0f;
		}
	}

	void SceneLoadSupport::Update()
	{
		if (waitLoad)
		{
			float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();
			elapsedTime += deltaTime;

			if (elapsedTime >= delay)
			{
				PurahEngine::SceneManager::GetInstance().LoadScene(targetScene);
			}
		}
	}

	void SceneLoadSupport::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other)
	{
		if (!waitLoad && loadWithCollision)
		{
			PurahEngine::Tag& otherTag = other->GetGameObject()->tag;
			
			bool containAllTags = true;

			for (int i = 0; i < targetTags.size(); i++)
			{
				if (otherTag.IsContain(targetTags[i]))
				{
					containAllTags = false;
					break;
				}
			}

			if (containAllTags)
			{
				waitLoad = true;
				elapsedTime = 0.0f;
			}
		}
	}

	void SceneLoadSupport::OnTriggerEnter(const PurahEngine::Collider* other)
	{
		if (!waitLoad && loadWithTrigger)
		{
			PurahEngine::Tag& otherTag = other->GetGameObject()->tag;

			bool containAllTags = true;

			for (int i = 0; i < targetTags.size(); i++)
			{
				if (otherTag.IsContain(targetTags[i]) == false)
				{
					containAllTags = false;
					break;
				}
			}

			if (containAllTags)
			{
				waitLoad = true;
				elapsedTime = 0.0f;
			}
		}
	}

	void SceneLoadSupport::PreSerialize(json& jsonData) const
	{
	}

	void SceneLoadSupport::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(loadWithStart);
		PREDESERIALIZE_VALUE(loadWithTrigger);
		PREDESERIALIZE_VALUE(loadWithCollision);

		targetTags.clear();
		for (int i = 0; i < jsonData["targetTags"].size(); i++)
		{
			std::string str = jsonData["targetTags"][i];
			std::wstring wstr(str.begin(), str.end());
			targetTags.push_back(wstr);
		}

		PREDESERIALIZE_WSTRING(targetScene);
		PREDESERIALIZE_VALUE(delay);
	}

	void SceneLoadSupport::PostSerialize(json& jsonData) const
	{
	}

	void SceneLoadSupport::PostDeserialize(const json& jsonData)
	{
	}
}